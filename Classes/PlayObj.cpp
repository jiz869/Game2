//
//  PlayObj.cpp
//  crossRoadMain
//
//  Created by Min Li on 2014-05-15.
//
//

#include "PlayObj.h"

PlayerObj::PlayerObj() : shouldReset(false) , movingState(WAIT)
{

}

PlayerObj::~PlayerObj(){
    waitFrameName->release();
}

B2Sprite * PlayerObj::load(const char * name , b2BodyType type , SpriteTag tag){

    waitFrameName = CCString::createWithFormat("%s", name);
    waitFrameName->retain();
	GameObj::load(name , type , tag);
	reset();
    return gameObj;
}

void PlayerObj::reset(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	setPosition(ccp(size.width/2 , gameObj->getContentSize().height/2));
	wait();
	shouldReset = false;
}

void PlayerObj::wait()
{
    gameObj->stopAllActions();
    movingState = WAIT;
    setVelocity(ccp(0 , 0));
    CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(waitFrameName->getCString());
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
        setVelocity(ccp(0 , speed));
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
        setVelocity(ccp(0 , -speed));
        //CCLog("set player state JMP_DOWN %f %f", gameObj->getB2Body()->GetPosition().x , gameObj->getB2Body()->GetPosition().y);
    }
}

void PlayerObj::resetNextUpdate(bool shouldReset){
	this->shouldReset = shouldReset;
}

void PlayerObj::step(float dt){
	if(shouldReset){
		reset();
	}
}
