/*
 * StartMenu.cpp
 *
 *  Created on: May 22, 2014
 *      Author: welcomelm
 */

#include "StartMenu.h"
#include "PlayScene.h"

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

    winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height*1.5));

    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game.png", "button_new_game.png", this, menu_selector(StartMenu::newGameHandler));

    CCMenuItemImage * options = CCMenuItemImage::create("button_options.png", "button_options.png");

    newGame->setScale(0.5);
    options->setScale(0.5);

    startMenu = CCMenu::create(newGame , options, NULL);
    startMenu->alignItemsInColumns(1 , 1);
    addChild(startMenu);
    scheduleOnce(schedule_selector(StartMenu::showStartMenu), 1);

    return true;
}

void StartMenu::newGameHandler(cocos2d::CCObject *sender){
	CCSequence * sequence = CCSequence::create(CCMoveTo::create(1, ccp(winSize.width/2, winSize.height*1.5)),
			CCCallFunc::create(getParent(), callfunc_selector(PlayScene::startNewGame)) , NULL);
    runAction(sequence);
}

void StartMenu::showStartMenu(float dt){
    runAction(CCMoveTo::create(1, ccp(winSize.width/2, winSize.height/2)));
}

