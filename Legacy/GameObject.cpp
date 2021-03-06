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
    if(strncmp(objName, "car1", 50) == 0) {
    	o.x += 9;
    }
}

GObject::~GObject()
{
    if(batchNode) {
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
    if(state == OBJ_ACTIVE) {
        batchNode->setPosition( pos + velocity );
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

void GObject::LoadSpeedUpCar()
{
    vector<CCPoint> tiles;
    tiles.reserve(1);
    tiles[0] = ccp(0, 0 );
    LoadTiles(1, 1, 16, 20, "player.png", tiles, false);
}

void GObject::Load(char* name)
{
    if(!name) return;

    if(strncmp(name, "car1", 50) == 0) {
        LoadCar1();
        carType = CAR_NORMAL;
    }else if(strncmp(name, "car2", 50) == 0) {
        LoadCar2();
        carType = CAR_NORMAL;
    }else if(strncmp(name, "speed_up_player", 50) == 0) {
        LoadSpeedUpCar();
        carType = CAR_SPEED_UP_PLAYER;
    }else {
        carType = CAR_INVALID;
        return;
    }

    strncpy(objName, name, 50);
    state = OBJ_LOADED;
}

bool GObject::InScreen(float screen_w, float screen_h)
{
	screen_w = screen_w*2;	//virtual size
	screen_h = screen_h*2;
    CCPoint pos = batchNode->getPosition();
    if( (pos.x + width < -screen_w || pos.y + height < -screen_h) ||
        (pos.x > screen_w  || pos.y > screen_h ) )
    {
        return false;
    }
    return true;
}




