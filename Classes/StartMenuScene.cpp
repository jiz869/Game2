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
    PURCHASE,
}BUTTON_TAG;

typedef enum{
    NAME_LABEL = 0,
    GEM_LABEL,
    SCORE_LABEL,
    LEGEND_LABEL,
    LEGEND_NAME_LABEL,
    LEGEND_DESCRIPTION_LABEL,
}SCORE_MENU_TAG;

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

    GameController::getGameController()->recoverSpecialDurations();

	CCSprite * background = CCSprite::create("background.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));
    addChild(background);

    menus.reserve(10);

    initMainMenu();

    initOptionsMenu();

    initScoreMenu();

    initInfoMenu();

    initUserMenu();

    initLegendsMenu();

    initPurchaseMenu();

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

void StartMenuScene::initLegendsMenu(){
    legendsMenu = CCLayer::create();
    legendsMenu->ignoreAnchorPointForPosition(false);
    legendsMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    addChild(legendsMenu);
    menus.push_back(legendsMenu);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.6);
    OK->setPosition(ccp(winSize.width * 0.5, winSize.height*(1.0/(LEGENDS_PERPAGE + 1) + 0.02)));

    CCMenu * okMenu = CCMenu::create(OK , NULL);
    okMenu->setPosition(ccp(0, 0));
    okMenu->setAnchorPoint(ccp(0,0));
    okMenu->setContentSize(CCSizeMake(winSize.width , winSize.height/(LEGENDS_PERPAGE + 1)*2));
    okMenu->ignoreAnchorPointForPosition(false);
    legendsMenu->addChild(okMenu);

    legendsTable = CCTableView::create(this ,
            CCSizeMake(winSize.width , winSize.height/(LEGENDS_PERPAGE + 1)*(LEGENDS_PERPAGE-2)));
    legendsTable->setDirection(kCCScrollViewDirectionVertical);
    legendsTable->setPosition(ccp(0, winSize.height/(LEGENDS_PERPAGE + 1) * 2));
    legendsTable->setVerticalFillOrder(kCCTableViewFillTopDown);
    legendsMenu->addChild(legendsTable);
    legendsTable->reloadData();
}

void StartMenuScene::initInfoMenu(){
	infoLabel = CCMenuItemLabel::create(CCLabelTTF::create("", INFO_FONT, 40 ,
			CCSizeMake(winSize.width * 0.8 , winSize.height * 0.6) ,  kCCTextAlignmentCenter));
    infoLabel->setDisabledColor( ccRED );
    infoLabel->setPosition(ccp(winSize.width/2 , winSize.height*0.5));
    infoLabel->setEnabled(false);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.7);
    OK->setPosition(ccp(winSize.width/2, winSize.height * 0.15));

	infoMenu = CCMenu::create(OK , infoLabel , NULL);
	infoMenu->ignoreAnchorPointForPosition(false);
	infoMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    addChild(infoMenu);
    menus.push_back(infoMenu);
}

