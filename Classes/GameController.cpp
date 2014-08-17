//
//  GameController.cpp
//  crossRoad
//
//  Created by Min Li on 2014-05-10.
//
//

#include "GameController.h"
#include "SpecialObj.h"
#include "PlayObj.h"
#include "PlayScene.h"
#include "ControlMenu.h"
#include "SimpleAudioEngine.h"
#include "App42API.h"
#include "app42base64.h"
#include "StartMenuScene.h"
#include "GameOverScene.h"
#include "city.h"

using namespace CocosDenshion;

void static stopBegin(PlayerObj * player);
void static stopEnd(PlayerObj * player);

void static strongBegin(PlayerObj * player);
bool static strongHitByCar(PlayerObj * player, CCSprite * car);

void static lifeBegin(PlayerObj * player);

void static timeBegin(PlayerObj * player);
void static timeEnd(PlayerObj * player);

void static skullBegin(PlayerObj * player);

void static policeBegin(PlayerObj * player);
void static policeEnd(PlayerObj * player);

void static slowBegin(PlayerObj * player);
void static slowEnd(PlayerObj * player);

void static curseBegin(PlayerObj * player);
void static curseEnd(PlayerObj * player);

void static bombBegin(PlayerObj * player);
void static bombStep(PlayerObj * player , float timer_count);
void static bombEnd(PlayerObj * player);

void static blessBegin(PlayerObj * player);
bool static blessHitByCar(PlayerObj * player, CCSprite * car);
void static blessEnd(PlayerObj * player);

void static allBadBegin(PlayerObj * player);
void static allBadEnd(PlayerObj * player);

void static doubleBegin(PlayerObj * player);

void static smallBegin(PlayerObj * player);
void static smallEnd(PlayerObj * player);

void static fastBegin(PlayerObj * player);
void static fastEnd(PlayerObj * player);

char * userDataValue[CHECKBOX_TYPE_NUM];

static GameController * controller;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void onAdClicked(){
    GameController::getGameController()->setJustFailed(false , true);
}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
extern "C"
{
    void Java_ca_welcomelm_crossRoad_crossRoad_onAdClicked( JNIEnv* env, jobject thiz ){
        GameController::getGameController()->setJustFailed(false , true);
    }

    void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentError( JNIEnv* env, jobject thiz ){
        GameController::getGameController()->onPaymentError();
    }

    void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentSuccess( JNIEnv* env, jobject thiz ){
        GameController::getGameController()->onPaymentSuccess();
    }
}
#endif

GameController * GameController::getGameController(){
    if (controller == NULL) {
        controller = new GameController();
        controller->retain();
        if (controller && !controller->init()) {
            delete controller;
            controller = NULL;
        }
    }

    return controller;
}

GameController::GameController(){

}

GameController::~GameController(){

}

bool GameController::init(){

	CCLOG("GameController::init");

	plistWritablePath = CCFileUtils::sharedFileUtils()->getWritablePath().append("game_controller.plist");

	if(!CCFileUtils::sharedFileUtils()->isFileExist(plistWritablePath)){
		string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("game_controller.plist");
		dict = CCDictionary::createWithContentsOfFileThreadSafe(path.c_str());
		dict->writeToFile(plistWritablePath.c_str());
	}else{
		dict = CCDictionary::createWithContentsOfFileThreadSafe(plistWritablePath.c_str());
	}

    if (dict == NULL) {
        return false;
    }

    userDataValue[LEFT]="left";
    userDataValue[RIGHT]="right";
    userDataValue[SIDE_LEFT_UP]="side_left_up";
	userDataValue[SIDE_LEFT_DOWN]="side_left_down";
    userDataValue[MUTE]="mute";
    userDataValue[UNMUTE]="unmute";

    designSize = CCDirector::sharedDirector()->getWinSize();

    if(initAnimationData((CCDictionary *)dict->objectForKey("animation_data")) == false){
        return false;
    }

    if(initPlaySceneData((CCArray *)dict->objectForKey("play_scene_data")) == false){
        return false;
    }

    if(initSpecialData((CCDictionary *)dict->objectForKey("special_data")) == false){
        return false;
    }

    if(initUserData((CCDictionary *)dict->objectForKey("user_data")) == false){
        return false;
    }

    initLeaderboard();

    return true;
}

