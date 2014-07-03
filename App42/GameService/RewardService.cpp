//
//  RewardService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#include "RewardService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42RewardResponse.h"
#include "Connector.h"

// define the static..
RewardService* RewardService::_instance = NULL;

RewardService::RewardService()
{
    
}

RewardService* RewardService::Initialize(string apikey, string secretkey)
{
    if(_instance == NULL)
    {
        _instance = new RewardService();
    }
    _instance->Init(apikey,secretkey);
    return _instance;
}

RewardService* RewardService::getInstance()
{
    return _instance;
}

void RewardService::Terminate()
{
    if(_instance != NULL)
    {
        delete _instance;
		_instance = NULL;
    }
}

string BuildCreateRewardBody(string rewardName, string descrption)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *rewardsJSON = cJSON_CreateObject();
    cJSON *rewardJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(rewardJSON, "name", rewardName.c_str());
    cJSON_AddStringToObject(rewardJSON, "description", descrption.c_str());
    cJSON_AddItemReferenceToObject(rewardsJSON, "reward", rewardJSON);
    
    cJSON_AddItemReferenceToObject(app42JSON, "rewards", rewardsJSON);
    
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(rewardsJSON);
    cJSON_Delete(rewardJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    
    free(cptrFormatted);
    
    return bodyString;
    
}

string BuildRewardBody(string gameName, string userName, string rewardName, double rewardPoints)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *rewardsJSON = cJSON_CreateObject();
    cJSON *rewardJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(rewardJSON, "gameName", gameName.c_str());
    cJSON_AddNumberToObject(rewardJSON, "points", rewardPoints);
    cJSON_AddStringToObject(rewardJSON, "name", rewardName.c_str());
    cJSON_AddStringToObject(rewardJSON, "userName", userName.c_str());
    cJSON_AddItemReferenceToObject(rewardsJSON, "reward", rewardJSON);
    
    cJSON_AddItemReferenceToObject(app42JSON, "rewards", rewardsJSON);
    
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(rewardsJSON);
    cJSON_Delete(rewardJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    
    free(cptrFormatted);
    
    return bodyString;
}

void RewardService::CreateReward(string rewardName,string description, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    
    App42RewardResponse *response = new App42RewardResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(description, "Description");
        Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
    }
    catch (App42Exception *e)
    {
        std::string ex = e->what();
        response->httpErrorCode = e->getHttpErrorCode();
        response->appErrorCode  = e->getAppErrorCode();
        response->errorDetails  = ex;
        response->isSuccess = false;
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)((cocos2d::CCNode *)pTarget, response);
        }
        delete e;
        e = NULL;
        return;
    }

    
    map<string, string> postMap;
    populateSignParams(postMap);
    string rewardbody = BuildCreateRewardBody(rewardName, description);
    postMap["body"] = rewardbody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/reward");
    baseUrl.append("?");
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n createRewardbody = %s",rewardbody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, rewardbody.c_str(), response, callfuncND_selector(App42RewardResponse::onComplete));
    
}

void RewardService::GetAllRewards(cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42RewardResponse *response = new App42RewardResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
    }
    catch (App42Exception *e)
    {
        std::string ex = e->what();
        response->httpErrorCode = e->getHttpErrorCode();
        response->appErrorCode  = e->getAppErrorCode();
        response->errorDetails  = ex;
        response->isSuccess = false;
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)((cocos2d::CCNode *)pTarget, response);
        }
        delete e;
        e = NULL;
        return;
    }

    string resource = "game/reward";
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42RewardResponse::onComplete));

}

void RewardService::EarnRewards(string gameName, string userName, string rewardName, double rewardPoints,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42RewardResponse *response = new App42RewardResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
        Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
        Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
    }
    catch (App42Exception *e)
    {
        std::string ex = e->what();
        response->httpErrorCode = e->getHttpErrorCode();
        response->appErrorCode  = e->getAppErrorCode();
        response->errorDetails  = ex;
        response->isSuccess = false;
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)((cocos2d::CCNode *)pTarget, response);
        }
        delete e;
        e = NULL;
        return;
    }

    map<string, string> postMap;
    populateSignParams(postMap);
    string rewardbody = BuildRewardBody(gameName, userName,rewardName,rewardPoints);
    postMap["body"] = rewardbody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/reward/earn");
    baseUrl.append("?");
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n createRewardbody = %s",rewardbody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, rewardbody.c_str(), response, callfuncND_selector(App42RewardResponse::onComplete));
}

void RewardService::RedeemRewards(string gameName, string userName, string rewardName, double rewardPoints,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42RewardResponse *response = new App42RewardResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
        Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
        Util::throwExceptionIfStringNullOrBlank(rewardName, "Reward Name");
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
    }
    catch (App42Exception *e)
    {
        std::string ex = e->what();
        response->httpErrorCode = e->getHttpErrorCode();
        response->appErrorCode  = e->getAppErrorCode();
        response->errorDetails  = ex;
        response->isSuccess = false;
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)((cocos2d::CCNode *)pTarget, response);
        }
        delete e;
        e = NULL;
        return;
    }

    map<string, string> postMap;
    populateSignParams(postMap);
    string rewardbody = BuildRewardBody(gameName, userName,rewardName,rewardPoints);
    postMap["body"] = rewardbody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/reward/redeem");
    baseUrl.append("?");
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n createRewardbody = %s",rewardbody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, rewardbody.c_str(), response, callfuncND_selector(App42RewardResponse::onComplete));
}

void RewardService::GetGameRewardPointsForUser(string gameName, string userName, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42RewardResponse *response = new App42RewardResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
        Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
    }
    catch (App42Exception *e)
    {
        std::string ex = e->what();
        response->httpErrorCode = e->getHttpErrorCode();
        response->appErrorCode  = e->getAppErrorCode();
        response->errorDetails  = ex;
        response->isSuccess = false;
        if (pTarget && pSelector)
        {
            (pTarget->*pSelector)((cocos2d::CCNode *)pTarget, response);
        }
        delete e;
        e = NULL;
        return;
    }

    string resource = "game/reward/";
    resource.append(gameName+"/");
    resource.append(userName);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["gameName"] = gameName;
    getMap["userName"] = userName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42RewardResponse::onComplete));
}