void StartMenuScene::initPurchaseMenu(){
	CCMenuItemLabel * benefits = CCMenuItemLabel::create(CCLabelTTF::create("", INFO_FONT, 40 ,
			CCSizeMake(winSize.width * 0.8 , winSize.height * 0.6) ,  kCCTextAlignmentCenter));
	benefits->setDisabledColor( ccRED );
	benefits->setPosition(ccp(winSize.width/2 , winSize.height*0.5));
	benefits->setString(PURCHASE_BENEFIT);
	benefits->setEnabled(false);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
    		this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.7);
    OK->setPosition(ccp(winSize.width*0.3, winSize.height * 0.15));

    CCMenuItemImage * purchase = CCMenuItemImage::create("purchase_normal.png", "purchase_selected.png" ,
    		this , menu_selector(StartMenuScene::purchaseHandler));
    purchase->setScale(0.7);
    purchase->setTag(PURCHASE);
    purchase->setPosition(ccp(winSize.width*0.7, winSize.height * 0.15));

	purchaseMenu = CCMenu::create(OK , purchase , benefits , NULL);
	purchaseMenu->ignoreAnchorPointForPosition(false);
	purchaseMenu->setPosition(ccp(winSize.width/2 , winSize.height*1.5));
    addChild(purchaseMenu);
    menus.push_back(purchaseMenu);
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
	menus.push_back(scoreMenu);

	rankLabel = CCLabelTTF::create("0", FONT, 72 );
	rankLabel->setColor( ccRED );
	CCString * rankInfo = CCString::createWithFormat("%s's rank is : %d",
			userData->userName.c_str(), userData->rank);
	rankLabel->setString(rankInfo->getCString());
	rankLabel->setPosition(ccp(winSize.width*0.5, winSize.height*(1.0/(RANK_PERPAGE + 1)*(RANK_PERPAGE)-0.02)));
	scoreMenu->addChild(rankLabel);

    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.6);
    OK->setPosition(ccp(winSize.width * 0.3, winSize.height*(1.0/(RANK_PERPAGE + 1)+0.02)));
    CCMenuItemImage * upload = CCMenuItemImage::create("upload_score_normal.png", "upload_score_selected.png" ,
            this , menu_selector(StartMenuScene::uploadHandler));
    upload->setScale(0.6);
    upload->setPosition(ccp(winSize.width*0.7, winSize.height*(1.0/(RANK_PERPAGE + 1)+0.02)));
    CCMenu * okMenu = CCMenu::create(OK , upload , NULL);
    okMenu->setPosition(ccp(0, 0));
    okMenu->setAnchorPoint(ccp(0,0));
    okMenu->setContentSize(CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1)*2));
    okMenu->ignoreAnchorPointForPosition(false);
    scoreMenu->addChild(okMenu);

    scoreTable = CCTableView::create(this ,
    		CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1)*(RANK_PERPAGE-3)));
    scoreTable->setDirection(kCCScrollViewDirectionVertical);
    //scoreTable->setAnchorPoint(ccp(0 , 1));
    scoreTable->setPosition(ccp(0, winSize.height/(RANK_PERPAGE + 1) * 2));
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
	menus.push_back(userMenu);
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
    OK->setScale(0.6);
    OK->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.45));
    CCMenuItemImage * login = CCMenuItemImage::create("login_normal.png", "login_selected.png" ,
            this , menu_selector(StartMenuScene::loginHandler));
    login->setScale(0.6);
    login->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.3));
    CCMenuItemImage * create = CCMenuItemImage::create("register_normal.png", "register_selected.png" ,
            this , menu_selector(StartMenuScene::RegisterHandler));
    create->setScale(0.6);
    create->setPosition(ccp(winSize.width*0.5, winSize.height * 0.15));
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
    if(checkName(nameField->getString()) == false){
        setInfoLabel(NAME_RULE , 0);
        return;
    }
    if(checkPWD(pwdField->getString()) == false){
        setInfoLabel(PASSWORD_RULE , 0);
        return;
    }
    GameController::getGameController()->createUser(nameField->getString() , pwdField->getString());
}

bool StartMenuScene::checkName(const char * name){
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

bool StartMenuScene::checkPWD(const char * password){
    int length = strlen(password);

    if(length > 12 || length < 4) return false;

    return true;
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
    if(userData->userName.length() == 0) nameField->setString(DEFAULT_NAME);
    if(userData->password.length() == 0) pwdField->setString(DEFAULT_PASSWORD);
    nameField->runAction(CCBlink::create(2, 6));
    pwdField->runAction(CCBlink::create(2, 6));
    showMenu(userMenu);
}

void StartMenuScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
	CCLOG("cell touched at index: %i", cell->getIdx());
}

CCTableViewCell * StartMenuScene::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell * cell = table->dequeueCell();

    if(table == scoreTable) return cellForScore(cell, idx);
    if(table == legendsTable) return cellForLegends(cell, idx);
}

