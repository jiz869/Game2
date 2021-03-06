#include "Player.h"

GPlayer::GPlayer() : width(0), height(0), state(WAIT), sprite(0)
{
	animRunRate = 1.0/8.0;
    animationJumpDown = 0;
    animationJumpUp = 0;
    animationWait = 0;
    designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
}

GPlayer::~GPlayer()
{
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

static bool PointInAABB(CCPoint &p, CCPoint pos, float w, float h)
{
    float sx = pos.x;
    float sy = pos.y;

    float l = sx;
    float r = sx + w - 1;
    float b = sy;
    float t = sy + h - 1;

    if( (p.x > l) && (p.x < r) && (p.y < t) && (p.y > b) )
        return true;
    return false;
}

//Customize to load player sprite
#if 0
CCSprite* GPlayer::CreatePlayerSprite()
{
    width=16;
    height=20;
    playerTexture = CCTextureCache::sharedTextureCache()->addImage("player.png");

    sprite = CCSprite::createWithTexture(this->playerTexture, CCRectMake(0,0, width, height));
    sprite->retain();
    sprite->setAnchorPoint(ccp(0,0));
    sprite->resetPlayerPosition();

    //load animations
    //jump up animation
    animationJumpUp = CCAnimation::create();
    animationJumpUp->retain();
    animationJumpUp->setDelayPerUnit(animRunRate);

    CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(5*width+1, 0, width, height));
    animationJumpUp->addSpriteFrame(frame);
    frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(6*width, 0, width, height));
    animationJumpUp->addSpriteFrame(frame);

    //wait animation
    animationWait = CCAnimation::create();
    animationWait->retain();
    animationWait->setDelayPerUnit(animRunRate);

    frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(112, 0, width, height));
    animationWait->addSpriteFrame(frame);
    //frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(128, 0, width, height));
    //animationWait->addSpriteFrame(frame);

    return sprite;
}
#endif

CCSprite* GPlayer::CreatePlayerSprite()
{
    width=20;
    height=32;
    playerTexture = CCTextureCache::sharedTextureCache()->addImage("sprites.png");
    playerTexture->retain();

    sprite = CCSprite::createWithTexture(this->playerTexture, CCRectMake(0,0, width, height));
    sprite->setAnchorPoint(ccp(0,0));
    sprite->setPosition(ccp(designSize.width / 2 , 0));

    //load animations
    //jump up animation
    animationJumpUp = CCAnimation::create();
    animationJumpUp->retain();
    animationJumpUp->setDelayPerUnit(animRunRate);

    CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(102, 36, width, height));
    animationJumpUp->addSpriteFrame(frame);
    frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(104, 2, width, height));
    animationJumpUp->addSpriteFrame(frame);

    //wait animation
    animationWait = CCAnimation::create();
    animationWait->retain();
    animationWait->setDelayPerUnit(animRunRate);

    frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(100,70, width, height));
    animationWait->addSpriteFrame(frame);
    //frame = CCSpriteFrame::createWithTexture(this->playerTexture, CCRectMake(128, 0, width, height));
    //animationWait->addSpriteFrame(frame);

    return sprite;
}

void GPlayer::Wait()
{
    sprite->stopAllActions();
    CCAnimate *aa = CCAnimate::create(animationWait);
    CCRepeatForever *rep = CCRepeatForever::create(aa);
    sprite->runAction(rep);
    state = WAIT;
    velocity = ccp(0.0, 0.0);
    //CCLog("set player state WAIT");
}

//Customize
void GPlayer::JumpUp()
{
    if( state != JMP_UP ) {
        sprite->stopAllActions();
        CCAnimate *aa = CCAnimate::create(animationJumpUp);
        CCRepeatForever *rep = CCRepeatForever::create(aa);
        sprite->runAction(rep);
        state = JMP_UP;
        velocity = ccp(0, speed);
        //CCLog("set player state JMP");
    }
}

