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

typedef enum{
    STOP = 0,
    HASTE,
    SPECIAL_NUM,
}SpecialID;

class PlayerObj;
class SpecialObj;

#define CCSTRING_FOR_KEY(dict , key)  ((CCString *)(dict)->objectForKey(key))
#define CCSTRING_AT_INDEX(array , index)  ((CCString *)(array)->objectAtIndex(index))

typedef void (* SpectialFunc)(PlayerObj * , SpecialObj *);

typedef struct{
    char carName[50];
    CCPoint initPos;
    CCPoint velocity;
    float height;
    float distance;
    bool left2right;
    float period;
    float specialChance;
}LaneDescription;

typedef struct{
    int laneNumber;
    float playerSpeed;
    vector<LaneDescription *> laneDescriptions;
}PlaySceneData;

typedef struct{
    CCString * playerWaitImageName;
    CCAnimation * playerMoveAnim;
    CCAnimation * specialStopAnim;
    CCAnimation * specialHasteAnim;
}AnimationData;

typedef struct{
    float duration;
    CCString * imageName;
    float userData1;
    float userData2;
    float userData3;
    SpectialFunc begin;
    SpectialFunc step;
    SpectialFunc end;
}SpecialData;

class GameController {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);
    AnimationData * getAnimationData();
    SpecialData * getSpecialData(int speciaId);

private:
    GameController();
    ~GameController();
    virtual bool init();
    CCDictionary * dict;
    vector<PlaySceneData *> playSceneDatas;
    AnimationData animationData;
    bool initPlaySceneData(CCArray * dataArray);
    bool initAnimationData(CCDictionary * dataDict);
    bool initSpecialData(CCDictionary * dataDict);
    CCAnimation * initAnimation(CCArray * frameNameArray);
    vector<SpecialData *> specialDatas;
    CCSize designSize;
};

#endif /* defined(__crossRoad__GameController__) */
