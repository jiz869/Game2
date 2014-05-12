#include <algorithm>
#include "GameWorld.h"
#include "GameOverScene.h"
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

        //score label
        InitScoreLabel();
        //time label
        InitTimeLabel();

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

void GameWorld::InitScoreLabel()
{
    _label = CCLabelTTF::create("0", "Helvetica", 32 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //_label->retain();
    _label->setColor( ccc3(0, 0, 128) );
    _label->setPosition( ccp(winSize.width/2, winSize.height - 50) );
    this->addChild(_label);
    score = 0;
}

void GameWorld::InitTimeLabel()
{
    timeLabel = CCLabelTTF::create("0", "Helvetica", 32 );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    timeLabel->setColor( ccc3(10, 10, 10) );
    timeLabel->setPosition( ccp(winSize.width - 80, winSize.height - 50) );
    this->addChild(timeLabel);
    seconds = 0;
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
    for(int i=0; i<numLanes; ++i) {
        GObject *last_car;
        bool addNewCar =false;
        CCPoint pos;
        float bw, bh;

        sort(lane[i].begin(), lane[i].end(), CompareX2);
        if(ld[i].l2r) {
            last_car = lane[i].front();
            last_car->GetAABB(pos, bw ,bh);
            addNewCar = ( pos.x > ld[i].interval);
        }else{
            last_car = lane[i].back();
            last_car->GetAABB(pos, bw ,bh);
            float offset = getRandom(0, 80);
            addNewCar = (pos.x+bw < (designSize.width - (ld[i].interval+offset)));
        }

        if(addNewCar) {
            LaneAddCar(i);
        }
    }
}

void GameWorld::LaneAddCar(int i)
{
    GObject *car;
    float offset = getRandom(0, 50);
    float initX = ld[i].initPos.x;
    car = GetObject( lane[i], ld[i].carName );

    if(initX < designSize.width/2) {
        initX -= offset;
    }

    car->SetObjectPosition( initX, ld[i].initPos.y );
    car->SetVelocity( ld[i].velocity );
}

//customize: level loading
void GameWorld::LoadMap(char *name)
{
    //name: used for different levels

    //temp map data
    float intervals[] = { 350, 500, 400, 250, 350, 500, 600, 510, 220, 310, 440};
    float speed[] = { -2, 3, -2.2, 2.2, -1.8, 4, -5, 3, -2, 2, -2};
    float laneY = 80;
    int i = 0;
    float rightX = designSize.width;
    while(designSize.height - laneY > 80) {
        if(i%2 == 0) {
            strncpy(ld[i].carName, "car1", 50);
            //ld[i].velocity = ccp(-2, 0);
            ld[i].initPos = ccp(rightX, laneY);
            ld[i].height = 50;
            ld[i].l2r = false;
            //ld[i].interval = 150;
            ld[i].interval = intervals[i];
        }else{
            strncpy(ld[i].carName, "car2", 50);
            //ld[i].velocity = ccp(2, 0);
            ld[i].initPos = ccp(0, laneY);
            ld[i].height = 60;
            ld[i].l2r = true;
            //ld[i].interval = 300;
        }
        ld[i].velocity = ccp(speed[i], 0);
        ld[i].interval = intervals[i];
        LaneAddCar(i);
        laneY += ld[i].height;
        ++i;
    }
    numLanes = i;

    numFrame = 0;
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
    int n;
    for(int i=0; i<numLanes; ++i) {
        n = lane[i].size();
        for(int j=0; j<n; ++j) {
            if(lane[i][j]->state == OBJ_ACTIVE) {
                if( player.CheckObjectCollision(lane[i][j]) ) {
                    player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
                }
            }
        }
    }

}

void GameWorld::step(float dt)
{
    numFrame++;
    if(numFrame % 60 == 0) {
        numFrame = 0;
        seconds++;
        char ss[10];
        sprintf(ss, "%d", seconds);
        timeLabel->setString(ss);
    }
    if(seconds == 60) GameOver();
    //player step
    player.Step(dt);

    //all game objects step and objects management

    int n;
    for(int i=0; i<numLanes; ++i) {
        n = lane[i].size();
        for(int j=0; j<n; ++j) {
            GObject *obj = lane[i][j];
            if(obj->state == OBJ_ACTIVE) {
                obj->Step(dt);
                if( !obj->InScreen( designSize.width, designSize.height) ) {
                    obj->state = OBJ_INACTIVE;
                }
            }

        }
    }

    //renew map (may drop cars based on level design)
    RenewMap();

    //collision test
    CheckCollision();

    //check if player has crossed the road
    if(player.GetPlayerPosition().y+40 >= designSize.height) {
        score++;
        char ss[10];
        sprintf(ss, "%d", score);
        player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
        _label->setString(ss);
    }else if( player.GetPlayerPosition().y < INIT_POS.y) {
        player.SetPlayerPosition(INIT_POS.x, INIT_POS.y);
    }
}

void GameWorld::GameOver()
{
    char s[200];
    snprintf(s, 200, "Score: %d ", score );
    GameOverScene *gameOverScene = GameOverScene::create();
    gameOverScene->getLayer()->getLabel()->setString(s);
	CCDirector::sharedDirector()->replaceScene( gameOverScene );
}

