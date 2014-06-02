//
//  StartMenuScene.h
//  crossRoad
//
//  Created by Min Li on 2014-05-30.
//
//

#ifndef __crossRoad__StartMenuScene__
#define __crossRoad__StartMenuScene__

#include "GameController.h"

USING_NS_CC;

class StartMenuScene: public cocos2d::CCLayerColor {
public:
	StartMenuScene();
	virtual ~StartMenuScene();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StartMenuScene);
    virtual bool init();
    void newGameHandler(CCObject * sender);
    void optionsHandler(CCObject * sender);
    void okHandler(CCObject * sender);
    void checkboxHandler(CCObject * sender);
    void changeSoundSetting(CheckboxType type);
    void scoreHandler(CCObject * sender);

private:
    CCMenu * startMenu;
    CCMenu * optionsMenu;
    CCMenu * scoreMenu;
    CCSize winSize;
    void initMainMenu();
    void initOptionsMenu();
    void initScoreMenu();
    CCMenuItemImage * checkboxLeft;
    CCMenuItemImage * checkboxRight;
    CCMenuItemImage * checkboxSide;
    CCMenuItemImage * checkboxMute;
    CCMenuItemImage * checkboxUnmute;
    UserData * userData;
    void checkboxChooser(CheckboxType tag);
    void saveUserData(CheckboxType tag);

    CCMenuItemImage * gem;
    CCMenuItemLabel * scoreLabel;
};

#endif /* defined(__crossRoad__StartMenuScene__) */
