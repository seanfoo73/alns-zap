#include "GameWorldScene.h"
#include "GameOverScene.h"

#include "../Utilities/GameManager.h"

#include "../Entities/BlueBug.h"
#include "../Entities/RedBug.h"
#include "../Entities/GreenBug.h"

#include "SimpleAudioEngine.h"

#include <algorithm>

using namespace cocos2d;
using namespace CocosDenshion;

bool GameWorld::init()
{
	if( !CCLayer::init() )
	{
		return false;
	}
	this->registerWithTouchDispatcher();

	GameManager::Instance()->setGameLayer( this );

	m_respawnTimer = 0;
	m_respawnInterval = 0.25;

	m_remainingGameTime = GameManager::Instance()->m_GameDuration;
	GameManager::Instance()->m_Score = 0;

	m_pBackground = CCSprite::create("Background.png", CCRectMake( 0, 0, 1280, 720 ) );
	m_pBackground->setPosition( ccp( 	CCDirector::sharedDirector()->getWinSize().width/2,
										CCDirector::sharedDirector()->getWinSize().height/2 ));
	this->addChild( m_pBackground, 1 );

	m_pLantern = CCSprite::create("Lantern.png", CCRectMake( 0, 0, 177, 177 ) );
	m_pLantern->setPosition( ccp(		CCDirector::sharedDirector()->getWinSize().width/2,
										CCDirector::sharedDirector()->getWinSize().height/2 ));
	this->addChild( m_pLantern, 3 );

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(GameWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 3 );

	CCLabelTTF* pLabel = CCLabelTTF::create("Zap", "fonts/Roboto-Regular.ttf", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 5 );

	m_countdownTextTimer = 0.0f;
	m_countdownTimerStart = 10.0f;
	sprintf(timeLeftStringBuf, "Time: %1.1f", m_remainingGameTime);
	const char* timeLeftString = timeLeftStringBuf;
	m_pTimeLeftLabel = CCLabelTTF::create( timeLeftString, "fonts/Roboto-Regular.ttf", 32);
	m_pTimeLeftLabel->setPosition( ccp( size.width - 176 , size.height - 32 ) );
	m_pTimeLeftLabel->setAnchorPoint( ccp( 0, 0.5f ) ); //aligned left
	this->addChild(m_pTimeLeftLabel, 5 );

	sprintf(scoreStringBuf, "Score: %d", GameManager::Instance()->m_Score);
	const char* scoreString = scoreStringBuf;
	m_pScoreLabel = CCLabelTTF::create( scoreString, "fonts/Roboto-Regular.ttf", 32);
	m_pScoreLabel->setPosition( ccp( 32, size.height - 32 ) );
	m_pScoreLabel->setAnchorPoint( ccp( 0, 0.5f ) ); //aligned left
	this->addChild(m_pScoreLabel, 5 );

	m_pTestLabel = CCLabelTTF::create( "test", "fonts/Roboto-Regular.ttf", 32 );
	m_pTestLabel->setPosition( ccp( 400, 500 ) );
	this->addChild(m_pTestLabel, 6 );

	if( m_LightningPoints )
	{
		for( int i = 0; i < m_LightningPoints->size(); i++ )
		{
			delete (*m_LightningPoints)[i];
		}
		m_LightningPoints->clear();
	}
	else
	{
		m_LightningPoints = new std::vector<LightningLine*>();
		for( int i = 0; i < m_LightningPoints->size(); i++ )
		{
			delete (*m_LightningPoints)[i];
		}
		m_LightningPoints->clear();
	}

	m_LightningLastCalc = 0;

	m_pLightningSegmentBatch = CCSpriteBatchNode::create( "LightningSegment.png" );
	m_pLightningSegmentBatch->removeAllChildrenWithCleanup( true );
	this->addChild(m_pLightningSegmentBatch, 2);

	m_pLightningEndBatch = CCSpriteBatchNode::create( "LightningEnd.png" );
	m_pLightningEndBatch->removeAllChildrenWithCleanup( true );
	this->addChild(m_pLightningEndBatch, 2);

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Zap_BackgroundMainLoop.mp3", true );

	m_pFloatingTextManager = new FloatingTextManager( this );

	m_bGameStarted = true;

	this->schedule( schedule_selector(GameWorld::update) );

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

void GameWorld::addGameTime( float time )
{
	m_remainingGameTime += time;
}

void GameWorld::spawnBug()
{
	int bugType = arc4random() % 3;

	if( bugType == 0 )
	{
		BugBase* blue = new BlueBug( "BlueBug.png" );
		this->addChild(blue->m_pSprite, 4);
		GameManager::Instance()->m_Bugs->push_back(blue);
	}
	else if( bugType == 1 )
	{
		BugBase* red = new RedBug( "RedBug.png" );
		this->addChild(red->m_pSprite, 4);
		GameManager::Instance()->m_Bugs->push_back(red);
	}
	else if( bugType == 2 )
	{
		BugBase* green = new GreenBug( "GreenBug.png" );
		this->addChild(green->m_pSprite, 4);
		GameManager::Instance()->m_Bugs->push_back(green);
	}
}

void GameWorld::addFloatingText( const char* txt, float _x, float _y, float size, float duration )
{
	if( m_pFloatingTextManager )
	{
		m_pFloatingTextManager->addFloatingText( txt, _x, _y, size, duration );
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

	if( m_remainingGameTime <= (m_countdownTimerStart+1.0f) )
	{
		m_countdownTextTimer += _dt;
	}

	UpdateGUI();
	CheckBugsOutOfBounds();
	RemoveBugsFromWorld();

	if( m_pFloatingTextManager )
		m_pFloatingTextManager->update( _dt );

	m_LightningLastCalc += _dt;

	m_remainingChainTime -= _dt;
	if( m_remainingChainTime <= 0 )
	{
		//GameManager::Instance()->DestroyChain();
		//DrawLightning( true );
	}

	//sprintf( testLabelStringBuf, "Bugs Hit by Lightning: %d", GameManager::Instance()->m_BugsHitByLightning->size() );
	sprintf( testLabelStringBuf, " " );
	const char* testLabelString = &testLabelStringBuf[0];
	m_pTestLabel->setString(testLabelString);
}

void GameWorld::UpdateGUI()
{
	/* This is to make sure we don't get "negative times" to display in the countdown */
	if( m_remainingGameTime < 0 )
	{
		sprintf(timeLeftStringBuf, "Time: %1.1f", 0.0f);
	}
	else
	{
		sprintf(timeLeftStringBuf, "Time: %1.1f", m_remainingGameTime);

		DoCountdown();
	}
	sprintf(scoreStringBuf, "Score: %d", GameManager::Instance()->m_Score);

	const char* timeLeftString = timeLeftStringBuf;
	m_pTimeLeftLabel->setString( timeLeftString );

	const char* scoreString = scoreStringBuf;
	m_pScoreLabel->setString( scoreString );
}

void GameWorld::DoCountdown()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	if( m_countdownTextTimer >= 1.0f )
	{
		m_countdownTextTimer = 0.0f;
		if( m_pFloatingTextManager )
		{
			char buf[8];
			sprintf( buf, "%d", (int)m_remainingGameTime+1);
			m_pFloatingTextManager->addFloatingText( 	buf,
														size.width/2,
														size.height - size.height/4,
														96.0f, 1.0f );
		}
	}
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
			bug->SetBugState( BugBase::BugState_Dead_OOB);
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
			DrawLightning( true );
		}
	}
	else if( GameManager::Instance()->m_CurrentChainType == GameManager::BugType_Red )
	{
		if( GameManager::Instance()->m_BugsHitByLightning->size() >=
			GameManager::Instance()->m_MaxRedChainLength )
		{
			GameManager::Instance()->DestroyChain();
			DrawLightning( true );
		}
	}
	else if( GameManager::Instance()->m_CurrentChainType == GameManager::BugType_Green )
	{
		if( GameManager::Instance()->m_BugsHitByLightning->size() >=
			GameManager::Instance()->m_MaxGreenChainLength )
		{
			GameManager::Instance()->DestroyChain();
			DrawLightning( true );
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

		/**/
		if( bug->GetBugState() == BugBase::BugState_Dead_KILLED )
		{
			char buf[64];
			sprintf(buf, "+%d", bug->GetPointValue());
			if( m_pFloatingTextManager )
			{
				m_pFloatingTextManager->addFloatingText( 	buf,
															bug->GetPositionX(),
															bug->GetPositionY(),
															24.0f, 2.0f );
			}
		}
		/**/

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

void GameWorld::DrawLightning(bool forceRecalc)
{
	if( !m_bGameStarted )
	{
		CCLOG("GAMEWORLD::DRAWLIGHTNING::GAME IS OVER");
		return;
	}
	std::vector<BugBase*>* bugs;
	bugs = GameManager::Instance()->m_BugsHitByLightning;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	cocos2d::ccDrawColor4F( 0.0f, 0.930f, 0.930f, 1.0f );

	m_pLightningSegmentBatch->removeAllChildrenWithCleanup( true );
	m_pLightningEndBatch->removeAllChildrenWithCleanup( true );

	for( int i = 0; i < m_LightningPoints->size(); i++ )
	{
		DrawLightningLine( 	(*m_LightningPoints)[i]->m_start,
							(*m_LightningPoints)[i]->m_end,
							(*m_LightningPoints)[i]->m_thickness );
	}

	if( m_LightningLastCalc >= GameManager::Instance()->m_LightningRecalcInterval || forceRecalc)
	{
		for( int i = 0; i < m_LightningPoints->size(); i++ )
		{
			delete (*m_LightningPoints)[i];
		}

		m_LightningPoints->clear();
		for( int i = 0; i < bugs->size(); i++ )
		{
			//first stretch of lightning
			if( i == 0 )
			{
				GenerateLightningPointsList( ccp( size.width/2, size.height/2 ), 			//from
											(*bugs)[i]->m_pSprite->getPosition(), i+1 ); 	//to
			}
			else
			{
				GenerateLightningPointsList( (*bugs)[i-1]->m_pSprite->getPosition(), 		//from
											   (*bugs)[i]->m_pSprite->getPosition(), i+1 ); //to
			}
		}
		m_LightningLastCalc = 0;
	}
}

void GameWorld::DrawLightningLine( CCPoint start, CCPoint end, float thickness )
{
	const float ImageThickness = 40;
	float thicknessScale = thickness / ImageThickness;

	CCPoint tangent = ccp( end.x - start.x, end.y - start.y );
	float rotation = float( atan2( tangent.y, tangent.x ) );
	float trueRotationRad = -rotation * (180 / 3.14 );

	CCSprite* pSegment = CCSprite::createWithTexture( m_pLightningSegmentBatch->getTexture() );

	float lengthScale = start.getDistance( end );

	pSegment->setPosition( start );
	pSegment->setScaleX( lengthScale );
	pSegment->setScaleY( thicknessScale );
	pSegment->setRotation( trueRotationRad );
	pSegment->setPosition( ccp( start.x + tangent.x/2, start.y + tangent.y/2 ) );

	m_pLightningSegmentBatch->addChild(pSegment, 0);

	CCSprite* pStart = CCSprite::createWithTexture( m_pLightningEndBatch->getTexture() );
	CCSprite* pEnd = CCSprite::createWithTexture( m_pLightningEndBatch->getTexture() );

	pStart->setPosition( start );
	pStart->setScaleX( thicknessScale );
	pStart->setScaleY( thicknessScale );
	pStart->setRotation( trueRotationRad );
	m_pLightningEndBatch->addChild( pStart, 0 );

	pEnd->setPosition( end );
	pEnd->setScaleX( thicknessScale );
	pEnd->setScaleY( thicknessScale );
	pEnd->setRotation( trueRotationRad + 3.14 );
	m_pLightningEndBatch->addChild( pEnd, 0 );
}

void GameWorld::GenerateLightningPointsList( CCPoint start, CCPoint end, int segment )
{
	std::vector<float> positions;

	CCPoint tangent = ccp( end.x - start.x, end.y - start.y );
	CCPoint normal = ccpNormalize( ccp( tangent.y, -tangent.x ) );
	float length = start.getDistance( end );

	float prevDisplacement = 0;
	float sway = 320;
	float jag = 1/sway;

	int segments = 8;//length/80;
	for( int i = 0; i < segments; i++ )
	{
		positions.push_back( float( arc4random() % 100 ) / 100.0 );
	}
	std::sort (positions.begin(), positions.end() );

	/**
	CCLOG("Start -----------------------------");
	for( int i = 0; i < positions.size(); i++ )
	{
		CCLOG("%d: %f ", i, positions[i]);
	}
	CCLOG("End -------------------------------");
	/**/

	CCPoint prevPoint = start;
	for( int i = 1; i < segments; i++ )
	{
		float scale = ( length * jag ) * ( positions[i] - positions[i-1] );
		float envelope = positions[i] > 0.95f ? 20 * (1 - (positions[i])) : 1;

		float displacement = ( arc4random() % int(sway) ) - (sway/2);
		displacement -= (displacement - prevDisplacement) * (1 - scale);
		displacement *= envelope;

		CCPoint nextPoint = ccp ( 	start.x + positions[i] * tangent.x + displacement * normal.x,
									start.y + positions[i] * tangent.y + displacement * normal.y );

		m_LightningPoints->push_back( new LightningLine( prevPoint, nextPoint, segment ) );

		prevPoint = nextPoint;
		prevDisplacement = displacement;
	}
	m_LightningPoints->push_back( new LightningLine( prevPoint, end, segment ) );
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
	m_bGameStarted = false;
	this->unschedule( schedule_selector(GameWorld::update) );
	GameManager::Instance()->DestroyChain();
	DrawLightning( true );

	/**/
	if( m_pFloatingTextManager )
		delete m_pFloatingTextManager;
	/**/

	m_pLightningSegmentBatch->removeAllChildrenWithCleanup( true );
	m_pLightningEndBatch->removeAllChildrenWithCleanup( true );

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
	std::vector<int> bugsHitLocation;
	bool bugAdded = false;
	bugs = GameManager::Instance()->m_Bugs;
	float touchLeniencyFactor = 3;

	/* collect all the bugs we have hit */
	for( int i = 0; i < bugs->size(); i++ )
	{
		float tarRadius = ( (*bugs)[i]->m_pSprite->getContentSize().width / 2 ) * touchLeniencyFactor;
		float tarX = (*bugs)[i]->GetPositionX();
		float tarY = (*bugs)[i]->GetPositionY();

		float distance = ( tarX - point.x ) * ( tarX - point.x ) + ( tarY - point.y ) * ( tarY - point.y );
		if( distance <= (tarRadius * tarRadius) )
		{
			/* bug is hit! */
			bugsHitLocation.push_back( i );
		}
	}

	/* choose the bug we want if we hit multiple bugs on accident */
	if( bugsHitLocation.size() > 1 )
	{
		for( int i = 0; i < bugsHitLocation.size(); i++ )
		{
			if( ( 	GameManager::Instance()->ReturnBugType((*bugs)[bugsHitLocation[i]]) ==
					GameManager::Instance()->m_CurrentChainType ) ||
				(	GameManager::Instance()->m_CurrentChainType == GameManager::BugType_None )
			  )
			{
				(*bugs)[bugsHitLocation[i]]->SetBugState(BugBase::BugState_Shocked);
				GameManager::Instance()->m_BugsHitByLightning->push_back( (*bugs)[bugsHitLocation[i]] );
				GameManager::Instance()->AddBugHit( (*bugs)[bugsHitLocation[i]] );

				bugs->erase( bugs->begin()+bugsHitLocation[i] );
				this->CheckChainLength();

				if( GameManager::Instance()->m_BugsHitByLightning->size() > 0 )
				{
					m_remainingChainTime = GameManager::Instance()->m_MaxNextBugTime;
				}

				bugAdded = true;
				break;
			}
		}
	}

	/* if we haven't added a bug to the hit list and we have hit a bug */
	if( !bugAdded && bugsHitLocation.size() >= 1 )
	{
		(*bugs)[bugsHitLocation[0]]->SetBugState(BugBase::BugState_Shocked);
		GameManager::Instance()->m_BugsHitByLightning->push_back( (*bugs)[bugsHitLocation[0]] );
		GameManager::Instance()->AddBugHit( (*bugs)[bugsHitLocation[0]] );

		bugs->erase( bugs->begin()+bugsHitLocation[0] );
		this->CheckChainLength();

		if( GameManager::Instance()->m_BugsHitByLightning->size() > 0 )
		{
			m_remainingChainTime = GameManager::Instance()->m_MaxNextBugTime;
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
	DrawLightning( true );

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

