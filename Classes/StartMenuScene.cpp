//
//  StartMenuScene.cpp
//  crossRoad
//
//  Created by Min Li on 2014-05-30.
//
//

#include "StartMenuScene.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "MultiPlayScene.h"

typedef enum{
    NEW_GAME = 0,
    OPTIONS,
    SCORE,
    PVP,
}BUTTON_TAG;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
extern void setBannerViewHidden(bool);
#define SET_BANNDER_HIDDEN(_hidden) \
if(userData->hasPayed) setBannerViewHidden(true); \
else setBannerViewHidden(_hidden)

void onAdClicked(){
    GameController::getGameController()->setJustFailed(false);
}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../admob.android/AdmobHelper.h"
#include <jni.h>

#define SET_BANNDER_HIDDEN(_hidden) \
if(userData->hasPayed) setBannerViewHidden(true); \
else setBannerViewHidden(_hidden)

extern "C"
{
	void Java_ca_welcomelm_crossRoad_crossRoad_onAdClicked( JNIEnv* env, jobject thiz ){
		GameController::getGameController()->setJustFailed(false);
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentError( JNIEnv* env, jobject thiz ){
		StartMenuScene * currentScene = (StartMenuScene *)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0);
		currentScene->onPaymentError();
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentSuccess( JNIEnv* env, jobject thiz ){
		StartMenuScene * currentScene = (StartMenuScene *)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0);
		currentScene->onPaymentSuccess();
	}
}

#else
#define SET_BANNDER_HIDDEN(_hidden)
#endif

using namespace CocosDenshion;

CCScene* StartMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    StartMenuScene *layer = StartMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    scene->setTag(STARTUP_MENU_SCENE);

    // return the scene
    return scene;
}

StartMenuScene::StartMenuScene() {
	// TODO Auto-generated constructor stub

}

StartMenuScene::~StartMenuScene() {
	// TODO Auto-generated destructor stub
}

bool StartMenuScene::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,176))) {
        return false;
    }

    winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height/2));
    userData = GameController::getGameController()->getUserData();

    userData->pvpMode = NONE;

    SET_BANNDER_HIDDEN(false);

    initMainMenu();

    initOptionsMenu();

    initScoreMenu();

    return true;
}

void StartMenuScene::initScoreMenu(){
	CCString * gemName = CCString::createWithFormat("gem%d.png", userData->topLevel);
	gem = CCMenuItemImage::create();
	gem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(gemName->getCString()));
	gem->setPosition(ccp(winSize.width/4, winSize.height*0.75));

    scoreLabel = CCMenuItemLabel::create(CCLabelTTF::create("0", "Times New Roman", 96 ));
    scoreLabel->setColor( ccc3(54, 255, 0) );
    CCString * score = CCString::createWithFormat("%.1f", userData->topScore);
    scoreLabel->setString(score->getCString());
    scoreLabel->setPosition(ccp(winSize.width*0.75, winSize.height*0.75));

    CCMenuItemImage * OK = CCMenuItemImage::create("ok_normal.png", "ok_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.5);
    OK->setPosition(ccp(winSize.width/2, winSize.height*0.25));

    scoreMenu = CCMenu::create(OK , NULL);
    scoreMenu->addChild(gem);
    scoreMenu->addChild(scoreLabel);

    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->ignoreAnchorPointForPosition(false);
    addChild(scoreMenu);
}

