#ifndef __GAMEOVER_SCENE_H__
#define _GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::CCLayer
{
	public:
		virtual bool init();

		static cocos2d::CCScene* scene();

		void createMenu();

		void menuCloseCallback(CCObject* pSender);
		void menuRestartGameCallback(CCObject* pSender);
		void menuExitGameCallback(CCObject* pSender);

		CREATE_FUNC(GameOver);
	private:
		cocos2d::CCMenu* pGameMenu;
};

#endif /* __GAMEOVER_SCENE_H__ */
