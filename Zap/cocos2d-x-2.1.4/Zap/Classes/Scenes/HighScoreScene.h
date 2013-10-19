#ifndef __HIGHSCORE_SCENE_H__
#define __HIGHSCORE_SCENE_H__

#include "cocos2d.h"
#include <vector>

class HighScore : public cocos2d::CCLayer
{
	public:
		virtual bool init();

		static cocos2d::CCScene* scene();

		void createMenu();

		void menuCloseCallback( CCObject* pSender );
		void menuGameMenuCallback( CCObject* pSender );

		CREATE_FUNC(HighScore);
	private:
		void PopulateHighScores();
		void ClearHighScores( std::vector<int>* pHighScores );
		void OutputHighScores( std::vector<int>* pHighScores );

		cocos2d::CCMenu* pGameMenu;

		int m_nMaxHighScores;
		int m_nHighScoreFontSize;
		int m_nHighScoreFontSpacing;

		char scoreBuf[128];

};

#endif /* __HIGHSCORE_SCENE_H__ */