void GPlayer::JumpDown()
{
    if (GetPlayerPosition().y <= 0) {
        return;
    }

    if( state != JMP_DOWN ) {
        sprite->stopAllActions();
        CCAnimate *aa = CCAnimate::create(animationJumpUp);
        CCRepeatForever *rep = CCRepeatForever::create(aa);
        sprite->runAction(rep);
        state = JMP_DOWN;
        velocity = ccp(0, -speed);
        //CCLog("set player state JMP_DOWN");
    }
}
//customize to control precision
void GPlayer::GetAABB(CCPoint &o, float &w, float &h)
{
    o = sprite->getPosition();
    o.x += 3;
    w = width - 3;
    h = height;
}

void GPlayer::SetPlayerPosition(float x, float y)
{
    sprite->setPosition(ccp(x,y));
}

//player collision test
bool GPlayer::SegmentsTest(CCPoint b, CCPoint t, CCPoint l, CCPoint r)
{
    if(l.y > b.y && l.y < t.y) {
        if(l.x < b.x && r.x > b.x){
            return true;
        }
    }

    return false;
}

bool GPlayer::RightBottomTest(GObject *obj)
{
    //collision test of player's right side and object's left side
    CCPoint player_pos;
    float player_w, player_h;
    this->GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x, pos.y+h-3);
    CCPoint p1(player_pos.x+5, player_pos.y+15);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+15);

    //if speed is very fast, tunneling could happen
    if( SegmentsTest(o1, o2, p1, p2) ) {
        return true;
    }

    return false;
}

bool GPlayer::RightTopTest(GObject *obj)
{
    //collision test of player's right side and object's left side
    CCPoint player_pos;
    float player_w, player_h;
    this->GetAABB(player_pos, player_w, player_h);

    CCPoint pos;
    float w, h;
    obj->GetAABB(pos, w, h);

    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x, pos.y+h-3);
    CCPoint p1(player_pos.x+5, player_pos.y+player_h-10);
    CCPoint p2(player_pos.x+player_w-12, player_pos.y+player_h-10);

    //if speed is very fast, tunneling could happen
    if( SegmentsTest(o1, o2, p1, p2) ) {
        return true;
    }

    return false;
}

bool GPlayer::CheckObjectCollision(GObject *obj)
{
    CCPoint pos;
    float w, h;
    this->GetAABB(pos, w, h);
    CCPoint o1(pos.x, pos.y);
    CCPoint o2(pos.x, pos.y+h-1);
    CCPoint o3(pos.x+w-1, pos.y);
    CCPoint o4(pos.x+w-1, pos.y+h-1);

    //PointInAABB(CCPoint &p, CCPoint pos, float w, float h)
    CCPoint objpos;
    float objw, objh;
    obj->GetAABB(objpos, objw, objh);

    //player agains object
    if( PointInAABB(o1, objpos, objw, objh) ||
        PointInAABB(o2, objpos, objw, objh) ||
        PointInAABB(o3, objpos, objw, objh) ||
        PointInAABB(o4, objpos, objw, objh) ) {
        return true;
    }

    //object against player
    CCPoint p1(objpos.x, objpos.y);
    CCPoint p2(objpos.x, objpos.y+objh-1);
    CCPoint p3(objpos.x+objw-1, objpos.y);
    CCPoint p4(objpos.x+objw-1, objpos.y+objh-1);
    if( PointInAABB(p1, pos, w, h) ||
        PointInAABB(p2, pos, w, h) ||
        PointInAABB(p3, pos, w, h) ||
        PointInAABB(p4, pos, w, h) ) {
        return true;
    }

    return false;
}

void GPlayer::Step(float dt)
{
    CCPoint pos = sprite->getPosition();
    pos = pos + velocity;
    sprite->setPosition(pos);
}

void GPlayer::ProcessEffect(ePlayerEffect effect)
{
    switch(effect) {
        case SPEED_UP:
            speed +=3;
            if(velocity.y > 0) {
            	velocity.y = speed;
            }else if(velocity.y < 0) {
            	velocity.y = -speed;
            }
            break;
        default:
            break;
    };
}


