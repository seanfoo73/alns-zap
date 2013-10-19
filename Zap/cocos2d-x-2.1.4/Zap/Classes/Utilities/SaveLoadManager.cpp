#include "SaveLoadManager.h"
#include "cocos2d.h" /* defines NULL */

using namespace cocos2d;

SaveLoadManager* SaveLoadManager::m_pInstance = NULL;

SaveLoadManager* SaveLoadManager::Instance()
{
	if( !m_pInstance )
	{
		m_pInstance = new SaveLoadManager;
		m_pInstance->init();
	}

	return m_pInstance;
}

bool SaveLoadManager::init()
{
	return true;
}

void SaveLoadManager::setHighScores( std::vector<int>* pHighScores )
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("numHighScores", pHighScores->size());

	for( int i = 0; i < pHighScores->size(); ++i )
	{
		sprintf( keyBuf, "highScore_%d", i);
		const char* keyString = keyBuf;
		CCUserDefault::sharedUserDefault()->setIntegerForKey(keyString, (*pHighScores)[i]);
	}

	CCUserDefault::sharedUserDefault()->flush();
}

void SaveLoadManager::getHighScores( std::vector<int>* pHighScores )
{
	pHighScores->clear();

	int numHighScores = CCUserDefault::sharedUserDefault()->getIntegerForKey("numHighScores");

	for( int i = 0; i < numHighScores; ++i )
	{
		sprintf( keyBuf, "highScore_%d", i);
		const char* keyString = keyBuf;
		pHighScores->push_back( CCUserDefault::sharedUserDefault()->getIntegerForKey(keyString));
	}
}
