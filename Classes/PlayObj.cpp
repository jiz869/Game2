//
//  PlayObj.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayObj.h"
#include "SpecialObj.h"
#include "PlayScene.h"
#include "ControlMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

PlayerObj::PlayerObj() : movingState(WAIT)
{
    specials.reserve(MAX_SPECIAL_NUM);
    UserData * userData = GameController::getGameController()->getUserData();
    data = GameController::getGameController()->getPlaySceneData(userData->currentLevel);
    playerAccSpeed = data->playerAccSpeed;
    playerStopAccSpeed = data->playerStopAccSpeed;
}

PlayerObj::~PlayerObj(){
}

B2Sprite * PlayerObj::load(PlayerOrder order){

	GameObj::load(data->playerWaitImageName->getCString() , b2_dynamicBody , PLAYER);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
	float resetPosX;

	if(order == PlayerObj::LEFT){
	    resetPosX = size.width/2 - 30;
	}else if(order == PlayerObj::RIGHT){
	    resetPosX = size.width/2 + 30;
	}else{
	    resetPosX = size.width/2;
	}

	resetPos = ccp(resetPosX , gameObj->getContentSize().height/2+1);
	reset();
    return gameObj;
}

void PlayerObj::reset(){
	setPosition(resetPos);

    velocity = ccp(0,0);
    acc = ccp(0,0);
    wait();
}

void PlayerObj::wait()
{
    movingState = WAIT;
    acc = (velocity.y > 0) ? ccp(0,-playerStopAccSpeed) : ccp(0, playerStopAccSpeed);
    //CCLog("set player state WAIT");
}

