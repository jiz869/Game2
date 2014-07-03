//
//  App42UserResponse.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 14/10/13.
//
//

#include "App42UserResponse.h"
#include "Common.h"

App42UserResponse::App42UserResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
:App42Response(pTarget,pSelector)
{
    
}

App42UserResponse::~App42UserResponse()
{
	users.clear();
}

void App42UserResponse::init()
{
    if(_result != 200)
    {
        Util::app42Trace("App42User failed result is %d", _result);
        buildErrorMessage();
        return;
    }
    // parse the body
    cJSON *ptrBody = cJSON_Parse(_body.c_str());
    cJSON* ptrApp42 = Util::getJSONChild("app42", ptrBody);
    cJSON* ptrResponse = Util::getJSONChild("response", ptrApp42);
    cJSON* ptrUsers = Util::getJSONChild("users", ptrResponse);
    cJSON* ptrUser = Util::getJSONChild("user", ptrUsers);
    
    
    cJSON* child = ptrUser;
    if(child->type == cJSON_Array)
    {
        child = child->child;
    }
    
    while(child != NULL && child->type == cJSON_Object)
    {
        App42User app42User;
        app42User.userName = Util::getJSONString("userName", child);
        app42User.email = Util::getJSONString("email", child);
        users.push_back(app42User);
        child = child->next;
    }
    
    cJSON_Delete(ptrBody);
}



void App42UserResponse::onComplete(cocos2d::CCNode *sender, void *data)
{
    App42Response::onComplete(sender, data);
    init();
    
    
    if (_pTarget && _pSelector)
    {
        (_pTarget->*_pSelector)((cocos2d::CCNode *)_pTarget, this);
    }

}