bool GameController::initUserData(cocos2d::CCDictionary *dataDict){
    if (dataDict == NULL) {
        return false;
    }

    userData.topScore = CCSTRING_FOR_KEY(dataDict , "top_score")->intValue();
    userData.lastLevel = CCSTRING_FOR_KEY(dataDict , "last_level")->intValue();
    userData.pvpInitDuration = CCSTRING_FOR_KEY(dataDict , "pvp_init_duration")->intValue();
    userData.pvpMaxDuration = CCSTRING_FOR_KEY(dataDict , "pvp_max_duration")->intValue();
    userData.lastScore = 0;
    userData.currentLevel = userData.lastLevel;

    CCArray * levels = (CCArray *)dataDict->objectForKey("levels");

    userData.maxLevel = levels->count();
    userData.levels.reserve(userData.maxLevel);

    for (int i = 0; i < userData.maxLevel; i++) {
        userData.levels.push_back(CCSTRING_AT_INDEX(levels, i)->intValue());
    }

    if (CCSTRING_FOR_KEY(dataDict, "sound")->isEqual(CCString::create("mute"))) {
        userData.sound = MUTE;
    }else{
        userData.sound = UNMUTE;
    }

    if (CCSTRING_FOR_KEY(dataDict, "controller_position")->isEqual(CCString::create("left"))) {
        userData.controllerPosition = LEFT;
    }else if (CCSTRING_FOR_KEY(dataDict, "controller_position")->isEqual(CCString::create("right"))){
        userData.controllerPosition = RIGHT;
    }
	else if (CCSTRING_FOR_KEY(dataDict, "controller_position")->isEqual(CCString::create("side_left_up"))){
        userData.controllerPosition = SIDE_LEFT_UP;
    }
    else{
        userData.controllerPosition = SIDE_LEFT_DOWN;
    }

    userData.justFailed = CCSTRING_FOR_KEY(dataDict, "just_failed")->intValue();

    if (CCSTRING_FOR_KEY(dataDict, "has_payed")->isEqual(CCString::create("false"))) {
        userData.hasPayed = false;
    }else{
        userData.hasPayed = true;
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if(userData.hasPayed == false){
        userData.currentLevel = 0;
    }
#endif

    userData.order = -1;
    userData.pvpMode = NONE;
    userData.justWon = false;

    if(userData.currentLevel > 0){
    	userData.lastScore = userData.levels[userData.currentLevel - 1];
    }

    userData.userName = CCSTRING_FOR_KEY(dataDict, "user_name")->getCString();
    userData.password = CCSTRING_FOR_KEY(dataDict, "password")->getCString();

    if(userData.userName.length() == 0) userData.userName = DEFAULT_NAME;
    if(userData.password.length() == 0) userData.password = DEFAULT_PASSWORD;

    userData.rank = 0;
    userData.isLogedIn = false;
    userData.lastUploadedScore = -100000;
    userData.topLevel = getLevelByScore(userData.topScore);
    //userData.currentLevel = 1;

    return true;
}

bool GameController::initPlaySceneData(cocos2d::CCArray *dataArray){
    if (dataArray == NULL) {
        return false;
    }

    playSceneDatas.reserve(dataArray->count());

    for (int i = 0; i < dataArray->count(); i++) {
        PlaySceneData * data = new PlaySceneData();
        CCDictionary * dict = (CCDictionary *)dataArray->objectAtIndex(i);
        data->initDuration = CCSTRING_FOR_KEY(dict , "init_duration")->intValue();
        data->durationIncrease = CCSTRING_FOR_KEY(dict , "duration_increase")->intValue();
        data->maxDuration = CCSTRING_FOR_KEY(dict , "max_duration")->intValue();
        data->laneNumber = CCSTRING_FOR_KEY(dict , "lane_number")->intValue();
        data->playerSpeed = CCSTRING_FOR_KEY(dict , "player_init_speed")->floatValue();
        data->playerWaitImageName = CCSTRING_FOR_KEY(dict, "player_wait_image");
        data->playerMoveAnim = animationData.playerMoveAnim;
        data->laneDescriptions.reserve(data->laneNumber);
        data->playerAccSpeed = CCSTRING_FOR_KEY(dict , "player_start_acc_speed")->floatValue();
        data->playerStopAccSpeed = CCSTRING_FOR_KEY(dict , "player_stop_acc_speed")->floatValue();
        data->specialInterval = CCSTRING_FOR_KEY(dict , "special_interval")->floatValue();
        data->goodMax = getSpecialIdForKey(CCSTRING_FOR_KEY(dict , "good_max"));
        data->badMax = getSpecialIdForKey(CCSTRING_FOR_KEY(dict , "bad_max"));
        data->music = CCSTRING_FOR_KEY(dict , "music");
        //CCLOG("good %d, bad %d", data->goodMax , data->badMax);

        CCArray * ldArray = (CCArray *)dict->objectForKey("lane_descriptions");
        for (int j = 0; j < ldArray->count(); j++) {
            LaneDescription * ld = new LaneDescription();
            CCDictionary * ldDict = (CCDictionary *)ldArray->objectAtIndex(j);

            CCPoint pos = CCPointFromString(CCSTRING_FOR_KEY(ldDict, "init_position")->getCString());
            ld->period = CCSTRING_FOR_KEY(ldDict, "period")->floatValue();
            ld->specialChance = CCSTRING_FOR_KEY(ldDict, "special_chance")->floatValue();
            if (CCSTRING_FOR_KEY(ldDict, "direction")->isEqual(CCString::create("left2right"))) {
                ld->left2right = true;
                ld->carSpeed = CCSTRING_FOR_KEY(ldDict, "speed")->floatValue();
                ld->initPos = ccp(0 , pos.y * designSize.height);
            }else{
                ld->left2right = false;
                ld->carSpeed = -CCSTRING_FOR_KEY(ldDict, "speed")->floatValue();
                ld->initPos = ccp(designSize.width , pos.y * designSize.height);
            }

            CCArray * carNumbers = (CCArray *)ldDict->objectForKey("car_numbers");
            ld->carNumbers.reserve(carNumbers->count());
            for(int k = 0 ; k < carNumbers->count() ; k++ ){
            	ld->carNumbers.push_back(CCSTRING_AT_INDEX(carNumbers , k)->intValue());
            }
            data->laneDescriptions.push_back(ld);
        }
        playSceneDatas.push_back(data);
    }

    return true;
}

int GameController::getSpecialIdForKey(CCString * key){
    CCDictionary * specialDict = (CCDictionary *)dict->objectForKey("special_data");
    CCDictionary * special = (CCDictionary *)specialDict->objectForKey(key->getCString());
    return CCSTRING_FOR_KEY(special, "order")->intValue();
}

bool GameController::initAnimationData(cocos2d::CCDictionary *dataDict){
    if (dataDict == NULL) {
        return false;
    }

    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");

    animationData.backgroundSoundImage = CCSTRING_FOR_KEY(dataDict, "background_sound_file");
    animationData.resetSoundImage = CCSTRING_FOR_KEY(dataDict, "reset_sound_file");
    animationData.scoreSoundImage = CCSTRING_FOR_KEY(dataDict, "score_sound_file");
    animationData.levelupSoundImage = CCSTRING_FOR_KEY(dataDict, "levelup_sound_file");
    animationData.stopSoundImage = CCSTRING_FOR_KEY(dataDict, "special_stop_sound");
    animationData.skullSoundImage = CCSTRING_FOR_KEY(dataDict, "special_skull_sound");
    animationData.scoreSpecialSoundImage = CCSTRING_FOR_KEY(dataDict, "special_score_sound");
    animationData.lifeSoundImage = CCSTRING_FOR_KEY(dataDict, "special_life_sound");
    animationData.timeSoundImage = CCSTRING_FOR_KEY(dataDict, "special_time_sound");
    animationData.strongSoundImage = CCSTRING_FOR_KEY(dataDict, "special_strong_sound");
    animationData.blessSoundImage = CCSTRING_FOR_KEY(dataDict, "special_bless_sound");
    animationData.policeSoundImage = CCSTRING_FOR_KEY(dataDict, "special_police_sound");
    animationData.slowSoundImage = CCSTRING_FOR_KEY(dataDict, "special_slow_sound");
    animationData.curseSoundImage = CCSTRING_FOR_KEY(dataDict, "special_curse_sound");
    animationData.bombSoundImage = CCSTRING_FOR_KEY(dataDict, "special_bomb_sound");
    animationData.allBadSoundImage = CCSTRING_FOR_KEY(dataDict, "special_allbad_sound");
    animationData.gameOverSoundImage = CCSTRING_FOR_KEY(dataDict, "game_over_sound");
    animationData.gameStartSoundImage = CCSTRING_FOR_KEY(dataDict, "game_start_sound");
    animationData.smallSoundImage = CCSTRING_FOR_KEY(dataDict, "special_small_sound");
    animationData.fastSoundImage = CCSTRING_FOR_KEY(dataDict, "special_fast_sound");

    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.resetSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.scoreSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.levelupSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.stopSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.skullSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.lifeSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.timeSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.scoreSpecialSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.strongSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.blessSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.policeSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.curseSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.slowSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.bombSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.allBadSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.gameOverSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.gameStartSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.smallSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.fastSoundImage->getCString());

    CCArray * hornSoundFiles = (CCArray *)dataDict->objectForKey("horn_sound_files");
    animationData.hornSoundImages.reserve(hornSoundFiles->count());
    for(int i=0 ; i<hornSoundFiles->count() ; i++){
        animationData.hornSoundImages.push_back(CCSTRING_AT_INDEX(hornSoundFiles , i));
        SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.hornSoundImages[i]->getCString());
    }

    CCArray * array = (CCArray *)dataDict->objectForKey("player_move_animation");
    animationData.playerMoveAnim = initAnimation(array);
    animationData.playerMoveAnim->setDelayPerUnit(0.1);

    array = (CCArray *)dataDict->objectForKey("special_stop_animation");
    animationData.specialStopAnim = initAnimation(array);
    animationData.specialStopAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_haste_animation");
    animationData.specialHasteAnim = initAnimation(array);
    animationData.specialHasteAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_strong_animation");
    animationData.specialStrongAnim = initAnimation(array);
    animationData.specialStrongAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_life_animation");
    animationData.specialLifeAnim = initAnimation(array);
    animationData.specialLifeAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_time_animation");
    animationData.specialTimeAnim = initAnimation(array);
    animationData.specialTimeAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_skull_animation");
    animationData.specialSkullAnim = initAnimation(array);
    animationData.specialSkullAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_score_animation");
    animationData.specialScoreAnim = initAnimation(array);
    animationData.specialScoreAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_police_animation");
    animationData.specialPoliceAnim = initAnimation(array);
    animationData.specialPoliceAnim->setDelayPerUnit(0.3);

    CCDictionary * animDict = (CCDictionary *)dataDict->objectForKey("explode_animation");
    animationData.explodeAnim = initAnimation(animDict);

    return true;
}

