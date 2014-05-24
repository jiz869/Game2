/*
 * ControlMenu.h
 *
 *  Created on: May 23, 2014
 *      Author: mli
 */

#ifndef CONTROLMENU_H_
#define CONTROLMENU_H_

#include "MenuForArrowButton.h"
#include "GameController.h"

class ControlMenu: public cocos2d::CCLayerColor {
public:
	ControlMenu();
	virtual ~ControlMenu();
    CREATE_FUNC(ControlMenu);
    virtual bool init();
    bool isUpButtonSelected();
    void startNewGame();
    void doScore();
    void step(float dt);
    void increaseDuration(int delta);
    void resumeDuration();
    void changeTime(int delta);

private:
    CCSize winSize;
    CCLabelTTF* scoreLabel;
    CCLabelTTF* timeLabel;
    MenuForArrowButton * menu;
    CCMenuItemImage * upButton;

    void initTimeLabel();
    void initScoreLabel();

    void updateGameTime();
    void updateScore();

    void GameOver();

    long numFrame;
    int score;
    int duration;
    int seconds;
    bool startUpdateTime;

    UserData * userData;

    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);
};

#endif /* CONTROLMENU_H_ */
