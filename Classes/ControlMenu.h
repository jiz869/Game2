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

typedef enum{
	PAUSE = 0,
	PLAY,
    OVER,
    LEVEL_UP
}GameStatus;

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
    void changeGameTime(int delta);
    void changeControllerPosition(CheckboxType type);
    void pauseAndPlayHandler(CCObject * sender);
    void showGo();
    void showOver();
    void showUp();
    void nextScene();

private:
    CCSize winSize;

    CCLabelTTF* scoreLabel;
    CCSprite * gem;
    CCSprite * gemLevelup;

    MenuForArrowButton * menu;
    CCMenuItemImage * upButton , * downButton;

    void initBloodBar();
    void initScoreLabel();

    void updateGameTime();
    void updateScore();

    void GameOver();

    long numFrame;
    int score;
    int initDuration;
    int durationIncrease;
    int maxDuration;
    int seconds;
    bool startUpdateTime;

    UserData * userData;

    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);

    GameStatus status;
    void pauseGame();
    void resumeGame();
    CCProgressTimer * bloodBar;
    void initLevelSplash();

    CCSprite * goSplash;
    CCSprite * gameSplash;
    CCSprite * overSplash;
    CCSprite * levelSplash;
    CCSprite * upSplash;

    void levelUp();
};

#endif /* CONTROLMENU_H_ */