CCAnimation * GameController::initAnimation(cocos2d::CCArray *frameNameArray){
    CCAnimation * anim = CCAnimation::create();
    anim->retain();
    for (int i = 0; i < frameNameArray->count(); i++) {
        CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCSTRING_AT_INDEX(frameNameArray, i)->getCString());
        anim->addSpriteFrame(frame);
    }
    return anim;
}

CCAnimation * GameController::initAnimation(CCDictionary * animDict){
    int picNum = CCSTRING_FOR_KEY(animDict, "number_of_pictures")->intValue();
    float interval = CCSTRING_FOR_KEY(animDict, "interval")->floatValue();
    CCAnimation * anim = CCAnimation::create();
    anim->retain();
    for (int i = 0; i < picNum; i++) {
        CCString * pic = CCString::createWithFormat("explode%d.png", i);
        CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic->getCString());
        anim->addSpriteFrame(frame);
    }
    anim->setDelayPerUnit(interval);
    return anim;
}

bool GameController::initSpecialData(cocos2d::CCDictionary *dataDict){

    if (dataDict == NULL) {
        return false;
    }

    specialDatas.reserve(dataDict->count());

    CCDictionary * dict = (CCDictionary *)dataDict->objectForKey("stop");
    specialDatas[STOP] = new SpecialData;
    specialDatas[STOP]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[STOP]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[STOP]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[STOP]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[STOP]->name = CCSTRING_FOR_KEY(dict, "name");
    specialDatas[STOP]->begin = &stopBegin;
    specialDatas[STOP]->step = NULL;
    specialDatas[STOP]->end = &stopEnd;
    specialDatas[STOP]->hitByCar = NULL;
    specialDatas[STOP]->animation = animationData.specialStopAnim;

    dict = (CCDictionary *)dataDict->objectForKey("strong");
    specialDatas[STRONG] = new SpecialData;
    specialDatas[STRONG]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[STRONG]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[STRONG]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[STRONG]->begin = &strongBegin;
    specialDatas[STRONG]->step = NULL;
    specialDatas[STRONG]->end = NULL;
    specialDatas[STRONG]->hitByCar = &strongHitByCar;
    specialDatas[STRONG]->animation = animationData.specialStrongAnim;
    specialDatas[STRONG]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[STRONG]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("life");
    specialDatas[LIFE] = new SpecialData;
    specialDatas[LIFE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[LIFE]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[LIFE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[LIFE]->userData1 = CCSTRING_FOR_KEY(dict, "time_increase")->floatValue();
    specialDatas[LIFE]->begin = &lifeBegin;
    specialDatas[LIFE]->step = NULL;
    specialDatas[LIFE]->end = NULL;
    specialDatas[LIFE]->hitByCar = NULL;
    specialDatas[LIFE]->animation = animationData.specialLifeAnim;
    specialDatas[LIFE]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[LIFE]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("time");
    specialDatas[TIME] = new SpecialData;
    specialDatas[TIME]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[TIME]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[TIME]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[TIME]->begin = &timeBegin;
    specialDatas[TIME]->step = NULL;
    specialDatas[TIME]->end = &timeEnd;
    specialDatas[TIME]->hitByCar = NULL;
    specialDatas[TIME]->animation = animationData.specialTimeAnim;
    specialDatas[TIME]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[TIME]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("skull");
    specialDatas[SKULL] = new SpecialData;
    specialDatas[SKULL]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SKULL]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[SKULL]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SKULL]->begin = &skullBegin;
    specialDatas[SKULL]->step = NULL;
    specialDatas[SKULL]->end = NULL;
    specialDatas[SKULL]->hitByCar = NULL;
    specialDatas[SKULL]->animation = animationData.specialSkullAnim;
    specialDatas[SKULL]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[SKULL]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("double");
    specialDatas[DOUBLE] = new SpecialData;
    specialDatas[DOUBLE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[DOUBLE]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[DOUBLE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[DOUBLE]->begin = &doubleBegin;
    specialDatas[DOUBLE]->step = NULL;
    specialDatas[DOUBLE]->end = NULL;
    specialDatas[DOUBLE]->hitByCar = NULL;
    specialDatas[DOUBLE]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[DOUBLE]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("police");
    specialDatas[POLICE] = new SpecialData;
    specialDatas[POLICE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[POLICE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[POLICE]->begin = &policeBegin;
    specialDatas[POLICE]->step = NULL;
    specialDatas[POLICE]->end = &policeEnd;
    specialDatas[POLICE]->hitByCar = NULL;
    specialDatas[POLICE]->animation = animationData.specialPoliceAnim;
    specialDatas[POLICE]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[POLICE]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("slow");
    specialDatas[SLOW] = new SpecialData;
    specialDatas[SLOW]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SLOW]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SLOW]->userData1 = CCSTRING_FOR_KEY(dict, "up_acc_delta")->floatValue();
    specialDatas[SLOW]->userData2 = CCSTRING_FOR_KEY(dict, "down_acc_delta")->floatValue();
    specialDatas[SLOW]->begin = &slowBegin;
    specialDatas[SLOW]->step = NULL;
    specialDatas[SLOW]->end = &slowEnd;
    specialDatas[SLOW]->hitByCar = NULL;
    specialDatas[SLOW]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[SLOW]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("curse");
    specialDatas[CURSE] = new SpecialData;
    specialDatas[CURSE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[CURSE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[CURSE]->userData1 = CCSTRING_FOR_KEY(dict, "time_decrease")->floatValue();
    specialDatas[CURSE]->begin = &curseBegin;
    specialDatas[CURSE]->step = NULL;
    specialDatas[CURSE]->end = &curseEnd;
    specialDatas[CURSE]->hitByCar = NULL;
    specialDatas[CURSE]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[CURSE]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("bomb");
    specialDatas[BOMB] = new SpecialData;
    specialDatas[BOMB]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[BOMB]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[BOMB]->userData1 = CCSTRING_FOR_KEY(dict, "deadline")->floatValue();
    specialDatas[BOMB]->begin = &bombBegin;
    specialDatas[BOMB]->step = &bombStep;
    specialDatas[BOMB]->end = &bombEnd;
    specialDatas[BOMB]->hitByCar = NULL;
    specialDatas[BOMB]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[BOMB]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("bless");
    specialDatas[BLESS] = new SpecialData;
    specialDatas[BLESS]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[BLESS]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[BLESS]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[BLESS]->userData1 = CCSTRING_FOR_KEY(dict, "chance")->floatValue();
    specialDatas[BLESS]->userData2 = CCSTRING_FOR_KEY(dict, "time_increase")->floatValue();
    specialDatas[BLESS]->userData3 = CCSTRING_FOR_KEY(dict, "score_increase")->floatValue();
    specialDatas[BLESS]->begin = &blessBegin;
    specialDatas[BLESS]->step = NULL;
    specialDatas[BLESS]->end = &blessEnd;
    specialDatas[BLESS]->hitByCar = &blessHitByCar;
    specialDatas[BLESS]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[BLESS]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("allBad");
    specialDatas[ALLBAD] = new SpecialData;
    specialDatas[ALLBAD]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[ALLBAD]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[ALLBAD]->userData1 = CCSTRING_FOR_KEY(dict, "special_chance")->floatValue();
    specialDatas[ALLBAD]->userData2 = CCSTRING_FOR_KEY(dict, "time")->floatValue();
    specialDatas[ALLBAD]->userData3 = CCSTRING_FOR_KEY(dict, "percent")->floatValue();
    specialDatas[ALLBAD]->begin = &allBadBegin;
    specialDatas[ALLBAD]->step = NULL;
    specialDatas[ALLBAD]->end = &allBadEnd;
    specialDatas[ALLBAD]->hitByCar = NULL;
    specialDatas[ALLBAD]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[ALLBAD]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("small");
    specialDatas[SMALL] = new SpecialData;
    specialDatas[SMALL]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SMALL]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[SMALL]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SMALL]->begin = &smallBegin;
    specialDatas[SMALL]->step = NULL;
    specialDatas[SMALL]->end = &smallEnd;
    specialDatas[SMALL]->hitByCar = NULL;
    specialDatas[SMALL]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[SMALL]->name = CCSTRING_FOR_KEY(dict, "name");

    dict = (CCDictionary *)dataDict->objectForKey("fast");
    specialDatas[FAST] = new SpecialData;
    specialDatas[FAST]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[FAST]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[FAST]->userData1 = CCSTRING_FOR_KEY(dict, "speed_increase")->floatValue();
    specialDatas[FAST]->begin = &fastBegin;
    specialDatas[FAST]->step = NULL;
    specialDatas[FAST]->end = &fastEnd;
    specialDatas[FAST]->hitByCar = NULL;
    specialDatas[FAST]->description = CCSTRING_FOR_KEY(dict, "description");
    specialDatas[FAST]->name = CCSTRING_FOR_KEY(dict, "name");

    return true;
}

//stop
void static stopBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->stopSoundImage->getCString());
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->stopAllLanes();
}

