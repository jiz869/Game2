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

class StartMenuScene: public cocos2d::CCLayerColor , IAPManagerDelegate ,
									CCTableViewDelegate , CCTableViewDataSource {
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
    void hideInfoLabel();
    virtual void onPaymentError();
    virtual void onPaymentSuccess();
    virtual bool hasPayed();
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void keyBackClicked();
    void setPaymentError();
    void setPaymentSuccess();

private:
    CCMenu * startMenu;
    CCMenu * optionsMenu;
#ifdef LISTVIEW_LEADERBOARD
    CCLayer * scoreMenu;
    CCTableView * scoreTable;
#else
    CCMenu * scoreMenu;
#endif
    CCSize winSize;
    void initMainMenu();
    void initOptionsMenu();
    void initScoreMenu();
    CCMenuItemImage * checkboxLeft;
    CCMenuItemImage * checkboxRight;
    CCMenuItemImage * checkboxSide;
    CCMenuItemImage * checkboxMute;
    CCMenuItemImage * checkboxUnmute;
    UserData * userData;
    void checkboxChooser(CheckboxType tag);
    void saveUserData(CheckboxType tag);

    CCMenuItemImage * gems[MAX_RANKS];
    CCMenuItemLabel * scoreLabels[MAX_RANKS];
    CCMenuItemLabel * nameLabels[MAX_RANKS];

    CCLabelTTF * infoLabel;

    void setInfoLabel(const char * info , float delay);

    void enableButtonsForIap(bool enable);

    CCLabelTTF * rankLabel;
};

#endif /* defined(__crossRoad__StartMenuScene__) */
