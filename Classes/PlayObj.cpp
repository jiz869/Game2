//
//  PlayObj.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayObj.h"
#include "SpecialObj.h"

PlayerObj::PlayerObj() : movingState(WAIT)
{
    specials.reserve(MAX_SPECIAL_NUM);
}

PlayerObj::~PlayerObj(){
}

B2Sprite * PlayerObj::load(){

	GameObj::load(animationData->playerWaitImageName->getCString() , b2_dynamicBody , PLAYER);
	reset();
    return gameObj;
}

void PlayerObj::reset(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	setPosition(ccp(size.width/2 , gameObj->getContentSize().height/2));
	wait();
}

void PlayerObj::wait()
{
    gameObj->stopAllActions();
    movingState = WAIT;
    setVelocity(b2Vec2(0 , 0));
    CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(animationData->playerWaitImageName->getCString());
    gameObj->setDisplayFrame(frame);
    //CCLog("set player state WAIT");
}

//Customize
void PlayerObj::jumpUp()
{
    if( movingState != JMP_UP ) {
        gameObj->stopAllActions();
        CCAnimate * moveUp= CCAnimate::create(animationData->playerMoveAnim);
        gameObj->runAction(CCRepeatForever::create(moveUp));
        movingState = JMP_UP;
        setVelocity(b2Vec2(0 , speed));
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
        CCAnimate * moveUp= CCAnimate::create(animationData->playerMoveAnim);
        gameObj->runAction(CCRepeatForever::create(moveUp));
        movingState = JMP_DOWN;
        setVelocity(b2Vec2(0 , -speed));
        //CCLog("set player state JMP_DOWN %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::speedUp(float delta){
    speed += delta;
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
    speed -= delta;
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
    for (int i = 0; i < specials.size(); i++) {
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
        contactWithSpecial(specialObj);
        
        return;
    }
    
    if (contact->getTag() == CAR) {
        reset();
        return;
    }
    
}

void PlayerObj::contactWithSpecial(SpecialObj * specialObj){
    
    if (hasSpecial(specialObj) || enoughSpecials()) {
        return;
    }
    
    specials.push_back(specialObj);
    
    tagPlayer(specialObj);
    
    specialObj->begin(this);
}

void PlayerObj::tagPlayer(SpecialObj *specialObj){
    CCSprite * tag = CCSprite::createWithSpriteFrameName(specialObj->getSpecialData()->imageName->getCString());
    gameObj->addChild(tag);
    tag->setAnchorPoint(ccp(0 , 0));
    tag->setFlipX(true);
    
    //Todo: need a better way and animation to tag player
    if (specials.size() == 1) {
        tag->setPosition(ccp(tag->getContentSize().width , gameObj->getContentSize().height/2));
    }else if (specials.size() == 2) {
        tag->setPosition(ccp(tag->getContentSize().width , gameObj->getContentSize().height/2 - tag->getContentSize().height*0.8));
    }
    
    tag->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.2, 1.5), CCScaleTo::create(0.2, 0.8)));
    tag->setTag(specialObj->getSpecialId());
}

bool PlayerObj::enoughSpecials(){
    return specials.size() == MAX_SPECIAL_NUM;
}

bool PlayerObj::hasSpecial(SpecialObj *specialObj){
    for (int i = 0; i < specials.size(); i++) {
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
    for (int i = 0; i < specials.size(); i++) {
        if (specials[i]->getSpecialId() == specialObj->getSpecialId()) {
            specials.erase(specials.begin() + i);
        }
    }
}