void static stopEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->restartAllLanes();
}

//strong
void static strongBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->strongSoundImage->getCString());
}

bool static strongHitByCar(PlayerObj * player, CCSprite * car){
	car->runAction(CCBlink::create(2, 10));
	return false;
}

//life
void static lifeBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->lifeSoundImage->getCString());
	SpecialData * data = GameController::getGameController()->getSpecialData(LIFE);
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->increaseDuration((int)data->userData1);
}

//time
void static timeBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->timeSoundImage->getCString());
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->stopTime();
}
void static timeEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->resumeTime();
}

//skull
void static skullBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->skullSoundImage->getCString());
	PlayScene * playScene = (PlayScene *)player->getParent();
	playScene->destroyRandomCar();
}

//double
void static doubleBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->scoreSpecialSoundImage->getCString());
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->city->addSpecial();
    playScene->city->addSpecial();
}

//police
void static policeBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->policeSoundImage->getCString());
	player->freeze();
	player->hitByCar(false);
}
void static policeEnd(PlayerObj * player){
	player->unfreeze();
}

//slow
void static slowBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->slowSoundImage->getCString());
    SpecialData * specialData = GameController::getGameController()->getSpecialData(SLOW);
	player->changeAcc(-specialData->userData1, -specialData->userData2);
	player->hitByCar(false);
}
void static slowEnd(PlayerObj * player){
	player->resumeAcc();
}

