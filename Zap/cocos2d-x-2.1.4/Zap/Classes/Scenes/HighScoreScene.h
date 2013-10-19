#ifndef __HIGHSCORE_SCENE_H__
#define __HIGHSCORE_SCENE_H__

#include "cocos2d.h"

class HighScore : public cocos2d::CCLayer
{
	public:
		virtual bool init();

		static cocos2d::CCScene* scene();

		void createMenu();

		void PopulateHighScores();

		void menuCloseCallback( CCObject* pSender );
		void menuGameMenuCallback( CCObject* pSender );

		CREATE_FUNC(HighScore);
	private:
		cocos2d::CCMenu* pGameMenu;

};

#endif /* __HIGHSCORE_SCENE_H__ */
