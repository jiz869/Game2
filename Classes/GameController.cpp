//
//  GameController.cpp
//  crossRoad
//
//  Created by Min Li on 2014-05-10.
//
//

#include "GameController.h"

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
    
    if(initPlaySceneData((CCArray *)dict->objectForKey("play_scene_data")) == false){
        return false;
    }
    
    return true;
}

bool GameController::initPlaySceneData(cocos2d::CCArray *dataArray){
    if (dataArray == NULL) {
        return false;
    }
    
    playSceneDatas = CCArray::createWithCapacity(dataArray->count());
    
    for (int i = 0; i < dataArray->count(); i++) {
        PlaySceneData * data = new PlaySceneData();
        CCDictionary * dict = (CCDictionary *)dataArray->objectAtIndex(i);
        data->laneNumber = CCSTRING_FOR_KEY(dict , "lane_number")->intValue();
        data->laneDescriptions = CCArray::createWithCapacity(data->laneNumber);
        CCArray * ldArray = (CCArray *)dict->objectForKey("lane_descriptions");
        for (int j = 0; j < ldArray->count(); j++) {
            LaneDescription * ld = new LaneDescription();
            CCDictionary * ldDict = (CCDictionary *)ldArray->objectAtIndex(j);
            if (j%2 == 0) {
                strncpy(ld->carName, "car1", 50);
            }
            ld->distance = CCSTRING_FOR_KEY(ldDict, "distance")->intValue();
            ld->velocity = ccp(CCSTRING_FOR_KEY(ldDict, "speed")->floatValue(), 0);
            if (CCSTRING_FOR_KEY(ldDict, "direction")->isEqual(CCString::create("left2right"))) {
                ld->left2right = true;
            }else{
                ld->left2right = false;
            }
            
        }
    }
    
    return true;
}


PlaySceneData * GameController::getPlaySceneData(int level){
    
    return (PlaySceneData *)playSceneDatas->objectAtIndex(level);
}
