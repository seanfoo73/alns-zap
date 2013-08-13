#include "BugBase.h"

using namespace cocos2d;

BugBase::BugBase( const char* spriteFileName )
{
	m_SpriteFileName = spriteFileName;

	m_spawnOffset = 300;

	m_pSprite = cocos2d::CCSprite::createWithTexture(
			CCTextureCache::sharedTextureCache()->addImage( spriteFileName ),
			CCRectMake(0, 0, 64, 64) );
	//m_pSprite = cocos2d::CCSprite::create( spriteFileName, CCRectMake(0, 0, 64, 64) );

	Spawn();
	m_State = BugState_Alive;

	m_pointValue = 0;
	m_bugNumberInChain = -1;
}

BugBase::~BugBase()
{
	/* Always make sure you remove the sprite from the ccnode it belongs to or else you will
	 * have visual errors as well as memory leaks
	 */
	//m_pSprite->release();
	//m_pSprite = NULL;
}

/* This function should be called to generate information about the bug's spawn location and
 * initial travel direction
 */
void BugBase::Spawn()
{
	/* we need to choose which side we want to spawn the bug on:
	 * 	top, bottom, left, or right.  Based on this information, we need to generate an
	 * 	appropriate travel Angle
	 */
	int side = arc4random() % 4;

	if( side == 0 )
	{
		SpawnNorth();
	}
	else if( side == 1 )
	{
		SpawnSouth();
	}
	else if( side == 2 )
	{
		SpawnWest();
	}
	else if( side == 3 )
	{
		SpawnEast();
	}
	m_pSprite->setAnchorPoint( ccp( 0.5, 0.5 ) );

	/* for whatever reason, the "rotation" of the node is done clockwise in degrees. */
	m_pSprite->setRotation( 360 - m_travelAngle );

	m_travelSpeed = 4;

}

void BugBase::SpawnNorth()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pSprite->setPosition( ccp( 	arc4random() % ( static_cast<int>(size.width) ),
									static_cast<int>(size.height) ) );

	/* should be traveling south 180-360*/
	m_travelAngle = (arc4random() % 180) + 180;
}

void BugBase::SpawnSouth()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pSprite->setPosition( ccp( 	arc4random() % ( static_cast<int>(size.width) ),
									0 ) );

	/* should be traveling north 0-180 */
	m_travelAngle = arc4random() % 180;
}

void BugBase::SpawnWest()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pSprite->setPosition( ccp( 	0 ,
									arc4random() % ( static_cast<int>(size.height) ) ) );

	/* should be traveling east 270-90*/
	m_travelAngle = (arc4random() % 180) + 270;
	if( m_travelAngle  >= 360 )
	{
		m_travelAngle -= 360; // to keep it between 270 - 90
	}
}

void BugBase::SpawnEast()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pSprite->setPosition( ccp( 	static_cast<int>(size.width) ,
									arc4random() % ( static_cast<int>(size.height) ) ) );

	/* should be traveling west 90-270*/
	m_travelAngle = (arc4random() % 180) + 90;
}

void BugBase::Update( float dt )
{
	if( m_State == BugState_Alive )
	{
		m_pSprite->setPosition(
			ccp( 	m_pSprite->getPositionX() + m_travelSpeed * cos( toRadians( m_travelAngle ) ),
					m_pSprite->getPositionY() + m_travelSpeed * sin( toRadians( m_travelAngle ) ) ) );
	}
}

bool BugBase::CheckIfInBounds()
{

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	int x = m_pSprite->getPositionX();
	int y = m_pSprite->getPositionY();

	int widthMin = 0 - m_spawnOffset;
	int widthMax = size.width + m_spawnOffset;
	int heightMin = 0 - m_spawnOffset;
	int heightMax = size.height + m_spawnOffset;

	if( (x < heightMin || x > heightMax ) && ( y < widthMin || y > widthMax ) )
	{
		return false;
	}
	return true;
}

void BugBase::SetPosition( cocos2d::CCPoint point )
{
	if( m_pSprite )
	{
		m_pSprite->setPosition( point );
	}
}

void BugBase::SetBugState( EBugState state )
{
	m_State = state;
}

enum BugBase::EBugState BugBase::GetBugState()
{
	return m_State;
}

int BugBase::GetPositionX()
{
	return static_cast<int>(m_pSprite->getPositionX());
}

int BugBase::GetPositionY()
{
	return static_cast<int>(m_pSprite->getPositionY());
}

float BugBase::toRadians( int const angle )
{
	return angle * ( 3.14 / 180 );
}
