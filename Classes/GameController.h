//
//  GameController.h
//  crossRoad
//
//  Created by Min Li on 2014-05-10.
//
//

#ifndef __crossRoad__GameController__
#define __crossRoad__GameController__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define CCSTRING_FOR_KEY(dict , key)  ((CCString *)(dict)->objectForKey(key))
#define CCSTRING_AT_INDEX(array , index)  ((CCString *)(array)->objectAtIndex(index))

typedef struct{
    char carName[50];
    CCPoint initPos;
    CCPoint velocity;
    float height;
    float distance;
    bool left2right;
    float period;
}LaneDescription;

typedef struct{
    int laneNumber;
    float playerSpeed;
    vector<LaneDescription *> laneDescriptions;
}PlaySceneData;

typedef struct{
    CCAnimation * playerMoveAnim;
}AnimationData;

class GameController {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);
    AnimationData * getAnimationData();

private:
    GameController();
    ~GameController();
    virtual bool init();
    CCDictionary * dict;
    vector<PlaySceneData *> playSceneDatas;
    AnimationData animationData;
    bool initPlaySceneData(CCArray * dataArray);
    bool initAnimationData(CCDictionary * dataDict);
    CCAnimation * initAnimation(CCArray * frameNameArray);
    CCSize designSize;
};

#endif /* defined(__crossRoad__GameController__) */
