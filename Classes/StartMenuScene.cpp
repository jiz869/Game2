//
//  StartMenuScene.cpp
//  crossRoad
//
//  Created by Min Li on 2014-05-30.
//
//

#include "StartMenuScene.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "MultiPlayScene.h"
#include "PWDField.h"

typedef enum{
    NEW_GAME = 0,
    OPTIONS,
    TOP_SCORE,
    PVP,
}BUTTON_TAG;

typedef enum{
    NAME_LABEL = 0,
    GEM_LABEL,
    SCORE_LABEL,
}SCORE_MENU_TAG;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
extern void setBannerViewHidden(bool);
#define SET_BANNDER_HIDDEN(_hidden) \
if(userData->hasPayed) setBannerViewHidden(true); \
else setBannerViewHidden(_hidden)

void onAdClicked(){
    GameController::getGameController()->setJustFailed(false , true);
}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../admob.android/AdmobHelper.h"
#include <jni.h>

#define SET_BANNDER_HIDDEN(_hidden) \
if(userData->hasPayed) setBannerViewHidden(true); \
else setBannerViewHidden(_hidden)

extern "C"
{
	void Java_ca_welcomelm_crossRoad_crossRoad_onAdClicked( JNIEnv* env, jobject thiz ){
		GameController::getGameController()->setJustFailed(false , true);
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentError( JNIEnv* env, jobject thiz ){
		StartMenuScene * currentScene = (StartMenuScene *)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0);
		currentScene->onPaymentError();
	}

	void Java_ca_welcomelm_crossRoad_crossRoad_onPaymentSuccess( JNIEnv* env, jobject thiz ){
		StartMenuScene * currentScene = (StartMenuScene *)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0);
		currentScene->onPaymentSuccess();
	}
}

#else
#define SET_BANNDER_HIDDEN(_hidden)
#endif

ccColor3B labelColors[MAX_RANKS] = {ccBLUE , ccRED , ccMAGENTA , ccBLACK , ccWHITE ,
		ccORANGE , ccRED , ccBLUE , ccMAGENTA , ccBLACK};

using namespace CocosDenshion;

CCScene* StartMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    StartMenuScene *layer = StartMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    scene->setTag(STARTUP_MENU_SCENE);

    // return the scene
    return scene;
}

StartMenuScene::StartMenuScene() : infoLabel(NULL) {
	// TODO Auto-generated constructor stub

}

StartMenuScene::~StartMenuScene() {
	// TODO Auto-generated destructor stub
}

bool StartMenuScene::init(){
    if (!CCLayerColor::initWithColor(ccc4(0,0,0,176))) {
        return false;
    }

    winSize = CCDirector::sharedDirector()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setPosition(ccp(winSize.width/2 , winSize.height/2));

    userData = GameController::getGameController()->getUserData();

    userData->pvpMode = NONE;

    SET_BANNDER_HIDDEN(false);

	CCSprite * background = CCSprite::create("background.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));
    addChild(background);

    initMainMenu();

    initOptionsMenu();

    initScoreMenu();

    initInfoMenu();

    initUserMenu();

    AnimationData * animationData = GameController::getGameController()->getAnimationData();
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
			animationData->backgroundSoundImage->getCString(), true);

    if(checkboxMute->isSelected()){
    	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
    	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }else{
    	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
    	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }

    setKeypadEnabled(true);

    return true;
}

void StartMenuScene::initInfoMenu(){
	infoLabel = CCMenuItemLabel::create(CCLabelTTF::create("", INFO_FONT, 48 ));
    infoLabel->setDisabledColor( ccRED );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height/2));
    infoLabel->setEnabled(false);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.5);
    OK->setPosition(ccp(winSize.width/2, winSize.height * 0.2));

	infoMenu = CCMenu::create(OK , infoLabel , NULL);
	infoMenu->ignoreAnchorPointForPosition(false);
	infoMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    addChild(infoMenu);
}

void StartMenuScene::keyBackClicked(){
	CCLayer::keyBackClicked();
	CCDirector::sharedDirector()->end();
}

