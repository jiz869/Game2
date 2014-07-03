//
//  App42RewardResponse.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#include "App42RewardResponse.h"
#include "Common.h"


App42RewardResponse::App42RewardResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
:App42Response(pTarget,pSelector)
{
    
}

App42RewardResponse::~App42RewardResponse()
{
    //games.clear();
}

void App42RewardResponse::onComplete(cocos2d::CCNode *sender, void *data)
{
    App42Response::onComplete(sender, data);
    init();
    
    
    if (_pTarget && _pSelector)
    {
        (_pTarget->*_pSelector)((cocos2d::CCNode *)_pTarget, this);
    }
    
}

void App42RewardResponse::init()
{
    if(_result != 200)
    {
        Util::app42Trace("App42Reward failed result is %d", _result);
        buildErrorMessage();
        return;
    }
    cJSON *ptrBody = cJSON_Parse(_body.c_str());
    cJSON* ptrApp42 = Util::getJSONChild("app42", ptrBody);
    cJSON* ptrResponse = Util::getJSONChild("response", ptrApp42);
    cJSON* ptrRewards = Util::getJSONChild("rewards", ptrResponse);
    cJSON* ptrReward = Util::getJSONChild("reward", ptrRewards);
    
    cJSON* child = ptrReward;
    if(child->type == cJSON_Array)
    {
        child = child->child;
    }
    while(child != NULL && child->type == cJSON_Object)
    {
        App42Reward app42Reward;
        app42Reward.name = Util::getJSONString("name", child);
        app42Reward.description = Util::getJSONString("description", child);
        app42Reward.gameName = Util::getJSONString("gameName", child);
		app42Reward.points = Util::getJSONDouble("points", child);
        rewards.push_back(app42Reward);
        child = child->next;
    }
    cJSON_Delete(ptrBody);
    
}