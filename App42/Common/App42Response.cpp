//
//  App42Response.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 14/10/13.
//
//

#include "App42Response.h"
#include "Common.h"

App42Response::App42Response(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    _pTarget = pTarget;
    _pSelector = pSelector;
    appErrorCode = 0;
    httpErrorCode = 0;
    _result = 0;
    errorMessage = "";
    errorDetails = "";
}

App42Response::~App42Response()
{
    if(_httpRequest)
    {
        delete _httpRequest;
    }
}

// The HTTP response code.
int App42Response::getCode()
{
    return _result;
}

// The JSON body of the HTTP response containing details
std::string App42Response::getBody()
{
    return _body;
}


void App42Response::onComplete(cocos2d::CCNode *sender, void *data)
{
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        Util::app42Trace("%s completed", response->getHttpRequest()->getTag());
    }
    
    _result = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", _result, response->getHttpRequest()->getTag());
    //m_labelStatusCode->setString(statusString);
    Util::app42Trace("response code: %d", _result);
    
    isSuccess = response->isSucceed();

    if (!isSuccess)
    {
        Util::app42Trace("response failed");
        Util::app42Trace("error buffer: %s", response->getErrorBuffer());
        //return;
    }
    _httpRequest = response->getHttpRequest();
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    //std::vector<char> v;
    std::string str(buffer->begin(),buffer->end());
    _body = str;
    Util::app42Trace("Response string=%s",str.c_str());
}



void App42Response::buildJsonDocument(cJSON *json, JSONDocument *jsonDocumnet)
{
    
    cJSON *docIdJson = Util::getJSONChild("_id", json);
    if (docIdJson!=NULL)
    {
        
        jsonDocumnet->setDocId(Util::getJSONString("$oid", docIdJson));
        cJSON_DeleteItemFromObject(json, "_id");
    }
    
    cJSON *ownerJson = Util::getJSONChild("_$owner", json);
    if (ownerJson!=NULL)
    {
        
        jsonDocumnet->setOwner(Util::getJSONString("owner", ownerJson));
        cJSON_DeleteItemFromObject(json,"_$owner");
    }
    
    string createdAt = Util::getJSONString("_$createdAt", json);
    if (createdAt.c_str()!=NULL)
    {
        jsonDocumnet->setCreatedAt(createdAt);
        cJSON_DeleteItemFromObject(json, "_$createdAt");
    }
    
    string updatedAt = Util::getJSONString("_$updatedAt", json);
    if (updatedAt.c_str()!=NULL)
    {
        jsonDocumnet->setUpdatedAt(updatedAt);
        cJSON_DeleteItemFromObject(json, "_$updatedAt");
    }
    
    string event = Util::getJSONString("_$event", json);
    if (event.c_str()!=NULL)
    {
        jsonDocumnet->setEvent(event);
        cJSON_DeleteItemFromObject(json, "_$event");
    }
    char *doc = cJSON_PrintUnformatted(json);
    jsonDocumnet->setJsonDoc(doc);
    free(doc);
    
}

void App42Response::buildErrorMessage()
{
    cJSON *ptrErrorBody = cJSON_Parse(_body.c_str());
    cJSON* ptrApp42Fault = Util::getJSONChild("app42Fault", ptrErrorBody);
    appErrorCode = Util::getJSONInt("appErrorCode", ptrApp42Fault);
    httpErrorCode = Util::getJSONInt("httpErrorCode", ptrApp42Fault);
    errorDetails = Util::getJSONString("details", ptrApp42Fault);
    errorMessage = Util::getJSONString("message", ptrApp42Fault);
}

