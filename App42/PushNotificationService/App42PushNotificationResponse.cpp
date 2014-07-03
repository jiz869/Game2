//
//  App42PushNotificationResponse.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#include "App42PushNotificationResponse.h"

#include "Common.h"


App42PushNotificationResponse::App42PushNotificationResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
:App42Response(pTarget,pSelector)
{
    
}

App42PushNotificationResponse::~App42PushNotificationResponse()
{
    
}

void App42PushNotificationResponse::onComplete(cocos2d::CCNode *sender, void *data)
{
    App42Response::onComplete(sender, data);
    init();
    
    
    if (_pTarget && _pSelector)
    {
        (_pTarget->*_pSelector)((cocos2d::CCNode *)_pTarget, this);
    }
    
}

void App42PushNotificationResponse::init()
{
    if(_result != 200)
    {
        Util::app42Trace("App42PushNotification failed result is %d", _result);
        buildErrorMessage();
        return;
    }
    // parse the body
    cJSON *ptrBody = cJSON_Parse(_body.c_str());
    cJSON* ptrApp42 = Util::getJSONChild("app42", ptrBody);
    cJSON* ptrResponse = Util::getJSONChild("response", ptrApp42);
    cJSON* ptrPush = Util::getJSONChild("push", ptrResponse);
    
    
    cJSON* child = ptrPush;
    if(child->type == cJSON_Array)
    {
        child = child->child;
    }
    
    while(child != NULL && child->type == cJSON_Object)
    {
        App42PushNotification app42PushNotification;
        app42PushNotification.userName = Util::getJSONString("userName", child);
        app42PushNotification.message = Util::getJSONString("message", child);
        app42PushNotification.type = Util::getJSONString("type", child);
        app42PushNotification.deviceToken = Util::getJSONString("deviceToken", child);
        notification.push_back(app42PushNotification);
        child = child->next;
    }
    
    cJSON_Delete(ptrBody);
}
