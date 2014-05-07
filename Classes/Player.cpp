#include "Player.h"

GPlayer::GPlayer() : width(0), height(0), state(WAIT), sprite(0)
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
    state = WAIT;
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

void GPlayer::Step(float dt)
{
    CCPoint pos = sprite->getPosition();
    pos = pos + velocity;
    sprite->setPosition(pos);
}


