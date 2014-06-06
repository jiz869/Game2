/*
 * MultiPlayControlMenu.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: mli
 */

#include "MultiPlayControlMenu.h"

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

}

void MultiPlayControlMenu::initScoreLabel(){

}

void MultiPlayControlMenu::initLevelSplash(){

}