void StartMenuScene::initScoreMenu(){
#ifdef LISTVIEW_LEADERBOARD
	scoreMenu = CCLayer::create();
	scoreMenu->ignoreAnchorPointForPosition(false);
	scoreMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
	addChild(scoreMenu);

	rankLabel = CCLabelTTF::create("0", FONT, 64 );
	rankLabel->setColor( ccRED );
	CCString * rankInfo = CCString::createWithFormat("%s's rank is : %d",
			userData->userName.c_str(), userData->rank);
	rankLabel->setString(rankInfo->getCString());
	rankLabel->setPosition(ccp(winSize.width*0.5, winSize.height/(RANK_PERPAGE + 1)*(RANK_PERPAGE)));
	scoreMenu->addChild(rankLabel);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.4);
    OK->setPosition(ccp(winSize.width * 0.25, winSize.height/(RANK_PERPAGE + 1)*0.85));
    CCMenuItemImage * account = CCMenuItemImage::create("account_normal.png", "account_selected.png" ,
            this , menu_selector(StartMenuScene::userHandler));
    account->setScale(0.4);
    account->setPosition(ccp(winSize.width * 0.75, winSize.height/(RANK_PERPAGE + 1)*0.85));
    CCMenuItemImage * upload = CCMenuItemImage::create("upload_score_normal.png", "upload_score_selected.png" ,
            this , menu_selector(StartMenuScene::uploadHandler));
    upload->setScale(0.4);
    upload->setPosition(ccp(winSize.width*0.5, winSize.height/(RANK_PERPAGE + 1)*0.85));
    CCMenu * okMenu = CCMenu::create(OK , account , upload , NULL);
    okMenu->setPosition(ccp(0, 0));
    okMenu->setAnchorPoint(ccp(0,0));
    okMenu->setContentSize(CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1)));
    okMenu->ignoreAnchorPointForPosition(false);
    scoreMenu->addChild(okMenu);

    scoreTable = CCTableView::create(this ,
    		CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1)*(RANK_PERPAGE-2)));
    scoreTable->setDirection(kCCScrollViewDirectionVertical);
    //scoreTable->setAnchorPoint(ccp(0 , 1));
    scoreTable->setPosition(ccp(0, winSize.height/(RANK_PERPAGE + 1) + winSize.height * 0.04));
    scoreTable->setVerticalFillOrder(kCCTableViewFillTopDown);
    scoreMenu->addChild(scoreTable);
    scoreTable->reloadData();
#else
	ScoreRank * rank;

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.3);
    OK->setPosition(ccp(winSize.width/2, winSize.height*0.07));

    scoreMenu = CCMenu::create(OK , NULL);

    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->ignoreAnchorPointForPosition(false);
    addChild(scoreMenu);

	for(int i = 0 ; i < MAX_RANKS ; i++){
		rank = &GameController::getGameController()->ranks[i];

	    nameLabels[i] = CCMenuItemLabel::create(CCLabelTTF::create("0", FONT, 56 ));
	    nameLabels[i]->setDisabledColor( labelColors[i] );
	    nameLabels[i]->setString(rank->userName.c_str());
	    nameLabels[i]->setPosition(ccp(winSize.width*0.25, winSize.height*(MAX_RANKS + 1 -i)*0.082));
	    nameLabels[i]->setEnabled(false);

		CCString * gemName = CCString::createWithFormat("gem%d.png", rank->level);
		gems[i] = CCMenuItemImage::create();
		gems[i]->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(gemName->getCString()));
		gems[i]->setPosition(ccp(winSize.width/2, winSize.height*(MAX_RANKS + 1 -i)*0.082));
		gems[i]->setEnabled(false);
		gems[i]->setScale(0.5);

	    scoreLabels[i] = CCMenuItemLabel::create(CCLabelTTF::create("0", FONT, 56 ));
	    scoreLabels[i]->setDisabledColor( labelColors[i] );
	    CCString * score = CCString::createWithFormat("%d", rank->score);
	    scoreLabels[i]->setString(score->getCString());
	    scoreLabels[i]->setPosition(ccp(winSize.width*0.75, winSize.height*(MAX_RANKS + 1 -i)*0.082));
	    scoreLabels[i]->setEnabled(false);

	    scoreMenu->addChild(gems[i]);
	    scoreMenu->addChild(scoreLabels[i]);
	    scoreMenu->addChild(nameLabels[i]);
	}
