//
//  App42RewardResponse.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#ifndef __App42CPPSDK__App42RewardResponse__
#define __App42CPPSDK__App42RewardResponse__

#include <iostream>
#include "cJSON.h"
#include "App42Response.h"

using namespace cocos2d::extension;
using namespace std;

struct App42Reward;
class App42RewardResponse : public App42Response
{
public:
    
    vector<App42Reward> rewards;
    App42RewardResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);
	~App42RewardResponse();
    void onComplete(cocos2d::CCNode *sender, void *data);
private:
    void init();
    
};

typedef struct App42Reward
{
    string name;
    string gameName;
	double points;
    string description;
}App42Reward;

#endif /* defined(__App42CPPSDK__App42RewardResponse__) */