void StartMenuScene::initMainMenu(){
    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game_normal.png", "button_new_game_selected.png", this, menu_selector(StartMenuScene::newGameHandler));
    newGame->setTag(NEW_GAME);

    CCMenuItemImage * options = CCMenuItemImage::create("button_options_normal.png", "button_options_selected.png" , this , menu_selector(StartMenuScene::optionsHandler));
    options->setTag(OPTIONS);

    CCMenuItemImage * score = CCMenuItemImage::create("top_score_normal.png", "top_score_selected.png" , this , menu_selector(StartMenuScene::scoreHandler));
    score->setTag(SCORE);

    newGame->setScale(0.5);
    options->setScale(0.5);
    score->setScale(0.5);

#if 0
    CCMenuItemImage * pvp = CCMenuItemImage::create("pvp_normal.png", "pvp_selected.png" , this , menu_selector(StartMenuScene::pvpHandler));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CCMenuItemImage * pvp = CCMenuItemImage::create("purchase_normal.png", "purchase_selected.png" , this , menu_selector(StartMenuScene::pvpHandler));
    pvp->setScale(0.5);
    pvp->setTag(PVP);
    
    startMenu = CCMenu::create(newGame , options, score , pvp , NULL);
    startMenu->alignItemsInColumns(2 , 2);
#else
    startMenu = CCMenu::create(newGame , options, score , NULL);
    startMenu->alignItemsInColumns(1 , 1 , 1);
#endif

    infoLabel = CCLabelTTF::create("0", "Times New Roman", 32 );
    infoLabel->setColor( ccc3(54, 255, 0) );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    infoLabel->setVisible(false);
    addChild(infoLabel);

    addChild(startMenu);
}

