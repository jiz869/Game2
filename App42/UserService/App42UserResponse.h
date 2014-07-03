//
//  App42UserResponse.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 14/10/13.
//
//

#ifndef __App42CPPSDK__App42UserResponse__
#define __App42CPPSDK__App42UserResponse__

#include <iostream>
#include "cJSON.h"
#include "App42Response.h"

using namespace cocos2d::extension;
using namespace std;
struct App42User;

class App42UserResponse : public App42Response
{
public:
    App42UserResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);
    vector<App42User> users;
	~App42UserResponse();
    void onComplete(cocos2d::CCNode *sender, void *data);
private:
    void init();
    
};

typedef struct App42User
{
    string userName;
    string email;
    bool accountLocked;
    
}App42User;

#endif /* defined(__App42CPPSDK__App42UserResponse__) */


