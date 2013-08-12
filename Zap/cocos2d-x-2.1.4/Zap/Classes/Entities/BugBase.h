#ifndef __BUGBASE_H__
#define __BUGBASE_H__

#include "cocos2d.h"

class BugBase
{
	public:
		enum EBugState
		{
			BugState_Alive = 0,
			BugState_Shocked,
			BugState_Dead
		};

		BugBase( const char* spriteFileName );
		virtual ~BugBase();

		void Spawn();

		void SpawnNorth();
		void SpawnSouth();
		void SpawnWest();
		void SpawnEast();

		float toRadians( int const angle );

		void Update( float dt );

		bool CheckIfInBounds();

		void SetPosition( cocos2d::CCPoint point );
		void SetBugState( EBugState state );

		int GetPositionX();
		int GetPositionY();
		enum EBugState GetBugState();

		cocos2d::CCSprite* m_pSprite;
		EBugState m_State;
	private:
		BugBase();		/* we don't want to call this until we have a solution where we do not have
							the sprite name */
		const char* m_SpriteFileName;
		int m_travelAngle;
		float m_travelSpeed;

		int m_spawnOffset;

		int m_pointValue;
		int m_bugNumberInChain;
};

#endif /* __BUGBASE_H__ */
