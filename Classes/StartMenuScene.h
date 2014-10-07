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

USING_NS_CC;
USING_NS_CC_EXT;

class PWDField;

class StartMenuScene: public cocos2d::CCLayerColor , CCTableViewDelegate ,
                                    CCTableViewDataSource , CCTextFieldDelegate{
public:
	StartMenuScene();
	virtual ~StartMenuScene();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(StartMenuScene);
    virtual bool init();
    void newGameHandler(CCObject * sender);
    void newGameMenuHandler(CCObject * sender);
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
    void purchaseHandler(CCObject * sender);
    void restoreHandler(CCObject * sender);
    void creditsHandler(CCObject * sender);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void keyBackClicked();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void setInfoLabel(const char * info , float delay);

    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
//    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    void enableButtonsForIap(bool enable);
    void showInfoLabel();
    void reloadScore();
    void onAdsClicked();

private:
    CCMenu * infoMenu;
    CCMenu * startMenu;
    CCMenu * optionsMenu;
    CCLayer * userMenu;
    CCLayer * legendsMenu;
    CCLayer * purchaseMenu;
    CCLayer * newGameMenu;
    CCMenu * creditsMenu;
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
    void initPurchaseMenu();
    void initNewGameMenu();
    void initCreditsMenu();
    CCMenuItemImage * checkboxLeft;
    CCMenuItemImage * checkboxRight;
    CCMenuItemImage * checkboxSideLeftUp;
	CCMenuItemImage * checkboxSideLeftDown;
    CCMenuItemImage * checkboxMute;
    CCMenuItemImage * checkboxUnmute;
    UserData * userData;
    StrData * strData;
    void checkboxChooser(CheckboxType tag);
    void saveUserData(CheckboxType tag);

    CCMenuItemImage * gems[MAX_RANKS];
    CCMenuItemLabel * scoreLabels[MAX_RANKS];
    CCMenuItemLabel * nameLabels[MAX_RANKS];

    CCMenuItemLabel * infoLabel;

    CCLabelTTF * rankLabel;

    CCTextFieldTTF * nameField;
    PWDField * pwdField;

    string password;

    void savePWD();
                                        
    vector<CCLayer *> menuStack;
    CCLayer * currentMenu;

    bool checkName(const char * name);
    bool checkPWD(const char * password);
    vector<CCLayer *> menus;
    void showMenu(CCLayer * menu);
    CCTableViewCell * cellForScore(CCTableViewCell * cell , int idx);
    CCTableViewCell * cellForLegends(CCTableViewCell * cell , int index);

    string info;
};

#endif /* defined(__crossRoad__StartMenuScene__) */
