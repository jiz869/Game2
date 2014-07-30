//
//  StartMenuScene.h
//  crossRoad
//
//  Created by Min Li on 2014-05-30.
//
//

#ifndef __crossRoad__StartMenuScene__
#define __crossRoad__StartMenuScene__

#define CC_ENABLE_CHIPMUNK_INTEGRATION 0
#define CC_ENABLE_BOX2D_INTEGRATION 0

#include "GameController.h"
#include "cocos-ext.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "../proj.ios/P2PCPPHelper.h"
#else
class IAPManagerDelegate {
public:
    virtual void onPaymentError() = 0;
    virtual bool hasPayed() = 0;
    virtual void onPaymentSuccess() = 0;
};
#endif

USING_NS_CC;
USING_NS_CC_EXT;

class PWDField;

class StartMenuScene: public cocos2d::CCLayerColor , IAPManagerDelegate ,
									CCTableViewDelegate , CCTableViewDataSource
									 , CCTextFieldDelegate{
public:
	StartMenuScene();
	virtual ~StartMenuScene();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StartMenuScene);
    virtual bool init();
    void newGameHandler(CCObject * sender);
    void optionsHandler(CCObject * sender);
    void okHandler(CCObject * sender);
    void checkboxHandler(CCObject * sender);
    void changeSoundSetting(CheckboxType type);
    void scoreHandler(CCObject * sender);
    void pvpHandler(CCObject * sender);
    void userHandler(CCObject * sender);
    void loginHandler(CCObject * sender);
    void RegisterHandler(CCObject * sender);
    void uploadHandler(CCObject * sender);
    void legendsHandler(CCObject * sender);
    virtual void onPaymentError();
    virtual void onPaymentSuccess();
    virtual bool hasPayed();
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void keyBackClicked();
    void setPaymentError();
    void setPaymentSuccess();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void setInfoLabel(const char * info , float delay);

    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
//    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);

private:
    CCMenu * infoMenu;
    CCMenu * startMenu;
    CCMenu * optionsMenu;
    CCLayer * userMenu;
    CCLayer * legendsMenu;
#ifdef LISTVIEW_LEADERBOARD
    CCLayer * scoreMenu;
    CCTableView * scoreTable;
    CCTableView * legendsTable;
#else
    CCMenu * scoreMenu;
#endif

    CCSize winSize;
    void initMainMenu();
    void initOptionsMenu();
    void initScoreMenu();
    void initInfoMenu();
    void initUserMenu();
    void initLegendsMenu();
    CCMenuItemImage * checkboxLeft;
    CCMenuItemImage * checkboxRight;
    CCMenuItemImage * checkboxSideLeftUp;
	CCMenuItemImage * checkboxSideLeftDown;
    CCMenuItemImage * checkboxMute;
    CCMenuItemImage * checkboxUnmute;
    UserData * userData;
    void checkboxChooser(CheckboxType tag);
    void saveUserData(CheckboxType tag);

    CCMenuItemImage * gems[MAX_RANKS];
    CCMenuItemLabel * scoreLabels[MAX_RANKS];
    CCMenuItemLabel * nameLabels[MAX_RANKS];

    CCMenuItemLabel * infoLabel;

    void enableButtonsForIap(bool enable);

    CCLabelTTF * rankLabel;

    CCTextFieldTTF * nameField;
    PWDField * pwdField;

    string password;

    void savePWD();

    bool checkName(const char * name);
    bool checkPWD(const char * password);
    vector<CCLayer *> menus;
    void showMenu(CCLayer * menu);
    CCTableViewCell * cellForScore(CCTableViewCell * cell , int idx);
    CCTableViewCell * cellForLegends(CCTableViewCell * cell , int index);
};

#endif /* defined(__crossRoad__StartMenuScene__) */
