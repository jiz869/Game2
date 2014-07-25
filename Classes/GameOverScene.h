/*
 * GameOverScene.h
 *
 *  Created on: Jul 4, 2014
 *      Author: mli
 */

#ifndef GAMEOVERSCENE_H_
#define GAMEOVERSCENE_H_

#include "GameController.h"

class GameOverScene: public cocos2d::CCLayer {
public:
	GameOverScene();
	virtual ~GameOverScene();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameOverScene);
    virtual bool init();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void keyBackClicked();
    void okHandler(CCObject * sender);
private:
    CCSize winSize;
    CCTextFieldTTF * nameField;
    CCSprite * OK;
    CCSprite * upload;
    CCSprite * mainMenu;
    CCMenuItemLabel * infoLabel;
    bool checkName(const char * name);
    void setInfoLabel(const char * info);
    UserData * userData;
    void initInfoMenu();
    void initMainMenu();
    CCMenu * infoMenu;
};

#endif /* GAMEOVERSCENE_H_ */
