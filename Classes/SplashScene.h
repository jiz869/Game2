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

    void initGameController();

private:
    CCSprite * splash;
    UserData * data;
};

#endif /* SPLASHSCENE_H_ */
