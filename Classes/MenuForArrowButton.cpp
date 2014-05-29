/*
 * MenuForArrowButton.cpp
 *
 *  Created on: May 14, 2014
 *      Author: mli
 */

#include "MenuForArrowButton.h"

MenuForArrowButton::MenuForArrowButton() {
	// TODO Auto-generated constructor stub

}

MenuForArrowButton::~MenuForArrowButton() {
	// TODO Auto-generated destructor stub
}

MenuForArrowButton * MenuForArrowButton::createWithArray(CCArray* pArrayOfItems){
	MenuForArrowButton *pRet = new MenuForArrowButton();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

bool MenuForArrowButton::ccTouchBegan(CCTouch* touch, CCEvent* event){
	bool ret = CCMenu::ccTouchBegan(touch , event);
	if(m_pSelectedItem && m_pSelectedItem->getTag() == PAUSE_PLAY) return ret;
    if (m_pSelectedItem)
    {
        m_pSelectedItem->activate();
    }
    return ret;
}

void MenuForArrowButton::ccTouchEnded(CCTouch *touch, CCEvent* event){
    CCMenu::ccTouchEnded(touch, event);
    if(m_pSelectedItem && m_pSelectedItem->getTag() == PAUSE_PLAY) return;
    if (m_pSelectedItem)
    {
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(m_pSelectedItem);
        }
    }
}

void MenuForArrowButton::ccTouchMoved(CCTouch *touch, CCEvent* event){
	CCMenu::ccTouchMoved(touch, event);
	if (m_pSelectedItem){
		m_pSelectedItem->activate();
	}
}

void MenuForArrowButton::registerTouchendHandler(CCObject *target, SEL_MenuHandler selector){
	m_pListener = target;
	m_pfnSelector = selector;
}

