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

PlayerObj::PlayerObj() : movingState(WAIT) , frozen(false)
{
    specials.reserve(MAX_SPECIAL_NUM);
    badSpecials.reserve(MAX_BAD_SPECIAL_NUM);
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
    setVelocity(b2Vec2(velocity.x , velocity.y));
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
	if(frozen == true) return;

    if( movingState != JMP_UP ) {
    	if(gameObj->getActionByTag(PLAYER_MOVE) == NULL){
			CCRepeatForever * move = CCRepeatForever::create(CCAnimate::create(data->playerMoveAnim));
			move->setTag(PLAYER_MOVE);
			gameObj->runAction(move);
    	}
        movingState = JMP_UP;
        acc = ccp(0, playerAccSpeed);
        //setVelocity(b2Vec2(0 , data->playerSpeed));
        //CCLog("set player state JMP %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::jumpDown()
{
    if (isBottom() || frozen == true) {
        return;
    }

    if( movingState != JMP_DOWN ) {
    	if(gameObj->getActionByTag(PLAYER_MOVE) == NULL){
			CCRepeatForever * move = CCRepeatForever::create(CCAnimate::create(data->playerMoveAnim));
			move->setTag(PLAYER_MOVE);
			gameObj->runAction(move);
    	}
        movingState = JMP_DOWN;
        acc = ccp(0, -playerAccSpeed);
        //setVelocity(b2Vec2(0 , -data->playerSpeed));
        //CCLog("set player state JMP_DOWN %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::speedUp(float delta){

	playerAccSpeed = playerStopAccSpeed = 10000;

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

void PlayerObj::freeze(){
	frozen = true;
}

void PlayerObj::unfreeze(){
	frozen = false;
}


void PlayerObj::changeAcc(float up_delta, float down_delta){

	playerAccSpeed += up_delta;

    playerStopAccSpeed += down_delta;
}

void PlayerObj::resumeAcc(){

    playerAccSpeed = data->playerAccSpeed;
    playerStopAccSpeed = data->playerStopAccSpeed;

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

    size = badSpecials.size();

    for (int i = 0; i < size; i++) {
        badSpecials[i]->step(this);
    }

    if(frozen == true){
    	gameObj->stopActionByTag(PLAYER_MOVE);
    	return;
    }

    velocity = velocity + acc;

    if(movingState == WAIT && velocity.y*acc.y >= 0) {
    	velocity = ccp(0,0);
    	acc = ccp(0,0);
    	gameObj->stopActionByTag(PLAYER_MOVE);
    	if(gameObj->getActionByTag(PLAYER_BOMB) == NULL){
    		CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->
    				spriteFrameByName(data->playerWaitImageName->getCString());
    		gameObj->setDisplayFrame(frame);
    	}
    }else if( velocity.y > data->playerSpeed) {
    	velocity = ccp(0, data->playerSpeed);
    }else if( velocity.y < -data->playerSpeed) {
    	velocity = ccp(0, -data->playerSpeed);
    }

    setVelocity(b2Vec2(velocity.x , velocity.y));
}

bool PlayerObj::processContact(cocos2d::CCSprite *contact){
    if (contact->getTag() == UPPER_BOUNDARY || contact->getTag() == LOWER_BOUNDARY) {
    	if( contact->getTag() == UPPER_BOUNDARY ) {
    		PlayScene * playScene = (PlayScene *)getParent();
    		playScene->controlMenu->doScore();
    		SpecialObj * bomb = hasSpecial(BOMB);
    		if(bomb != NULL) endWithSpecial(bomb);
    	}
        reset();
        return false;
    }

    if (contact->getTag() == SPECIAL) {
        SpecialObj * specialObj = (SpecialObj *)contact->getUserData();
        beginWithSpecial(specialObj);

        return false;
    }

    if (contact->getTag() == CAR) {

    	int size = specials.size();
        for (int i = 0; i < size; i++) {
            if(specials[i]->hitByCar(this , contact) == false){
            	return false;
            }
        }

        CarObj * carObj = (CarObj *)contact->getUserData();
        if(carObj->isDoomed() == true) return false;

        //remove all specials when hit by car
		hitByCar();
        return true;
    }

    return false;
}

void PlayerObj::hitByCar(bool playDefaultSound){
    if(playDefaultSound == true){
        AnimationData * animData = GameController::getGameController()->getAnimationData();
        SimpleAudioEngine::sharedEngine()->playEffect(animData->resetSoundImage->getCString());
    }
	PlayScene * playScene = (PlayScene *)getParent();
	playScene->controlMenu->changeScore(-2 , false);
	reset();
	removeAllSpecials();
}

void PlayerObj::removeAllSpecials(){
    while(specials.size()){
    	vector<SpecialObj *>::iterator it = specials.begin();
    	(*it)->end(this);
    }
}

void PlayerObj::removeAllBadSpecials(){
    while(badSpecials.size()){
    	vector<SpecialObj *>::iterator it = badSpecials.begin();
    	(*it)->end(this);
    }
}

void PlayerObj::beginWithSpecial(SpecialObj * specialObj){

    if (hasSpecial(specialObj->getSpecialId()) != NULL || enoughSpecials(specialObj->getSpecialId())) {
    	if(specialObj->getSpecialId() > SPECIAL_NUM){
    		hitByCar();
    	}
        return;
    }

    if (hasSpecial(STRONG) != NULL && specialObj->getSpecialId() > SPECIAL_NUM) {
        specialObj->runAction(CCBlink::create(2, 10));
        return;
    }

    if (hasSpecial(BLESS) != NULL && specialObj->getSpecialId() > SPECIAL_NUM) {
//    	SpecialData * data = GameController::getGameController()->getSpecialData(BLESS);
//    	if(toss(data->userData1) == false) hitByCar();
//    	else specialObj->runAction(CCBlink::create(2, 10));
    	hitByCar();
        return;
    }

    if (hasSpecial(CURSE) != NULL && specialObj->getSpecialId() < SPECIAL_NUM
    		&& specialObj->getSpecialId() != BLESS) {
        return;
    }

    specialObj->take();

    if(specialObj->getSpecialId() > SPECIAL_NUM){
    	badSpecials.push_back(specialObj);
    }else{
    	specials.push_back(specialObj);
    }

    tagPlayer(specialObj);

    specialObj->begin(this);
}

void PlayerObj::tagPlayer(SpecialObj *specialObj){
    CCSprite * tag = CCSprite::createWithSpriteFrameName(specialObj->getSpecialData()->imageName->getCString());

    CCSize tagSize = tag->getContentSize();
    CCSize playerSize = gameObj->getContentSize();

    gameObj->addChild(tag);

    if(specialObj->getSpecialId() > SPECIAL_NUM){
    	tag->setPosition(ccp(-(badSpecials.size()-0.5)*tagSize.width , playerSize.height/2));
    }else{
    //Todo: need a better way and animation to tag player
    	tag->setPosition(ccp(playerSize.width+(specials.size()-0.5)*tagSize.width , playerSize.height/2));
    }

    tag->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.2, 1.5), CCScaleTo::create(0.2, 0.8)));
    tag->setTag(specialObj->getSpecialId());
}

bool PlayerObj::enoughSpecials(int specialId){
	if(specialId > SPECIAL_NUM){
		return badSpecials.size() == MAX_BAD_SPECIAL_NUM;
	}else{
		return specials.size() == MAX_SPECIAL_NUM;
	}
}

SpecialObj * PlayerObj::hasSpecial(int specialId){
	if(specialId < SPECIAL_NUM){
		int size = specials.size();
	    for (int i = 0; i < size; i++) {
	        if (specials[i]->getSpecialId() == specialId) {
	            return specials[i];
	        }
	    }

	    return NULL;
	}else{
		int size = badSpecials.size();
	    for (int i = 0; i < size; i++) {
	        if (badSpecials[i]->getSpecialId() == specialId) {
	            return badSpecials[i];
	        }
	    }

	    return NULL;
	}
}

void PlayerObj::endWithSpecial(SpecialObj *specialObj){
    gameObj->removeChildByTag(specialObj->getSpecialId());
    removeSpecial(specialObj);
    specialObj->unTake();
}

void PlayerObj::removeSpecial(SpecialObj *specialObj){
	if(specialObj->getSpecialId() > SPECIAL_NUM){
		int size = badSpecials.size();
		for (int i = 0; i < size; i++) {
			if (badSpecials[i]->getSpecialId() == specialObj->getSpecialId()) {
				badSpecials.erase(badSpecials.begin() + i);
				break;
			}
		}

		CCSize tagSize;
		CCSize playerSize = gameObj->getContentSize();

		size = badSpecials.size();
		for (int i = 0; i < size; i++) {
			CCSprite * tag = (CCSprite *)gameObj->getChildByTag(badSpecials[i]->getSpecialId());

			tagSize = tag->getContentSize();

			tag->setPosition(ccp(-(size-0.5)*tagSize.width , playerSize.height/2));
		}
	}else{
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
}

CCNode * PlayerObj::getParent(){
    return gameObj->getParent();
}

void PlayerObj::setTag(int tag){
    gameObj->setTag(tag);
}
