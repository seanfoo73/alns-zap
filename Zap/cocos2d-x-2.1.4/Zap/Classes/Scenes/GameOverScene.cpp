#include "GameOverScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"

#include <vector>
#include "../Utilities/SaveLoadManager.h"
#include "../Utilities/GameManager.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameOver::scene()
{
	CCScene *scene = CCScene::create();

	GameOver *layer = GameOver::create();

	scene->addChild(layer);

	return scene;
}

bool GameOver::init()
{
	if( !CCLayer::init() )
	{
		return false;
	}

	m_nMaxHighScores = 5;
	m_nHighScoreFontSize = 40;
	m_nHighScoreFontSpacing = 20;

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(GameOver::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Game Over", "fonts/Roboto-Regular.ttf", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 1);

	createMenu();
	PopulateHighScores();
	DisplayCurrentScore();

	if( SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	return true;
}

void GameOver::createMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 72 );
	CCMenuItemFont::setFontName( "fonts/Roboto-Regular.ttf" );

	CCMenuItemFont* pRestartButton = CCMenuItemFont::create(
										"Restart",
										this,
										menu_selector(GameOver::menuRestartGameCallback ) );
	pRestartButton->setPosition( ccp( 0, 0 ) );

	CCMenuItemFont* pExitButton = CCMenuItemFont::create(
										"Exit",
										this,
										menu_selector(GameOver::menuExitGameCallback ) );
	pExitButton->setPosition( ccp( 0, 0 ) );

	pGameMenu = CCMenu::create(pRestartButton, pExitButton, NULL);
	pGameMenu->alignItemsVertically();
	pGameMenu->setPosition( ccp( size.width/2, 160 ) );
	this->addChild(pGameMenu, 1);
}


void GameOver::PopulateHighScores()
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

void GameOver::ClearHighScores( std::vector<int>* pHighScores )
{
	pHighScores->clear();

	for( int i = 0; i < m_nMaxHighScores; ++i )
	{
		pHighScores->push_back( 0 );
	}
}

void GameOver::OutputHighScores( std::vector<int>* pHighScores )
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
	}
}

void GameOver::DisplayCurrentScore()
{
	sprintf(scoreBuf, "%d", GameManager::Instance()->m_Score);
	const char* scoreString = scoreBuf;

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCLabelTTF* pCurrentScoreLabel = CCLabelTTF::create("Score:", "fonts/Roboto-Regular.ttf", m_nHighScoreFontSize );
	pCurrentScoreLabel->setPosition( ccp( 	size.width / 8,
											size.height - size.height / 4 + (m_nHighScoreFontSize+m_nHighScoreFontSpacing )));
	this->addChild( pCurrentScoreLabel, 1);

	CCLabelTTF* pScore = CCLabelTTF::create(scoreString, "fonts/Roboto-Regular.ttf", m_nHighScoreFontSize );
	pScore->setPosition( ccp( 	size.width / 8,
								size.height - size.height / 4 ) );
	this->addChild( pScore, 1);
}

void GameOver::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameOver::menuRestartGameCallback(CCObject* pSender)
{
	CCScene* pScene = GameMenu::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

void GameOver::menuExitGameCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
