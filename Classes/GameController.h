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

class GameService;
class ScoreBoardService;

typedef enum{
	STARTUP_MENU_SCENE=0,
	PLAY_SCENE,
	SPLASH_SCENE,
	GAME_OVER_SCENE,
}SceneTag;

typedef enum{
    STOP = 0,
    STRONG,
    LIFE,
    TIME,
    SKULL,
    SCORE,
    BLESS,
    SPECIAL_NUM,
    HASTE = SPECIAL_NUM,
    POLICE = SPECIAL_NUM + 1,
    SLOW,
    CURSE,
    BOMB,
    BAD_SPECIAL_NUM,
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

typedef enum{
	PLAYER_MOVE=0,
	PLAYER_BOMB,
}ActionTag;

class PlayerObj;
class SpecialObj;

#define CCSTRING_FOR_KEY(dict , key)  ((CCString *)(dict)->objectForKey(key))
#define CCSTRING_AT_INDEX(array , index)  ((CCString *)(array)->objectAtIndex(index))

typedef void (* SpectialFunc)(PlayerObj *);
typedef void (* SpectialStepFunc)(PlayerObj * , float timer_count);
typedef bool (* SpectialHitByCarFunc)(PlayerObj * , CCSprite * car);

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
	int initDuration;
    int maxDuration;
    int durationIncrease;
    float specialInterval;
    vector<LaneDescription *> laneDescriptions;
}PlaySceneData;

typedef struct{
    CCString * backgroundSoundImage;
    CCString * resetSoundImage;
    CCString * scoreSoundImage;
    CCString * levelupSoundImage;
    CCString * stopSoundImage;
    CCString * skullSoundImage;
    CCString * lifeSoundImage;
    CCString * timeSoundImage;
    CCString * strongSoundImage;
    CCString * blessSoundImage;
    CCString * policeSoundImage;
    CCString * slowSoundImage;
    CCString * curseSoundImage;
    CCString * bombSoundImage;
    CCString * scoreSpecialSoundImage;
    vector<CCString *> hornSoundImages;
    CCAnimation * playerMoveAnim;
    CCAnimation * specialStopAnim;
    CCAnimation * specialHasteAnim;
    CCAnimation * specialStrongAnim;
    CCAnimation * specialLifeAnim;
    CCAnimation * specialTimeAnim;
    CCAnimation * specialSkullAnim;
    CCAnimation * specialScoreAnim;
    CCAnimation * specialPoliceAnim;
    CCAnimation * explodeAnim;
}AnimationData;

typedef struct{
    float duration;
    float life;
    CCString * imageName;
    float userData1;
    float userData2;
    float userData3;
    SpectialFunc begin;
    SpectialStepFunc step;
    SpectialFunc end;
    SpectialHitByCarFunc hitByCar;
    CCAnimation * animation;
}SpecialData;

typedef struct{
    CheckboxType sound;
    CheckboxType controllerPosition;
	int topScore;
    int topLevel;
	int lastScore;
    int currentLevel;
    vector<int> levels;
    int maxLevel;
    int order;
    int pvpInitDuration;
    int pvpMaxDuration;
    PvpMode pvpMode;
    bool justFailed;
    bool hasPayed;
    bool justWon;
    string userName;
    string rank;
}UserData;

typedef struct{
	string userName;
	int level;
	int score;
}ScoreRank;

#define LISTVIEW_LEADERBOARD

#ifdef LISTVIEW_LEADERBOARD
#define MAX_RANKS (50)
#else
#define MAX_RANKS (10)
#endif
#define RANK_PERPAGE (8)

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define FONT "CroissantD"
#define INFO_FONT "Arial"
#else
#define FONT "fonts/CroissantD.ttf"
#define INFO_FONT "fonts/Arial.ttf"
#endif

class GameController : CCObject {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);
    AnimationData * getAnimationData();
    SpecialData * getSpecialData(int speciaId);
    UserData * getUserData();
    void setUserData(const char * key , CheckboxType data , int value);
    void levelUp();
    void setLastScore(int lastScore , bool justFailed);
    void setJustFailed(bool justFailed , bool updatePlist);
    void setHasPayed(bool justFailed);
    void onGameRequestCompleted(CCNode * node , void * response);
    void onScoreBoardSaveCompleted(CCNode * node , void * response);
    void onScoreBoardGetCompleted(CCNode * node , void * response);
    void onScoreBoardGetUserRankingCompleted(CCNode * node , void * response);
    ScoreRank ranks[MAX_RANKS];
    void getTopRankings();
    void saveLastScore();
    void saveUserName(const char * userName);

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
    CCAnimation * initAnimation(CCDictionary * animDict);
    vector<SpecialData *> specialDatas;
    CCSize designSize;
    string plistWritablePath;
    void initLeaderboard();
    GameService *gameService;
    ScoreBoardService *scoreBoardService;
    int getLevelByScore(int score);
};

#endif /* defined(__crossRoad__GameController__) */
