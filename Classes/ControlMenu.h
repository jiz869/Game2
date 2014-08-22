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
    void increaseDuration(float delta);
    void resumeDuration();
    void changeScore(int delta , bool isGood);
    void changeControllerPosition(CheckboxType type);
    void pauseAndPlayHandler(CCObject * sender);
    void showGo();
    void showOver();
    virtual void showUp();
    void nextScene();
    virtual void gameOver();
    void hideMenu();
    void stopTime();
    void resumeTime();
    void changeDurationIncrease(float delta);
    void resumeDurationIncrease();
    void changeScoreIncrease(int scoreIncrease);
    void resumeScore();

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
    int score;
    float initDuration;
    float durationIncrease;
    float maxDuration;
    float seconds;
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

    CCSprite * bar25;
    CCSprite * bar50;
    CCSprite * bar75;
    CCSprite * bar100;

    CCLabelTTF *  goSplash;
    CCLabelTTF *  gameSplash;
    CCLabelTTF *  overSplash;
    CCLabelTTF *  levelSplash;
    CCLabelTTF *  upSplash;

    void levelUp();
    int scoreIncrease;
};

#endif /* CONTROLMENU_H_ */
