/*
 * GameOverScene.h
 *
 *  Created on: Jul 4, 2014
 *      Author: mli
 */

#ifndef GAMEOVERSCENE_H_
#define GAMEOVERSCENE_H_

#include "GameController.h"

class GameOverScene: public cocos2d::CCLayer , CCIMEDelegate {
public:
	GameOverScene();
	virtual ~GameOverScene();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameOverScene);
    virtual bool init();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
protected:
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
private:
    CCSize winSize;
    CCTextFieldTTF * nameField;
    CCSprite * OK;
};

#endif /* GAMEOVERSCENE_H_ */