//curse
void static curseBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->curseSoundImage->getCString());
    SpecialData * specialData = GameController::getGameController()->getSpecialData(CURSE);
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->changeDurationIncrease(-specialData->userData1);
	player->hitByCar(false);
}
void static curseEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->resumeDurationIncrease();
}

//bomb
void static bombBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->bombSoundImage->getCString());
	player->hitByCar(false);
}
void static bombEnd(PlayerObj * player){

}
void static bombStep(PlayerObj * player , float timer_count){
    SpecialData * specialData = GameController::getGameController()->getSpecialData(BOMB);
    if(timer_count > specialData->userData1){
        AnimationData * animData = GameController::getGameController()->getAnimationData();
        SimpleAudioEngine::sharedEngine()->playEffect(animData->skullSoundImage->getCString());
    	CCAnimate * animate = CCAnimate::create(GameController::getGameController()->getAnimationData()->explodeAnim);
    	animate->setTag(PLAYER_BOMB);
    	player->runAction(animate);
    	player->hitByCar();
    	player->endWithSpecial(player->hasSpecial(BOMB));
    }
}

//bless
void static blessBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->blessSoundImage->getCString());
    player->removeAllBadSpecials();
    SpecialData * specialData = GameController::getGameController()->getSpecialData(BLESS);
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->changeScore(specialData->userData3, false);
    playScene->controlMenu->increaseDuration(specialData->userData2);

    if(toss(specialData->userData1) == false) return;
    SpecialObj * specialObj = new SpecialObj();
    playScene->addChild(specialObj->load(BLESS));
    player->beginWithSpecial(specialObj);
}
void static blessEnd(PlayerObj * player){

}
bool static blessHitByCar(PlayerObj * player, CCSprite * car){
//	SpecialData * specialData = GameController::getGameController()->getSpecialData(BLESS);
//	if(toss(specialData->userData1) == true) return false;
	return true;
}

//allBad
void static allBadBegin(PlayerObj * player){
    SpecialData * specialData = GameController::getGameController()->getSpecialData(ALLBAD);
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->allBadSoundImage->getCString());
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->setSpecialChance(specialData->userData1 , specialData->userData2);
    for(int i = SPECIAL_NUM + 1 ; i < BAD_SPECIAL_NUM ; i++){
    	if(i != ALLBAD){
    		GameController::getGameController()->getSpecialData(i)->duration *=
    				(1 + specialData->userData3);
    	}
    }
	player->hitByCar(false);
}
void static allBadEnd(PlayerObj * player){
	SpecialData * specialData = GameController::getGameController()->getSpecialData(ALLBAD);
    for(int i = SPECIAL_NUM + 1 ; i < BAD_SPECIAL_NUM ; i++){
    	if(i != ALLBAD){
    		GameController::getGameController()->getSpecialData(i)->duration /=
    				(1 + specialData->userData3);
    	}
    }
}

