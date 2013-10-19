#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include <vector>

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
		void PopulateHighScores();
		void ClearHighScores( std::vector<int>* pHighScores );
		void OutputHighScores( std::vector<int>* pHighScores );

		void DisplayCurrentScore();

		cocos2d::CCMenu* pGameMenu;

		int m_nMaxHighScores;
		int m_nHighScoreFontSize;
		int m_nHighScoreFontSpacing;

		char scoreBuf[128];
};

#endif /* __GAMEOVER_SCENE_H__ */
