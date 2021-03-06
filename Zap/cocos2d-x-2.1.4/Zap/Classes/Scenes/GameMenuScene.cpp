#include "GameMenuScene.h"
#include "GameWorldScene.h"
#include "HighScoreScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameMenu::scene()
{
	CCScene *scene = CCScene::create();

	GameMenu *layer = GameMenu::create();

	scene->addChild(layer);

	return scene;
}

bool GameMenu::init()
{
	if( !CCLayer::init() )
	{
		return false;
	}


	/* this is the close button at the bottom left of the screen */
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(GameMenu::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/* this is the header at the top of the screen */
	CCLabelTTF* pLabel = CCLabelTTF::create("Menu", "fonts/Roboto-Regular.ttf", 34);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp( size.width / 2, size.height - 20 ) );
	this->addChild(pLabel, 1);

	createMenu();

	return true;
}

/* call this only once when we create the layer */
void GameMenu::createMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 72 );
	CCMenuItemFont::setFontName( "fonts/Roboto-Regular.ttf" );

	CCMenuItemFont* pStartButton = CCMenuItemFont::create(
										"Start Game",
										this,
										menu_selector(GameMenu::menuStartGameCallback) );
	pStartButton->setPosition( ccp( 0, 0 ) );

	CCMenuItemFont* pHighScoreButton = CCMenuItemFont::create(
										"High Scores",
										this,
										menu_selector(GameMenu::menuHighScoreCallback) );
	pHighScoreButton->setPosition( ccp( 0, 0) );


	CCMenuItemFont* pExitButton = CCMenuItemFont::create(
										"Exit",
										this,
										menu_selector(GameMenu::menuExitGameCallback) );
	pExitButton->setPosition( ccp( 0, 0 ) );

	pGameMenu = CCMenu::create(pStartButton, pHighScoreButton, pExitButton, NULL);
	pGameMenu->alignItemsVertically();
	pGameMenu->setPosition( ccp( size.width/2, size.height/2 ) );
	this->addChild(pGameMenu, 1);
}

void GameMenu::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameMenu::menuStartGameCallback(CCObject* pSender)
{
	CCScene* pScene = GameWorld::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

void GameMenu::menuHighScoreCallback(CCObject* pSender)
{
	CCScene* pScene = HighScore::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, pScene));
}

void GameMenu::menuExitGameCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
