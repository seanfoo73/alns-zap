#include "HighScoreScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"
#include "../Utilities/SaveLoadManager.h"
#include <vector>

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HighScore::scene()
{
	CCScene* scene = CCScene::create();

	HighScore* layer = HighScore::create();

	scene->addChild( layer );

	return scene;
}

bool HighScore::init()
{
	if( !CCLayer::init() )
	{
		return false;
	}

	m_nMaxHighScores = 5;
	m_nHighScoreFontSize = 40;
	m_nHighScoreFontSpacing = 20;

	m_HighScoreLabels.clear();

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HighScore::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("High Scores", "fonts/Roboto-Regular.ttf", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 1);

	createMenu();
	createFunctionsMenu();
	PopulateHighScores();

	if( SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	return true;
}

void HighScore::createMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 72 );
	CCMenuItemFont::setFontName( "fonts/Roboto-Regular.ttf" );

	CCMenuItemFont* pGameMenuButton = CCMenuItemFont::create(
										"Main Menu",
										this,
										menu_selector(HighScore::menuGameMenuCallback ) );
	pGameMenuButton->setPosition( ccp( 0, 0 ) );

	pGameMenu = CCMenu::create(pGameMenuButton, NULL);
	pGameMenu->alignItemsVertically();
	pGameMenu->setPosition( ccp( size.width/2, 80 ) );
	this->addChild(pGameMenu, 1);
}

void HighScore::createFunctionsMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 48 );
	CCMenuItemFont::setFontName( "fonts/Roboto-Regular.ttf" );

	CCMenuItemFont* pClearHighScoresButton = CCMenuItemFont::create(
													"Clear Scores",
													this,
													menu_selector(HighScore::clearHighScoresCallback ) );
	pClearHighScoresButton->setPosition( ccp( 0, 0 ) );

	pFunctionsMenu = CCMenu::create(pClearHighScoresButton, NULL);
	pFunctionsMenu->alignItemsVertically();
	pFunctionsMenu->setPosition( ccp( (size.width/4),  size.height/2 ) );
	this->addChild( pFunctionsMenu, 1);
}

void HighScore::PopulateHighScores()
{
	std::vector<int>* pHighScores = new std::vector<int>();

	SaveLoadManager::Instance()->getHighScores( pHighScores );

	if( pHighScores->size() == 0 )
	{
		ClearHighScores( pHighScores );
		OutputHighScores( pHighScores );
		SaveLoadManager::Instance()->setHighScores( pHighScores );
	}
	else
	{
		OutputHighScores( pHighScores );
	}

	delete pHighScores;
}

void HighScore::ClearHighScores( std::vector<int>* pHighScores )
{
	pHighScores->clear();

	for( int i = 0; i < m_nMaxHighScores; ++i )
	{
		pHighScores->push_back( 0 );
	}
}

void HighScore::OutputHighScores( std::vector<int>* pHighScores )
{
	for( int i = 0; i < pHighScores->size(); ++i )
	{
		sprintf(scoreBuf, "%d. %d", i+1, (*pHighScores)[i]);
		const char* scoreString = scoreBuf;

		CCLabelTTF* pScore = CCLabelTTF::create(scoreString, "fonts/Roboto-Regular.ttf", m_nHighScoreFontSize );
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pScore->setPosition( ccp( 	size.width / 2,
									size.height - size.height/4 - ((m_nHighScoreFontSize+m_nHighScoreFontSpacing)*i) ) );
		this->addChild( pScore, 1 );
		m_HighScoreLabels.push_back( pScore );
	}
}

void HighScore::clearHighScoresCallback( CCObject* pSender )
{
	std::vector<int>* pHighScores = new std::vector<int>();

	ClearHighScores( pHighScores );
	SaveLoadManager::Instance()->setHighScores( pHighScores );

	for( 	std::vector<CCLabelTTF*>::iterator i = m_HighScoreLabels.begin();
			i != m_HighScoreLabels.end();
			)
	{
		this->removeChild((*i), true );
		i = m_HighScoreLabels.erase(i);
	}

	OutputHighScores( pHighScores );

	delete pHighScores;
}

void HighScore::menuCloseCallback( CCObject* pSender )
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HighScore::menuGameMenuCallback( CCObject* pSender )
{
	CCScene* pScene = GameMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}
