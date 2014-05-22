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

void static stopBegin(PlayerObj * player, SpecialObj * specialObj);
void static stopStep(PlayerObj * player, SpecialObj * specialObj);
void static stopEnd(PlayerObj * player, SpecialObj * specialObj);
bool static stopHitByCar(PlayerObj * player, SpecialObj * specialObj);

void static hasteBegin(PlayerObj * player, SpecialObj * specialObj);
void static hasteStep(PlayerObj * player, SpecialObj * specialObj);
void static hasteEnd(PlayerObj * player, SpecialObj * specialObj);
bool static hasteHitByCar(PlayerObj * player, SpecialObj * specialObj);

void static slowBegin(PlayerObj * player, SpecialObj * specialObj);
void static slowEnd(PlayerObj * player, SpecialObj * specialObj);

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
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("game_controller.plist");
    dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());

    if (dict == NULL) {
        return false;
    }

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

    userData.levelDuration = CCSTRING_FOR_KEY(dataDict , "level_duration")->intValue();
    userData.topScore = CCSTRING_FOR_KEY(dataDict , "top_score")->intValue();
    userData.lastScore = 0;

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
        data->laneNumber = CCSTRING_FOR_KEY(dict , "lane_number")->intValue();
        data->playerSpeed = CCSTRING_FOR_KEY(dict , "player_init_speed")->floatValue();
        data->playerWaitImageName = CCSTRING_FOR_KEY(dict, "player_wait_image");
        data->playerMoveAnim = animationData.playerMoveAnim;
        data->laneDescriptions.reserve(data->laneNumber);

        CCArray * ldArray = (CCArray *)dict->objectForKey("lane_descriptions");
        for (int j = 0; j < ldArray->count(); j++) {
            LaneDescription * ld = new LaneDescription();
            CCDictionary * ldDict = (CCDictionary *)ldArray->objectAtIndex(j);

            ld->height = designSize.height * CCSTRING_FOR_KEY(ldDict, "ccp_y_percent")->floatValue();
            ld->period = CCSTRING_FOR_KEY(ldDict, "period")->floatValue();
            ld->specialChance = CCSTRING_FOR_KEY(ldDict, "special_chance")->floatValue();
            ld->roadImage = CCSTRING_FOR_KEY(ldDict, "road_image");
            if (CCSTRING_FOR_KEY(ldDict, "direction")->isEqual(CCString::create("left2right"))) {
                ld->left2right = true;
                ld->initPos = ccp(0, ld->height);
                ld->carSpeed = CCSTRING_FOR_KEY(ldDict, "speed")->floatValue();
            }else{
                ld->left2right = false;
                ld->initPos = ccp(designSize.width, ld->height);
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

    CCArray * array = (CCArray *)dataDict->objectForKey("player_move_animation");
    animationData.playerMoveAnim = initAnimation(array);
    animationData.playerMoveAnim->setDelayPerUnit(0.05);

    array = (CCArray *)dataDict->objectForKey("special_stop_animation");
    animationData.specialStopAnim = initAnimation(array);
    animationData.specialStopAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_haste_animation");
    animationData.specialHasteAnim = initAnimation(array);
    animationData.specialHasteAnim->setDelayPerUnit(0.3);

    array = (CCArray *)dataDict->objectForKey("special_slow_animation");
    animationData.specialSlowAnim = initAnimation(array);
    animationData.specialSlowAnim->setDelayPerUnit(0.3);

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
    specialDatas[STOP]->step = &stopStep;
    specialDatas[STOP]->end = &stopEnd;
    specialDatas[STOP]->hitByCar = &stopHitByCar;
    specialDatas[STOP]->animation = animationData.specialStopAnim;

    dict = (CCDictionary *)dataDict->objectForKey("haste");
    specialDatas[HASTE] = new SpecialData;
    specialDatas[HASTE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[HASTE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[HASTE]->userData1 = CCSTRING_FOR_KEY(dict, "speed_increase")->floatValue();
    specialDatas[HASTE]->begin = &hasteBegin;
    specialDatas[HASTE]->step = &hasteStep;
    specialDatas[HASTE]->end = &hasteEnd;
    specialDatas[HASTE]->hitByCar = &hasteHitByCar;
    specialDatas[HASTE]->animation = animationData.specialHasteAnim;

    dict = (CCDictionary *)dataDict->objectForKey("slow");
    specialDatas[SLOW] = new SpecialData;
    specialDatas[SLOW]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SLOW]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SLOW]->userData1 = CCSTRING_FOR_KEY(dict, "speed_decrease")->floatValue();
    specialDatas[SLOW]->userData2 = CCSTRING_FOR_KEY(dict, "interval_increase")->floatValue();
    specialDatas[SLOW]->begin = &slowBegin;
    specialDatas[SLOW]->step = NULL;
    specialDatas[SLOW]->end = &slowEnd;
    specialDatas[SLOW]->hitByCar = NULL;
    specialDatas[SLOW]->animation = animationData.specialSlowAnim;

    return true;
}

//stop
void static stopBegin(PlayerObj * player, SpecialObj * specialObj){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->stopAllLanes();
}
void static stopStep(PlayerObj * player, SpecialObj * specialObj){
}
void static stopEnd(PlayerObj * player, SpecialObj * specialObj){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->restartAllLanes();
}
bool static stopHitByCar(PlayerObj * player, SpecialObj * specialObj){
	return true;
}

//haste
void static hasteBegin(PlayerObj * player, SpecialObj * specialObj){
    player->speedUp(specialObj->getSpecialData()->userData1);
}
void static hasteStep(PlayerObj * player, SpecialObj * specialObj){

}
void static hasteEnd(PlayerObj * player, SpecialObj * specialObj){
    player->slowDown(specialObj->getSpecialData()->userData1);
}
bool static hasteHitByCar(PlayerObj * player, SpecialObj * specialObj){
	return true;
}

//slow
void static slowBegin(PlayerObj * player, SpecialObj * specialObj){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->slowAllLanes(specialObj->getSpecialData()->userData1 , specialObj->getSpecialData()->userData2);
}
void static slowEnd(PlayerObj * player, SpecialObj * specialObj){
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->resumeAllLanesFromSlow(specialObj->getSpecialData()->userData1 , specialObj->getSpecialData()->userData2);
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
