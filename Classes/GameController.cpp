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

using namespace CocosDenshion;

void static stopBegin(PlayerObj * player);
void static stopEnd(PlayerObj * player);

void static hasteBegin(PlayerObj * player);
void static hasteEnd(PlayerObj * player);

bool static strongHitByCar(PlayerObj * player, CCSprite * car);

void static lifeBegin(PlayerObj * player);

void static timeBegin(PlayerObj * player);
void static timeEnd(PlayerObj * player);

void static skullBegin(PlayerObj * player);

void static scoreBegin(PlayerObj * player);
void static scoreStep(PlayerObj * player);

char * userDataValue[CHECKBOX_TYPE_NUM];

static GameController * controller;

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

    initLeaderboard();

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
    userData.currentLevel = 6;

    if(userData.currentLevel > 0){
    	userData.lastScore = userData.levels[userData.currentLevel - 1];
    }

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

    array = (CCArray *)dataDict->objectForKey("special_score_animation");
    animationData.specialScoreAnim = initAnimation(array);
    animationData.specialScoreAnim->setDelayPerUnit(0.3);

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
    specialDatas[STOP]->begin = &stopBegin;
    specialDatas[STOP]->step = NULL;
    specialDatas[STOP]->end = &stopEnd;
    specialDatas[STOP]->hitByCar = NULL;
    specialDatas[STOP]->animation = animationData.specialStopAnim;

    dict = (CCDictionary *)dataDict->objectForKey("haste");
    specialDatas[HASTE] = new SpecialData;
    specialDatas[HASTE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[HASTE]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
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
    specialDatas[STRONG]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[STRONG]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[STRONG]->begin = NULL;
    specialDatas[STRONG]->step = NULL;
    specialDatas[STRONG]->end = NULL;
    specialDatas[STRONG]->hitByCar = &strongHitByCar;
    specialDatas[STRONG]->animation = animationData.specialStrongAnim;

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

    dict = (CCDictionary *)dataDict->objectForKey("score");
    specialDatas[SCORE] = new SpecialData;
    specialDatas[SCORE]->duration = CCSTRING_FOR_KEY(dict, "duration")->floatValue();
    specialDatas[SCORE]->life = CCSTRING_FOR_KEY(dict, "life")->floatValue();
    specialDatas[SCORE]->imageName = CCSTRING_FOR_KEY(dict, "image_name");
    specialDatas[SCORE]->userData1 = CCSTRING_FOR_KEY(dict, "interval")->floatValue();
    specialDatas[SCORE]->userData2 = CCSTRING_FOR_KEY(dict, "score_increase")->floatValue();
    specialDatas[SCORE]->begin = &scoreBegin;
    specialDatas[SCORE]->step = &scoreStep;
    specialDatas[SCORE]->end = NULL;
    specialDatas[SCORE]->hitByCar = NULL;
    specialDatas[SCORE]->animation = animationData.specialScoreAnim;

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
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->increaseDuration((int)data->userData1 , 0);
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

//score
void static scoreBegin(PlayerObj * player){
    SpecialData * specialData = GameController::getGameController()->getSpecialData(SCORE);
    specialData->count = 0;
    PlayScene * playScene = (PlayScene *)player->getParent();
    playScene->controlMenu->changeScore(specialData->userData2, true);
}
void static scoreStep(PlayerObj * player){
    SpecialData * specialData = GameController::getGameController()->getSpecialData(SCORE);
    int interval = (int)(specialData->userData1 * 60);
    if(++specialData->count % interval == 0){
        PlayScene * playScene = (PlayScene *)player->getParent();
        playScene->controlMenu->changeScore(specialData->userData2, false);
    }
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

void GameController::setLastScore(int lastScore , bool justFailed){
	if(justFailed == false){
		userData.lastScore = lastScore;
		return;
	}

    if(userData.currentLevel > 0){
    	userData.lastScore = userData.levels[userData.currentLevel - 1];
    }

    if (lastScore > userData.topScore) {
        userData.topScore = lastScore;
        scoreBoardService->SaveUserScore("crossRoad", "crossRoad", lastScore,
        		this, callfuncND_selector(GameController::onScoreBoardSaveCompleted));
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

void GameController::initLeaderboard(){
	App42API::Initialize("6a62bddeb3c503c6ad766390357a403f2ea778316726be19c29d309064d37862",
						"f87c2719986446f3d07816a58b5e89e7a7b8af1b04795e5cac8cb5e88afe0e16");
	gameService = App42API::BuildGameService();
	scoreBoardService = App42API::BuildScoreBoardService();

	CCLOG("ranks size is %d", sizeof(ranks));

	memset((void *)&ranks , 0 , sizeof(ranks));

    scoreBoardService->GetTopRankings("crossRoad",
    		this, callfuncND_selector(GameController::onScoreBoardGetCompleted));
	//gameService->CreateGame("crossRoad","crossRoad", this, callfuncND_selector(GameController::onGameRequestCompleted));
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
	CCLOG("code=%d",scoreResponse->getCode());
	CCLOG("Response Body=%s",scoreResponse->getBody().c_str());
	if (scoreResponse->isSuccess)
	{
		for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
		{
			CCLOG("CreatedAt=%s",it->getCreatedOn().c_str());
			CCLOG("Rank=%s\n",it->getRank().c_str());
			CCLOG("ScoreId=%s\n",it->getScoreId().c_str());
			CCLOG("ScoreValue=%f\n",it->getScoreValue());
			CCLOG("UserName=%s\n",it->getUserName().c_str());

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

void GameController::onScoreBoardGetCompleted(CCNode * node , void * response){
	App42GameResponse *scoreResponse = (App42GameResponse*)response;
	CCLOG("code=%d",scoreResponse->getCode());
	CCLOG("Response Body=%s",scoreResponse->getBody().c_str());
	int i = 0;
	if (scoreResponse->isSuccess)
	{
		for(std::vector<App42Score>::iterator it = scoreResponse->scores.begin(); it != scoreResponse->scores.end(); ++it)
		{
			if(i == MAX_RANKS) break;
			CCLOG("CreatedAt=%s",it->getCreatedOn().c_str());
			CCLOG("Rank=%s\n",it->getRank().c_str());
			CCLOG("ScoreId=%s\n",it->getScoreId().c_str());
			CCLOG("ScoreValue=%f\n",it->getScoreValue());
			CCLOG("UserName=%s\n",it->getUserName().c_str());
			ranks[i].score = (int)it->getScoreValue();
			ranks[i].level = getLevelByScore(ranks[i].score);
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

int GameController::getLevelByScore(int score){
	for(int i = 0 ; i < userData.maxLevel ; i++){
		if(score < userData.levels[i]){
			return i;
		}
	}
}