CCTableViewCell * StartMenuScene::cellForScore(CCTableViewCell * cell, int idx){
    ScoreRank * rank = &GameController::getGameController()->ranks[idx];
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

CCTableViewCell * StartMenuScene::cellForLegends(CCTableViewCell *cell , int index){

    if (index > BAD_SPECIAL_NUM - 1) return NULL;

    if(index > SPECIAL_NUM - 1) index+=1;

    SpecialData * data = GameController::getGameController()->getSpecialData(index);

    float patch = 1.5/2;

    if(!cell){
        cell = new CCTableViewCell();
        cell->autorelease();

        CCSprite * legend = CCSprite::createWithSpriteFrameName(data->imageName->getCString());
        legend->setPosition(ccp(winSize.width*0.075, winSize.height/(LEGENDS_PERPAGE + 1)*patch*2));
        legend->setTag(LEGEND_LABEL);
        legend->setAnchorPoint(ccp(0.5 , 1.05));
        cell->addChild(legend);

        CCLabelTTF * name = CCLabelTTF::create("0", INFO_FONT, 32 ,
                CCSizeMake(winSize.width*0.2 , winSize.height/(LEGENDS_PERPAGE + 1)*patch*2) ,
                kCCTextAlignmentLeft);
        name->setColor( ccRED );
        name->setString(data->name->getCString());
        name->setPosition(ccp(winSize.width*0.25, winSize.height/(LEGENDS_PERPAGE + 1)*patch));
        name->setTag(LEGEND_NAME_LABEL);
        cell->addChild(name);

        CCLabelTTF * description = CCLabelTTF::create("0", INFO_FONT, 30 ,
                CCSizeMake(winSize.width*0.7 , winSize.height/(LEGENDS_PERPAGE + 1)*patch*2) ,
                kCCTextAlignmentLeft);
        description->setColor( ccRED );
        description->setString(data->description->getCString());
        description->setPosition(ccp(winSize.width*0.675, winSize.height/(LEGENDS_PERPAGE + 1)*patch));
        description->setTag(LEGEND_DESCRIPTION_LABEL);
        cell->addChild(description);
    }else{
        CCLabelTTF * description = (CCLabelTTF *)cell->getChildByTag(LEGEND_DESCRIPTION_LABEL);
        description->setString(data->description->getCString());

        CCLabelTTF * name = (CCLabelTTF *)cell->getChildByTag(LEGEND_NAME_LABEL);
        name->setString(data->name->getCString());

        cell->removeChildByTag(LEGEND_LABEL);
        CCSprite * legend = CCSprite::createWithSpriteFrameName(data->imageName->getCString());
        legend->setPosition(ccp(winSize.width*0.075, winSize.height/(LEGENDS_PERPAGE + 1)*patch*2));
        legend->setTag(LEGEND_LABEL);
        legend->setAnchorPoint(ccp(0.5 , 1.05));
        cell->addChild(legend);
    }

    return cell;
}

unsigned int StartMenuScene::numberOfCellsInTableView(CCTableView *table){
    if(table == scoreTable) return MAX_RANKS;
    if(table == legendsTable) return BAD_SPECIAL_NUM - 1;
}

CCSize StartMenuScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
    if(table == scoreTable) return CCSizeMake(winSize.width , winSize.height/(RANK_PERPAGE + 1));
    if(table == legendsTable){
        return CCSizeMake(winSize.width , winSize.height/(LEGENDS_PERPAGE + 1)*1.5);
    }
}

void StartMenuScene::scrollViewDidScroll(CCScrollView* view){

}

void StartMenuScene::scrollViewDidZoom(CCScrollView* view){

}

void StartMenuScene::initMainMenu(){
    CCMenuItemImage * newGame = CCMenuItemImage::create("button_new_game_normal.png",
            "button_new_game_selected.png", this, menu_selector(StartMenuScene::newGameHandler));
    newGame->setTag(NEW_GAME);

    CCMenuItemImage * options = CCMenuItemImage::create("button_options_normal.png",
            "button_options_selected.png" , this , menu_selector(StartMenuScene::optionsHandler));
    options->setTag(OPTIONS);

    CCMenuItemImage * score = CCMenuItemImage::create("top_score_normal.png",
            "top_score_selected.png" , this , menu_selector(StartMenuScene::scoreHandler));
    score->setTag(TOP_SCORE);
    
    CCMenuItemImage * account = CCMenuItemImage::create("account_normal.png", "account_selected.png" , this , menu_selector(StartMenuScene::userHandler));
    
    CCMenuItemImage * legends = CCMenuItemImage::create("legends_normal.png", "legends_selected.png" , this , menu_selector(StartMenuScene::legendsHandler));

    CCMenuItemImage * background = CCMenuItemImage::create("background_main.png" , "background_main.png");
    CCSize size = background->getContentSize();
    background->setScaleY(winSize.height/size.height);
    background->setScaleX(winSize.width/size.width);
    background->setAnchorPoint(ccp(0,0));
    background->setEnabled(false);

#ifdef MULTIPLAY
    CCMenuItemImage * pvp = CCMenuItemImage::create("pvp_normal.png", "pvp_selected.png" ,
    		this , menu_selector(StartMenuScene::pvpHandler));
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    newGame->setScale(0.7);
    options->setScale(0.7);
    score->setScale(0.7);
    CCMenuItemImage * pvp = CCMenuItemImage::create("purchase_normal.png", "purchase_selected.png" ,
    		this , menu_selector(StartMenuScene::pvpHandler));
    pvp->setScale(0.7);
    pvp->setTag(PVP);
    account->setScale(0.7);
    legends->setScale(0.7);

    startMenu = CCMenu::create(background , newGame , options, score , pvp , legends,account,NULL);
    startMenu->ignoreAnchorPointForPosition(false);
    newGame->setPosition(ccp(winSize.width*0.3 , winSize.height*0.45));
    options->setPosition(ccp(winSize.width*0.7 , winSize.height*0.45));
    score->setPosition(ccp(winSize.width*0.3 , winSize.height*0.3));
    pvp->setPosition(ccp(winSize.width*0.7 , winSize.height*0.3));
    account->setPosition(ccp(winSize.width*0.3, winSize.height*0.15));
    legends->setPosition(ccp(winSize.width*0.7, winSize.height*0.15));
#else
    CCMenuItemImage * credits = CCMenuItemImage::create("credits_normal.png", "credits_selected.png" , this , menu_selector(StartMenuScene::okHandler));
    credits->setScale(0.7);
    newGame->setScale(0.7);
    options->setScale(0.7);
    score->setScale(0.7);
    account->setScale(0.7);
    legends->setScale(0.7);
    startMenu = CCMenu::create(background, newGame, options, score, legends, account, NULL);
    startMenu->ignoreAnchorPointForPosition(false);
    newGame->setPosition(ccp(winSize.width*0.3 , winSize.height*0.45));
    options->setPosition(ccp(winSize.width*0.7 , winSize.height*0.45));
    score->setPosition(ccp(winSize.width*0.3 , winSize.height*0.3));
    credits->setPosition(ccp(winSize.width*0.7, winSize.height*0.3));
    account->setPosition(ccp(winSize.width*0.3, winSize.height*0.15));
    legends->setPosition(ccp(winSize.width*0.7, winSize.height*0.15));
#endif

    addChild(startMenu);
    menus.push_back(startMenu);
}

