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

class PlayScene : public cocos2d::CCLayer , public b2ContactListener
{
public:
    ~PlayScene();
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(PlayScene);
    
    virtual void update(float dt);
    
    virtual void BeginContact(b2Contact* contact);
    
private:
    
    b2World * world;
    
    float ptmRatio;
    
    CCSize winSize;
    
    PlaySceneData * data;
    
    CCSpriteBatchNode * batchNode;
    
    void initWorld();
    
    void initMisc();
    
    void initPlayer();
    
    void initMenu();
    
    void upHandler(CCObject * sender);
    void downHandler(CCObject * sender);
    void touchendHandler(CCObject * sender);
    
    PlayerObj player;
};

#endif /* defined(__crossRoadMain__PlayScene__) */
