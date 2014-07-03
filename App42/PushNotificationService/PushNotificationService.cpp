//
//  PushNotificationService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#include "PushNotificationService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42PushNotificationResponse.h"
#include "Connector.h"



// define the static..
PushNotificationService* PushNotificationService::_instance = NULL;

PushNotificationService* PushNotificationService::Initialize(string apikey, string secretkey)
{
	if(_instance == NULL)
    {
		_instance = new PushNotificationService();
	}
    _instance->Init(apikey, secretkey);
    return _instance;
}

PushNotificationService* PushNotificationService::getInstance()
{
	return _instance;
}

void PushNotificationService::Terminate()
{
	if(_instance != NULL)
    {
		delete _instance;
		_instance = NULL;
	}
}

PushNotificationService::PushNotificationService()
{
    
}

string BuildRegisterDeviceTokenBody(string deviceToken, string userName, string deviceType)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "deviceToken", deviceToken.c_str());
    cJSON_AddStringToObject(sJSON, "type", deviceType.c_str());
    cJSON_AddStringToObject(sJSON, "username", userName.c_str());
    cJSON_AddItemReferenceToObject(pushJSON, "push", sJSON);
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

string BuildPushBody(string userName, string message,string expiry)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *messageJson = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "username", userName.c_str());
    cJSON_AddStringToObject(sJSON, "payload", message.c_str());
    cJSON_AddStringToObject(sJSON, "expiry", expiry.c_str());
    
    cJSON_AddItemReferenceToObject(messageJson, "message", sJSON);
    cJSON_AddItemReferenceToObject(pushJSON, "push", messageJson);
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(messageJson);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

string BuildPushBody(string userName, cJSON *message,string expiry)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *messageJson = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "userName", userName.c_str());
    //cJSON_AddStringToObject(sJSON, "payload", message.c_str());
    cJSON_AddStringToObject(sJSON, "expiry", expiry.c_str());
    cJSON_AddItemReferenceToObject(sJSON, "payload", message);
    
    cJSON_AddItemReferenceToObject(messageJson, "message", sJSON);
    cJSON_AddItemReferenceToObject(pushJSON, "push", messageJson);
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(messageJson);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

string BuildSubscribeChannelBody(string userName, string channel)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "name", channel.c_str());
    cJSON_AddStringToObject(sJSON, "username", userName.c_str());
    cJSON_AddItemReferenceToObject(pushJSON, "channel", sJSON);
    cJSON_AddItemReferenceToObject(bodyJSON, "push", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

string BuildSendPushToChannelBody(string channel, string message,string expiry)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *messageJson = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "channel", channel.c_str());
    cJSON_AddStringToObject(sJSON, "payload", message.c_str());
    cJSON_AddStringToObject(sJSON, "expiry", expiry.c_str());
    
    cJSON_AddItemReferenceToObject(messageJson, "message", sJSON);
    cJSON_AddItemReferenceToObject(pushJSON, "push", messageJson);
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(messageJson);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

string BuildSendPushToChannelBody(string channel, cJSON *message,string expiry)
{
    
    cJSON *bodyJSON = cJSON_CreateObject();
    cJSON *pushJSON = cJSON_CreateObject();
    cJSON *messageJson = cJSON_CreateObject();
    cJSON *sJSON = cJSON_CreateObject();
    
    cJSON_AddStringToObject(sJSON, "channel", channel.c_str());
    cJSON_AddStringToObject(sJSON, "expiry", expiry.c_str());
    cJSON_AddItemReferenceToObject(sJSON, "payload", message);
    
    cJSON_AddItemReferenceToObject(messageJson, "message", sJSON);
    cJSON_AddItemReferenceToObject(pushJSON, "push", messageJson);
    cJSON_AddItemReferenceToObject(bodyJSON, "app42", pushJSON);
    
    char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
    string bodyString = cptrFormatted;
    
    cJSON_Delete(bodyJSON);
    cJSON_Delete(pushJSON);
    cJSON_Delete(messageJson);
    cJSON_Delete(sJSON);
    
    free(cptrFormatted);
    return bodyString;
    
}

cJSON* getJsonFromMap(map<string, string>messageMap)
{
    cJSON *messageJSON = cJSON_CreateObject();
    
    for( std::map<string,string>::iterator it=messageMap.begin(); it!=messageMap.end(); ++it)
    {
       cJSON_AddStringToObject(messageJSON, it->first.c_str(), it->second.c_str());
    }
    
//    char *cptrFormatted = cJSON_PrintUnformatted(messageJSON);
//    string bodyString = cptrFormatted;
//    
//    cJSON_Delete(messageJSON);
//    
//    free(cptrFormatted);
//    printf("\n%s",bodyString.c_str());
    return messageJSON;
}



void PushNotificationService::registerDeviceToken(string deviceToken, string userName, string deviceType, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
        Util::throwExceptionIfStringNullOrBlank(deviceType, "Device Type");
        Util::throwExceptionIfStringNullOrBlank(deviceToken, "Device Token");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    string pushBody = BuildRegisterDeviceTokenBody(deviceToken, userName,deviceType);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/storeDeviceToken/";
	resource.append(userName);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
    
}

void PushNotificationService::sendPushMessageToUser(string username,  string message, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(username, "User Name");
        Util::throwExceptionIfStringNullOrBlank(message, "Message");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    string pushBody = BuildPushBody(username,message,timestamp);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/sendMessage/";
	resource.append(username);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
}

void PushNotificationService::sendPushMessageToUser(string username, map<string, string>messageMap, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(username, "User Name");
        Util::throwExceptionIfTargetIsNull(pTarget, "Callback's Target");
        Util::throwExceptionIfCallBackIsNull(pSelector, "Callback");
        Util::throwExceptionIfMapIsNullOrBlank(messageMap, "Push Message");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    cJSON *message = getJsonFromMap(messageMap);
    string pushBody = BuildPushBody(username,message,timestamp);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/sendMessage/";
	resource.append(username);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
}


void PushNotificationService::subscribeToChannel(string channel, string userName, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(userName, "User Name");
        Util::throwExceptionIfStringNullOrBlank(channel, "Channel");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    string pushBody = BuildSubscribeChannelBody(userName,channel);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/subscribeToChannel/";
	resource.append(userName);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
    
}

void PushNotificationService::sendPushMessageToChannel(string channel, string message, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(channel, "Channel Name");
        Util::throwExceptionIfStringNullOrBlank(message, "Message");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    string pushBody = BuildSendPushToChannelBody(channel, message,timestamp);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/sendPushMessageToChannel/";
	resource.append(channel);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    char *encodedUrl = Util::url_encode(baseUrl);
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(encodedUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
}

void PushNotificationService::sendPushMessageToChannel(string channel, map<string, string>messageMap, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42PushNotificationResponse *response = new App42PushNotificationResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(channel, "Channel Name");
        Util::throwExceptionIfMapIsNullOrBlank(messageMap, "Push Message");
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
    string timestamp = Util::getTimeStamp();
    populateSignParams(postMap);
    cJSON *message = getJsonFromMap(messageMap);
    string pushBody = BuildSendPushToChannelBody(channel, message, timestamp);
    postMap["body"] = pushBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "push/sendPushMessageToChannel/";
	resource.append(channel);
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
    //Util::app42Trace("\n addScoreBody = %s",pushBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePost(baseUrl, headers, pushBody.c_str(), response, callfuncND_selector(App42PushNotificationResponse::onComplete));
}

