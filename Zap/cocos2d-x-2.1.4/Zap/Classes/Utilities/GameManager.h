#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"
#include <vector>
#include "../Entities/BugBase.h"
#include "../Entities/BlueBug.h"
#include "../Entities/RedBug.h"
#include "../Entities/GreenBug.h"

class GameManager
{
	public:
		static GameManager* Instance();
		~GameManager();

		enum EBugType
		{
			BugType_None = 0,
			BugType_Blue,
			BugType_Red,
			BugType_Green
		};

		float m_GameDuration;
		float m_MaxNextBugTime;

		int m_MaxBlueChainLength;
		int m_MaxRedChainLength;
		int m_MaxGreenChainLength;

		int m_BaseBluePoints;
		int m_BaseRedPoints;
		int m_BaseGreenPoints;

		EBugType m_CurrentChainType;

		int m_Score;

		int m_BugsDeleted;
		int m_NumReachedOutOfBounds;

		float m_LightningRecalcInterval;

		virtual bool init();
		virtual void initData();

		void AddBugHit( BugBase* bug );
		const char* GetChainTypeString();

		void DestroyChain();
		void CalculateChainPoints();
		int CalculateBlueBugChain( int _numBugs, std::vector<BugBase*>* bugsHitByLightning );
		int CalculateRedBugChain( int _numBugs, std::vector<BugBase*>* bugsHitByLightning );
		int CalculateGreenBugChain( int _numBugs, std::vector<BugBase*>* bugsHitByLightning );
		void ApplyBlueBonus();
		void ApplyRedBonus();
		void ApplyGreenBonus();

		enum EBugType ReturnBugType( BugBase* bug );

		std::vector<BugBase*>* m_Bugs;
		std::vector<BugBase*>* m_BugsToDelete;
		std::vector<BugBase*>* m_BugsHitByLightning;

	private:
		GameManager() {};
		GameManager(GameManager const&) {};
		GameManager& operator=(GameManager const&) {};
		static GameManager* m_pInstance;
};

#endif /* __GAMEMANAGER_H__ */
