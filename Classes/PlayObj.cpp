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

PlayerObj::PlayerObj() : movingState(WAIT)
{
    specials.reserve(MAX_SPECIAL_NUM);
}

PlayerObj::~PlayerObj(){
}

void PlayerObj::setData(PlaySceneData * data){
	this->data = data;
}

B2Sprite * PlayerObj::load(){

	GameObj::load(data->playerWaitImageName->getCString() , b2_dynamicBody , PLAYER);
	reset();
    return gameObj;
}

void PlayerObj::reset(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	setPosition(ccp(size.width/2 , gameObj->getContentSize().height/2+2));

    PlayScene * playScene = (PlayScene *)gameObj->getParent();

    if (playScene && playScene->isUpButtonSelected()) {
        return;
    }

    wait();
}

void PlayerObj::wait()
{
    gameObj->stopAllActions();
    movingState = WAIT;
    setVelocity(b2Vec2(0 , 0));
    CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(data->playerWaitImageName->getCString());
    gameObj->setDisplayFrame(frame);
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
        setVelocity(b2Vec2(0 , data->playerSpeed));
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
        setVelocity(b2Vec2(0 , -data->playerSpeed));
        //CCLog("set player state JMP_DOWN %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::speedUp(float delta){

    data->playerSpeed += delta;

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

	data->playerSpeed -= delta;

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
}

void PlayerObj::processContact(cocos2d::CCSprite *contact){
    if (contact->getTag() == UPPER_BOUNDARY || contact->getTag() == LOWER_BOUNDARY) {
        reset();
        return;
    }

    if (contact->getTag() == SPECIAL) {
        SpecialObj * specialObj = (SpecialObj *)contact->getUserData();
        beginWithSpecial(specialObj);

        return;
    }

    if (contact->getTag() == CAR) {
    	bool shouldPlayerReset = true;

    	int size = specials.size();
        for (int i = 0; i < size; i++) {
            if(specials[i]->hitByCar(this) == false){
            	shouldPlayerReset = false;
            }
        }

        //remove all specials when hit by car
        if(shouldPlayerReset == true){
        	reset();
        	removeAllSpecials();
        }
        return;
    }

}

void PlayerObj::removeAllSpecials(){

	int size = specials.size();

    for (int i = 0; i < size; i++) {
    	specials[i]->end(this);
    }
}

void PlayerObj::beginWithSpecial(SpecialObj * specialObj){

    if (hasSpecial(specialObj) || enoughSpecials()) {
        return;
    }

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
    delete specialObj;
}

void PlayerObj::removeSpecial(SpecialObj *specialObj){
	int size = specials.size();
    for (int i = 0; i < size; i++) {
        if (specials[i]->getSpecialId() == specialObj->getSpecialId()) {
            specials.erase(specials.begin() + i);
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
