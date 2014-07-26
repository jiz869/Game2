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
    CCString * info;

    CCMenuItemLabel * nameField = CCMenuItemLabel::create(CCLabelTTF::create("0", FONT, 80));
    nameField->setString(userData->userName.c_str());
    nameField->setDisabledColor( ccRED );
    nameField->setPosition(ccp(winSize.width * 0.5 , winSize.height*0.8));
    nameField->setEnabled(false);


    CCMenuItemLabel * topScoreLabel = CCMenuItemLabel::create(CCLabelTTF::create("0", FONT, 64));
    topScoreLabel->setDisabledColor( ccMAGENTA );
    topScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    info = CCString::createWithFormat("TOP score is %d", userData->topScore);
    topScoreLabel->setString(info->getCString());
    topScoreLabel->setEnabled(false);

    CCMenuItemLabel * lastScoreLabel =  CCMenuItemLabel::create(CCLabelTTF::create("0", FONT, 64));
    lastScoreLabel->setDisabledColor( ccBLACK );
    lastScoreLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.4));
    info = CCString::createWithFormat("Last score is %d", userData->lastScore);
    lastScoreLabel->setString(info->getCString());
    lastScoreLabel->setEnabled(false);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png" , "return_selected.png" ,
            this , menu_selector(GameOverScene::returnHandler));
    OK->setPosition(ccp(winSize.width * 0.375 , winSize.height*0.2));
    OK->setScale(0.45);

    CCMenuItemImage * upload = CCMenuItemImage::create("upload_score_normal.png" , "upload_score_selected.png" ,
            this , menu_selector(GameOverScene::uploadHandler));
    upload->setPosition(ccp(winSize.width * 0.675 , winSize.height*0.2));
    upload->setScale(0.45);

    mainMenu = CCMenu::create(nameField , topScoreLabel , lastScoreLabel , OK , upload , NULL);
    mainMenu->ignoreAnchorPointForPosition(false);
//    mainMenu->setContentSize(winSize);
//    mainMenu->setPosition(ccp(winSize.width/2 , winSize.height/2));
    addChild(mainMenu);
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

void GameOverScene::returnHandler(cocos2d::CCObject *sender){
    CCDirector::sharedDirector()->replaceScene(StartMenuScene::scene());
}

void GameOverScene::uploadHandler(cocos2d::CCObject *sender){
    if(userData->isLogedIn == false){
        setInfoLabel("Please login or register first");
        return;
    }
    GameController::getGameController()->uploadLastScore();
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
