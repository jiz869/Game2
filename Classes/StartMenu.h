/*
 * StartMenu.h
 *
 *  Created on: May 22, 2014
 *      Author: welcomelm
 */

#ifndef STARTMENU_H_
#define STARTMENU_H_

#include "cocos2d.h"

USING_NS_CC;

class StartMenu: public cocos2d::CCLayerColor {
public:
	StartMenu();
	virtual ~StartMenu();
    CREATE_FUNC(StartMenu);
    virtual bool init();
    void newGameHandler(CCObject * sender);
    void showStartMenu(float dt);

private:
    CCMenu * startMenu;
    CCSize winSize;
};

#endif /* STARTMENU_H_ */
