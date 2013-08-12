#include "GameOverScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"

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

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(GameOver::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Game Over", "Thonburi", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 1);

	createMenu();

	if( SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	return true;
}

void GameOver::createMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 108 );
	CCMenuItemFont::setFontName( "Thonburi" );

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
	pGameMenu->setPosition( ccp( size.width/2, size.height/2 ) );
	this->addChild(pGameMenu, 1);
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
