/*
 * GameOverScene.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: mli
 */

#include "GameOverScene.h"
#include "StartMenuScene.h"

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

    winSize = CCDirector::sharedDirector()->getWinSize();
    UserData * userData = GameController::getGameController()->getUserData();
    CCString * info;

    CCSprite * background = CCSprite::create("background.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(background);

    CCLabelTTF * yourNameLabel = CCLabelTTF::create("0", FONT, 48);
    yourNameLabel->setColor( ccBLUE );
    yourNameLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.8));
    yourNameLabel->setAnchorPoint(ccp(1.1 , 0.5));
    info = CCString::create("Input your name");
    yourNameLabel->setString(info->getCString());
    addChild(yourNameLabel);

    nameField = CCTextFieldTTF::textFieldWithPlaceHolder("Penguin" , FONT, 64);
    nameField->setColorSpaceHolder( ccGREEN );
    nameField->setColor( ccGREEN );
    nameField->setPosition(ccp(winSize.width * 0.6 , winSize.height*0.8));
    nameField->setAnchorPoint(ccp(0 , 0.5));
    addChild(nameField);


    CCLabelTTF * topScoreLabel = CCLabelTTF::create("0", FONT, 64);
    topScoreLabel->setColor( ccMAGENTA );
    topScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    info = CCString::createWithFormat("TOP score is %d", userData->topScore);
    topScoreLabel->setString(info->getCString());
    addChild(topScoreLabel);

    CCLabelTTF * lastScoreLabel = CCLabelTTF::create("0", FONT, 64);
    lastScoreLabel->setColor( ccBLACK );
    lastScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.4));
    info = CCString::createWithFormat("Last score is %d", userData->lastScore);
    lastScoreLabel->setString(info->getCString());
    addChild(lastScoreLabel);

    OK = CCSprite::create("ok_normal.png");
    OK->setPosition(ccp(winSize.width/2 , winSize.height*0.2));
    OK->setScale(0.5);
    addChild(OK);

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
		const char * name = nameField->getString();
		if(strlen(name) == 0){
			nameField->setPlaceHolder("Penguin");
		}
		nameField->detachWithIME();
		if(OK->boundingBox().containsPoint(touchPoint)){
			GameController::getGameController()->saveLastScore(name);
			CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
		}
	}
}

void GameOverScene::keyboardWillShow(CCIMEKeyboardNotificationInfo& info){
    nameField->setPlaceHolder("");
}
