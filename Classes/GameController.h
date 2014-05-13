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

typedef struct{
    char carName[50];
    CCPoint initPos;
    CCPoint velocity;
    float height;
    float distance;
    bool left2right;
}LaneDescription;

typedef struct{
    int laneNumber;
    vector<LaneDescription *> laneDescriptions;
}PlaySceneData;

class GameController {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);

private:
    GameController();
    ~GameController();
    virtual bool init();
    CCDictionary * dict;
    vector<PlaySceneData *> playSceneDatas;
    bool initPlaySceneData(CCArray * dataArray);
    CCSize designSize;
};

#endif /* defined(__crossRoad__GameController__) */
