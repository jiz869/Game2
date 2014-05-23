/*
 * StartMenu.cpp
 *
 *  Created on: May 22, 2014
 *      Author: welcomelm
 */

#include "StartMenu.h"

StartMenu::StartMenu() {
	// TODO Auto-generated constructor stub

}

StartMenu::~StartMenu() {
	// TODO Auto-generated destructor stub
}

bool StartMenu::init(){
    if (!CCLayerColor::initWithColor(ccc4(176,0,0,0))) {
        return false;
    }
    
    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game.png", "button_new_game.png", this, menu_selector(StartMenu::newGameHandler));
    
    CCMenuItemImage * options = CCMenuItemImage::create("button_options.png", "button_options.png");
    
    return true;
}

void StartMenu::newGameHandler(cocos2d::CCObject *sender){
    
}

