/*
 * ControlMenu.cpp
 *
 *  Created on: May 23, 2014
 *      Author: mli
 */

#include "ControlMenu.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "StartMenuScene.h"

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

    initMisc();
    initMenu();
    initBloodBar();
    initScoreLabel();
    initLevelSplash();

    return true;
}

void ControlMenu::initMisc(){
    userData = GameController::getGameController()->getUserData();
    winSize = CCDirector::sharedDirector()->getWinSize();

    ignoreAnchorPointForPosition(false);

    setPosition(ccp(winSize.width/2 , winSize.height/2));

    numFrame=0;
    score = 0;
    initDuration = userData->initDuration;
    maxDuration = userData->maxDuration;
    durationIncrease = userData->durationIncrease;
    seconds = initDuration;

    startUpdateTime = false;
}

void ControlMenu::initMenu(){
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

    pauseAndPlay = CCMenuItemImage::create("pause.png" ,
            "play.png" , this , menu_selector(ControlMenu::pauseAndPlayHandler));

    pauseAndPlay->setScale(0.8);

    //pauseAndPlay->setAnchorPoint(ccp(1.5 , 0.5));

    pauseAndPlay->setPosition(ccp(winSize.width * 0.875 , winSize.height - 50));

    pauseAndPlay->setTag(PAUSE_PLAY);

    CCArray * array = CCArray::createWithCapacity(CONTROL_BUTTON_MAX);

    array->addObject(upButton);

    array->addObject(downButton);

    array->addObject(pauseAndPlay);

    menu = MenuForArrowButton::createWithArray(array);

    menu->ignoreAnchorPointForPosition(false);

    menu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));

    menu->registerTouchendHandler(this , menu_selector(ControlMenu::touchendHandler));

    addChild(menu);
}

void ControlMenu::initLevelSplash(){
    goSplash = CCSprite::create("go.png");
    goSplash->setPosition( ccp(winSize.width/2, winSize.height/2) );
    addChild(goSplash);
    goSplash->setScale(0);

    gameSplash = CCSprite::create("game.png");
    gameSplash->setPosition(ccp(winSize.width/4, winSize.height*0.5));
    addChild(gameSplash);
    gameSplash->setScale(0.0);

    overSplash = CCSprite::create("over.png");
    overSplash->setPosition(ccp(winSize.width*0.75, winSize.height*0.5));
    addChild(overSplash);
    overSplash->setScale(0.0);

    levelSplash = CCSprite::create("level.png");
    levelSplash->setPosition(ccp(winSize.width/4, winSize.height*1.5));
    addChild(levelSplash);
    levelSplash->setScale(0.6);

    upSplash = CCSprite::create("up.png");
    upSplash->setPosition(ccp(winSize.width*0.75, winSize.height*1.5));
    addChild(upSplash);
    upSplash->setScale(0.6);

    showGo();
}

void ControlMenu::showGo(){
    goSplash->runAction( CCSequence::create(CCScaleTo::create(0.8, 1.0) , CCDelayTime::create(0.6) ,CCCallFunc::create(this, callfunc_selector(ControlMenu::startNewGame)), NULL));
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
	emptyBar->setPosition(ccp(winSize.width/4, winSize.height - 50));
	addChild(emptyBar);

	bloodBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("blood_bar.png"));
	bloodBar->setType(kCCProgressTimerTypeBar);
	bloodBar->setPosition(ccp(winSize.width/4, winSize.height - 50));
	bloodBar->setBarChangeRate(ccp(1,0));
	bloodBar->setMidpoint(ccp(0 , 1));
	updateGameTime();
	addChild(bloodBar);
}

void ControlMenu::initScoreLabel(){
    CCString * gemName = CCString::createWithFormat("gem%d.png", userData->currentLevel);
    gem = CCSprite::createWithSpriteFrameName(gemName->getCString());
    gem->setScale(0.8);
    gem->setPosition(ccp(winSize.width * 0.625 , winSize.height-50));
    addChild(gem);

    if(userData->currentLevel < userData->maxLevel){
        gemName = CCString::createWithFormat("gem%d.png", userData->currentLevel + 1);
        gemLevelup = CCSprite::createWithSpriteFrameName(gemName->getCString());
        gemLevelup->setScale(0.8);
        gemLevelup->setPosition(ccp(winSize.width * 0.625 , winSize.height-50));
        gemLevelup->setOpacity(0);
        addChild(gemLevelup);
    }

    scoreLabel = CCLabelTTF::create("0", "Verdana-Bold", 64 );
    scoreLabel->setColor( ccc3(168, 0, 0) );
    scoreLabel->setPosition( ccp(winSize.width * 0.75, winSize.height - 50) );
    addChild(scoreLabel);
}

