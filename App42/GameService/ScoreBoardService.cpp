//
//  ScoreBoardService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#include "ScoreBoardService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42GameResponse.h"
#include "Connector.h"

// define the static..
ScoreBoardService* ScoreBoardService::_instance = NULL;

ScoreBoardService::ScoreBoardService()
{
    
}

ScoreBoardService* ScoreBoardService::Initialize(string apikey, string secretkey)
{
    if(_instance == NULL)
    {
        _instance = new ScoreBoardService();
    }
    _instance->Init(apikey,secretkey);
    return _instance;
}

ScoreBoardService* ScoreBoardService::getInstance()
{
    return _instance;
}

void ScoreBoardService::Terminate()
{
    if(_instance != NULL)
    {
        delete _instance;
		_instance = NULL;
    }
}

string BuildSaveUserScoreBody(string gameName, string userName, double score)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *gameJSON = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    cJSON *scoreJSON = cJSON_CreateObject();
    
    cJSON_AddNumberToObject(scoreJSON, "value", score);
    cJSON_AddStringToObject(scoreJSON, "userName", userName.c_str());
    cJSON_AddItemReferenceToObject(sJSON, "score", scoreJSON);
    
    cJSON_AddItemReferenceToObject(gameJSON, "scores", sJSON);
    cJSON_AddStringToObject(gameJSON, "name", gameName.c_str());

    cJSON_AddItemReferenceToObject(app42JSON, "game", gameJSON);
    
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(gameJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    cJSON_Delete(sJSON);
    cJSON_Delete(scoreJSON);
    
    free(cptrFormatted);
    
    return bodyString;
    
}

string BuildEditUserScoreBody(string scoreId, double score)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *gameJSON = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    cJSON *scoreJSON = cJSON_CreateObject();
    
    cJSON_AddNumberToObject(scoreJSON, "value", score);
    cJSON_AddStringToObject(scoreJSON, "scoreId", scoreId.c_str());
    cJSON_AddItemReferenceToObject(sJSON, "score", scoreJSON);
    
    cJSON_AddItemReferenceToObject(gameJSON, "scores", sJSON);

    cJSON_AddItemReferenceToObject(app42JSON, "game", gameJSON);
    
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(gameJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    cJSON_Delete(sJSON);
    cJSON_Delete(scoreJSON);
   //"{"app42":{"game":{"scores":{"score":{"value":1100,"scoreId":"oHBMmGocgkwwTPl02oMsAKtrHVc="}}}}}"
    free(cptrFormatted);
    
    return bodyString;
    
}


void ScoreBoardService::SaveUserScore(string gameName,string userName, double score, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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
    
    map<string, string> postMap;
    populateSignParams(postMap);
    string saveScoreBody = BuildSaveUserScoreBody(gameName, userName,score);
    postMap["body"] = saveScoreBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/scoreboard");
    baseUrl.append("?");
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",saveScoreBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, saveScoreBody.c_str(), response, callfuncND_selector(App42GameResponse::onComplete));

}

void ScoreBoardService::EditScoreValueById(string scoreId, double gameScore, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(scoreId, "Score ID");
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
    string saveScoreBody = BuildEditUserScoreBody(scoreId, gameScore);
    postMap["body"] = saveScoreBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string baseUrl = getBaseUrl("game/scoreboard/editscore");
    baseUrl.append("?");
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    string timestamp = Util::getTimeStamp();
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePut(baseUrl, headers, saveScoreBody.c_str(), response, callfuncND_selector(App42GameResponse::onComplete));
    
}

void ScoreBoardService::GetAverageScoreByUser(string gameName, string userName, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
	resource.append("/average");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
    getMap["userName"] = userName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));

}

void ScoreBoardService::GetHighestScoreByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

    
    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
	resource.append("/highest");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
    getMap["userName"] = userName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));

}

void ScoreBoardService::GetLastScoreByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

    
    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
	resource.append("/lastscore");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
    getMap["userName"] = userName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));

}

void ScoreBoardService::GetTopRankings(string gameName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
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

    string resource = "game/scoreboard/";
    resource.append(gameName);
	resource.append("/ranking");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));

}


