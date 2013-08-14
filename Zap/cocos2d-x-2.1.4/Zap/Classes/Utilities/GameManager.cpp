#include "GameManager.h"
#include "cocos2d.h" /* defines NULL */

GameManager* GameManager::m_pInstance = NULL;

GameManager* GameManager::Instance()
{
	if( !m_pInstance )
	{
		m_pInstance = new GameManager;
		m_pInstance->init();
	}

	return m_pInstance;
}

bool GameManager::init()
{
	m_CurrentChainType = BugType_None;
	m_BugsDeleted = 0;
	m_NumReachedOutOfBounds = 0;
	m_Score = 0;

	this->initData();

	m_Bugs = new std::vector<BugBase*>();
	m_BugsToDelete = new std::vector<BugBase*>();
	m_BugsHitByLightning = new std::vector<BugBase*>();

	return true;
}

void GameManager::initData()
{
	m_GameDuration = 30.0f;
	m_MaxNextBugTime = 2.0f;

	m_MaxBlueChainLength = 10;
	m_MaxRedChainLength = 10;
	m_MaxGreenChainLength = 5;

	m_BaseBluePoints = 10;
	m_BaseRedPoints = 30;
	m_BaseGreenPoints = 20;

	m_LightningRecalcInterval = 0.5f;
}

void GameManager::AddBugHit( BugBase* bug )
{
	if( dynamic_cast<BlueBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			m_CurrentChainType = BugType_Blue;
		}
		else if( m_CurrentChainType == BugType_Blue )
		{
			//Do nothing
		}
		else
		{
			DestroyChain();
			m_CurrentChainType = BugType_None;
		}
	}
	else if( dynamic_cast<RedBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			m_CurrentChainType = BugType_Red;
		}
		else if( m_CurrentChainType == BugType_Red )
		{
			//Do Nothing
		}
		else
		{
			DestroyChain();
			m_CurrentChainType = BugType_None;
		}
	}
	else if( dynamic_cast<GreenBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			m_CurrentChainType = BugType_Green;
		}
		else if( m_CurrentChainType == BugType_Green )
		{
			//Do Nothing
		}
		else
		{
			DestroyChain();
			m_CurrentChainType = BugType_None;
		}
	}
}

const char* GameManager::GetChainTypeString()
{
	if( m_CurrentChainType == BugType_Blue )
	{
		return "BugType_Blue";
	}
	else if( m_CurrentChainType == BugType_Red )
	{
		return "BugType_Red";
	}
	else if( m_CurrentChainType == BugType_Green )
	{
		return "BugType_Green";
	}
	else
	{
		return "BugType_None";
	}
}

void GameManager::DestroyChain()
{
	int numToDestroy = m_BugsHitByLightning->size();
	BugBase* bugToDelete;

	CalculateChainPoints();

	for( int i = 0; i < numToDestroy; i++ )
	{
		bugToDelete = (*m_BugsHitByLightning)[0];
		m_BugsHitByLightning->erase(m_BugsHitByLightning->begin());

		bugToDelete->SetBugState( BugBase::BugState_Dead );
		m_BugsToDelete->push_back( bugToDelete );
	}
	m_CurrentChainType = BugType_None;
}

void GameManager::CalculateChainPoints()
{
	int numBlueBugs = 0;
	int numRedBugs = 0;
	int numGreenBugs = 0;

	for( int i = 0; i < m_BugsHitByLightning->size(); i++ )
	{
		if( ReturnBugType( (*m_BugsHitByLightning)[i] ) == BugType_Blue )
		{
			numBlueBugs++;
		}
		else if( ReturnBugType( (*m_BugsHitByLightning)[i] ) == BugType_Red )
		{
			numRedBugs++;
		}
		else if( ReturnBugType( (*m_BugsHitByLightning)[i] ) == BugType_Green )
		{
			numGreenBugs++;
		}
	}

	m_Score += CalculateBlueBugChain ( numBlueBugs );
	m_Score += CalculateRedBugChain( numRedBugs );
	m_Score += CalculateGreenBugChain( numGreenBugs );

	/**/
	if( numBlueBugs >= m_MaxBlueChainLength )
	{
		ApplyBlueBonus();
	}
	else if( numRedBugs >= m_MaxRedChainLength )
	{
		ApplyRedBonus();
	}
	else if( numGreenBugs >= m_MaxGreenChainLength )
	{
		ApplyGreenBonus();
	}
	/**/
}

int GameManager::CalculateBlueBugChain( int _numBugs )
{
	int pointsEarned = 0;

	for( int i = 1; i <= _numBugs; i++ )
	{
		pointsEarned += m_BaseBluePoints * i;
	}

	return pointsEarned;
}

int GameManager::CalculateRedBugChain( int _numBugs )
{
	int pointsEarned = 0;

	for( int i = 0; i < _numBugs; i++ )
	{
		pointsEarned += m_BaseRedPoints;
	}

	return pointsEarned;
}

int GameManager::CalculateGreenBugChain( int _numBugs )
{
	int pointsEarned = 0;

	for( int i = 0; i < _numBugs; i++ )
	{
		pointsEarned += m_BaseGreenPoints;
	}

	return pointsEarned;
}

void GameManager::ApplyBlueBonus()
{
	//Need to think of a bonus
}

void GameManager::ApplyRedBonus()
{
	//score x2
	m_Score *= 2;
}

void GameManager::ApplyGreenBonus()
{
	//Grant extra time
}

enum GameManager::EBugType GameManager::ReturnBugType( BugBase* bug )
{
	if( dynamic_cast<BlueBug*>(bug) )
	{
		return BugType_Blue;
	}
	else if( dynamic_cast<RedBug*>(bug) )
	{
		return BugType_Red;
	}
	else if( dynamic_cast<GreenBug*>(bug) )
	{
		return BugType_Green;
	}
	else
	{
		return BugType_None;
	}
}

GameManager::~GameManager()
{
	delete m_Bugs;
	delete m_BugsToDelete;
	delete m_BugsHitByLightning;
}