#endif
}

void StartMenuScene::uploadHandler(CCObject * sender){
    if(userData->isLogedIn == false){
        setInfoLabel("Please login or register first" , 0);
        return;
    }
    GameController::getGameController()->uploadLastScore();
}

void StartMenuScene::initUserMenu(){
	userMenu = CCLayer::create();
	userMenu->ignoreAnchorPointForPosition(false);
	userMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
	addChild(userMenu);
	setTouchEnabled(true);

    CCLabelTTF * yourNameLabel = CCLabelTTF::create("0", FONT, 50);
    yourNameLabel->setColor( ccBLUE );
    yourNameLabel->setPosition(ccp(winSize.width/4 , winSize.height*0.8));
    //yourNameLabel->setAnchorPoint(ccp(1 , 0.5));
    yourNameLabel->setString(CCString::create("UserName")->getCString());
    userMenu->addChild(yourNameLabel);

    nameField = CCTextFieldTTF::textFieldWithPlaceHolder(NULL , FONT, 64);
    nameField->setString(userData->userName.c_str());
    nameField->setColorSpaceHolder( ccRED );
    nameField->setColor( ccRED );
    nameField->setPosition(ccp(winSize.width * 0.4 , winSize.height*0.8));
    nameField->setAnchorPoint(ccp(0 , 0.5));
    nameField->setDelegate(this);
    userMenu->addChild(nameField);

    CCLabelTTF * yourPwdLabel = CCLabelTTF::create("0", FONT, 50);
    yourPwdLabel->setColor( ccBLUE );
    yourPwdLabel->setPosition(ccp(winSize.width/4 , winSize.height*0.6));
    //yourPwdLabel->setAnchorPoint(ccp(1 , 0.5));
    yourPwdLabel->setString(CCString::create("PassWord")->getCString());
    userMenu->addChild(yourPwdLabel);

    pwdField = PWDField::textFieldWithPlaceHolder(NULL , FONT, 64);
    pwdField->setString(userData->password.c_str());
    pwdField->setColorSpaceHolder( ccRED );
    pwdField->setColor( ccRED );
    pwdField->setPosition(ccp(winSize.width * 0.4 , winSize.height*0.6));
    pwdField->setAnchorPoint(ccp(0 , 0.5));
    pwdField->setDelegate(this);
    userMenu->addChild(pwdField);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.45);
    OK->setPosition(ccp(winSize.width * 0.25, winSize.height * 0.3));
    CCMenuItemImage * login = CCMenuItemImage::create("login_normal.png", "login_selected.png" ,
            this , menu_selector(StartMenuScene::loginHandler));
    login->setScale(0.45);
    login->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.3));
    CCMenuItemImage * create = CCMenuItemImage::create("register_normal.png", "register_selected.png" ,
            this , menu_selector(StartMenuScene::RegisterHandler));
    create->setScale(0.45);
    create->setPosition(ccp(winSize.width*0.75, winSize.height * 0.3));
    CCMenu * okMenu = CCMenu::create(OK , login , create , NULL);
    okMenu->setPosition(ccp(0, 0));
    okMenu->setAnchorPoint(ccp(0,0));
    okMenu->setContentSize(CCSizeMake(winSize.width , winSize.height * 0.5));
    okMenu->ignoreAnchorPointForPosition(false);
    userMenu->addChild(okMenu);
}

void StartMenuScene::loginHandler(CCObject * sender){
    GameController::getGameController()->authenticate(nameField->getString() , pwdField->getString());
}

void StartMenuScene::RegisterHandler(CCObject * sender){
    GameController::getGameController()->createUser(nameField->getString() , pwdField->getString());
}

void StartMenuScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch * touch = (CCTouch *)pTouches->anyObject();

    CCPoint touchPoint = convertToNodeSpace(touch->getLocation());

    if(nameField->boundingBox().containsPoint(touchPoint)){
        nameField->attachWithIME();
    }else if(pwdField->boundingBox().containsPoint(touchPoint)){
        pwdField->attachWithIME();
    }
    else{
        nameField->detachWithIME();
        pwdField->detachWithIME();
        const char * name = nameField->getString();
        if(strlen(name) == 0){
            nameField->setString(userData->userName.c_str());
            return;
        }
        name = pwdField->getString();
        if(strlen(name) == 0){
            pwdField->setString(userData->password.c_str());
            return;
        }
    }
}

