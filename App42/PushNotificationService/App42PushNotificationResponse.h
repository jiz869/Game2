//
//  App42PushNotificationResponse.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#ifndef __App42CPPSDK__App42PushNotificationResponse__
#define __App42CPPSDK__App42PushNotificationResponse__

#include <iostream>
#include "cJSON.h"
#include "App42Response.h"

using namespace cocos2d::extension;
using namespace std;

struct App42PushNotification;

class App42PushNotificationResponse : public App42Response
{
public:
    vector<App42PushNotification> notification;
    App42PushNotificationResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);
	~App42PushNotificationResponse();
    void onComplete(cocos2d::CCNode *sender, void *data);
private:
    void init();
    
};

typedef struct App42PushNotification
{
    string message;
    string userName;
    string type;
    string deviceToken;
    
}App42PushNotification;

#endif /* defined(__App42CPPSDK__App42PushNotificationResponse__) */