//small
void static smallBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->smallSoundImage->getCString());
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->allSmallCars();
}

void static smallEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->resumeCarNumbers();
}

//fast
void static fastBegin(PlayerObj * player){
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->fastSoundImage->getCString());
    SpecialData * specialData = GameController::getGameController()->getSpecialData(FAST);
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->speedUp(specialData->userData1);
    player->hitByCar(false);
}
void static fastEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->resumeSpeed();
}

PlaySceneData * GameController::getPlaySceneData(int level){
    return playSceneDatas[level];
}

AnimationData * GameController::getAnimationData(){
    return &animationData;
}

SpecialData * GameController::getSpecialData(int speciaId){
    return specialDatas[speciaId];
}

UserData * GameController::getUserData(){
	return &userData;
}

void GameController::setUserData(const char * key, CheckboxType type , int value){
    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");

    if (type == CHECKBOX_TYPE_NUM) {
        userDataDict->setObject(CCString::createWithFormat("%d", value), key);
    }else{
        userDataDict->setObject(CCString::createWithFormat("%s", userDataValue[type]), key);
    }

    dict->setObject(userDataDict, "user_data");
    dict->writeToFile(plistWritablePath.c_str());
}

void GameController::levelUp(){
    userData.currentLevel++;

    userData.lastLevel = userData.currentLevel;
    userData.topLevel = getLevelByScore(userData.topScore);
    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
    userDataDict->setObject(CCString::createWithFormat("%d", userData.lastLevel), "last_level");
    dict->setObject(userDataDict, "user_data");
    dict->writeToFile(plistWritablePath.c_str());
}

void GameController::setLastScore(int lastScore , bool justFailed){

	userData.lastScore = lastScore;

	if(justFailed == false){
		return;
	}

    if (lastScore > userData.topScore) {
        userData.topScore = lastScore;
        CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
        userDataDict->setObject(CCString::createWithFormat("%d", userData.topScore), "top_score");
        dict->setObject(userDataDict, "user_data");
        dict->writeToFile(plistWritablePath.c_str());
    }
}

void GameController::setJustFailed(bool justFailed , bool reset){
	bool updatePlist = false;

	if(reset == true){
		userData.justFailed = 0;
		updatePlist = true;
	}
	else{
		if(justFailed == true){
			userData.justFailed++;
			userData.justWon = false;
			updatePlist = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			if(userData.hasPayed == false){
			    userData.currentLevel = 0;
			}
#endif
		}
		else userData.justWon = true;
	}

	if(updatePlist == false) return;

    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
    userDataDict->setObject(CCString::createWithFormat("%d", userData.justFailed), "just_failed");
    dict->setObject(userDataDict, "user_data");
    dict->writeToFile(plistWritablePath.c_str());
}

void GameController::setHasPayed(bool hasPayed){
    if (userData.hasPayed == hasPayed) {
        return;
    }
    userData.hasPayed = hasPayed;
    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
    if (hasPayed == true) {
        userDataDict->setObject(CCString::create("true"), "has_payed");
    }else{
        userDataDict->setObject(CCString::create("false"), "has_payed");
    }
    dict->setObject(userDataDict, "user_data");
    dict->writeToFile(plistWritablePath.c_str());
}

void GameController::initLeaderboard(){
	App42API::Initialize("6a62bddeb3c503c6ad766390357a403f2ea778316726be19c29d309064d37862",
						"f87c2719986446f3d07816a58b5e89e7a7b8af1b04795e5cac8cb5e88afe0e16");
	gameService = App42API::BuildGameService();
	scoreBoardService = App42API::BuildScoreBoardService();
	userService = App42API::BuildUserService();
    rewardService = App42API::BuildRewardService();

	for(int i = 0; i < MAX_RANKS ; i++){
		ranks[i].userName = "nobody";
		ranks[i].score = 0;
		ranks[i].level = 0;
	}

	getTopRankings();

	if(userData.password != "Penguin#"){
	    authenticate(userData.userName.c_str() , userData.password.c_str());
	}
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    rewardService->GetAllRewards(this , callfuncND_selector(GameController::onGetRewardsCompleted));
#endif
	//gameService->CreateGame("crossRoad","crossRoad", this, callfuncND_selector(GameController::onGameRequestCompleted));
}

void GameController::onGetRewardsCompleted(cocos2d::CCNode *node, void *response){
    App42RewardResponse *rewardResponse = (App42RewardResponse*)response;
    
//    CCLOG("code=%d",rewardResponse->getCode());
//    CCLOG("Response Body=%s",rewardResponse->getBody().c_str());
    if (rewardResponse->isSuccess)
    {
        for(std::vector<App42Reward>::iterator it = rewardResponse->rewards.begin(); it != rewardResponse->rewards.end(); ++it)
        {
            if(it->name == "admob_id"){
                changeAdmobId(it->description.c_str());
            }
        }
    }
    else
    {
        CCLOG("errordetails:%s",rewardResponse->errorDetails.c_str());
        CCLOG("errorMessage:%s",rewardResponse->errorMessage.c_str());
        CCLOG("appErrorCode:%d",rewardResponse->appErrorCode);
        CCLOG("httpErrorCode:%d",rewardResponse->httpErrorCode);
    }
}

