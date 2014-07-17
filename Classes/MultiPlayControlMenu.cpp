/*
 * MultiPlayControlMenu.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: mli
 */
#ifdef MULTIPLAY
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
    enemyScore = 0;
    isEnemyOver=false;

    initDuration = userData->pvpInitDuration;
    maxDuration = userData->pvpMaxDuration;
    seconds = initDuration;
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
        playerCPPX = winSize.width * 0.66;
        enemyCCPX = winSize.width * 0.82;
    }else if (userData->order == SECOND){
        enemyCCPX = winSize.width * 0.66;
        playerCPPX = winSize.width * 0.82;
    }

    scoreLabel = CCLabelTTF::create("0", FONT, 64 );
    scoreLabel->setColor( ccRED );
    scoreLabel->setPosition( ccp(playerCPPX, winSize.height - 50) );
    addChild(scoreLabel);

    scoreLabelEnemy = CCLabelTTF::create("0", FONT, 64 );
    scoreLabelEnemy->setColor( ccBLACK );
    scoreLabelEnemy->setPosition( ccp(enemyCCPX, winSize.height - 50) );
    addChild(scoreLabelEnemy);
}

void MultiPlayControlMenu::initLevelSplash(){
    ControlMenu::initLevelSplash();
}

bool MultiPlayControlMenu::doScore(){
    if (status != PLAY) {
        return false;
    }
    score+=1;
    updateScore(true);
    return true;
}

void MultiPlayControlMenu::updateScore(bool isGood){
    ControlMenu::updateScore(isGood);
}

void MultiPlayControlMenu::gameOver(){
    status = OVER;
    menu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    ((MultiPlayScene*)getParent())->sendOver();
}

void MultiPlayControlMenu::updateEnemyScore(float score){
    char ss[10];
    enemyScore += score;
    sprintf(ss, "%.1f", enemyScore);
    scoreLabelEnemy->setString(ss);
}

void MultiPlayControlMenu::step(float dt){
    ControlMenu::step(dt);
    if(status == OVER && isEnemyOver == true){
        isEnemyOver=false;
        ((MultiPlayScene*)getParent())->gameOver();

        if(score <= enemyScore){
            gameSplash->runAction(CCSequence::create(CCScaleTo::create(0.8, 0.6) ,CCCallFunc::create(this, callfunc_selector(ControlMenu::showOver)), NULL));
        }else{
            levelSplash->runAction(CCSequence::create(CCMoveTo::create(0.8, ccp(winSize.width/4, winSize.height*0.5)) ,CCCallFunc::create(this, callfunc_selector(ControlMenu::showUp)), NULL));
        }
    }
)

void MultiPlayControlMenu::showUp(){
    upSplash->runAction(CCSequence::create(CCMoveTo::create(0.8, ccp(winSize.width*0.75, winSize.height*0.5)) , CCDelayTime::create(1) , CCCallFunc::create(this, callfunc_selector(ControlMenu::nextScene)), NULL));
}
#endif
