#include "GameObject.h"

CCNode* GObject::Node()
{
	return (CCNode*)batchNode;
}

void GObject::GetAABB(CCPoint &o, float &w, float &h)
{
    o = batchNode->getPosition();
    w = width;
    h = height;
}

GObject::~GObject()
{
    if(batchNode) {
        batchNode->release();
        batchNode = 0;
    }
}

void GObject::SetObjectPosition(float x, float y)
{
    batchNode->setPosition(ccp(x, y));
}

void GObject::Step(float dt)
{
    CCPoint pos = batchNode->getPosition();
    CCPoint newpos = pos + velocity;
    if(state != OBJ_INACTIVE) {
        batchNode->setPosition( pos + velocity );
        if( newpos.x+width < 0 || newpos.y + height < 0 ) {
            state = OBJ_INACTIVE;
        }
    }
}

//nc - number of columns
//nr - number of rows
//fn - file name
//tile_xy - vector of x,y coordinates for corresponding tile
//tile_xy should contain corordinates in row based way
// tile_xy[nc*r + c]
void GObject::LoadTiles(int nc, int nr, float w, float h,
        char *fn, vector<CCPoint> &tile_xy, bool flipY)
{
    batchNode = CCSpriteBatchNode::create(fn);
    texture = batchNode->getTexture();
    batchNode->retain();
    batchNode->setAnchorPoint(ccp(0.0, 0.0));

    for(int r=0; r<nr; ++r) {
        for(int c=0; c<nc; ++c) {
            CCPoint xy = tile_xy[nc*r+c];
            CCSprite *tile = CCSprite::createWithTexture( texture, CCRectMake(xy.x, xy.y, w, h) );
            tile->setOpacity(160);
            tile->setAnchorPoint(ccp(0.0, 0.0));
            //relative position to batchNode
            tile->setPosition(ccp(c*w, r*h));
            tile->setFlipY(flipY);
            batchNode->addChild(tile);
        }
    }

    width = nc*w;
    height = nr*h;
}

void GObject::LoadCar1()
{
    vector<CCPoint> tiles;
    tiles.reserve(1);
    tiles[0] = ccp(0, 38 );
    LoadTiles(1, 1, 96, 36, "sprites.png", tiles, false);
}

void GObject::LoadCar2()
{
    vector<CCPoint> tiles;
    tiles.reserve(1);
    tiles[0] = ccp(0, 7 );
    LoadTiles(1, 1, 89, 45, "cars.png", tiles, false);
}

void GObject::Load(char* name)
{
    if(!name) return;

    if(strncmp(name, "car1", 50) == 0) {
        LoadCar1();
    }else if(strncmp(name, "car2", 50) == 0) {
        LoadCar2();
    }else {
        return;
    }

    strncpy(objName, name, 50);
    state = OBJ_LOADED;
}

bool GObject::InScreen(float screen_w, float screen_h)
{
    CCPoint pos = batchNode->getPosition();
    if( (pos.x + width < 0 || pos.y + height < 0) ||
        (pos.x > screen_w  || pos.y > screen_h ) )
    {
        return false;
    }
    return true;
}




