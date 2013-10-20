#include "GameManager.h"
#include "../Scenes/GameWorldScene.h"
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
	m_GameDuration = 60.0f;
	m_MaxNextBugTime = 2.0f;

	m_MaxBlueChainLength = 10;
	m_MaxRedChainLength = 10;
	m_MaxGreenChainLength = 5;

	m_BaseBluePoints = 10;
	m_BaseRedPoints = 30;
	m_BaseGreenPoints = 20;

	m_LightningRecalcInterval = 0.5f;
}

void GameManager::setGameLayer( cocos2d::CCLayer* gameLayer )
{
	m_pGameLayer = gameLayer;
}

void GameManager::AddBugHit( BugBase* bug )
{
	if( dynamic_cast<BlueBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			m_CurrentChainType = BugType_Blue;
			bug->SetPointValue( m_BaseBluePoints*m_BugsHitByLightning->size() );
		}
		else if( m_CurrentChainType == BugType_Blue )
		{
			bug->SetPointValue( m_BaseBluePoints*m_BugsHitByLightning->size() );
		}
		else
		{
			bug->SetPointValue( m_BaseBluePoints );
			DestroyChain();
			m_CurrentChainType = BugType_None;
		}
	}
	else if( dynamic_cast<RedBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			m_CurrentChainType = BugType_Red;
			bug->SetPointValue( m_BaseRedPoints );
		}
		else if( m_CurrentChainType == BugType_Red )
		{
			bug->SetPointValue( m_BaseRedPoints );
		}
		else
		{
			bug->SetPointValue( m_BaseRedPoints );
			DestroyChain();
			m_CurrentChainType = BugType_None;
		}
	}
	else if( dynamic_cast<GreenBug*>(bug) )
	{
		if( m_CurrentChainType == BugType_None )
		{
			bug->SetPointValue( m_BaseGreenPoints );
			m_CurrentChainType = BugType_Green;
		}
		else if( m_CurrentChainType == BugType_Green )
		{
			bug->SetPointValue( m_BaseGreenPoints );
		}
		else
		{
			bug->SetPointValue( m_BaseGreenPoints );
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

		bugToDelete->SetBugState( BugBase::BugState_Dead_KILLED );
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
		m_Score += (*m_BugsHitByLightning)[i]->GetPointValue();
	}

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

void GameManager::ApplyBlueBonus()
{
	//Need to think of a bonus
	m_Score += 1000;
}

void GameManager::ApplyRedBonus()
{
	//score x2
	m_Score *= 2;
}

void GameManager::ApplyGreenBonus()
{
	//Grant extra time
	if( m_pGameLayer )
		static_cast<GameWorld*>(m_pGameLayer)->addGameTime( 5.0f );
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
