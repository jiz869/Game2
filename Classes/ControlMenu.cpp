/*
 * ControlMenu.cpp
 *
 *  Created on: May 23, 2014
 *      Author: mli
 */

#include "ControlMenu.h"
#include "GameOverScene.h"
#include "PlayScene.h"

#define BUTTON_SCALE 0.5

ControlMenu::ControlMenu() {
	// TODO Auto-generated constructor stub

}

ControlMenu::~ControlMenu() {
	// TODO Auto-generated destructor stub
}

bool ControlMenu::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,0))) {
        return false;
    }

    userData = GameController::getGameController()->getUserData();
    winSize = CCDirector::sharedDirector()->getWinSize();

    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height*1.5));

	upButton = CCMenuItemImage::create("button_arrow_normal.png" ,
                                                         "button_arrow_selected.png" , this , menu_selector(ControlMenu::upHandler));
	CCSize buttonSize = upButton->getContentSize();

	//upButton is on the left side
	upButton->setPosition(ccp(winSize.width - buttonSize.width/2 , buttonSize.height*1.5*BUTTON_SCALE));
	upButton->setRotation(180);
    upButton->setScale(BUTTON_SCALE);

	CCMenuItemImage * downButton = CCMenuItemImage::create("button_arrow_normal.png" ,
                                                           "button_arrow_selected.png" , this , menu_selector(ControlMenu::downHandler));
	//downButton is on the right side
	downButton->setPosition(ccp(winSize.width - buttonSize.width/2 + 5, buttonSize.height/2*BUTTON_SCALE));
    downButton->setScale(BUTTON_SCALE);

	CCArray * array = CCArray::createWithCapacity(2);

	array->addObject(upButton);

	array->addObject(downButton);

	menu = MenuForArrowButton::createWithArray(array);

    menu->ignoreAnchorPointForPosition(false);

	menu->registerTouchendHandler(this , menu_selector(ControlMenu::touchendHandler));

	addChild(menu);

    numFrame=0;
    score = 0;
    duration = userData->levelDuration;
    seconds = duration;

    startUpdateTime = false;

    initTimeLabel();
    initScoreLabel();

    return true;
}

void ControlMenu::upHandler(CCObject * sender){
	((PlayScene *)getParent())->upHandler(NULL);
}
void ControlMenu::downHandler(CCObject * sender){
	((PlayScene *)getParent())->downHandler(NULL);
}
void ControlMenu::touchendHandler(CCObject * sender){
	((PlayScene *)getParent())->touchendHandler(NULL);
}

void ControlMenu::initTimeLabel(){
    timeLabel = CCLabelTTF::create("0", "Helvetica", 64 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    timeLabel->setColor( ccc3(255, 10, 10) );
    timeLabel->setPosition( ccp(50, winSize.height - 50) );
    this->addChild(timeLabel);
}

void ControlMenu::initScoreLabel(){
    scoreLabel = CCLabelTTF::create("0", "Helvetica", 64 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    scoreLabel->setColor( ccc3(0, 0, 128) );
    scoreLabel->setPosition( ccp(winSize.width - 50, winSize.height - 50) );

    this->addChild(scoreLabel);
}

bool ControlMenu::isUpButtonSelected(){
	return upButton->isSelected();
}

void ControlMenu::startNewGame(){
	setPosition(ccp(winSize.width/2 , winSize.height/2));
	startUpdateTime=true;
}

void ControlMenu::updateGameTime()
{
    char ss[10];
    sprintf(ss, "%d", seconds);
    timeLabel->setString(ss);
}

void ControlMenu::updateScore()
{
    char ss[10];
    sprintf(ss, "%d", score);
    scoreLabel->setString(ss);
}

void ControlMenu::GameOver()
{
    userData->lastScore = score;
    GameOverScene *gameOverScene = GameOverScene::create();
	CCDirector::sharedDirector()->replaceScene( gameOverScene );
}

void ControlMenu::step(float dt){
    if (startUpdateTime==false) {
        return;
    }

    if(++numFrame % 60 == 0) {
        seconds--;
        updateGameTime();
    }

    if(seconds == 0)
    	GameOver();
}

void ControlMenu::doScore(){
	score++;
	updateScore();
	seconds += duration;
	updateGameTime();
}
