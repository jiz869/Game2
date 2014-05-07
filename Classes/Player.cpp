#include "Player.h"

GPlayer::GPlayer() : width(0), height(0), state(WAIT), sprite(0), applyGravity(false)
{
	animRunRate = 1.0/8.0;
    designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
}

GPlayer::~GPlayer()
{
    sprite->release();
    playerTexture->release();
    if(animationWait) {
        animationWait->release();
        animationWait = 0;
    }
    if(animationJumpUp) {
        animationJumpUp->release();
        animationJumpUp = 0;
    }
    if(animationJumpDown) {
        animationJumpDown = 0;
        animationJumpDown->release();
    }
}

//Customize
CCSprite* GPlayer::CreatePlayerSprite()
{
    width=50;
    height=34;
    playerTexture = CCTextureCache::sharedTextureCache()->addImage("");
    playerTexture->retain();

    sprite = CCSprite::createWithTexture(this->playerTexture, CCRectMake(0,0, width, height));
    sprite->retain();
    sprite->setAnchorPoint(ccp(0,0));
    sprite->setPosition(INIT_POS);

    //load wait animation
    animationWait = CCAnimation::create();
    animationWait->retain();
    animationWait->setDelayPerUnit(animRunRate);

    for(int i=0; i<8; ++i) {
        CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(i*width, 0, width, height));
        animationWait->addSpriteFrame(frame);
    }

    return sprite;
}

void GPlayer::Wait()
{
    sprite->stopAllActions();
    CCAnimate *aa = CCAnimate::create(animationWait);
    CCRepeatForever *rep = CCRepeatForever::create(aa);
    sprite->runAction(rep);
    state = Wait;
    velocity = ccp(0.0, 0.0);
    //CCLog("set player state WAIT");
}

//Customize
void GPlayer::JumpUp()
{
    //sprite->stopAllActions();
    //sprite->setTextureRect( CCRectMake(5*width, 1*height+1, width, height) );
    //state = (state == JMP1) ? JMP2 : JMP1;
    //CCLog("set player state JMPx");
}

//customize to control precision
void GPlayer::GetAABB(CCPoint &o, float &w, float &h)
{
    o = sprite->getPosition();
    w = width;
    h = height;
}

void GPlayer::SetPlayerPosition(float x, float y)
{
    sprite->setPosition(ccp(x,y));
}

void GPlayer::Step(float dt)
{
    CCPoint oldVelocity = velocity;
    CCPoint oldPosition = GetPlayerPosition();

    CCPoint pos = sprite->getPosition();
    pos = pos + velocity;
    sprite->setPosition(pos);

    if(applyGravity)
    	velocity = velocity + gravity;

    if( oldVelocity.y == 0.0 && velocity.y != 0.0 && state == WAIT) {
        state = JMP1;
        //CCLog("set player state JMP1");
    }

    //check and set gravity
    UpdateGravity();

    //velocity and gravity have the same direction

    if( (oldPosition.y <= designSize.height/2 && pos.y > designSize.height/2) ||
        (oldPosition.y > designSize.height/2 && pos.y <= designSize.height/2) ) {
        SwitchGravity();
        JumpDown();
        CCLog("flip gravity!");
    }else if(oldVelocity.y * velocity.y <= 0.0 && velocity.y * gravity.y > 0.0) {
    	//CCLog("player's old velocity.y %f new velocity.y %f)", oldVelocity.y, velocity.y);
        JumpDown();
    }

    //CCLog("player step position(%f, %f) velocity(%f, %f) gravity(%f, %f) state %d",
    //		pos.x, pos.y, velocity.x, velocity.y, gravity.x, gravity.y, state);
}


