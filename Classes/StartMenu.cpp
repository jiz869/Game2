/*
 * StartMenu.cpp
 *
 *  Created on: May 22, 2014
 *      Author: welcomelm
 */

#include "StartMenu.h"
#include "PlayScene.h"
#include "ControlMenu.h"
#include "SimpleAudioEngine.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
extern void setBannerViewHidden(bool);
#define SET_BANNDER_HIDDEN(_hidden) setBannerViewHidden(_hidden)
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../admob.android/AdmobHelper.h"
#define SET_BANNDER_HIDDEN(_hidden) AdmobHelper::setBannerViewHidden(_hidden)
#else
#define SET_BANNDER_HIDDEN(_hidden)
#endif

using namespace CocosDenshion;

StartMenu::StartMenu() {
	// TODO Auto-generated constructor stub

}

StartMenu::~StartMenu() {
	// TODO Auto-generated destructor stub
}

bool StartMenu::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,176))) {
        return false;
    }

    SET_BANNDER_HIDDEN(false);

    winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    userData = GameController::getGameController()->getUserData();

    initMainMenu();

    initOptionsMenu();

    return true;
}

void StartMenu::initMainMenu(){
    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game_normal.png", "button_new_game_selected.png", this, menu_selector(StartMenu::newGameHandler));

    CCMenuItemImage * options = CCMenuItemImage::create("button_options_normal.png", "button_options_selected.png" , this , menu_selector(StartMenu::optionsHandler));

    newGame->setScale(0.5);
    options->setScale(0.5);

    startMenu = CCMenu::create(newGame , options, NULL);
    startMenu->alignItemsInColumns(1 , 1);
    addChild(startMenu);
    scheduleOnce(schedule_selector(StartMenu::showStartMenu), 1);
}

void StartMenu::initOptionsMenu(){

    //row 1
    CCMenuItemImage * controllerPositions = CCMenuItemImage::create("controller_positions.png", NULL);
    controllerPositions->setPosition(ccp(winSize.width/2, winSize.height*0.9));

    //row 2
    CCMenuItemImage * left = CCMenuItemImage::create("left.png", NULL);
    checkboxLeft = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenu::checkboxHandler));
    checkboxLeft->setTag(LEFT);
    checkboxLeft->setScale(0.25);
    left->setPosition(ccp(winSize.width/4, winSize.height*0.75));
    checkboxLeft->setPosition(ccp(winSize.width*0.75, winSize.height*0.75));

    //row 3
    CCMenuItemImage * right = CCMenuItemImage::create("right.png", NULL);
    checkboxRight = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenu::checkboxHandler));
    checkboxRight->setTag(RIGHT);
    checkboxRight->setScale(0.25);
    right->setPosition(ccp(winSize.width/4, winSize.height*0.65));
    checkboxRight->setPosition(ccp(winSize.width*0.75, winSize.height*0.65));

    //row 4
    CCMenuItemImage * side_by_side = CCMenuItemImage::create("side_by_side.png", NULL);
    checkboxSide = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenu::checkboxHandler));
    checkboxSide->setTag(SIDE);
    checkboxSide->setScale(0.25);
    side_by_side->setPosition(ccp(winSize.width/4, winSize.height*0.55));
    checkboxSide->setPosition(ccp(winSize.width*0.75, winSize.height*0.55));

    //row 5
    CCMenuItemImage * sound = CCMenuItemImage::create("sound.png", NULL);
    sound->setPosition(ccp(winSize.width/2, winSize.height*0.4));

    //row 6
    CCMenuItemImage * mute = CCMenuItemImage::create("mute.png", NULL);
    checkboxMute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenu::checkboxHandler));
    checkboxMute->setTag(MUTE);
    mute->setPosition(ccp(winSize.width/4, winSize.height*0.3));
    checkboxMute->setPosition(winSize.width*0.75, winSize.height*0.3);
    checkboxMute->setScale(0.25);

    //row 7
    CCMenuItemImage * unmute = CCMenuItemImage::create("unmute.png", NULL);
    checkboxUnmute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenu::checkboxHandler));
    checkboxUnmute->setTag(UNMUTE);
    unmute->setPosition(ccp(winSize.width/4, winSize.height*0.2));
    checkboxUnmute->setPosition(winSize.width*0.75, winSize.height*0.2);
    checkboxUnmute->setScale(0.25);

    //row 8
    CCMenuItemImage * OK = CCMenuItemImage::create("ok_normal.png", "ok_selected.png" , this , menu_selector(StartMenu::okHandler));
    OK->setScale(0.25);
    OK->setPosition(ccp(winSize.width/2, winSize.height*0.1));

    optionsMenu = CCMenu::create(controllerPositions , left , checkboxLeft , right , checkboxRight , side_by_side , checkboxSide , sound , mute , checkboxMute , unmute, checkboxUnmute, OK , NULL);

    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->ignoreAnchorPointForPosition(false);
    addChild(optionsMenu);

    checkboxChooser(userData->sound);
    checkboxChooser(userData->controllerPosition);
}

void StartMenu::newGameHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(true);
	CCSequence * sequence = CCSequence::create(CCMoveTo::create(1, ccp(winSize.width/2, winSize.height*1.5)),
			CCCallFunc::create(((PlayScene *)getParent())->controlMenu, callfunc_selector(ControlMenu::startNewGame)) , NULL);
    runAction(sequence);
}

void StartMenu::showStartMenu(float dt){
    runAction(CCMoveTo::create(1, ccp(winSize.width/2, winSize.height/2)));
}

void StartMenu::optionsHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(true);
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenu::okHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(false);
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    startMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenu::checkboxHandler(cocos2d::CCObject *sender){
    CCMenuItem * item = (CCMenuItem *)sender;

    checkboxChooser((CheckboxType)item->getTag());
}

void StartMenu::checkboxChooser(CheckboxType tag){
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

void StartMenu::saveUserData(CheckboxType tag){
    switch (tag) {
        case LEFT:
        case RIGHT:
        case SIDE:
            if (userData->controllerPosition != tag) {
                PlayScene * playScene = (PlayScene *)getParent();
                playScene->controlMenu->changeControllerPosition(tag);
                GameController::getGameController()->setUserData("controller_position", tag);
                userData->controllerPosition = tag;
            }
            break;
        case MUTE:
        case UNMUTE:
            if (userData->sound != tag) {
                GameController::getGameController()->setUserData("sound", tag);
                userData->sound = tag;
                changeSoundSetting(tag);
            }
            break;

        default:
            break;
    }
}

void StartMenu::changeSoundSetting(CheckboxType type){
    switch (type) {
        case MUTE:
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            break;
        case UNMUTE:
        {
            PlayScene * playScene = (PlayScene *)getParent();
            playScene->startPlayBackgroundMusic();
            break;
        }
        default:
            break;
    }
}

