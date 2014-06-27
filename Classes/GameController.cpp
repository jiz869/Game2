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

using namespace CocosDenshion;

void static stopBegin(PlayerObj * player);
void static stopEnd(PlayerObj * player);

void static hasteBegin(PlayerObj * player);
void static hasteEnd(PlayerObj * player);

bool static strongHitByCar(PlayerObj * player, CCSprite * car);

void static lifeBegin(PlayerObj * player);
void static lifeEnd(PlayerObj * player);

void static timeBegin(PlayerObj * player);
void static timeEnd(PlayerObj * player);

void static skullBegin(PlayerObj * player);
bool static skullHitByCar(PlayerObj * player, CCSprite * car);

char * userDataValue[CHECKBOX_TYPE_NUM];

static GameController * controller;

GameController * GameController::getGameController(){
    if (controller == NULL) {
        controller = new GameController();
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
    userDataValue[SIDE]="side";
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

    return true;
}

bool GameController::initUserData(cocos2d::CCDictionary *dataDict){
    if (dataDict == NULL) {
        return false;
    }

    userData.topScore = CCSTRING_FOR_KEY(dataDict , "top_score")->intValue();
    userData.topLevel = CCSTRING_FOR_KEY(dataDict , "top_level")->intValue();
    userData.pvpInitDuration = CCSTRING_FOR_KEY(dataDict , "pvp_init_duration")->intValue();
    userData.pvpMaxDuration = CCSTRING_FOR_KEY(dataDict , "pvp_max_duration")->intValue();
    userData.lastScore = 0;
    userData.currentLevel = 0;

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
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(animationData.backgroundSoundImage->getCString(), true);
    }

    if (CCSTRING_FOR_KEY(dataDict, "controller_position")->isEqual(CCString::create("left"))) {
        userData.controllerPosition = LEFT;
    }else if (CCSTRING_FOR_KEY(dataDict, "controller_position")->isEqual(CCString::create("right"))){
        userData.controllerPosition = RIGHT;
    }
    else{
        userData.controllerPosition = SIDE;
    }

    if (CCSTRING_FOR_KEY(dataDict, "just_failed")->isEqual(CCString::create("false"))) {
        userData.justFailed = false;
    }else{
        userData.justFailed = true;
    }

    if (CCSTRING_FOR_KEY(dataDict, "has_payed")->isEqual(CCString::create("false"))) {
        userData.hasPayed = false;
    }else{
        userData.hasPayed = true;
    }

    userData.order = -1;
    userData.pvpMode = NONE;
    userData.currentLevel = 3;

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

        CCArray * ldArray = (CCArray *)dict->objectForKey("lane_descriptions");
        for (int j = 0; j < ldArray->count(); j++) {
            LaneDescription * ld = new LaneDescription();
            CCDictionary * ldDict = (CCDictionary *)ldArray->objectAtIndex(j);

            CCPoint pos = CCPointFromString(CCSTRING_FOR_KEY(ldDict, "init_position")->getCString());
            ld->initPos = ccp(pos.x * designSize.width , pos.y * designSize.height);
            ld->period = CCSTRING_FOR_KEY(ldDict, "period")->floatValue();
            ld->specialChance = CCSTRING_FOR_KEY(ldDict, "special_chance")->floatValue();
            if (CCSTRING_FOR_KEY(ldDict, "direction")->isEqual(CCString::create("left2right"))) {
                ld->left2right = true;
                ld->carSpeed = CCSTRING_FOR_KEY(ldDict, "speed")->floatValue();
            }else{
                ld->left2right = false;
                ld->carSpeed = -CCSTRING_FOR_KEY(ldDict, "speed")->floatValue();
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

bool GameController::initAnimationData(cocos2d::CCDictionary *dataDict){
    if (dataDict == NULL) {
        return false;
    }

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");

    animationData.backgroundSoundImage = CCSTRING_FOR_KEY(dataDict, "background_sound_file");
    animationData.resetSoundImage = CCSTRING_FOR_KEY(dataDict, "reset_sound_file");
    animationData.scoreSoundImage = CCSTRING_FOR_KEY(dataDict, "score_sound_file");
    animationData.levelupSoundImage = CCSTRING_FOR_KEY(dataDict, "levelup_sound_file");

    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(animationData.backgroundSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.resetSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.scoreSoundImage->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(animationData.levelupSoundImage->getCString());

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

bool GameController::initSpecialData(cocos2d::CCDictionary *dataDict){

    if (dataDict == NULL) {
        return false;
    }

    specialDatas.reserve(dataDict->count());

    CCDictionary * dict = (CCDictionary *)dataDict->objectForKey("stop");
    specialDatas[STOP] = new SpecialData;
    specialDatas[STOP]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[STOP]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[STOP]->begin = &stopBegin;
    specialDatas[STOP]->step = NULL;
    specialDatas[STOP]->end = &stopEnd;
    specialDatas[STOP]->hitByCar = NULL;
    specialDatas[STOP]->animation = animationData.specialStopAnim;

    dict = (CCDictionary *)dataDict->objectForKey("haste");
    specialDatas[HASTE] = new SpecialData;
    specialDatas[HASTE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[HASTE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[HASTE]->userData1 = CCSTRING_FOR_KEY(dict, "speed_increase")->floatValue();
    specialDatas[HASTE]->begin = &hasteBegin;
    specialDatas[HASTE]->step = NULL;
    specialDatas[HASTE]->end = &hasteEnd;
    specialDatas[HASTE]->hitByCar = NULL;
    specialDatas[HASTE]->animation = animationData.specialHasteAnim;

    dict = (CCDictionary *)dataDict->objectForKey("strong");
    specialDatas[STRONG] = new SpecialData;
    specialDatas[STRONG]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[STRONG]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[STRONG]->begin = NULL;
    specialDatas[STRONG]->step = NULL;
    specialDatas[STRONG]->end = NULL;
    specialDatas[STRONG]->hitByCar = &strongHitByCar;
    specialDatas[STRONG]->animation = animationData.specialStrongAnim;

    dict = (CCDictionary *)dataDict->objectForKey("life");
    specialDatas[LIFE] = new SpecialData;
    specialDatas[LIFE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[LIFE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[LIFE]->userData1 = CCSTRING_FOR_KEY(dict, "min")->floatValue();
    specialDatas[LIFE]->userData2 = CCSTRING_FOR_KEY(dict, "max")->floatValue();
    specialDatas[LIFE]->begin = &lifeBegin;
    specialDatas[LIFE]->step = NULL;
    specialDatas[LIFE]->end = &lifeEnd;
    specialDatas[LIFE]->hitByCar = NULL;
    specialDatas[LIFE]->animation = animationData.specialLifeAnim;

    dict = (CCDictionary *)dataDict->objectForKey("time");
    specialDatas[TIME] = new SpecialData;
    specialDatas[TIME]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[TIME]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[TIME]->begin = &timeBegin;
    specialDatas[TIME]->step = NULL;
    specialDatas[TIME]->end = &timeEnd;
    specialDatas[TIME]->hitByCar = NULL;
    specialDatas[TIME]->animation = animationData.specialTimeAnim;

    dict = (CCDictionary *)dataDict->objectForKey("skull");
    specialDatas[SKULL] = new SpecialData;
    specialDatas[SKULL]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SKULL]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SKULL]->begin = &skullBegin;
    specialDatas[SKULL]->step = NULL;
    specialDatas[SKULL]->end = NULL;
    specialDatas[SKULL]->hitByCar = NULL;
    specialDatas[SKULL]->animation = animationData.specialSkullAnim;

    return true;
}

//stop
void static stopBegin(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->stopAllLanes();
}

void static stopEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->restartAllLanes();
}

//haste
void static hasteBegin(PlayerObj * player){
	SpecialData * data = GameController::getGameController()->getSpecialData(HASTE);
    player->speedUp(data->userData1);
}

void static hasteEnd(PlayerObj * player){
	SpecialData * data = GameController::getGameController()->getSpecialData(HASTE);
    player->slowDown(data->userData1);
}

//strong
bool static strongHitByCar(PlayerObj * player, CCSprite * car){
	car->runAction(CCBlink::create(2, 10));
	return false;
}

//life
void static lifeBegin(PlayerObj * player){
	SpecialData * data = GameController::getGameController()->getSpecialData(LIFE);

    int life = getRandom(data->userData1, data->userData2);

    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->increaseDuration(life);
}
void static lifeEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->resumeDuration();
}

//time
void static timeBegin(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->stopTime();
}
void static timeEnd(PlayerObj * player){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->resumeTime();
}

//skull
void static skullBegin(PlayerObj * player){
	PlayScene * playScene = (PlayScene *)player->getParent();
	playScene->destroyRandomCar();
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
    if (userData.currentLevel > userData.topLevel) {
        userData.topLevel = userData.currentLevel;
        CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
        userDataDict->setObject(CCString::createWithFormat("%d", userData.topLevel), "top_level");
        dict->setObject(userDataDict, "user_data");
        dict->writeToFile(plistWritablePath.c_str());
    }
}

void GameController::setLastScore(int lastScore){
    userData.lastScore = lastScore;
    if (lastScore > userData.topScore) {
        userData.topScore = lastScore;
        CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
        userDataDict->setObject(CCString::createWithFormat("%d", userData.topScore), "top_score");
        dict->setObject(userDataDict, "user_data");
        dict->writeToFile(plistWritablePath.c_str());
    }
}

void GameController::setJustFailed(bool justFailed){
    if (userData.justFailed == justFailed) {
        return;
    }
    userData.justFailed = justFailed;
    CCDictionary * userDataDict = (CCDictionary *)dict->objectForKey("user_data");
    if (justFailed == true) {
        userDataDict->setObject(CCString::create("true"), "just_failed");
    }else{
        userDataDict->setObject(CCString::create("false"), "just_failed");
    }
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


