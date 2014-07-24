//
//  PlayScene.h
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#ifndef __crossRoadMain__PlayScene__
#define __crossRoadMain__PlayScene__

#include "PlayObj.h"
#include "GameController.h"
#include "Lane.h"

class ControlMenu;

class PlayScene : public cocos2d::CCLayerColor , public b2ContactListener
{
public:
    PlayScene();
    ~PlayScene();

    virtual bool init();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(PlayScene);

    virtual void update(float dt);

    virtual void BeginContact(b2Contact* contact);

    virtual void stopAllLanes();
    virtual void freezeAllLanes();

    virtual void restartAllLanes();

    virtual void upHandler(int tag);
    virtual void downHandler(int tag);
    virtual void touchendHandler(int tag);

    virtual void destroyRandomCar();

    ControlMenu * controlMenu;

    virtual void keyBackClicked();

    void setSpecialChance(float chance , float length);
    void resetSpecialChance();

protected:

    b2World * world;

    float ptmRatio;

    CCSize winSize;

    PlaySceneData * data;
    UserData * userData;

    virtual void initMisc();

    virtual void initPlayer();

    virtual void initControlMenu();

    virtual void initBoundary();

    virtual void initLanes();

    virtual void processContact(float dt);

    virtual void initCityObj();

    PlayerObj * player;
    GameObj * lowerBoundary , * upperBoundary;

    vector<Lane *> lanes;

    CCSprite * contact;

    unsigned long getCurrentTime();
};

#endif /* defined(__crossRoadMain__PlayScene__) */
