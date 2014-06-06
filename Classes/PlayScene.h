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

    virtual void slowAllLanes(float speed_decrease, float interval_increase);
    virtual void resumeAllLanesFromSlow(float speed_decrease, float interval_increase);

    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);

    ControlMenu * controlMenu;

protected:

    b2World * world;

    float ptmRatio;

    CCSize winSize;

    PlaySceneData * data;
    UserData * userData;

    virtual void initMisc();

    void initPlayer();

    void initControlMenu();

    void initBoundary();

    void initLanes();

    void processContact(float dt);

    void initCityObj();

    PlayerObj * player;
    GameObj * lowerBoundary , * upperBoundary;

    vector<Lane *> lanes;

    CCSprite * contact;
};

#endif /* defined(__crossRoadMain__PlayScene__) */
