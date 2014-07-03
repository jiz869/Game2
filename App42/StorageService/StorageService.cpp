//
//  StorageService.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#include "StorageService.h"
#include "App42Constants.h"
#include "cJSON.h"
#include <map>
//#include "Exceptions.h"
//#include "App42Exception.h"
#include "Signing.h"
#include "BodyBuilder.h"
#include "cocos-ext.h"
#include "App42StorageResponse.h"
#include "Connector.h"


// define the static..
StorageService* StorageService::_instance = NULL;

StorageService* StorageService::Initialize(string apikey, string secretkey)
{
	if(_instance == NULL)
    {
		_instance = new StorageService();
	}
    _instance->Init(apikey, secretkey);
    return _instance;
}

StorageService* StorageService::getInstance()
{
	return _instance;
}

void StorageService::Terminate()
{
	if(_instance != NULL)
    {
		delete _instance;
		_instance = NULL;
	}
}

StorageService::StorageService()
{
    
}

string BuildStorageBody(string json)
{
	cJSON *bodyJSON = cJSON_CreateObject();
	cJSON *app42JSON = cJSON_CreateObject();
	cJSON *storageJSON = cJSON_CreateObject();
    
	cJSON_AddStringToObject(storageJSON, "jsonDoc", json.c_str());
	cJSON_AddItemReferenceToObject(app42JSON, "storage", storageJSON);
    
	cJSON_AddItemReferenceToObject(bodyJSON, "app42", app42JSON);
	char *cptrFormatted = cJSON_PrintUnformatted(bodyJSON);
	string bodyString = cptrFormatted;
    
	cJSON_Delete(storageJSON);
	cJSON_Delete(app42JSON);
	cJSON_Delete(bodyJSON);
	free(cptrFormatted);
	return bodyString;
}


void StorageService::InsertJsonDocument(string dbName, string collectionName, string json, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(json, "Json String");
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
    
    try {
        map<string, string> signParams;
        string timestamp = Util::getTimeStamp();
        populateSignParams(signParams);
        signParams["dbName"] = dbName;
        signParams["collectionName"] = collectionName;
        
        
        string storageBody = BuildStorageBody(json);
        signParams["body"] = storageBody;
        
        
        string signature = Util::signMap(secretKey, signParams);
        
        
        string resource = "storage/insert/dbName/";
        resource.append(dbName + "/collectionName/");
        resource.append(collectionName);
        string baseUrl = getBaseUrl(resource);
        
        baseUrl.append("?");
        //Util::app42Trace("\n baseUrl = %s",baseUrl.c_str());
        //Util::app42Trace("\n createRewardbody = %s",storageBody.c_str());
        
        std::vector<std::string> headers;
        map<string, string> metaHeaders;
        populateMetaHeaderParams(metaHeaders);
        Util::BuildHeaders(metaHeaders, headers);
        Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
        
        Util::executePost(baseUrl, headers, storageBody.c_str(), response, callfuncND_selector(App42StorageResponse::onComplete));

    }
    catch (exception *e)
    {
        throw e;
    }
}


void StorageService::FindAllCollections(string dbName, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
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
    
    string resource = "storage/findCollections/dbName/";
    resource.append(dbName);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42StorageResponse::onComplete));
}

void StorageService::FindAllDocuments(string dbName, string collectionName, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
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
    
    string resource = "storage/findAll/dbName/";
	resource.append(dbName+ "/collectionName/");
	resource.append(collectionName);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
    getMap["collectionName"] = collectionName;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42StorageResponse::onComplete));

}

void StorageService::FindDocumentById(string dbName, string collectionName, string docId, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(docId, "Doc ID");
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
    
    string resource = "storage/findDocById/dbName/";
	resource.append(dbName + "/collectionName/");
	resource.append(collectionName + "/docId/");
	resource.append(docId);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
    getMap["collectionName"] = collectionName;
    getMap["docId"] = docId;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42StorageResponse::onComplete));

}

void StorageService::FindDocumentByQuery(string dbName, string collectionName, Query *query, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
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
    
    string resource = "storage/findDocsByQuery/dbName/";
	resource.append(dbName + "/collectionName/");
	resource.append(collectionName);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
    getMap["collectionName"] = collectionName;
    getMap["jsonQuery"] = query->getString();
    //Util::printMap(getMap);
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");

    map<string, string> queryParamsMap;
    queryParamsMap["jsonQuery"]=query->getString();
    string queryString = buildQueryString(queryParamsMap);
    url.append(queryString);
    //char *encodedUrl = Util::url_encode(url);
    printf("\nQueryString=%s",queryString.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url.c_str(),headers, response, callfuncND_selector(App42StorageResponse::onComplete));
    
}


