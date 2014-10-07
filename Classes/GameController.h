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
class UserService;
class RewardService;

typedef enum{
    STR_CREDITS=0,
    STR_CONTROLLER,
    STR_LEFT,
    STR_RIGHT,
    STR_LEFTUP,
    STR_LEFTDOWN,
    STR_SOUND,
    STR_MUTE,
    STR_UNMUTE,
    STR_RANK,
    STR_MAX,
}strType;

typedef enum{
	STARTUP_MENU_SCENE=0,
	PLAY_SCENE,
	SPLASH_SCENE,
	GAME_OVER_SCENE,
}SceneTag;

//must be in the same order with game_controller.plist
typedef enum{
    STOP = 0,
    TIME,
    STRONG,
    LIFE,
    SKULL,
    DOUBLES,
    SMALL,
    BLESS,
    SPECIAL_NUM = BLESS,
    POLICE = SPECIAL_NUM + 1,
    SLOW,
    NOSCORE,
    CURSE,
    BOMB,
    ALLBAD,
    FAST,
    BAD_SPECIAL_NUM,
}SpecialID;

typedef enum{
    LEFT=0,
    RIGHT,
    SIDE_LEFT_UP,
    SIDE_LEFT_DOWN,
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
    int scoreIncrease;
    float specialInterval;
    int goodMax;
    int badMax;
    CCString * music;
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
    CCString * allBadSoundImage;
    CCString * smallSoundImage;
    CCString * fastSoundImage;
    CCString * noscoreSoundImage;
    CCString * gameOverSoundImage;
    CCString * gameStartSoundImage;
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

typedef struct {
    CCString * strInfo[STR_MAX];
}StrData;

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
    CCString * description;
    CCString * name;
}SpecialData;

typedef struct{
    CheckboxType sound;
    CheckboxType controllerPosition;
	int topScore;
	int topLevel;
    int lastLevel;
	int lastScore;
    int currentLevel;
    vector<int> levels;
    int maxLevel;
    int order;
    int pvpInitDuration;
    int pvpMaxDuration;
    PvpMode pvpMode;
    int justFailed;
    bool hasPayed;
    bool justWon;
    string userName;
    string password;
    int rank;
    bool isLogedIn;
    int lastUploadedScore;
    bool adsClicked;
}UserData;

typedef struct{
	string userName;
	int level;
	int score;
}ScoreRank;

#define LISTVIEW_LEADERBOARD

//#define AMAZON_BUILD
//#define BB_TRIAL
//#define CHINA
#define TRIAL_LEVEL 1

#ifdef LISTVIEW_LEADERBOARD
#define MAX_RANKS (50)
#else
#define MAX_RANKS (10)
#endif
#define RANK_PERPAGE (8)
#define LEGENDS_PERPAGE (8)

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define FONT "迷你简祥隶"
#define INFO_FONT "迷你简祥隶"
#else
#define FONT "fonts/迷你简祥隶.ttf"
#define INFO_FONT "fonts/迷你简祥隶.ttf"
#endif

#define FREE_PLAY (3)

#define NAME_RULE "Username must:\n" \
                  "have maximum 12 characters\n" \
                  "have minimum 4 characters\n" \
                  "start with an alphabet\n\n" \
                  "Legal characters allowed:\n" \
                  "a-z , A-Z, 0-9 , _ , -\n"

#define PASSWORD_RULE "Password must have:\n" \
                  "Maximum 12 characters\n" \
                  "Minimum 4 characters\n"
#if defined(BB_TRIAL) || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
#define PURCHASE_BENEFIT "Please spend 99 cents to purchase the\n" \
                  "full version to access higher levels\n"
#else
#define PURCHASE_BENEFIT "Spend 99 cents to:\n\n" \
                  "1) Remove Ads\n\n" \
                  "2) Continue game from where you left off\n"
#endif

#define DEFAULT_NAME "Penguin"
#define DEFAULT_PASSWORD "Penguin#"

extern bool isChineseEnabled;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "../proj.ios/P2PCPPHelper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../StartApp.android/AdsHelper.h"
class IAPManagerDelegate {
public:
    virtual void onPaymentError() = 0;
    virtual bool hasPayed() = 0;
    virtual void onPaymentSuccess() = 0;
};
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WP8
extern void purchase();
class IAPManagerDelegate {
public:
    virtual void onPaymentError() = 0;
    virtual bool hasPayed() = 0;
    virtual void onPaymentSuccess() = 0;
};
#else
class IAPManagerDelegate {
public:
    virtual void onPaymentError() = 0;
    virtual bool hasPayed() = 0;
    virtual void onPaymentSuccess() = 0;
};
#endif

class GameController : public CCObject , public IAPManagerDelegate {

public:
    static GameController * getGameController();
    PlaySceneData * getPlaySceneData(int level);
    AnimationData * getAnimationData();
    SpecialData * getSpecialData(int speciaId);
    StrData * getStrData();
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
    void onAuthenticateCompleted(CCNode * node , void * response);
    void onGetRewardsCompleted(CCNode * node , void * response);
    ScoreRank ranks[MAX_RANKS];
    void getTopRankings();
    void authenticate(const char * userName , const char * passwd);
    void createUser(const char * userName , const char * passwd);
    void uploadLastScore();
    void saveUser(const char * userName , const char * password);
    void recoverSpecialDurations();
    virtual void onPaymentError();
    virtual void onPaymentSuccess();
    virtual void onPaymentLogin();
    virtual bool hasPayed();
    void getAdsId();
    void onAdsClicked();
    void setAdsClicked(bool);

private:
    GameController();
    ~GameController();
    virtual bool init();
    CCDictionary * dict;
    vector<PlaySceneData *> playSceneDatas;
    AnimationData animationData;
    UserData userData;
    StrData strData;
    bool initMisc();
    bool initString(CCDictionary * dataDict);
    bool initPlaySceneData(CCArray * dataArray);
    bool initAnimationData(CCDictionary * dataDict);
    bool initSpecialData(CCDictionary * dataDict);
    bool initUserData(CCDictionary * dataDict);
    void updateVersion();
    CCAnimation * initAnimation(CCArray * frameNameArray);
    CCAnimation * initAnimation(CCDictionary * animDict);
    vector<SpecialData *> specialDatas;
    CCSize designSize;
    string plistWritablePath;
    void initLeaderboard();
    GameService *gameService;
    UserService *userService;
    RewardService * rewardService;
    ScoreBoardService *scoreBoardService;
    int getLevelByScore(int score);
    void setInfoLabel(const char * info);
    int getSpecialIdForKey(CCString * key);
};

#endif /* defined(__crossRoad__GameController__) */
