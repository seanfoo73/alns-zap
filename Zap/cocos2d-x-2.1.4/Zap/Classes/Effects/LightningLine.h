#ifndef __LIGHTNINGLINE_H__
#define __LIGHTNINGLINE_H__

#include "cocos2d.h"

class LightningLine
{
	public:
		LightningLine( cocos2d::CCPoint start, cocos2d::CCPoint end );

		cocos2d::CCPoint m_start;
		cocos2d::CCPoint m_end;
	private:
		LightningLine();
};

#endif /* __LIGHTNINGLINE_H__ */
