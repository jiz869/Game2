/*
 * StartMenu.h
 *
 *  Created on: May 22, 2014
 *      Author: welcomelm
 */

#ifndef STARTMENU_H_
#define STARTMENU_H_

#include "GameController.h"

USING_NS_CC;

class StartMenu: public cocos2d::CCLayerColor {
public:
	StartMenu();
	virtual ~StartMenu();
    CREATE_FUNC(StartMenu);
    virtual bool init();
    void newGameHandler(CCObject * sender);
    void showStartMenu(float dt);
    void optionsHandler(CCObject * sender);
    void okHandler(CCObject * sender);
    void checkboxHandler(CCObject * sender);

private:
    CCMenu * startMenu;
    CCMenu * optionsMenu;
    CCSize winSize;
    void initMainMenu();
    void initOptionsMenu();
    CCMenuItemImage * checkboxLeft;
    CCMenuItemImage * checkboxRight;
    CCMenuItemImage * checkboxSide;
    CCMenuItemImage * checkboxMute;
    CCMenuItemImage * checkboxUnmute;
    UserData * userData;
    void checkboxChooser(CheckboxType tag);
    void saveUserData(CheckboxType tag);
};

#endif /* STARTMENU_H_ */
