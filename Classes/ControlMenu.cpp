/*
 * ControlMenu.cpp
 *
 *  Created on: May 23, 2014
 *      Author: mli
 */

#include "ControlMenu.h"
#include "GameOverScene.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define BUTTON_SCALE 0.6

ControlMenu::ControlMenu() : status(PLAY) {
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

    upButton->setRotation(180);
    upButton->setScale(BUTTON_SCALE);
    upButton->setTag(UP);

	downButton = CCMenuItemImage::create("button_arrow_normal.png" ,
                                                           "button_arrow_selected.png" , this , menu_selector(ControlMenu::downHandler));

    downButton->setScale(BUTTON_SCALE);
    downButton->setTag(DOWN);

    changeControllerPosition(userData->controllerPosition);

    CCMenuItemImage * pauseAndPlay = CCMenuItemImage::create("pause.png" ,
            "play.png" , this , menu_selector(ControlMenu::pauseAndPlayHandler));

    pauseAndPlay->setScale(0.5);

    pauseAndPlay->setAnchorPoint(ccp(1.1 , 1.05));

    pauseAndPlay->setPosition(ccp(winSize.width , winSize.height));

    pauseAndPlay->setTag(PAUSE_PLAY);

    CCArray * array = CCArray::createWithCapacity(CONTROL_BUTTON_MAX);

	array->addObject(upButton);

	array->addObject(downButton);

	array->addObject(pauseAndPlay);

	menu = MenuForArrowButton::createWithArray(array);

    menu->ignoreAnchorPointForPosition(false);

	menu->registerTouchendHandler(this , menu_selector(ControlMenu::touchendHandler));

	addChild(menu);

    numFrame=0;
    score = 0;
    duration = userData->levelDuration;
    maxDuration = 30;
    seconds = duration;

    startUpdateTime = false;

    initBloodBar();
    initScoreLabel();

    return true;
}

void ControlMenu::pauseAndPlayHandler(CCObject * sender){
	CCMenuItem * item = (CCMenuItem *)sender;

	if(status == PLAY){
		status = PAUSE;
		item->selected();
		pauseGame();
	}else{
		status = PLAY;
		item->unselected();
		resumeGame();
	}
}

void ControlMenu::pauseGame(){
	((PlayScene *)getParent())->freezeAllLanes();
	upButton->setEnabled(false);
	downButton->setEnabled(false);
	startUpdateTime = false;
    if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
    	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
}

void ControlMenu::resumeGame(){
	((PlayScene *)getParent())->restartAllLanes();
	upButton->setEnabled(true);
	downButton->setEnabled(true);
	startUpdateTime = true;
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
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

void ControlMenu::initBloodBar(){
	CCSprite * emptyBar = CCSprite::createWithSpriteFrameName("empty_bar.png");
	emptyBar->setPosition(ccp(100, winSize.height - 50));
	emptyBar->setScaleX(0.6);
	addChild(emptyBar);

	bloodBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("blood_bar.png"));
	bloodBar->setType(kCCProgressTimerTypeBar);
	bloodBar->setPosition(ccp(100, winSize.height - 50));
	bloodBar->setBarChangeRate(ccp(1,0));
	bloodBar->setMidpoint(ccp(0 , 1));
	bloodBar->setScaleX(0.6);
	updateGameTime();
	addChild(bloodBar);
}

void ControlMenu::initScoreLabel(){
    scoreLabel = CCLabelTTF::create("0", "Helvetica", 64 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    scoreLabel->setColor( ccc3(0, 0, 128) );
    scoreLabel->setPosition( ccp(300, winSize.height - 50) );
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
	if(seconds > maxDuration){
		seconds = maxDuration;
	}
    bloodBar->setPercentage((float)seconds/maxDuration * 100);
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

    if(seconds <= 0)
    	GameOver();
}

void ControlMenu::doScore(){
	score++;
	updateScore();
	seconds += duration;
	updateGameTime();
}

void ControlMenu::changeGameTime(int delta){
	seconds += delta;
	updateGameTime();
}

void ControlMenu::increaseDuration(int delta){
    duration+=delta;
}

void ControlMenu::resumeDuration(){
    duration = userData->levelDuration;
}

void ControlMenu::changeControllerPosition(CheckboxType type){
    CCSize buttonSize = upButton->getContentSize();
    switch (type) {
        case LEFT:
            //upButton is on the left side
            upButton->setPosition(ccp(buttonSize.width/2 , buttonSize.height*1.5*BUTTON_SCALE));
            //downButton is on the right side
            downButton->setPosition(ccp(buttonSize.width/2 + 15, buttonSize.height/2*BUTTON_SCALE));
            break;

        case RIGHT:
            //upButton is on the left side
            upButton->setPosition(ccp(winSize.width - buttonSize.width/2 , buttonSize.height*1.5*BUTTON_SCALE));
            //downButton is on the right side
            downButton->setPosition(ccp(winSize.width - buttonSize.width/2 + 15, buttonSize.height/2*BUTTON_SCALE));
            break;

        case SIDE:
            //upButton is on the left side
            upButton->setPosition(ccp(buttonSize.width/2 , buttonSize.height/2*BUTTON_SCALE));
            //downButton is on the right side
            downButton->setPosition(ccp(winSize.width - buttonSize.width/2, buttonSize.height/2*BUTTON_SCALE));
            break;

        default:
            break;
    }
}
