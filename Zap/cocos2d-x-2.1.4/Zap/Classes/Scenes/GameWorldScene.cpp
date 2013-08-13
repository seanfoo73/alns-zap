#include "GameWorldScene.h"
#include "GameOverScene.h"

#include "../Utilities/GameManager.h"

#include "../Entities/BlueBug.h"
#include "../Entities/RedBug.h"
#include "../Entities/GreenBug.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

bool GameWorld::init()
{
	if( !CCLayer::init() )
	{
		return false;
	}
	this->registerWithTouchDispatcher();

	m_respawnTimer = 0;
	m_respawnInterval = 0.25;

	m_remainingGameTime = GameManager::Instance()->m_GameDuration;
	GameManager::Instance()->m_Score = 0;

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(GameWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Zap", "Thonburi", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 1);

	sprintf(timeLeftStringBuf, "%1.1f", m_remainingGameTime);
	const char* timeLeftString = timeLeftStringBuf;
	m_pTimeLeftLabel = CCLabelTTF::create( timeLeftString, "Thonburi", 108);
	m_pTimeLeftLabel->setPosition( ccp( size.width / 2, size.height / 2 ) );
	this->addChild(m_pTimeLeftLabel, 2);

	m_pTestLabel = CCLabelTTF::create( "test", "Thonburi", 32 );
	m_pTestLabel->setPosition( ccp( 400, 500 ) );
	this->addChild(m_pTestLabel, 3 );

	this->schedule( schedule_selector(GameWorld::update) );

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Zap_BackgroundMainLoop.mp3", true );

	return true;
}


CCScene* GameWorld::scene()
{
	CCScene *scene = CCScene::create();

	GameWorld *layer = GameWorld::create();

	scene->addChild(layer);

	return scene;
}

void GameWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void GameWorld::spawnBug()
{
	int bugType = arc4random() % 3;

	if( bugType == 0 )
	{
		BugBase* blue = new BlueBug( "BlueBug.png" );
		this->addChild(blue->m_pSprite);
		GameManager::Instance()->m_Bugs->push_back(blue);
	}
	else if( bugType == 1 )
	{
		BugBase* red = new RedBug( "RedBug.png" );
		this->addChild(red->m_pSprite);
		GameManager::Instance()->m_Bugs->push_back(red);
	}
	else if( bugType == 2 )
	{
		BugBase* green = new GreenBug( "GreenBug.png" );
		this->addChild(green->m_pSprite);
		GameManager::Instance()->m_Bugs->push_back(green);
	}
}

void GameWorld::update(float _dt)
{
	if( IsGameOver() )
	{
		TransitionToGameOver();
	}
	UpdateGameTime( _dt );

	for( int i = 0; i < GameManager::Instance()->m_Bugs->size(); i++ )
	{
		(*GameManager::Instance()->m_Bugs)[i]->Update( _dt );
	}

	m_respawnTimer += _dt;
	if( m_respawnTimer >= m_respawnInterval )
	{
		spawnBug();
		m_respawnTimer = 0;
	}

	UpdateGUI();
	CheckBugsOutOfBounds();
	RemoveBugsFromWorld();

	m_remainingChainTime -= _dt;
	if( m_remainingChainTime <= 0 )
	{
		GameManager::Instance()->DestroyChain();
	}

	//sprintf( testLabelStringBuf, "Bugs Hit by Lightning: %d", GameManager::Instance()->m_BugsHitByLightning->size() );
	sprintf( testLabelStringBuf, "Score: %d", GameManager::Instance()->m_Score );
	const char* testLabelString = &testLabelStringBuf[0];
	m_pTestLabel->setString(testLabelString);
}

void GameWorld::UpdateGUI()
{
	/* This is to make sure we don't get "negative times" to display in the countdown */
	if( m_remainingGameTime < 0 )
	{
		sprintf(timeLeftStringBuf, "%1.1f", 0.0f);
	}
	else
	{
		sprintf(timeLeftStringBuf, "%1.1f", m_remainingGameTime);
	}

	const char* timeLeftString = timeLeftStringBuf;
	m_pTimeLeftLabel->setString( timeLeftString );
}

void GameWorld::CheckBugsOutOfBounds()
{
	std::vector<BugBase*>* bugs = GameManager::Instance()->m_Bugs;
	BugBase* bug;
	for( int i = 0; i < bugs->size(); i++ )
	{
		bug = (*bugs)[i];
		if( !bug->CheckIfInBounds() )
		{
			bug->SetBugState( BugBase::BugState_Dead );
			bugs->erase( bugs->begin() + i );
			GameManager::Instance()->m_BugsToDelete->push_back( bug );
			GameManager::Instance()->m_NumReachedOutOfBounds++;
		}
	}
}

void GameWorld::CheckChainLength()
{
	if( GameManager::Instance()->m_CurrentChainType == GameManager::BugType_Blue )
	{
		if( GameManager::Instance()->m_BugsHitByLightning->size() >=
			GameManager::Instance()->m_MaxBlueChainLength )
		{
			GameManager::Instance()->DestroyChain();
		}
	}
	else if( GameManager::Instance()->m_CurrentChainType == GameManager::BugType_Red )
	{
		if( GameManager::Instance()->m_BugsHitByLightning->size() >=
			GameManager::Instance()->m_MaxRedChainLength )
		{
			GameManager::Instance()->DestroyChain();
		}
	}
	else if( GameManager::Instance()->m_CurrentChainType == GameManager::BugType_Green )
	{
		if( GameManager::Instance()->m_BugsHitByLightning->size() >=
			GameManager::Instance()->m_MaxGreenChainLength )
		{
			GameManager::Instance()->DestroyChain();
		}
	}
}

void GameWorld::RemoveBugsFromWorld()
{
	std::vector<BugBase*>* bugsToDelete = GameManager::Instance()->m_BugsToDelete;
	BugBase* bug;
	int numBugsToRemove = bugsToDelete->size();

	for( int i = 0; i < numBugsToRemove; i++ )
	{
		bug = (*bugsToDelete)[0];
		this->removeChild( bug->m_pSprite, true );
		bugsToDelete->erase( bugsToDelete->begin() + 0 );
		delete bug;

		GameManager::Instance()->m_BugsDeleted++;
	}
}

void GameWorld::draw()
{
	DrawLightning();
}

void GameWorld::DrawLightning()
{
	std::vector<BugBase*>* bugs;
	bugs = GameManager::Instance()->m_BugsHitByLightning;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	cocos2d::ccDrawColor4F( 0.0f, 0.930f, 0.930f, 1.0f );

	for( int i= 0; i < bugs->size(); i++ )
	{
		//first stretch of lightning
		if( i == 0 )
		{
			ccDrawLine( ccp( size.width/2, size.height/2 ), 		//from
						(*bugs)[i]->m_pSprite->getPosition() );		//to
		}
		else
		{
			ccDrawLine( (*bugs)[i-1]->m_pSprite->getPosition(), 	//from
						(*bugs)[i]->m_pSprite->getPosition() );		//to
		}
	}
}

void GameWorld::UpdateGameTime( float _dt )
{
	m_remainingGameTime -= _dt;
}

bool GameWorld::IsGameOver()
{
	if( m_remainingGameTime <= 0.0f )
	{
		return true;
	}
	return false;
}

void GameWorld::TransitionToGameOver()
{
	CCScene* pScene = GameOver::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create( 0.5, pScene));
}

