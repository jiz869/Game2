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
    virtual bool doScore();
    virtual void step(float dt);
    void increaseDuration(int delta);
    void resumeDuration();
    void changeGameTime(int delta);
    void changeScore(float delta , bool isGood);
    void changeControllerPosition(CheckboxType type);
    void pauseAndPlayHandler(CCObject * sender);
    void showGo();
    void showOver();
    virtual void showUp();
    void nextScene();
    virtual void gameOver();
    void hideMenu();

protected:
    CCSize winSize;

    CCLabelTTF* scoreLabel;
    CCSprite * gem;
    CCSprite * gemLevelup;

    MenuForArrowButton * menu;
    CCMenuItemImage * upButton , * downButton , * pauseAndPlay;

    virtual void initBloodBar();
    virtual void initScoreLabel();
    virtual void initMenu();
    virtual void initMisc();
    virtual void initLevelSplash();

    void updateGameTime();
    virtual void updateScore(bool isGood);

    long numFrame;
    float score;
    int initDuration;
    int durationIncrease;
    int maxDuration;
    int seconds;
    bool startUpdateTime;

    UserData * userData;
    PlaySceneData * playSceneData;

    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);

    GameStatus status;
    void pauseGame();
    void resumeGame();
    CCProgressTimer * bloodBar;

    CCSprite * goSplash;
    CCSprite * gameSplash;
    CCSprite * overSplash;
    CCSprite * levelSplash;
    CCSprite * upSplash;

    void levelUp();
};

#endif /* CONTROLMENU_H_ */
