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
    SLOW,
    LIFE,
    SPECIAL_NUM,
}SpecialID;

typedef enum{
    LEFT=0,
    RIGHT,
    SIDE,
    MUTE,
    UNMUTE,
    CHECKBOX_TYPE_NUM,
}CheckboxType;

typedef enum{
    NONE=0X1<<0,
    CONNECTED=0X1<<1,
    JOIN_ROOM1=0X1<<2,
    CREATE_ROOM=0X1<<3,
    JOIN_ROOM2=0X1<<4,
    SUBSCRIBE_ROOM=0X1<<5,
    PREPARE_PLAY=0X1<<6,
}PvpMode;

class PlayerObj;
class SpecialObj;

#define CCSTRING_FOR_KEY(dict , key)  ((CCString *)(dict)->objectForKey(key))
#define CCSTRING_AT_INDEX(array , index)  ((CCString *)(array)->objectAtIndex(index))

typedef void (* SpectialFunc)(PlayerObj * , SpecialObj *);
typedef bool (* SpectialHitByCarFunc)(PlayerObj * , SpecialObj *);

typedef struct{
    CCPoint initPos;
    float carSpeed;
    float height;
    bool left2right;
    float period;
    float specialChance;
    vector<int> carNumbers;
}LaneDescription;

typedef struct{
    int laneNumber;
    CCString * playerWaitImageName;
    CCAnimation * playerMoveAnim;
    float playerSpeed;
    float playerAccSpeed;
    float playerStopAccSpeed;
    vector<LaneDescription *> laneDescriptions;
}PlaySceneData;

typedef struct{
    CCString * backgroundSoundImage;
    CCString * resetSoundImage;
    CCString * scoreSoundImage;
    CCString * levelupSoundImage;
    vector<CCString *> hornSoundImages;
    CCAnimation * playerMoveAnim;
    CCAnimation * specialStopAnim;
    CCAnimation * specialHasteAnim;
    CCAnimation * specialSlowAnim;
    CCAnimation * specialLifeAnim;
    CCAnimation * clockAnim;
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
    SpectialHitByCarFunc hitByCar;
    CCAnimation * animation;
}SpecialData;

typedef struct{
    CheckboxType sound;
    CheckboxType controllerPosition;
	int initDuration;
    int durationIncrease;
	float topScore;
    int topLevel;
	float lastScore;
    int currentLevel;
    int maxDuration;
    vector<int> levels;
    int maxLevel;
    int order;
    int pvpInitDuration;
    int pvpMaxDuration;
    PvpMode pvpMode;
    bool justFailed;
    bool hasPayed;
}UserData;

class GameController {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);
    AnimationData * getAnimationData();
    SpecialData * getSpecialData(int speciaId);
    UserData * getUserData();
    void setUserData(const char * key , CheckboxType data , int value);
    void levelUp();
    void setLastScore(float lastScore);
    void setJustFailed(bool justFailed);
    void setHasPayed(bool justFailed);

private:
    GameController();
    ~GameController();
    virtual bool init();
    CCDictionary * dict;
    vector<PlaySceneData *> playSceneDatas;
    AnimationData animationData;
    UserData userData;
    bool initPlaySceneData(CCArray * dataArray);
    bool initAnimationData(CCDictionary * dataDict);
    bool initSpecialData(CCDictionary * dataDict);
    bool initUserData(CCDictionary * dataDict);
    CCAnimation * initAnimation(CCArray * frameNameArray);
    vector<SpecialData *> specialDatas;
    CCSize designSize;
    string plistWritablePath;
};

#endif /* defined(__crossRoad__GameController__) */