void StartMenuScene::userHandler(CCObject * sender){
    SET_BANNDER_HIDDEN(true);
    nameField->runAction(CCBlink::create(2, 6));
    pwdField->runAction(CCBlink::create(2, 6));
    userMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height * 1.5));
}

void StartMenuScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	CCLOG("cell touched at index: %i", cell->getIdx());
}

CCTableViewCell * StartMenuScene::tableCellAtIndex(CCTableView *table, unsigned int idx){
	ScoreRank * rank = &GameController::getGameController()->ranks[idx];

    CCTableViewCell * cell = table->dequeueCell();

    if(!cell){
    	cell = new CCTableViewCell();
    	cell->autorelease();

    	CCLabelTTF * name = CCLabelTTF::create("0", FONT, 64 ,
    			CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft);
    	name->setColor( labelColors[idx%RANK_PERPAGE] );
    	CCString * body = CCString::createWithFormat("%d  %s", idx+1 , rank->userName.c_str());
        name->setString(body->getCString());
        name->setPosition(ccp(winSize.width*0.35 , winSize.height/(RANK_PERPAGE + 1)/2));
        name->setTag(NAME_LABEL);
        cell->addChild(name);

    	CCString * gemName = CCString::createWithFormat("gem%d.png", rank->level);
    	CCSprite * gem = CCSprite::createWithSpriteFrameName(gemName->getCString());
    	gem->setPosition(ccp(winSize.width*0.65, winSize.height/(RANK_PERPAGE + 1)/2));
    	gem->setScale(0.7);
    	gem->setTag(GEM_LABEL);
    	cell->addChild(gem);

    	CCLabelTTF * scoreLabel = CCLabelTTF::create("0", FONT, 64 ,
    			CCSizeMake(winSize.width*0.25 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft);
    	scoreLabel->setColor( labelColors[idx%RANK_PERPAGE] );
        CCString * score = CCString::createWithFormat("%d", rank->score);
        scoreLabel->setString(score->getCString());
        scoreLabel->setPosition(ccp(winSize.width*0.85, winSize.height/(RANK_PERPAGE + 1)/2));
        scoreLabel->setTag(SCORE_LABEL);
        cell->addChild(scoreLabel);
    }else{
    	CCLabelTTF * name = (CCLabelTTF *)cell->getChildByTag(NAME_LABEL);
    	name->setColor( labelColors[idx%RANK_PERPAGE] );
    	CCString * body = CCString::createWithFormat("%d  %s", idx+1 , rank->userName.c_str());
        name->setString(body->getCString());

    	cell->removeChildByTag(GEM_LABEL);
    	CCString * gemName = CCString::createWithFormat("gem%d.png", rank->level);
    	CCSprite * gem = CCSprite::createWithSpriteFrameName(gemName->getCString());
    	gem->setPosition(ccp(winSize.width*0.65, winSize.height/(RANK_PERPAGE + 1)/2));
    	gem->setScale(0.7);
    	gem->setTag(GEM_LABEL);
    	cell->addChild(gem);


    	CCLabelTTF * scoreLabel = (CCLabelTTF *)cell->getChildByTag(SCORE_LABEL);
    	CCString * score = CCString::createWithFormat("%d", rank->score);
        scoreLabel->setColor( labelColors[idx%RANK_PERPAGE] );
        scoreLabel->setString(score->getCString());
    }

    return cell;
}

unsigned int StartMenuScene::numberOfCellsInTableView(CCTableView *table){
	return MAX_RANKS;
}

CCSize StartMenuScene::cellSizeForTable(CCTableView *table){
	return CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1));
}

void StartMenuScene::scrollViewDidScroll(CCScrollView* view){

}

void StartMenuScene::scrollViewDidZoom(CCScrollView* view){

}