void StorageService::FindDocumentByKeyValue(string dbName, string collectionName, string key,string value, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(key, "Key");
        Util::throwExceptionIfStringNullOrBlank(value, "Value");
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
    
    string resource = "storage/findDocByKV/dbName/";
	resource.append(dbName+ "/collectionName/");
	resource.append(collectionName+ "/");
	resource.append(key+ "/");
	resource.append(value+ "/");
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
    getMap["collectionName"] = collectionName;
    getMap["key"] = key;
	getMap["value"] = value;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    //Util::app42Trace("\n baseUrl = %s",url.c_str());
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeGet(url,headers, response, callfuncND_selector(App42StorageResponse::onComplete));

}

void StorageService::UpdateDocumentByDocId(string dbName, string collectionName, string docId,string json, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(docId, "Doc ID");
        Util::throwExceptionIfStringNullOrBlank(json, "Json String");
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
    postMap["dbName"] = dbName;
	postMap["collectionName"] = collectionName;
    postMap["docId"] = docId;
    
    
    string storageBody = BuildStorageBody(json);
    postMap["body"] = storageBody;
    
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "storage/updateByDocId/dbName/";
    resource.append(dbName + "/collectionName/");
	resource.append(collectionName+"/docId/");
    resource.append(docId);
    
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePut(baseUrl, headers, storageBody.c_str(), response, callfuncND_selector(App42StorageResponse::onComplete));
}

void StorageService::UpdateDocumentByKeyValue(string dbName, string collectionName, string key,string value,string json, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(key, "Key");
        Util::throwExceptionIfStringNullOrBlank(value, "Value");
        Util::throwExceptionIfStringNullOrBlank(json, "Json String");
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
    postMap["dbName"] = dbName;
	postMap["collectionName"] = collectionName;
    postMap["key"] = key;
    postMap["value"] = value;
    
    string storageBody = BuildStorageBody(json);
    postMap["body"] = storageBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "storage/update/dbName/";
    resource.append(dbName + "/collectionName/");
	resource.append(collectionName+"/");
    resource.append(key+"/");
    resource.append(value);

    
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    printf("baseUrl=%s",baseUrl.c_str());
    printf("storageBody=%s",storageBody.c_str());

    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePut(baseUrl, headers, storageBody.c_str(), response, callfuncND_selector(App42StorageResponse::onComplete));
}

void StorageService::SaveOrUpdateDocumentByKeyValue(string dbName, string collectionName, string key,string value,string json, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(key, "Key");
        Util::throwExceptionIfStringNullOrBlank(value, "Value");
        Util::throwExceptionIfStringNullOrBlank(json, "Json String");
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
    postMap["dbName"] = dbName;
	postMap["collectionName"] = collectionName;
    postMap["key"] = key;
    postMap["value"] = value;
    
    string storageBody = BuildStorageBody(json);
    postMap["body"] = storageBody;
    
    string signature = Util::signMap(secretKey, postMap);
    
    string resource = "storage/saveorupdate/dbName/";
    resource.append(dbName + "/collectionName/");
	resource.append(collectionName+"/");
    resource.append(key+"/");
    resource.append(value);
    
    
    string baseUrl = getBaseUrl(resource);
    baseUrl.append("?");
    
    printf("baseUrl=%s",baseUrl.c_str());
    printf("storageBody=%s",storageBody.c_str());
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executePut(baseUrl, headers, storageBody.c_str(), response, callfuncND_selector(App42StorageResponse::onComplete));
}

void StorageService::DeleteDocumentsById(string dbName, string collectionName, string docId, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector)
{
    App42StorageResponse *response = new App42StorageResponse(pTarget,pSelector);
    
    try
    {
        Util::throwExceptionIfStringNullOrBlank(dbName, "Database Name");
        Util::throwExceptionIfStringNullOrBlank(collectionName, "Collection Name");
        Util::throwExceptionIfStringNullOrBlank(docId, "Doc ID");
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
    
    string resource = "storage/deleteDocById/dbName/";
	resource.append(dbName+ "/collectionName/");
	resource.append(collectionName+ "/docId/");
	resource.append(docId);
    
	string url = getBaseUrl(resource);
	string timestamp = Util::getTimeStamp();
    
    map<string, string> getMap;
	Util::BuildGetSigningMap(apiKey, timestamp, VERSION, getMap);
    getMap["dbName"] = dbName;
    getMap["collectionName"] = collectionName;
    getMap["docId"] = docId;
	string signature = Util::signMap(secretKey, getMap);
    url.append("?");
    
    std::vector<std::string> headers;
    map<string, string> metaHeaders;
    populateMetaHeaderParams(metaHeaders);
    Util::BuildHeaders(metaHeaders, headers);
    
    Util::BuildHeaders(apiKey, timestamp, VERSION, signature, headers);
    
    Util::executeDelete(url,headers, response, callfuncND_selector(App42StorageResponse::onComplete));
    
}



