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
    UserData * userData = GameController::getGameController()->getUserData();
    CCString * info;

    CCSprite * background = CCSprite::create("background.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(background);

    infoLabel = CCLabelTTF::create("" , FONT , 40);
    infoLabel->setString("");
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    infoLabel->setVisible(false);
    infoLabel->setColor(ccRED);
    addChild(infoLabel);

    container = CCSprite::create();
    container->setContentSize(winSize);
    container->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(container);

    CCLabelTTF * yourNameLabel = CCLabelTTF::create("0", FONT, 50);
    yourNameLabel->setColor( ccBLUE );
    yourNameLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.8));
    yourNameLabel->setAnchorPoint(ccp(1 , 0.5));
    info = CCString::create("Input your name");
    yourNameLabel->setString(info->getCString());
    container->addChild(yourNameLabel);

    nameField = CCTextFieldTTF::textFieldWithPlaceHolder(NULL , FONT, 64);
    nameField->setString(DEFAULT_NAME);
    nameField->setColorSpaceHolder( ccRED );
    nameField->setColor( ccRED );
    nameField->setPosition(ccp(winSize.width * 0.55 , winSize.height*0.8));
    nameField->setAnchorPoint(ccp(0 , 0.5));
    nameField->runAction(CCBlink::create(3, 10));
    container->addChild(nameField);


    CCLabelTTF * topScoreLabel = CCLabelTTF::create("0", FONT, 64);
    topScoreLabel->setColor( ccMAGENTA );
    topScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    info = CCString::createWithFormat("TOP score is %d", userData->topScore);
    topScoreLabel->setString(info->getCString());
    container->addChild(topScoreLabel);

    CCLabelTTF * lastScoreLabel = CCLabelTTF::create("0", FONT, 64);
    lastScoreLabel->setColor( ccBLACK );
    lastScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.4));
    info = CCString::createWithFormat("Last score is %d", userData->lastScore);
    lastScoreLabel->setString(info->getCString());
    container->addChild(lastScoreLabel);

    OK = CCSprite::create("ok_normal.png");
    OK->setPosition(ccp(winSize.width/2 , winSize.height*0.2));
    OK->setScale(0.4);
    container->addChild(OK);

    setTouchEnabled(true);

	return true;
}

void GameOverScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch * touch = (CCTouch *)pTouches->anyObject();

	CCPoint touchPoint = convertToNodeSpace(touch->getLocation());

	if(nameField->boundingBox().containsPoint(touchPoint)){
		nameField->attachWithIME();
	}else{
		nameField->detachWithIME();
		const char * name = nameField->getString();
		if(strlen(name) == 0){
			nameField->setString(DEFAULT_NAME);
			return;
		}
		if(OK->boundingBox().containsPoint(touchPoint)){
			if(checkName(name) == false){
				nameField->setString(DEFAULT_NAME);
				setInfoLabel(NAME_RULE);
				return;
			}
			GameController::getGameController()->saveLastScore(name);
			CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
		}
	}
}

void GameOverScene::keyboardWillShow(CCIMEKeyboardNotificationInfo& info){
    nameField->setString("");
}

bool GameOverScene::checkName(const char * name){
	int length = strlen(name);

	if(length > 12 || length < 4) return false;

	if((name[0] < 'a' || name[0] > 'z') && (name[0] < 'A' || name[0] > 'Z')) return false;

	for(int i = 0 ; i < length ; i++){
		if((name[i] < 'a' || name[i] > 'z')
				&& (name[i] < 'A' || name[i] > 'Z')
				&& (name[i] < '1' || name[i] > '9')
				&& (name[i] != '_') && (name[i] != '-')){
			return false;
		}
	}

	return true;
}

void GameOverScene::setInfoLabel(const char * info){
	container->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
	infoLabel->setString(info);
	infoLabel->setVisible(true);
    scheduleOnce(schedule_selector(GameOverScene::hideInfoLabel), 3);
}

void GameOverScene::hideInfoLabel(){
	container->setPosition(ccp(winSize.width/2, winSize.height/2));
    infoLabel->setVisible(false);
}

void GameOverScene::keyBackClicked(){
	CCLayer::keyBackClicked();
	CCDirector::sharedDirector()->end();
}