//Customize
void PlayerObj::jumpUp()
{
    if( movingState != JMP_UP ) {
        gameObj->stopAllActions();
        CCAnimate * moveUp= CCAnimate::create(data->playerMoveAnim);
        gameObj->runAction(CCRepeatForever::create(moveUp));
        movingState = JMP_UP;
        acc = ccp(0, playerAccSpeed);
        //setVelocity(b2Vec2(0 , data->playerSpeed));
        //CCLog("set player state JMP %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::jumpDown()
{
    if (isBottom()) {
        return;
    }

    if( movingState != JMP_DOWN ) {
        gameObj->stopAllActions();
        CCAnimate * moveUp= CCAnimate::create(data->playerMoveAnim);
        gameObj->runAction(CCRepeatForever::create(moveUp));
        movingState = JMP_DOWN;
        acc = ccp(0, -playerAccSpeed);
        //setVelocity(b2Vec2(0 , -data->playerSpeed));
        //CCLog("set player state JMP_DOWN %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::speedUp(float delta){

	playerAccSpeed = playerStopAccSpeed = 10000;

//    data->playerSpeed += delta;

    if (movingState == WAIT) {
        return;
    }else if (movingState == JMP_UP){
        wait();
        jumpUp();
    }else{
        wait();
        jumpDown();
    }
}

void PlayerObj::slowDown(float delta){

    playerAccSpeed = data->playerAccSpeed;
    playerStopAccSpeed = data->playerStopAccSpeed;

//	data->playerSpeed -= delta;
//
    if (movingState == WAIT) {
        return;
    }else if (movingState == JMP_UP){
        wait();
        jumpUp();
    }else{
        wait();
        jumpDown();
    }
}

void PlayerObj::step(float dt){
	int size = specials.size();

    for (int i = 0; i < size; i++) {
        specials[i]->step(this);
    }

    velocity = velocity + acc;

    if(movingState == WAIT && velocity.y*acc.y >= 0) {
    	velocity = ccp(0,0);
    	acc = ccp(0,0);
    	gameObj->stopAllActions();
        CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(data->playerWaitImageName->getCString());
        gameObj->setDisplayFrame(frame);
    }else if( velocity.y > data->playerSpeed) {
    	velocity = ccp(0, data->playerSpeed);
    }else if( velocity.y < -data->playerSpeed) {
    	velocity = ccp(0, -data->playerSpeed);
    }

    setVelocity(b2Vec2(velocity.x , velocity.y));
}

bool PlayerObj::processContact(cocos2d::CCSprite *contact){
    if (contact->getTag() == UPPER_BOUNDARY || contact->getTag() == LOWER_BOUNDARY) {
        reset();
        return false;
    }

    if (contact->getTag() == SPECIAL) {
        SpecialObj * specialObj = (SpecialObj *)contact->getUserData();
        beginWithSpecial(specialObj);

        return false;
    }

    if (contact->getTag() == CAR) {
    	bool shouldPlayerReset = true;

    	int size = specials.size();
        for (int i = 0; i < size; i++) {
            if(specials[i]->hitByCar(this , contact) == false){
            	shouldPlayerReset = false;
            }
        }

        //remove all specials when hit by car
        if(shouldPlayerReset == true){
			AnimationData * animData = GameController::getGameController()->getAnimationData();
            SimpleAudioEngine::sharedEngine()->playEffect(animData->resetSoundImage->getCString());
            PlayScene * playScene = (PlayScene *)getParent();
            playScene->controlMenu->changeScore(-0.1 , false);
        	reset();
        	removeAllSpecials();
        }
        return shouldPlayerReset;
    }

    return false;
}

void PlayerObj::removeAllSpecials(){
    while(specials.size()){
    	vector<SpecialObj *>::iterator it = specials.begin();
    	(*it)->end(this);
    }
}

void PlayerObj::beginWithSpecial(SpecialObj * specialObj){

    if (hasSpecial(specialObj) || enoughSpecials()) {
        return;
    }

    specialObj->take();

    specials.push_back(specialObj);

    tagPlayer(specialObj);

    specialObj->begin(this);
}

void PlayerObj::tagPlayer(SpecialObj *specialObj){
    CCSprite * tag = CCSprite::createWithSpriteFrameName(specialObj->getSpecialData()->imageName->getCString());

    CCSize tagSize = tag->getContentSize();
    CCSize playerSize = gameObj->getContentSize();

    gameObj->addChild(tag);

    //Todo: need a better way and animation to tag player
    tag->setPosition(ccp(playerSize.width+(specials.size()-0.5)*tagSize.width , playerSize.height/2));

    tag->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.2, 1.5), CCScaleTo::create(0.2, 0.8)));
    tag->setTag(specialObj->getSpecialId());
}

bool PlayerObj::enoughSpecials(){
    return specials.size() == MAX_SPECIAL_NUM;
}

bool PlayerObj::hasSpecial(SpecialObj *specialObj){
	int size = specials.size();
    for (int i = 0; i < size; i++) {
        if (specials[i]->getSpecialId() == specialObj->getSpecialId()) {
            return true;
        }
    }

    return false;
}

void PlayerObj::endWithSpecial(SpecialObj *specialObj){
    gameObj->removeChildByTag(specialObj->getSpecialId());
    removeSpecial(specialObj);
    specialObj->unTake();
}

void PlayerObj::removeSpecial(SpecialObj *specialObj){
	int size = specials.size();
    for (int i = 0; i < size; i++) {
        if (specials[i]->getSpecialId() == specialObj->getSpecialId()) {
            specials.erase(specials.begin() + i);
            break;
        }
    }

    CCSize tagSize;
    CCSize playerSize = gameObj->getContentSize();

    size = specials.size();
    for (int i = 0; i < size; i++) {
        CCSprite * tag = (CCSprite *)gameObj->getChildByTag(specials[i]->getSpecialId());

        tagSize = tag->getContentSize();

        tag->setPosition(ccp(playerSize.width+(size-0.5)*tagSize.width , playerSize.height/2));
    }

}

CCNode * PlayerObj::getParent(){
    return gameObj->getParent();
}

void PlayerObj::setTag(int tag){
    gameObj->setTag(tag);
}
