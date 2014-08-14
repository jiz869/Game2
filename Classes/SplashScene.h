/*
 * SplashScene.h
 *
 *  Created on: 2014年7月1日
 *      Author: Administrator
 */

#ifndef SPLASHSCENE_H_
#define SPLASHSCENE_H_

#include "GameController.h"


class SplashScene: public cocos2d::CCLayer
{
public:
    SplashScene();
    virtual ~SplashScene();

    static cocos2d::CCScene* scene();
    CREATE_FUNC(SplashScene);
    virtual bool init();

    void splashOver();

    virtual void update(float dt);

private:
    void initProgressBar();
    CCSize winSize;
    CCProgressTimer * bloodBar;
    float progress;

    CCSprite * bar25;
    CCSprite * bar50;
    CCSprite * bar75;
    CCSprite * bar100;
};

#endif /* SPLASHSCENE_H_ */