void GameController::onGameRequestCompleted(CCNode * node , void * response){
	App42GameResponse *gameResponse = (App42GameResponse*)response;
	CCLOG("code=%d",gameResponse->getCode());
	CCLOG("Response Body=%s",gameResponse->getBody().c_str());
	if (gameResponse->isSuccess)
	{
		for(std::vector<App42Game>::iterator it = gameResponse->games.begin(); it != gameResponse->games.end(); ++it)
		{
			CCLOG("Game Name=%s",it->name.c_str());
			CCLOG("Description=%s\n",it->description.c_str());
		}
	}
	else
	{
		CCLOG("errordetails:%s",gameResponse->errorDetails.c_str());
		CCLOG("errorMessage:%s",gameResponse->errorMessage.c_str());
		CCLOG("appErrorCode:%d",gameResponse->appErrorCode);
		CCLOG("httpErrorCode:%d",gameResponse->httpErrorCode);
	}
}

void GameController::onScoreBoardSaveCompleted(CCNode * node , void * response){
	App42GameResponse *scoreResponse = (App42GameResponse*)response;
//	CCLOG("code=%d",scoreResponse->getCode());
//	CCLOG("Response Body=%s",scoreResponse->getBody().c_str());
	if (scoreResponse->isSuccess)
	{
		for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
		{
//			CCLOG("CreatedAt=%s",it->getCreatedOn().c_str());
//			CCLOG("Rank=%s\n",it->getRank().c_str());
//			CCLOG("ScoreId=%s\n",it->getScoreId().c_str());
//			CCLOG("ScoreValue=%f\n",it->getScoreValue());
//			CCLOG("UserName=%s\n",it->getUserName().c_str());
		    CCString * info = CCString::createWithFormat("Upload score %d successfully", (int)it->getScoreValue());
		    setInfoLabel(info->getCString());
		}
	}
	else
	{
		CCLOG("errordetails:%s",scoreResponse->errorDetails.c_str());
		CCLOG("errorMessage:%s",scoreResponse->errorMessage.c_str());
		CCLOG("appErrorCode:%d",scoreResponse->appErrorCode);
		CCLOG("httpErrorCode:%d",scoreResponse->httpErrorCode);

        if(scoreResponse->errorDetails.length() == 0){
            setInfoLabel("Upload score failed");
            return;
        }

        size_t pos = scoreResponse->errorDetails.find(". ");

        CCString * info;

        if(pos == string::npos){
            info = CCString::create(scoreResponse->errorDetails.c_str());
        }else{
            string part1 = scoreResponse->errorDetails.substr(0 , pos);
            string part2 = scoreResponse->errorDetails.substr(pos+1);
            info = CCString::createWithFormat("%s\n%s", part1.c_str(), part2.c_str());
        }
        setInfoLabel(info->getCString());
	}
}

void GameController::onScoreBoardGetCompleted(CCNode * node , void * response){
	App42GameResponse *scoreResponse = (App42GameResponse*)response;
//	CCLOG("code=%d",scoreResponse->getCode());
//	CCLOG("Response Body=%s",scoreResponse->getBody().c_str());
	int i = 0;
	if (scoreResponse->isSuccess)
	{
		for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
		{
			if(i == MAX_RANKS) break;
//			CCLOG("CreatedAt=%s",it->getCreatedOn().c_str());
//			CCLOG("Rank=%s\n",it->getRank().c_str());
//			CCLOG("ScoreId=%s\n",it->getScoreId().c_str());
//			CCLOG("ScoreValue=%f\n",it->getScoreValue());
//			CCLOG("UserName=%s\n",it->getUserName().c_str());
			ranks[i].score = (int)it->getScoreValue();
			ranks[i].level = getLevelByScore(ranks[i].score);
			ranks[i].userName = it->getUserName();
			i++;
		}
	}
	else
	{
		CCLOG("errordetails:%s",scoreResponse->errorDetails.c_str());
		CCLOG("errorMessage:%s",scoreResponse->errorMessage.c_str());
		CCLOG("appErrorCode:%d",scoreResponse->appErrorCode);
		CCLOG("httpErrorCode:%d",scoreResponse->httpErrorCode);
	}
}

void GameController::onScoreBoardGetUserRankingCompleted(CCNode * node , void * response){
	App42GameResponse *scoreResponse = (App42GameResponse*)response;
//	CCLOG("code=%d",scoreResponse->getCode());
//	CCLOG("Response Body=%s",scoreResponse->getBody().c_str());
	if (scoreResponse->isSuccess)
	{
		for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
		{
//			CCLOG("CreatedAt=%s",it->getCreatedOn().c_str());
//			CCLOG("Rank=%d\n",it->getRank());
//			CCLOG("ScoreId=%s\n",it->getScoreId().c_str());
//			CCLOG("ScoreValue=%f\n",it->getScoreValue());
//			CCLOG("UserName=%s\n",it->getUserName().c_str());
			userData.rank = it->getRank();
			break;
		}
	}
	else
	{
		CCLOG("errordetails:%s",scoreResponse->errorDetails.c_str());
		CCLOG("errorMessage:%s",scoreResponse->errorMessage.c_str());
		CCLOG("appErrorCode:%d",scoreResponse->appErrorCode);
		CCLOG("httpErrorCode:%d",scoreResponse->httpErrorCode);
	}
}

int GameController::getLevelByScore(int score){
	int i;

	for(i = 0 ; i < userData.maxLevel ; i++){
		if(score < userData.levels[i]){
			return i;
		}
	}

	return i-1;
}

void GameController::getTopRankings(){
    scoreBoardService->GetTopNRankers("crossRoad", MAX_RANKS ,
    		this, callfuncND_selector(GameController::onScoreBoardGetCompleted));
    if(userData.isLogedIn == true){
        scoreBoardService->GetUserRanking("crossRoad",
                userData.userName,
                this, callfuncND_selector(GameController::onScoreBoardGetUserRankingCompleted));
    }
}