void StartMenuScene::pvpHandler(cocos2d::CCObject *sender){
	showMenu(purchaseMenu);
}

void StartMenuScene::purchaseHandler(cocos2d::CCObject *sender){
#ifdef MULTIPLAY
    CCScene * pvpScene = MultiPlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(pvpScene);
#else
    enableButtonsForIap(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    purchase(GameController::getGameController());
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
    showMenu(scoreMenu);
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
    checkboxLeft->setScale(0.22);
    left->setPosition(ccp(winSize.width * 0.4, winSize.height*0.78));
    checkboxLeft->setPosition(ccp(winSize.width*0.8, winSize.height*0.78));
    left->setEnabled(false);

    //row 3
    CCMenuItemLabel * right = CCMenuItemLabel::create(
    		CCLabelTTF::create("ON THE RIGHT", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    right->setDisabledColor(ccMAGENTA);
    checkboxRight = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxRight->setTag(RIGHT);
    checkboxRight->setScale(0.22);
    right->setPosition(ccp(winSize.width * 0.4, winSize.height*0.7));
    checkboxRight->setPosition(ccp(winSize.width*0.8, winSize.height*0.7));
    right->setEnabled(false);

    //row 4
    CCMenuItemLabel * leftUp = CCMenuItemLabel::create(
    		CCLabelTTF::create("LEFT UP RIGHT DOWN", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    leftUp->setDisabledColor(ccORANGE);
    checkboxSideLeftUp = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxSideLeftUp->setTag(SIDE_LEFT_UP);
    checkboxSideLeftUp->setScale(0.22);
    leftUp->setPosition(ccp(winSize.width * 0.4, winSize.height*0.62));
    checkboxSideLeftUp->setPosition(ccp(winSize.width*0.8, winSize.height*0.62));
    leftUp->setEnabled(false);

    CCMenuItemLabel * leftDown = CCMenuItemLabel::create(
    		CCLabelTTF::create("LEFT DOWN RIGHT UP", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    leftDown->setDisabledColor(ccYELLOW);
    checkboxSideLeftDown = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxSideLeftDown->setTag(SIDE_LEFT_DOWN);
    checkboxSideLeftDown->setScale(0.22);
    leftDown->setPosition(ccp(winSize.width * 0.4, winSize.height*0.54));
    checkboxSideLeftDown->setPosition(ccp(winSize.width*0.8, winSize.height*0.54));
    leftDown->setEnabled(false);

    //row 5
    CCMenuItemLabel * sound = CCMenuItemLabel::create(
    		CCLabelTTF::create("SOUND", FONT , 64));
    sound->setDisabledColor(ccBLUE);
    sound->setPosition(ccp(winSize.width/2, winSize.height*0.42));
    sound->setEnabled(false);

    //row 6
    CCMenuItemLabel * mute = CCMenuItemLabel::create(
    		CCLabelTTF::create("MUTE", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    mute->setDisabledColor(ccRED);
    checkboxMute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxMute->setTag(MUTE);
    mute->setPosition(ccp(winSize.width * 0.4, winSize.height*0.32));
    checkboxMute->setPosition(winSize.width*0.8, winSize.height*0.32);
    checkboxMute->setScale(0.22);
    mute->setEnabled(false);

    //row 7
    CCMenuItemLabel * unMute = CCMenuItemLabel::create(
    		CCLabelTTF::create("UNMUTE", FONT , 48 ,
    		CCSizeMake(winSize.width*0.5 , winSize.height/(RANK_PERPAGE + 1)) ,  kCCTextAlignmentLeft));
    unMute->setDisabledColor(ccBLACK);
    checkboxUnmute = CCMenuItemImage::create("check_box_normal.png", "check_box_selected.png" , this , menu_selector(StartMenuScene::checkboxHandler));
    checkboxUnmute->setTag(UNMUTE);
    unMute->setPosition(ccp(winSize.width * 0.4, winSize.height*0.24));
    checkboxUnmute->setPosition(winSize.width*0.8, winSize.height*0.24);
    checkboxUnmute->setScale(0.22);
    unMute->setEnabled(false);

    //row 8
    CCMenuItemImage * OK = CCMenuItemImage::create("return_normal.png", "return_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    OK->setScale(0.6);
    OK->setPosition(ccp(winSize.width*0.3, winSize.height*0.12));

    CCMenuItemImage * credits = CCMenuItemImage::create("credits_normal.png", "credits_selected.png" ,
            this , menu_selector(StartMenuScene::okHandler));
    credits->setScale(0.6);
    credits->setPosition(ccp(winSize.width*0.7, winSize.height*0.12));

    optionsMenu = CCMenu::create(controllerPositions , left , checkboxLeft , right , checkboxRight ,
    		leftUp , checkboxSideLeftUp , leftDown , checkboxSideLeftDown ,
    		sound , mute , checkboxMute , unMute, checkboxUnmute, OK , credits , NULL);

    optionsMenu->setPosition(ccp(winSize.width/2, winSize.height*1.5));
    optionsMenu->ignoreAnchorPointForPosition(false);
    addChild(optionsMenu);
    menus.push_back(optionsMenu);

    checkboxChooser(userData->sound);
    checkboxChooser(userData->controllerPosition);
}

void StartMenuScene::newGameHandler(cocos2d::CCObject *sender){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (!userData->hasPayed && userData->justFailed%FREE_PLAY == 0 && userData->justFailed != 0)
    {
            showAds();
            userData->justFailed = 0;
			return;
    }
#endif
	CCScene * playScene = PlayScene::scene();
    CCDirector::sharedDirector()->replaceScene(playScene);
}

void StartMenuScene::setInfoLabel(const char *info , float delay){
    if(infoLabel == NULL) return;
    infoLabel->setString(info);
    showMenu(infoMenu);
}

void StartMenuScene::optionsHandler(cocos2d::CCObject *sender){
    showMenu(optionsMenu);
}

void StartMenuScene::okHandler(cocos2d::CCObject *sender){
    showMenu(startMenu);
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

void StartMenuScene::enableButtonsForIap(bool enable){
    CCMenuItem * purchase = (CCMenuItem *)purchaseMenu->getChildByTag(PURCHASE);
    if(purchase) purchase->setEnabled(enable);
}

bool StartMenuScene::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *sender){
    sender->setString("");
    return false;
}

//bool StartMenuScene::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen){
//    for(int i = 0 ; i < nLen ; i++){
//        int c = *(text+i);
//        CCLOG("onTextFieldInsertText %d %d", c , '\n');
//    }
//    return false;
//}

void StartMenuScene::showMenu(CCLayer * menu){
    for(int i = 0 ; i < menus.size() ; i++){
        if(menu == menus[i]){
            menus[i]->setPosition(ccp(winSize.width/2, winSize.height/2));
        }else{
            menus[i]->setPosition(ccp(winSize.width/2, winSize.height*1.5));
        }
    }
}

void StartMenuScene::legendsHandler(CCObject * sender){
    showMenu(legendsMenu);
}


