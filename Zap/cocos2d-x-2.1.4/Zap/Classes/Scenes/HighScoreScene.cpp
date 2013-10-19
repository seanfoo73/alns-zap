#include "HighScoreScene.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"

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

	if( SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	return true;
}

void HighScore::createMenu()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemFont::setFontSize( 108 );
	CCMenuItemFont::setFontName( "fonts/Roboto-Regular.ttf" );

	CCMenuItemFont* pGameMenuButton = CCMenuItemFont::create(
										"Main Menu",
										this,
										menu_selector(HighScore::menuGameMenuCallback ) );
	pGameMenuButton->setPosition( ccp( 0, 0 ) );

	pGameMenu = CCMenu::create(pGameMenuButton, NULL);
	pGameMenu->alignItemsVertically();
	pGameMenu->setPosition( ccp( size.width/2, size.height/2 ) );
	this->addChild(pGameMenu, 1);
}

void HighScore::PopulateHighScores()
{

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
