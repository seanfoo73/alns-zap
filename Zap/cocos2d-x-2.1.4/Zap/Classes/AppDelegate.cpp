#include "AppDelegate.h"

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Scenes/GameWorldScene.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/GameMenuScene.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
    /**
    if( m_GameWorldScene == NULL )
    {
    	m_GaveWorldScene = GameWorld::scene();
    }
    if( m_GameMenuScene == NULL )
    {
    	m_GameMenuScene = GameMenu::scene();
    }
    if( m_GameOverScene = NULL )
    {
    	m_GameOverScene = GameOver::scene();
    }
    /**/
    CCScene *pScene = GameMenu::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