void ScoreBoardService::GetTopNRankers(string gameName,int max,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
        Util::throwExceptionIfMaxIsNotValid(max, "Max");
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

    string resource = "game/scoreboard/";
    resource.append(gameName);
	resource.append("/rankers/");
    resource.append(Util::ItoA(max));
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
    getMap["max"]=Util::ItoA(max);
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}


void ScoreBoardService::GetUserRanking(string gameName, string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
	resource.append("/ranking");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["name"] = gameName;
    getMap["userName"] = userName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));

}

void ScoreBoardService::GetScoresByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

	string timestamp = Util::getTimeStamp();
    
    /**
     * Creating SignParams and signature
     */
    map<string, string> signParams;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
	signParams["name"] = gameName;
    signParams["userName"] = userName;
	string signature = Util::signMap(secretKey, signParams);
    
    /**
     * Creating URL
     */
    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
	string url = getBaseUrl(resource);
    url.append("?");
    
    /**
     * Creating Headers
     */
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    /**
     * Initiating Http call
     */
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}


void ScoreBoardService::GetLowestScoreByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
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

	string timestamp = Util::getTimeStamp();
    
    /**
     * Creating SignParams and signature
     */
    map<string, string> signParams;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
	signParams["name"] = gameName;
    signParams["userName"] = userName;
	string signature = Util::signMap(secretKey, signParams);
    
    /**
     * Creating URL
     */
    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append(userName);
    resource.append("/lowest");
	string url = getBaseUrl(resource);
    url.append("?");
    
    /**
     * Creating Headers
     */
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    /**
     * Initiating Http call
     */
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}

void ScoreBoardService::GetTopRankersByGroup(string gameName,std::vector<std::string>group,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
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

	string timestamp = Util::getTimeStamp();
    /**
     * Creating SignParams and signature
     */
    map<string, string> signParams;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
    
    string groupString = "[";
    groupString.append(Util::GetStringFromVector(group));
    groupString.append("]");
    signParams["userList"] = groupString;
    
    string queryString = buildQueryString(signParams);
    
    signParams["name"] = gameName;
	string signature = Util::signMap(secretKey, signParams);
    
    //    printf("\nSignParams: ");
    //    Util::printMap(signParams);
    //    printf("\n");
    /**
     * Creating URL
     */
    string resource = "game/scoreboard/";
    resource.append(gameName+"/");
    resource.append("rankers/group");
	string url = getBaseUrl(resource);
    url.append("?");
    url.append(queryString);
    string encodedUrl = url_encode(url);
    /**
     * Creating Headers
     */
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);

    
    /**
     * Initiating Http call
     */
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}

void ScoreBoardService::GetLastGameScore(string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
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

	string timestamp = Util::getTimeStamp();
    
    /**
     * Creating SignParams and signature
     */
    map<string, string> signParams;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
    signParams["userName"] = userName;
	string signature = Util::signMap(secretKey, signParams);
    
    /**
     * Creating URL
     */
    string resource = "game/scoreboard/";
    resource.append(userName);
    resource.append("/lastgame");
	string url = getBaseUrl(resource);
    url.append("?");
    
    /**
     * Creating Headers
     */
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    /**
     * Initiating Http call
     */
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}

void ScoreBoardService::GetTopNTargetRankers(string gameName,int max,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42GameResponse *response = new App42GameResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(gameName, "Game Name");
        Util::throwExceptionIfMaxIsNotValid(max, "Max");
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

	string timestamp = Util::getTimeStamp();
    
    /**
     * Creating SignParams and signature
     */
    map<string, string> signParams;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, signParams);
	signParams["name"] = gameName;
    signParams["max"] = Util::ItoA(max);
	string signature = Util::signMap(secretKey, signParams);
    
    /**
     * Creating URL
     */
    string resource = "game/scoreboard/";
    resource.append(gameName);
    resource.append("/targetedrankers/");
    resource.append(Util::ItoA(max));
	string url = getBaseUrl(resource);
    url.append("?");
    
    /**
     * Creating Headers
     */
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    /**
     * Initiating Http call
     */
    Util::executeGet(url,headers, response, callfuncND_selector(App42GameResponse::onComplete));
    
}