/* function will check if the bug collides with a point then destroy the bug at the point */
/* the collision  detection area will be a little larger than the actual bug on screen
	 * to make it easier to click
	 */
void GameWorld::checkBugsCollideWithPoint( CCPoint point )
{
	std::vector<BugBase*>* bugs;
	bugs = GameManager::Instance()->m_Bugs;
	float touchLeniencyFactor = 2;//1.5;

	for( int i = 0; i < bugs->size(); i++ )
	{
		float tarRadius = ( (*bugs)[i]->m_pSprite->getContentSize().width / 2 ) * touchLeniencyFactor;
		float tarX = (*bugs)[i]->GetPositionX();
		float tarY = (*bugs)[i]->GetPositionY();

		float distance = ( tarX - point.x ) * ( tarX - point.x ) + ( tarY - point.y ) * ( tarY - point.y );
		if( distance <= (tarRadius * tarRadius) )
		{
			/* bug is hit! */
			(*bugs)[i]->SetBugState(BugBase::BugState_Shocked);
			GameManager::Instance()->m_BugsHitByLightning->push_back( (*bugs)[i] );
			GameManager::Instance()->AddBugHit( (*bugs)[i] );

			bugs->erase( bugs->begin()+i );
			this->CheckChainLength();

			if( GameManager::Instance()->m_BugsHitByLightning->size() > 0 )
			{
				m_remainingChainTime = GameManager::Instance()->m_MaxNextBugTime;
			}
		}
	}
}

void GameWorld::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameWorld::ccTouchBegan( CCTouch* touch, CCEvent* event )
{
	CCPoint touchLocation = touch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

	checkBugsCollideWithPoint( touchLocation );

	return true;
}

void GameWorld::ccTouchEnded( CCTouch* touch, CCEvent* event )
{
	CCPoint touchLocation = touch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
}

void GameWorld::ccTouchCancelled( CCTouch* touch, CCEvent* event )
{

}

void GameWorld::ccTouchMoved( CCTouch* touch, CCEvent* event )
{

}

