#include "LightningLine.h"

using namespace cocos2d;

LightningLine::LightningLine( CCPoint start, CCPoint end, int segment )
{
	m_baseLightningThickness = 40;
	m_thickness = m_baseLightningThickness / segment;

	m_start = ccp( start.x, start.y );
	m_end = ccp( end.x, end.y );
}
