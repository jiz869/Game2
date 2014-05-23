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
    ~PlayScene();

    virtual bool init();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(PlayScene);

    virtual void update(float dt);

    virtual void BeginContact(b2Contact* contact);

    virtual void stopAllLanes();

    virtual void restartAllLanes();

    virtual void slowAllLanes(float speed_decrease, float interval_increase);
    virtual void resumeAllLanesFromSlow(float speed_decrease, float interval_increase);

    virtual bool isUpButtonSelected();
    void startNewGame();

    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);

private:

    b2World * world;

    float ptmRatio;

    CCSize winSize;

    PlaySceneData * data;
    UserData * userData;

    void initMisc();

    void initPlayer();

    void initControlMenu();

    void initBoundary();

    void initLanes();

    void initStartMenu();

    void processContact(float dt);

    PlayerObj * player;
    GameObj * lowerBoundary , * upperBoundary;

    vector<Lane *> lanes;

    CCSprite * contact;

    ControlMenu * controlMenu;
};

#endif /* defined(__crossRoadMain__PlayScene__) */
