/*
 * GameOverScene.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: mli
 */

#include "GameOverScene.h"
#include "StartMenuScene.h"

#define NAME_RULE "Maximum 12 characters\n" \
	              "Minimum 4 characters\n" \
	              "Legal characters are\n" \
                  "a-z , A-Z, 0-9 , _ , -\n" \
	              "Must start with alphabet"

#define DEFAULT_NAME "Penguin"

GameOverScene::GameOverScene() {
	// TODO Auto-generated constructor stub

}

GameOverScene::~GameOverScene() {
	// TODO Auto-generated destructor stub
}

CCScene* GameOverScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GameOverScene *layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    scene->setTag(GAME_OVER_SCENE);

    // return the scene
    return scene;
}

bool GameOverScene::init(){
    if (!CCLayer::init()) {
        return false;
    }

    GameController::getGameController()->getTopRankings();

    setKeypadEnabled(true);

    //ignoreAnchorPointForPosition(false);

    winSize = CCDirector::sharedDirector()->getWinSize();
    userData = GameController::getGameController()->getUserData();

    CCSprite * background = CCSprite::create("background.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(background);

    initInfoMenu();
    initMainMenu();

	return true;
}

void GameOverScene::initMainMenu(){
    mainMenu = CCSprite::create();
    mainMenu->setContentSize(winSize);
    mainMenu->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(mainMenu);

    CCString * info;

    CCLabelTTF * yourNameLabel = CCLabelTTF::create("0", FONT, 50);
    yourNameLabel->setColor( ccBLUE );
    yourNameLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.8));
    yourNameLabel->setAnchorPoint(ccp(1 , 0.5));
    info = CCString::create("Input your name");
    yourNameLabel->setString(info->getCString());
    mainMenu->addChild(yourNameLabel);

    CCLabelTTF * nameField = CCLabelTTF::create("0", FONT, 64);
    nameField->setString(userData->userName.c_str());
    nameField->setColor( ccRED );
    nameField->setPosition(ccp(winSize.width * 0.55 , winSize.height*0.8));
    nameField->setAnchorPoint(ccp(0 , 0.5));
    mainMenu->addChild(nameField);


    CCLabelTTF * topScoreLabel = CCLabelTTF::create("0", FONT, 64);
    topScoreLabel->setColor( ccMAGENTA );
    topScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    info = CCString::createWithFormat("TOP score is %d", userData->topScore);
    topScoreLabel->setString(info->getCString());
    mainMenu->addChild(topScoreLabel);

    CCLabelTTF * lastScoreLabel = CCLabelTTF::create("0", FONT, 64);
    lastScoreLabel->setColor( ccBLACK );
    lastScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.4));
    info = CCString::createWithFormat("Last score is %d", userData->lastScore);
    lastScoreLabel->setString(info->getCString());
    mainMenu->addChild(lastScoreLabel);

    OK = CCSprite::create("return_normal.png");
    OK->setPosition(ccp(winSize.width * 0.375 , winSize.height*0.2));
    OK->setScale(0.4);
    mainMenu->addChild(OK);

    upload = CCSprite::create("upload_score_normal.png");
    upload->setPosition(ccp(winSize.width * 0.75 , winSize.height*0.2));
    upload->setScale(0.4);
    mainMenu->addChild(upload);

    setTouchEnabled(true);
}

void GameOverScene::initInfoMenu(){
	infoLabel = CCMenuItemLabel::create(CCLabelTTF::create("", INFO_FONT, 48 ));
    infoLabel->setDisabledColor( ccRED );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    infoLabel->setEnabled(false);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" , this , menu_selector(GameOverScene::okHandler));
    OK->setScale(0.5);
    OK->setPosition(ccp(winSize.width/2, winSize.height * 0.2));

	infoMenu = CCMenu::create(OK , infoLabel , NULL);
	infoMenu->ignoreAnchorPointForPosition(false);
	infoMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    addChild(infoMenu);
}

void GameOverScene::okHandler(cocos2d::CCObject *sender){
    infoMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    mainMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void GameOverScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch * touch = (CCTouch *)pTouches->anyObject();

	CCPoint touchPoint = convertToNodeSpace(touch->getLocation());

    if(OK->boundingBox().containsPoint(touchPoint)){
        CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
    }else if(upload->boundingBox().containsPoint(touchPoint)){
        if(userData->isLogedIn == false){
            setInfoLabel("Please login or register first");
            return;
        }
        GameController::getGameController()->uploadLastScore();
    }
}

bool GameOverScene::checkName(const char * name){
	int length = strlen(name);

	if(length > 12 || length < 4) return false;

	if((name[0] < 'a' || name[0] > 'z') && (name[0] < 'A' || name[0] > 'Z')) return false;

	for(int i = 0 ; i < length ; i++){
		if((name[i] < 'a' || name[i] > 'z')
				&& (name[i] < 'A' || name[i] > 'Z')
				&& (name[i] < '0' || name[i] > '9')
				&& (name[i] != '_') && (name[i] != '-')){
			return false;
		}
	}

	return true;
}

void GameOverScene::setInfoLabel(const char * info){
	infoLabel->setString(info);
	mainMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    infoMenu->setPosition(ccp(winSize.width/2 , winSize.height*0.5));
}

void GameOverScene::keyBackClicked(){
	CCLayer::keyBackClicked();
	CCDirector::sharedDirector()->end();
}
