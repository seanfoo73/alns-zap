#include "LightningLine.h"

using namespace cocos2d;

LightningLine::LightningLine( CCPoint start, CCPoint end )
{
	m_start = ccp( start.x, start.y );
	m_end = ccp( end.x, end.y );
}
