/*
 * MenuForArrowButton.h
 *
 *  Created on: May 14, 2014
 *      Author: mli
 */

#ifndef MENUFORARROWBUTTON_H_
#define MENUFORARROWBUTTON_H_

#include "cocos2d.h"

typedef enum{
	UP = 0,
	DOWN,
	PAUSE_PLAY,
	CONTROL_BUTTON_MAX,
}ControlButtonType;

using namespace cocos2d;

class MenuForArrowButton: public cocos2d::CCMenu {
public:
	MenuForArrowButton();
	virtual ~MenuForArrowButton();

	static MenuForArrowButton * createWithArray(CCArray* pArrayOfItems);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch *touch, CCEvent* event);

	void registerTouchendHandler(CCObject *target, SEL_MenuHandler selector);

protected:
    CCObject*       m_pListener;
    SEL_MenuHandler    m_pfnSelector;
};

#endif /* MENUFORARROWBUTTON_H_ */