void StartMenuScene::initMainMenu(){
    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game_normal.png", "button_new_game_selected.png", this, menu_selector(StartMenuScene::newGameHandler));
    newGame->setTag(NEW_GAME);

    CCMenuItemImage * options = CCMenuItemImage::create("button_options_normal.png", "button_options_selected.png" , this , menu_selector(StartMenuScene::optionsHandler));
    options->setTag(OPTIONS);

    CCMenuItemImage * score = CCMenuItemImage::create("top_score_normal.png", "top_score_selected.png" , this , menu_selector(StartMenuScene::scoreHandler));
    score->setTag(TOP_SCORE);

    CCMenuItemImage * background = CCMenuItemImage::create("background_main.png" , "background_main.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));
    background->setEnabled(false);

#ifdef MULTIPLAY
    CCMenuItemImage * pvp = CCMenuItemImage::create("pvp_normal.png", "pvp_selected.png" , this , menu_selector(StartMenuScene::pvpHandler));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    newGame->setScale(0.6);
    options->setScale(0.6);
    score->setScale(0.6);
    CCMenuItemImage * pvp = CCMenuItemImage::create("purchase_normal.png", "purchase_selected.png" , this , menu_selector(StartMenuScene::pvpHandler));
    pvp->setScale(0.6);
    pvp->setTag(PVP);

    startMenu = CCMenu::create(background , newGame , options, score , pvp , NULL);
    startMenu->ignoreAnchorPointForPosition(false);
    newGame->setPosition(ccp(winSize.width*0.3 , winSize.height*0.45));
    options->setPosition(ccp(winSize.width*0.7 , winSize.height*0.45));
    score->setPosition(ccp(winSize.width*0.3 , winSize.height*0.3));
    pvp->setPosition(ccp(winSize.width*0.7 , winSize.height*0.3));
#else
    newGame->setScale(0.6);
    options->setScale(0.6);
    score->setScale(0.6);
    startMenu = CCMenu::create(background , newGame , options, score , NULL);
    startMenu->ignoreAnchorPointForPosition(false);
    newGame->setPosition(ccp(winSize.width*0.2 , winSize.height*0.375));
    options->setPosition(ccp(winSize.width*0.5 , winSize.height*0.375));
    score->setPosition(ccp(winSize.width*0.8 , winSize.height*0.375));
#endif

    addChild(startMenu);
}

void StartMenuScene::pvpHandler(cocos2d::CCObject *sender){
#ifdef MULTIPLAY
    SET_BANNDER_HIDDEN(true);
    CCScene * pvpScene = MultiPlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(pvpScene);
#else
    enableButtonsForIap(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    purchase(this);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    purchase();
#endif
#endif
}

void StartMenuScene::scoreHandler(cocos2d::CCObject *sender){
#ifdef LISTVIEW_LEADERBOARD
	GameController::getGameController()->getTopRankings();
	scoreTable->reloadData();
    CCString * rankInfo = CCString::createWithFormat("%s's rank is : %d",
            userData->userName.c_str(), userData->rank);
    rankLabel->setString(rankInfo->getCString());
#else
	ScoreRank * rank;

	GameController::getGameController()->getTopRankings();

	for(int i = 0 ; i < MAX_RANKS ; i++){
		rank = &GameController::getGameController()->ranks[i];

		nameLabels[i]->setString(rank->userName.c_str());

		CCString * gemName = CCString::createWithFormat("gem%d.png", rank->level);
		gems[i]->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(gemName->getCString()));

	    CCString * score = CCString::createWithFormat("%d", rank->score);
	    scoreLabels[i]->setString(score->getCString());
	}
#endif

    SET_BANNDER_HIDDEN(true);
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::initOptionsMenu(){

    //row 1
    CCMenuItemLabel * controllerPositions = CCMenuItemLabel::create(
    		CCLabelTTF::create("CONTROLLER POSITION", FONT , 64));
    controllerPositions->setDisabledColor(ccBLUE);
    controllerPositions->setPosition(ccp(winSize.width/2, winSize.height*0.88));
    controllerPositions->setEnabled(false);

    //row 2
    CCMenuItemLabel * left = CCMenuItemLabel::create(
    		CCLabelTTF::create("ON THE LEFT", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    left->setDisabledColor(ccBLACK);
    checkboxLeft = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxLeft->setTag(LEFT);
    checkboxLeft->setScale(0.25);
    left->setPosition(ccp(winSize.width * 0.4, winSize.height*0.76));
    checkboxLeft->setPosition(ccp(winSize.width*0.8, winSize.height*0.76));
    left->setEnabled(false);

    //row 3
    CCMenuItemLabel * right = CCMenuItemLabel::create(
    		CCLabelTTF::create("ON THE RIGHT", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    right->setDisabledColor(ccMAGENTA);
    checkboxRight = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxRight->setTag(RIGHT);
    checkboxRight->setScale(0.25);
    right->setPosition(ccp(winSize.width * 0.4, winSize.height*0.67));
    checkboxRight->setPosition(ccp(winSize.width*0.8, winSize.height*0.67));
    right->setEnabled(false);

    //row 4
    CCMenuItemLabel * leftUp = CCMenuItemLabel::create(
    		CCLabelTTF::create("LEFT UP RIGHT DOWN", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    leftUp->setDisabledColor(ccORANGE);
    checkboxSideLeftUp = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxSideLeftUp->setTag(SIDE_LEFT_UP);
    checkboxSideLeftUp->setScale(0.25);
    leftUp->setPosition(ccp(winSize.width * 0.4, winSize.height*0.58));
    checkboxSideLeftUp->setPosition(ccp(winSize.width*0.8, winSize.height*0.58));
    leftUp->setEnabled(false);

    CCMenuItemLabel * leftDown = CCMenuItemLabel::create(
    		CCLabelTTF::create("LEFT DOWN RIGHT UP", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    leftDown->setDisabledColor(ccYELLOW);
    checkboxSideLeftDown = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxSideLeftDown->setTag(SIDE_LEFT_DOWN);
    checkboxSideLeftDown->setScale(0.25);
    leftDown->setPosition(ccp(winSize.width * 0.4, winSize.height*0.49));
    checkboxSideLeftDown->setPosition(ccp(winSize.width*0.8, winSize.height*0.49));
    leftDown->setEnabled(false);

    //row 5
    CCMenuItemLabel * sound = CCMenuItemLabel::create(
    		CCLabelTTF::create("SOUND", FONT , 64));
    sound->setDisabledColor(ccBLUE);
    sound->setPosition(ccp(winSize.width/2, winSize.height*0.37));
    sound->setEnabled(false);

    //row 6
    CCMenuItemLabel * mute = CCMenuItemLabel::create(
    		CCLabelTTF::create("MUTE", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    mute->setDisabledColor(ccRED);
    checkboxMute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxMute->setTag(MUTE);
    mute->setPosition(ccp(winSize.width * 0.4, winSize.height*0.28));
    checkboxMute->setPosition(winSize.width*0.8, winSize.height*0.28);
    checkboxMute->setScale(0.25);
    mute->setEnabled(false);

    //row 7
    CCMenuItemLabel * unMute = CCMenuItemLabel::create(
    		CCLabelTTF::create("UNMUTE", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    unMute->setDisabledColor(ccBLACK);
    checkboxUnmute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxUnmute->setTag(UNMUTE);
    unMute->setPosition(ccp(winSize.width * 0.4, winSize.height*0.19));
    checkboxUnmute->setPosition(winSize.width*0.8, winSize.height*0.19);
    checkboxUnmute->setScale(0.25);
    unMute->setEnabled(false);

    //row 8
    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.45);
    OK->setPosition(ccp(winSize.width/2, winSize.height*0.1));

    optionsMenu = CCMenu::create(controllerPositions , left , checkboxLeft , right , checkboxRight ,
    		leftUp , checkboxSideLeftUp , leftDown , checkboxSideLeftDown ,
    		sound , mute , checkboxMute , unMute, checkboxUnmute, OK , NULL);

    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->ignoreAnchorPointForPosition(false);
    addChild(optionsMenu);

    checkboxChooser(userData->sound);
    checkboxChooser(userData->controllerPosition);
}

void StartMenuScene::newGameHandler(cocos2d::CCObject *sender){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if(userData->currentLevel > FREE_LEVEL){
		if (!userData->hasPayed && userData->justFailed%FREE_PLAY == 0 && userData->justFailed != 0) {
			setInfoLabel("Please click the ads to support us \nor spend 99 cents to remove the ads.\nThank you!!!" , 4);
			return;
		}
	}
#endif
    SET_BANNDER_HIDDEN(true);
	CCScene * playScene = PlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(playScene);
}

void StartMenuScene::setInfoLabel(const char *info , float delay){
    if(infoLabel == NULL) return;
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    userMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    infoLabel->setString(info);
    infoMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::optionsHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(true);
    startMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::okHandler(cocos2d::CCObject *sender){
    SET_BANNDER_HIDDEN(false);
    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    scoreMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    infoMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    userMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    startMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
}

void StartMenuScene::checkboxHandler(cocos2d::CCObject *sender){
    CCMenuItem * item = (CCMenuItem *)sender;

    checkboxChooser((CheckboxType)item->getTag());
}

void StartMenuScene::checkboxChooser(CheckboxType tag){
    switch (tag) {
        case LEFT:
            checkboxLeft->selected();
            checkboxRight->unselected();
            checkboxSideLeftUp->unselected();
            checkboxSideLeftDown->unselected();
            break;

        case RIGHT:
            checkboxLeft->unselected();
            checkboxRight->selected();
            checkboxSideLeftUp->unselected();
            checkboxSideLeftDown->unselected();
            break;

        case SIDE_LEFT_UP:
            checkboxLeft->unselected();
            checkboxRight->unselected();
            checkboxSideLeftDown->unselected();
            checkboxSideLeftUp->selected();
            break;

        case SIDE_LEFT_DOWN:
            checkboxLeft->unselected();
            checkboxRight->unselected();
            checkboxSideLeftUp->unselected();
            checkboxSideLeftDown->selected();
            break;

        case MUTE:
            checkboxMute->selected();
            checkboxUnmute->unselected();
            break;

        case UNMUTE:
            checkboxMute->unselected();
            checkboxUnmute->selected();
            break;

        default:
            break;
    }
    saveUserData(tag);
}

void StartMenuScene::saveUserData(CheckboxType tag){
    switch (tag) {
        case LEFT:
        case RIGHT:
        case SIDE_LEFT_DOWN:
		case SIDE_LEFT_UP:
            if (userData->controllerPosition != tag) {
                GameController::getGameController()->setUserData("controller_position", tag , 0);
                userData->controllerPosition = tag;
            }
            break;
        case MUTE:
        case UNMUTE:
            if (userData->sound != tag) {
                GameController::getGameController()->setUserData("sound", tag , 0);
                userData->sound = tag;
                changeSoundSetting(tag);
            }
            break;

        default:
            break;
    }
}

void StartMenuScene::changeSoundSetting(CheckboxType type){
    switch (type) {
        case MUTE:
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
            break;
        case UNMUTE:
        {
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            break;
        }
        default:
            break;
    }
}

void StartMenuScene::onPaymentError(){
	CCLOG("onPaymentError");
	scheduleOnce(schedule_selector(StartMenuScene::setPaymentError),0);
    enableButtonsForIap(true);
}

void StartMenuScene::onPaymentSuccess(){
	CCLOG("onPaymentSuccess");
	scheduleOnce(schedule_selector(StartMenuScene::setPaymentSuccess),0);
    GameController::getGameController()->setHasPayed(true);
    SET_BANNDER_HIDDEN(true);
    enableButtonsForIap(true);
}

void StartMenuScene::setPaymentError(){
	setInfoLabel("Payment Error" , 2);
}

void StartMenuScene::setPaymentSuccess(){
	setInfoLabel("Payment Success" , 2);
}

bool StartMenuScene::hasPayed(){
    return userData->hasPayed;
}

void StartMenuScene::enableButtonsForIap(bool enable){
    CCMenuItem * newGame = (CCMenuItem *)startMenu->getChildByTag(NEW_GAME);
    CCMenuItem * pvp = (CCMenuItem *)startMenu->getChildByTag(PVP);
    newGame->setEnabled(enable);
    pvp->setEnabled(enable);
}

bool StartMenuScene::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *sender){
    sender->setString("");
    return false;
}
