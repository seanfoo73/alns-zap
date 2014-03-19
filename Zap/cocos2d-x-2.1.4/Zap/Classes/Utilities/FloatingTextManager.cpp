#include "FloatingTextManager.h"
#include "cocos2d.h" /* defines NULL */

using namespace cocos2d;

FloatingTextObject::FloatingTextObject()
{

}

FloatingTextObject::~FloatingTextObject()
{

}

FloatingTextManager::FloatingTextManager( CCLayer* _parent )
{
	m_pLayer = _parent;

	m_pFloatingText = new std::vector<FloatingTextObject*>();

	m_fMoveSpeed = 20;
	m_fAlphaSpeed = 2;
}

void FloatingTextManager::addFloatingText( const char* _txt, float _x, float _y, float _size, float _duration )
{
	if( !m_pLayer || !m_pFloatingText )
	{
		CCLOG("FLOATINGTEXTMANAGER::ADDFLOATINGTEXT:: does not have refernce to layer or floating text list deleted.");
		return;
	}

	CCLabelTTF* newLabel = CCLabelTTF::create( _txt, "fonts/Roboto-Regular.ttf", _size );

	FloatingTextObject* newTextObject = new FloatingTextObject();
	newTextObject->txt = _txt;
	newTextObject->size = _size;
	newTextObject->xLoc = _x;
	newTextObject->yLoc = _y;
	newTextObject->currentDuration = 0;
	newTextObject->maxDuration = _duration;
	newTextObject->label = newLabel;

	newTextObject->label->setPosition( ccp( _x, _y ) );

	m_pLayer->addChild( newLabel, 7 );
	m_pFloatingText->push_back( newTextObject );
}

void FloatingTextManager::update( float _dt )
{
	if( !m_pFloatingText || !m_pLayer )
	{
		CCLOG("FLOATINGTEXTMANAGER::UPDATE:: does not have refernce to layer or floating text list deleted.");
		return;
	}

	FloatingTextObject* obj;

	for(	std::vector<FloatingTextObject*>::iterator i = m_pFloatingText->begin();
			i != m_pFloatingText->end();
			//Handle iterator increment based on whether or not we remove
		)
	{
		obj = (*i);
        
        if( obj == NULL )
        {
            ++i;
            break;
        }
		obj->label->setPosition( ccp( 	obj->label->getPositionX(),
										obj->label->getPositionY() + ( m_fMoveSpeed * _dt ) ) );
		obj->label->setOpacity( obj->label->getOpacity()-m_fAlphaSpeed );
		obj->currentDuration += _dt;

		if( obj->currentDuration >= obj->maxDuration )
		{
			/* the label are autorelease objects */
			m_pLayer->removeChild( obj->label );
			i = m_pFloatingText->erase(i);
			delete obj;
		}
		else
		{
			++i;
		}
	}
}

FloatingTextManager::~FloatingTextManager()
{
	FloatingTextObject* obj;
	for(	std::vector<FloatingTextObject*>::iterator i = m_pFloatingText->begin();
			i != m_pFloatingText->end();
			//Handle iterator increment on remove
		)
	{
		obj = (*i);
		i = m_pFloatingText->erase(i);

		if( obj )
		{
			m_pLayer->removeChild( obj->label, true );
			delete obj;
		}
	}

	if( m_pFloatingText )
		delete m_pFloatingText;
}