void GameController::authenticate(const char * userName , const char * passwd){
    userData.isLogedIn = false;
    userService->Authenticate(userName, passwd ,
            this, callfuncND_selector(GameController::onAuthenticateCompleted));
    saveUser(userName , passwd);
}

void GameController::onAuthenticateCompleted(CCNode * node , void * response){
    App42UserResponse *userResponse = (App42UserResponse*)response;
//    CCLOG("code=%d...=%d",userResponse->getCode(),userResponse->isSuccess);
//    CCLOG("Response Body=%s",userResponse->getBody().c_str());
    if (userResponse->isSuccess)
    {
        for(std::vector<App42User>::iterator it = userResponse->users.begin(); it != userResponse->users.end(); ++it)
        {
//            CCLOG("UserName=%s",it->userName.c_str());
//            CCLOG("Email=%s",it->email.c_str());
            userData.isLogedIn = true;
            scoreBoardService->GetUserRanking("crossRoad",
                    userData.userName,
                    this, callfuncND_selector(GameController::onScoreBoardGetUserRankingCompleted));
            CCString * info = CCString::createWithFormat("Welcome back %s", it->userName.c_str());
            setInfoLabel(info->getCString());
        }
    }
    else
    {
        CCLOG("errordetails:%s",userResponse->errorDetails.c_str());
        CCLOG("errorMessage:%s",userResponse->errorMessage.c_str());
        CCLOG("appErrorCode:%d",userResponse->appErrorCode);
        CCLOG("httpErrorCode:%d",userResponse->httpErrorCode);

        if(userResponse->errorDetails.length() == 0){
            setInfoLabel("Authentication failed");
            return;
        }

        size_t pos = userResponse->errorDetails.find(". ");

        CCString * info;

        if(pos == string::npos){
            info = CCString::create(userResponse->errorDetails.c_str());
        }else{
            string part1 = userResponse->errorDetails.substr(0 , pos);
            string part2 = userResponse->errorDetails.substr(pos+1);
            info = CCString::createWithFormat("%s\n%s", part1.c_str(), part2.c_str());
        }
        setInfoLabel(info->getCString());
    }
}

void GameController::setInfoLabel(const char * info){
    CCScene * currentScene = CCDirector::sharedDirector()->getRunningScene();
    if(currentScene && currentScene->getTag() == STARTUP_MENU_SCENE){
        StartMenuScene * startup = (StartMenuScene *)currentScene->getChildren()->objectAtIndex(0);
        startup->setInfoLabel(info , 0);
    }else if(currentScene && currentScene->getTag() == GAME_OVER_SCENE){
        GameOverScene * gameOver = (GameOverScene *)currentScene->getChildren()->objectAtIndex(0);
        gameOver->setInfoLabel(info);
    }
}

void GameController::createUser(const char * userName , const char * passwd){
    string email = userName;
    email.append("@hotmail.com");
    userData.isLogedIn = false;
    userService->CreateUser(userName, passwd , email ,
            this, callfuncND_selector(GameController::onAuthenticateCompleted));
    saveUser(userName , passwd);
}

void GameController::uploadLastScore(){

    if(userData.lastScore == userData.lastUploadedScore){
        return;
    }

    if(userData.lastScore < 100){
        return;
    }

    userData.lastUploadedScore = userData.lastScore;

    scoreBoardService->SaveUserScore("crossRoad",
    		userData.userName, userData.lastScore,
    		this, callfuncND_selector(GameController::onScoreBoardSaveCompleted));
}

void GameController::saveUser(const char * userName , const char * password){
    if (userData.userName == userName && userData.password == password) {
        return;
    }
    userData.userName = userName;
    userData.password = password;
    userData.rank = 0;
    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
    userDataDict->setObject(CCString::create(userName), "user_name");
    userDataDict->setObject(CCString::create(password), "password");
    dict->setObject(userDataDict, "user_data");
    dict->writeToFile(plistWritablePath.c_str());
}

void GameController::recoverSpecialDurations(){
    CCDictionary * specialDict = (CCDictionary *)dict->objectForKey("special_data");
    CCArray * keys = specialDict->allKeys();
    for(int i = 0 ; i < keys->count() ; i++){
        //CCLOG("%s", ((CCString *)keys->objectAtIndex(i))->getCString());
        CCDictionary * dict = (CCDictionary *)specialDict->objectForKey(
                ((CCString *)keys->objectAtIndex(i))->getCString());
        specialDatas[CCSTRING_FOR_KEY(dict, "order")->intValue()]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
        //CCLOG("%s , %f", ((CCString *)keys->objectAtIndex(i))->getCString() , CCSTRING_FOR_KEY(dict, "duration")->floatValue());
    }
}

void GameController::onPaymentError(){
    CCLOG("onPaymentError");
    setInfoLabel("Payment Error");
    CCScene * currentScene = CCDirector::sharedDirector()->getRunningScene();
    if(currentScene && currentScene->getTag() == STARTUP_MENU_SCENE){
        StartMenuScene * startup = (StartMenuScene *)currentScene->getChildren()->objectAtIndex(0);
        startup->enableButtonsForIap(true);
    }
}

void GameController::onPaymentSuccess(){
    CCLOG("onPaymentSuccess");
    setInfoLabel("Payment Success");
    setHasPayed(true);
    CCScene * currentScene = CCDirector::sharedDirector()->getRunningScene();
    if(currentScene && currentScene->getTag() == STARTUP_MENU_SCENE){
        StartMenuScene * startup = (StartMenuScene *)currentScene->getChildren()->objectAtIndex(0);
        startup->enableButtonsForIap(true);
    }
}

bool GameController::hasPayed(){
    return userData.hasPayed;
}