void StartMenuScene::pvpHandler(cocos2d::CCObject *sender){
#if 0
    SET_BANNDER_HIDDEN(true);
    CCScene * pvpScene = MultiPlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(pvpScene);
#else
    enableButtonsForIap(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    purchase(this);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    purchase();
#endif
#endif
}

void StartMenuScene::scoreHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(true);
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::initOptionsMenu(){

    //row 1
    CCMenuItemImage * controllerPositions = CCMenuItemImage::create("controller_positions.png", NULL);
    controllerPositions->setPosition(ccp(winSize.width/2, winSize.height*0.9));

    //row 2
    CCMenuItemImage * left = CCMenuItemImage::create("left.png", NULL);
    checkboxLeft = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxLeft->setTag(LEFT);
    checkboxLeft->setScale(0.25);
    left->setPosition(ccp(winSize.width/4, winSize.height*0.75));
    checkboxLeft->setPosition(ccp(winSize.width*0.75, winSize.height*0.75));

    //row 3
    CCMenuItemImage * right = CCMenuItemImage::create("right.png", NULL);
    checkboxRight = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxRight->setTag(RIGHT);
    checkboxRight->setScale(0.25);
    right->setPosition(ccp(winSize.width/4, winSize.height*0.65));
    checkboxRight->setPosition(ccp(winSize.width*0.75, winSize.height*0.65));

    //row 4
    CCMenuItemImage * side_by_side = CCMenuItemImage::create("side_by_side.png", NULL);
    checkboxSide = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxSide->setTag(SIDE);
    checkboxSide->setScale(0.25);
    side_by_side->setPosition(ccp(winSize.width/4, winSize.height*0.55));
    checkboxSide->setPosition(ccp(winSize.width*0.75, winSize.height*0.55));

    //row 5
    CCMenuItemImage * sound = CCMenuItemImage::create("sound.png", NULL);
    sound->setPosition(ccp(winSize.width/2, winSize.height*0.4));

    //row 6
    CCMenuItemImage * mute = CCMenuItemImage::create("mute.png", NULL);
    checkboxMute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxMute->setTag(MUTE);
    mute->setPosition(ccp(winSize.width/4, winSize.height*0.3));
    checkboxMute->setPosition(winSize.width*0.75, winSize.height*0.3);
    checkboxMute->setScale(0.25);

    //row 7
    CCMenuItemImage * unmute = CCMenuItemImage::create("unmute.png", NULL);
    checkboxUnmute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxUnmute->setTag(UNMUTE);
    unmute->setPosition(ccp(winSize.width/4, winSize.height*0.2));
    checkboxUnmute->setPosition(winSize.width*0.75, winSize.height*0.2);
    checkboxUnmute->setScale(0.25);

    //row 8
    CCMenuItemImage * OK = CCMenuItemImage::create("ok_normal.png", "ok_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.25);
    OK->setPosition(ccp(winSize.width/2, winSize.height*0.1));

    optionsMenu = CCMenu::create(controllerPositions , left , checkboxLeft , right , checkboxRight , side_by_side , checkboxSide , sound , mute , checkboxMute , unmute, checkboxUnmute, OK , NULL);

    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->ignoreAnchorPointForPosition(false);
    addChild(optionsMenu);

    checkboxChooser(userData->sound);
    checkboxChooser(userData->controllerPosition);
}

void StartMenuScene::newGameHandler(cocos2d::CCObject *sender){
    if (!userData->hasPayed && userData->justFailed) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        setInfoLabel("Please click the ad to support us \n or spend 99 cents to remove the ad. Thank you");
        return;
#endif
    }
    SET_BANNDER_HIDDEN(true);
	CCScene * playScene = PlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(playScene);
}

void StartMenuScene::setInfoLabel(const char *info){
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    infoLabel->setString(info);
    infoLabel->setVisible(true);
    scheduleOnce(schedule_selector(StartMenuScene::hideInfoLabel), 2);
}

void StartMenuScene::hideInfoLabel(){
    startMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
    infoLabel->setVisible(false);
}

void StartMenuScene::optionsHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(true);
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::okHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(false);
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    startMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::checkboxHandler(cocos2d::CCObject *sender){
    CCMenuItem * item = (CCMenuItem *)sender;

    checkboxChooser((CheckboxType)item->getTag());
}

void StartMenuScene::checkboxChooser(CheckboxType tag){
    switch (tag) {
        case LEFT:
            checkboxLeft->selected();
            checkboxRight->unselected();
            checkboxSide->unselected();
            break;

        case RIGHT:
            checkboxLeft->unselected();
            checkboxRight->selected();
            checkboxSide->unselected();
            break;

        case SIDE:
            checkboxLeft->unselected();
            checkboxRight->unselected();
            checkboxSide->selected();
            break;

        case MUTE:
            checkboxMute->selected();
            checkboxUnmute->unselected();
            break;

        case UNMUTE:
            checkboxMute->unselected();
            checkboxUnmute->selected();
            break;

        default:
            break;
    }
    saveUserData(tag);
}

void StartMenuScene::saveUserData(CheckboxType tag){
    switch (tag) {
        case LEFT:
        case RIGHT:
        case SIDE:
            if (userData->controllerPosition != tag) {
                GameController::getGameController()->setUserData("controller_position", tag , 0);
                userData->controllerPosition = tag;
            }
            break;
        case MUTE:
        case UNMUTE:
            if (userData->sound != tag) {
                GameController::getGameController()->setUserData("sound", tag , 0);
                userData->sound = tag;
                changeSoundSetting(tag);
            }
            break;

        default:
            break;
    }
}

void StartMenuScene::changeSoundSetting(CheckboxType type){
    switch (type) {
        case MUTE:
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
            break;
        case UNMUTE:
        {
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            break;
        }
        default:
            break;
    }
}

void StartMenuScene::onPaymentError(){
	CCLOG("onPaymentError");
	setInfoLabel("Payment Error");
    enableButtonsForIap(true);
}

void StartMenuScene::onPaymentSuccess(){
	CCLOG("onPaymentSuccess");
    setInfoLabel("Payment Success");
    GameController::getGameController()->setHasPayed(true);
    SET_BANNDER_HIDDEN(true);
    enableButtonsForIap(true);
}

bool StartMenuScene::hasPayed(){
    return userData->hasPayed;
}

void StartMenuScene::enableButtonsForIap(bool enable){
    CCMenuItem * newGame = (CCMenuItem *)startMenu->getChildByTag(NEW_GAME);
    CCMenuItem * pvp = (CCMenuItem *)startMenu->getChildByTag(PVP);
    newGame->setEnabled(enable);
    pvp->setEnabled(enable);
}