bool ControlMenu::isUpButtonSelected(){
	return upButton->isSelected();
}

void ControlMenu::startNewGame(){
    goSplash->setVisible(false);
	menu->setPosition(ccp(winSize.width/2 , winSize.height/2));
	startUpdateTime=true;
}

void ControlMenu::updateGameTime()
{
	if(seconds > maxDuration){
		seconds = maxDuration;
	}
    bloodBar->setPercentage((float)seconds/maxDuration * 100);
}

void ControlMenu::updateScore(bool isGood)
{
    char ss[10];
    sprintf(ss, "%.1f", score);
    scoreLabel->setString(ss);
    if(!isGood) return;
    scoreLabel->runAction(CCSequence::create(CCScaleTo::create(0.5 , 2.5) , CCScaleTo::create(0.5 , 1.0), NULL));
	AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->scoreSoundImage->getCString());
}

void ControlMenu::GameOver()
{
    status=OVER;
    gameSplash->runAction(CCSequence::create(CCScaleTo::create(0.8, 0.6) ,CCCallFunc::create(this, callfunc_selector(ControlMenu::showOver)), NULL));
    menu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    GameController::getGameController()->setLastScore(score);
}

void ControlMenu::showOver(){
    overSplash->runAction(CCSequence::create(CCScaleTo::create(0.8, 0.6) , CCDelayTime::create(1) , CCCallFunc::create(this, callfunc_selector(ControlMenu::nextScene)), NULL));
}

void ControlMenu::nextScene(){
    if (status == OVER) {
        CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
    }else if (status == LEVEL_UP){
        CCDirector::sharedDirector()->replaceScene(PlayScene::scene());
    }
}

void ControlMenu::step(float dt){
    if (startUpdateTime==false) {
        return;
    }

    if(++numFrame % 60 == 0) {
        seconds--;
        updateGameTime();
    }

    if(seconds <= 0 && status != OVER)
    	GameOver();
}

void ControlMenu::doScore(){
    if (status != PLAY) {
        return;
    }
	score+=1;
	updateScore(true);
    if (userData->levels[userData->currentLevel] != 0 &&
        score >= userData->levels[userData->currentLevel]) {
        levelUp();
    }
	seconds += durationIncrease;
	updateGameTime();
}

void ControlMenu::levelUp(){
    status=LEVEL_UP;
    levelSplash->runAction(CCSequence::create(CCMoveTo::create(0.8, ccp(winSize.width/4, winSize.height*0.5)) ,CCCallFunc::create(this, callfunc_selector(ControlMenu::showUp)), NULL));
    gem->runAction(CCFadeOut::create(0.8));
    menu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    AnimationData * animData = GameController::getGameController()->getAnimationData();
    SimpleAudioEngine::sharedEngine()->playEffect(animData->levelupSoundImage->getCString());
    GameController::getGameController()->levelUp();
}

void ControlMenu::showUp(){
    upSplash->runAction(CCSequence::create(CCMoveTo::create(0.8, ccp(winSize.width*0.75, winSize.height*0.5)) , CCDelayTime::create(1) , CCCallFunc::create(this, callfunc_selector(ControlMenu::nextScene)), NULL));
    gemLevelup->runAction(CCFadeIn::create(0.8));
}

void ControlMenu::changeGameTime(int delta){
	seconds += delta;
	updateGameTime();
}

void ControlMenu::changeScore(float delta , bool isGood){
    score += delta;
    updateScore(isGood);
}

void ControlMenu::increaseDuration(int delta){
    durationIncrease+=delta;
}

void ControlMenu::resumeDuration(){
    durationIncrease = userData->initDuration;
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
