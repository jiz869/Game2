#include <algorithm>
#include "GameWorld.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;
static bool PointInSprite(CCPoint &p, CCSprite &sprite);

static int getRandom(int low, int high)
{
	if ( low - high < 0x10000L )
	      return low + ( ( random() >> 8 ) % ( high + 1 - low ) );

	return low + ( random() % ( high + 1 - low ) );
}

GameWorld::~GameWorld()
{
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

GameWorld::GameWorld()
{

}

CCScene* GameWorld::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GameWorld *layer = GameWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameWorld::init()
{
	bool bRet = false;
	do
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );
		designSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();

#if 0
		//Add a menu item with "X" image, which is clicked to quit the program.
		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(GameWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to GameWorld layer as a child layer.
		this->addChild(pMenu, 2);
#endif

		this->setTouchEnabled(true);
        this->schedule( schedule_selector(GameWorld::step) );

        //add player
        this->addChild( player.CreatePlayerSprite(), 0.1 );
        player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
        player.Wait();

        //load level
        LoadMap("level1");
        bRet = true;

        // play background music
        //SimpleAudioEngine::sharedEngine()->playBackgroundMusic(BACKGROUND_MUSIC1, true);
	} while (0);

	return bRet;
}

void GameWorld::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->popScene();
#endif
}

// cpp with cocos2d-x
void GameWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	//CCLog("++++++++after  x:%f, y:%f", location.x, location.y);
    player.Wait();
}

void GameWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
    if(location.y > designSize.height/2) {
        player.JumpUp();
    }else{
        player.JumpDown();
    }
}

void GameWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

}

GObject* GameWorld::GetObject(vector<GObject*> &objs, char *name)
{
    if(objs.size() > 0) {
        for(int i=0; i < objs.size(); ++i) {
            if(objs[i]->state == OBJ_INACTIVE && (strncmp(objs[i]->objName, name, 50) == 0) ) {
            	objs[i]->state = OBJ_ACTIVE;
                return objs[i];
            }
        }
    }

    CCLog("creat a new %s object", name);
    GObject *obj;
    obj =  new GObject();
    obj->Load(name);
    objs.push_back(obj);
    this->addChild(obj->Node());
    obj->state = OBJ_ACTIVE;
    return obj;
}

static bool CompareX2(GObject* a, GObject* b)
{
    CCPoint pos_a, pos_b;
    float wa, wb;
    float ha, hb;
    a->GetAABB( pos_a, wa, hb);
    b->GetAABB( pos_b, wb, hb);

    return ( (pos_a.x+wa) < (pos_b.x+wb) );
}

void GameWorld::RenewMap()
{
    //lane1
    sort(lane1.begin(), lane1.end(), CompareX2);
    GObject *last_car = lane1.back();
    CCPoint pos;
    float bw, bh;
    last_car->GetAABB(pos, bw ,bh);
    if((pos.x+bw) < (designSize.width - 200)) {
        GObject* car;
        car = GetObject(lane1, "car1");
        car->SetObjectPosition(designSize.width, 200);

        CCPoint v = ccp( -2, 0);
        car->SetVelocity(v);
    }

    //lane2
    sort(lane2.begin(), lane2.end(), CompareX2);
    last_car = lane2.front();
    last_car->GetAABB(pos, bw ,bh);
    if(pos.x > 250) {
        GObject* car;
        car = GetObject(lane2, "car2");
        car->SetObjectPosition(0, 100);

        CCPoint v = ccp( 3, 0);
        car->SetVelocity(v);
    }
}

//customize: level loading
void GameWorld::LoadMap(char *name)
{
    //name: used for different levels

    //lane1
    GObject* car;
    car = GetObject(lane1, "car1");
    car->SetObjectPosition(designSize.width, 200);

    CCPoint v = ccp( -2, 0);
    car->SetVelocity(v);

    //lane2
    car = GetObject(lane2, "car2");
    car->SetObjectPosition(0, 100);

    v = ccp( 3, 0);
    car->SetVelocity(v);
}

static bool PointInSprite(CCPoint &p, CCSprite &sprite)
{
    float sx = sprite.getPositionX();
    float sy = sprite.getPositionY();
    float w = sprite.getContentSize().width;
    float h = sprite.getContentSize().height;
    float l = sx - w/2;
    float r = sx + w/2;
    float b = sy - h/2;
    float t = sy + h/2;

    if( (p.x > l) && (p.x < r) && (p.y < t) && (p.y > b) )
        return true;
    return false;
}

void GameWorld::CheckCollision()
{
    //lane1
    for(int i=0; i<lane1.size(); ++i) {
        if( lane1[i]->state == OBJ_ACTIVE ) {
            if( player.CheckObjectCollision(lane1[i]) ) {
                player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
            }
        }
    }

    //lane2
    for(int i=0; i<lane2.size(); ++i) {
        if( lane2[i]->state == OBJ_ACTIVE ) {
            if( player.CheckObjectCollision(lane2[i]) ) {
                player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
            }
        }
    }
}

void GameWorld::step(float dt)
{
    //player step
    player.Step(dt);

    //all game objects step and objects management

    //lane1
    int n = lane1.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = lane1[i];
        if(obj->state == OBJ_ACTIVE) {
            obj->Step(dt);
            if( !obj->InScreen( designSize.width, designSize.height) ) {
                obj->state = OBJ_INACTIVE;
            }
        }
    }

    n = lane2.size();
    for(int i=0; i<n; ++i) {
        GObject *obj = lane2[i];
        if(obj->state == OBJ_ACTIVE) {
            obj->Step(dt);
            if( !obj->InScreen( designSize.width, designSize.height) ) {
                obj->state = OBJ_INACTIVE;
            }
        }
    }
    //renew map (may drop cars based on level design)
    RenewMap();

    //collision test
    CheckCollision();
}

