#ifndef __FLOATINGTEXTMANAGER_H__
#define __FLOATINGTEXTMANGAER_H__

#include "cocos2d.h"

#include <vector>

class FloatingTextObject
{
	public:
		FloatingTextObject();
		virtual ~FloatingTextObject();

		const char* txt;
		float size;
		float maxDuration;
		float currentDuration;

		float xLoc;
		float yLoc;

		cocos2d::CCLabelTTF* label;
};

class FloatingTextManager
{
	public:
		FloatingTextManager( cocos2d::CCLayer* _parent );
		virtual ~FloatingTextManager();

		void addFloatingText( const char* _txt, float _x, float _y, float _size, float _duration );

		void update( float _dt );

	private:

		/* class stuff */
		cocos2d::CCLayer* m_pLayer;

		std::vector<FloatingTextObject*>* m_pFloatingText;

		float m_fMoveSpeed;
		float m_fAlphaSpeed;
};

#endif /* __FLOATINGTEXTMANAGER_H__ */
