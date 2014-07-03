//
//  UserService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 08/10/13.
//
//

#include "UserService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42UserResponse.h"
#include "Connector.h"

using namespace cocos2d;

// define the static..
UserService* UserService::_instance = NULL;

UserService::UserService()
{
    
}

UserService* UserService::Initialize(string apikey, string secretkey)
{
	if(_instance == NULL)
    {
		_instance = new UserService();
	}
    _instance->Init(apikey, secretkey);
    return _instance;
}

UserService* UserService::getInstance()
{
	return _instance;
}

void UserService::Terminate()
{
	if(_instance != NULL)
    {
		delete _instance;
		_instance = NULL;
	}
}


string UserService::buildCreateUserBody(string username, string password, string email,string roleList)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *app42JSON = cJSON_CreateObject();
    cJSON *userJSON = cJSON_CreateObject();
    
    // first construct the user
    if (username.length())
    {
        cJSON_AddStringToObject(userJSON, "userName", username.c_str());
    }
    
    if (email.length())
    {
        cJSON_AddStringToObject(userJSON, "email", email.c_str());
    }
    
    if (password.length())
    {
        cJSON_AddStringToObject(userJSON, "password", password.c_str());
    }
    
    if (roleList.length())
    {
        cJSON_AddStringToObject(userJSON, "roles", roleList.c_str());
    }
    
    
    // add user to app42
    cJSON_AddItemReferenceToObject(app42JSON, "user", userJSON);
    
    // add app42 to body
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(userJSON);
    cJSON_Delete(app42JSON);
    cJSON_Delete(bodyJSON);
    
    free(cptrFormatted);
    
    return bodyString;
    
}




void UserService::CreateUser(string username, string password, string email,CCObject* pTarget, SEL_CallFuncND pSelector)
{
    App42UserResponse *response = new App42UserResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(username, "User Name");
        Util::throwExceptionIfStringNullOrBlank(password, "Password");
        Util::throwExceptionIfStringNullOrBlank(email, "Email");
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
    string createUserbody = buildCreateUserBody(username, password, email,"");
    postMap["body"] = createUserbody;
    
    string signature = Util::signMap(secretKey, postMap);
    string timestamp = Util::getTimeStamp();
    // now build query string and append it to the url
    string queryString = Util::BuildQueryString(apiKey, timestamp, VERSION, signature);
    
    string baseUrl = getBaseUrl("user");
    baseUrl.append("?").append(queryString);
   // Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
   // Util::app42Trace("\n createUserbody = %s",createUserbody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    Util::executePost(baseUrl, headers, createUserbody.c_str(), response, callfuncND_selector(App42UserResponse::onComplete));
    
}

/*
void UserService::CreateUser(string username, string password, string email, string roleList, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    map<string, string> postMap;
    populateSignParams(postMap);
    string createUserbody = buildCreateUserBody(username, password, email,roleList);
    postMap["body"] = createUserbody;
    
    string signature = Util::signMap(secretKey, postMap);
    string timestamp = Util::getTimeStamp();
    // now build query string and append it to the url
    //string queryString = Util::BuildQueryString(apiKey, timestamp, VERSION, signature);
    
    string baseUrl = getBaseUrl("user");
    baseUrl.append("?");//.append(queryString);
    printf("\n baseUrl = %s",baseUrl.c_str());
    printf("\n createUserbody = %s",createUserbody.c_str());
    
    std::vector<std::string> headers;
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature,headers);
    Util::executePost(baseUrl, headers, createUserbody.c_str(), pTarget, pSelector);
}
*/

void UserService::Authenticate(string username, string password, cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42UserResponse *response = new App42UserResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(username, "User Name");
        Util::throwExceptionIfStringNullOrBlank(password, "Password");
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
    string postBody = buildCreateUserBody(username, password, "","");
    postMap["body"] = postBody;
    
    string signature = Util::signMap(secretKey, postMap);
    string timestamp = Util::getTimeStamp();
    // now build query string and append it to the url
    //string queryString = Util::BuildQueryString(apiKey, timestamp, VERSION, signature);
    
    string baseUrl = getBaseUrl("user/authenticate");
    baseUrl.append("?");//.append(queryString);
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n createUserbody = %s",postBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    Util::executePost(baseUrl, headers, postBody.c_str(), response, callfuncND_selector(App42UserResponse::onComplete));
}

void UserService::GetUser(string username, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42UserResponse *response = new App42UserResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(username, "User Name");
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
    
    string resource = "user/";
	resource.append(username);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	getMap["userName"] = username;
	string signature = Util::signMap(secretKey, getMap);
    //string queryString = Util::BuildQueryString(apiKey, timestamp, VERSION, signature);
    url.append("?");//.append(queryString);
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    Util::executeGet(url,headers, response, callfuncND_selector(App42UserResponse::onComplete));
}


void UserService::GetAllUsers(CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42UserResponse *response = new App42UserResponse(pTarget,pSelector);
    
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
        e = NULL;
        return;
    }
    
    string resource = "user/";
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    Util::executeGet(url,headers, response, callfuncND_selector(App42UserResponse::onComplete));
}

