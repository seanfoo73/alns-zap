#ifndef __GAMEMENU_SCENE_H__
#define __GAMEMENU_SCENE_H__

#include "cocos2d.h"

class GameMenu : public cocos2d::CCLayer
{
	public:
		virtual bool init();

		static cocos2d::CCScene* scene();

		void createMenu();

		void menuCloseCallback(CCObject* pSender);
		void menuStartGameCallback(CCObject* pSender);
		void menuHighScoreCallback(CCObject* pSender);
		void menuExitGameCallback(CCObject* pSender);

		CREATE_FUNC(GameMenu);
	private:
		cocos2d::CCMenu* pGameMenu;
};

#endif /* __GAMEMENU_SCENE_H__ */
