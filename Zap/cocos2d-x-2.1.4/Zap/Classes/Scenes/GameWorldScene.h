#ifndef __GAMEWORLD_SCENE_H__
#define __GAMEWORLD_SCENE_H__

#include "cocos2d.h"
#include "../Entities/BugBase.h"
#include <vector>

class GameWorld : public cocos2d::CCLayer
{
	public:
		virtual bool init();

		static cocos2d::CCScene* scene();

		void menuCloseCallback(CCObject* pSender);

		void update( float _dt );
		void UpdateGameTime( float _dt );
		void UpdateGUI();
		void CheckBugsOutOfBounds();
		void CheckChainLength();
		void RemoveBugsFromWorld();

		virtual void draw();
		void DrawLightning();

		/* Lightning Draw calls, perhaps move this to its own class? */
		void DrawLightningLine( cocos2d::CCPoint start, cocos2d::CCPoint end, float thickness );
		void GenerateLightningPointsList( cocos2d::CCPoint start, cocos2d::CCPoint end );

		bool IsGameOver();
		void TransitionToGameOver();

		void checkBugsCollideWithPoint( cocos2d::CCPoint point );

		void spawnBug();

		virtual void registerWithTouchDispatcher();
		virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
		virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

		CREATE_FUNC(GameWorld);
	private:
		cocos2d::CCLabelTTF* m_pTimeLeftLabel;
		char timeLeftStringBuf[16];

		char testLabelStringBuf[64];
		cocos2d::CCLabelTTF* m_pTestLabel;

		float m_respawnTimer;
		float m_respawnInterval;

		float m_remainingGameTime;
		float m_remainingChainTime;

		int endCount;

		cocos2d::CCSpriteBatchNode* m_pLightningSegmentBatch;
		cocos2d::CCSpriteBatchNode* m_pLightningEndBatch;
};

#endif /* __GAMEWORLD_SCENE_H__ */
