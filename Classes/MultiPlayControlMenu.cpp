/*
 * MultiPlayControlMenu.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: mli
 */

#include "MultiPlayControlMenu.h"
#include "MultiPlayScene.h"

MultiPlayControlMenu::MultiPlayControlMenu()
{
    // TODO Auto-generated constructor stub

}

MultiPlayControlMenu::~MultiPlayControlMenu()
{
    // TODO Auto-generated destructor stub
}

bool MultiPlayControlMenu::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,0))) {
        return false;
    }

    initMisc();
    initMenu();
    initBloodBar();
    initScoreLabel();
    initLevelSplash();

    return true;
}

void MultiPlayControlMenu::initMisc(){
    ControlMenu::initMisc();
}

void MultiPlayControlMenu::initMenu(){
    ControlMenu::initMenu();
    menu->removeChild(pauseAndPlay , true);
}

void MultiPlayControlMenu::initBloodBar(){
    ControlMenu::initBloodBar();
}

void MultiPlayControlMenu::initScoreLabel(){
    float playerCPPX, enemyCCPX;
    
    if (userData->order == FIRST) {
        playerCPPX = winSize.width * 0.33;
        enemyCCPX = winSize.width * 0.66;
    }else if (userData->order == SECOND){
        enemyCCPX = winSize.width * 0.33;
        playerCPPX = winSize.width * 0.66;
    }
    
    scoreLabel = CCLabelTTF::create("0", "Verdana-Bold", 64 );
    scoreLabel->setColor( ccc3(168, 0, 0) );
    scoreLabel->setPosition( ccp(playerCPPX, winSize.height - 50) );
    addChild(scoreLabel);
    
    scoreLabelEnemy = CCLabelTTF::create("0", "Verdana-Bold", 64 );
    scoreLabelEnemy->setColor( ccc3(168, 0, 0) );
    scoreLabelEnemy->setPosition( ccp(enemyCCPX, winSize.height - 50) );
    addChild(scoreLabelEnemy);
}

void MultiPlayControlMenu::initLevelSplash(){

}

