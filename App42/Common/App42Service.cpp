//
//  App42Service.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 07/10/13.
//
//

#include "App42Service.h"
#include "cJSON.h"
#include "App42Constants.h"
#include "Signing.h"
#include <sstream>
#include "App42API.h"

App42Service::App42Service()
{
    pageMaxRecords = -1;
    pageOffset = -1;
}

App42Service::~App42Service()
{
    
}


void App42Service::Init(string _apikey, string _secretkey)
{
    apiKey = _apikey;
    secretKey = _secretkey;
}


void App42Service::populateSignParams(std::map<string, string>& signParamsMap)
{
    signParamsMap["apiKey"] = apiKey;
    signParamsMap["timeStamp"] = Util::getTimeStamp();
    signParamsMap["version"] = VERSION;
}

void App42Service::populateMetaHeaderParams(std::map<string, string>& metaHeaderParamsMap)
{
    if (pageMaxRecords!=-1)
    {
        std::stringstream ss;
        ss << pageMaxRecords;
        metaHeaderParamsMap[PAGE_MAX_RECORDS] = ss.str();
    }
    
    if (pageOffset!=-1)
    {
        std::stringstream ss;
        ss << pageOffset;
        metaHeaderParamsMap[PAGE_OFFSET] = ss.str();
    }
    
    if((geoTag.c_str()!=NULL) && geoTag.length())
    {
        metaHeaderParamsMap[GEO_TAG] = geoTag;
    }
    
    if ((event.c_str()!=NULL) && event.length())
    {
        metaHeaderParamsMap[EVENT] = event;
    }
    
    string loggedInUser = App42API::getLoggedInUser();
    if ((loggedInUser.c_str()!=NULL) && loggedInUser.length())
    {
        metaHeaderParamsMap["loggedInUser"] = loggedInUser;
    }
    
    if ((query.c_str()!=NULL) && query.length())
    {
        metaHeaderParamsMap["metaQuery"] = query;
    }
    
    if ((jsonObject.c_str()!=NULL) && jsonObject.length())
    {
        metaHeaderParamsMap["jsonObject"] = jsonObject;
    }
    
    if ((dbName.c_str()!=NULL) && dbName.length() && (collectionName.c_str()!=NULL) && collectionName.length())
    {
        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "dbName", dbName.c_str());
        cJSON_AddStringToObject(obj, "collectionName", collectionName.c_str());
        metaHeaderParamsMap["dbCredentials"] = cJSON_PrintUnformatted(obj);
    }
    
    std::map<string,string>::iterator it;
    for(it=otherMetaHearders.begin(); it!=otherMetaHearders.end(); ++it)
    {
        metaHeaderParamsMap[it->first] = it->second;
    }
}

string App42Service::getBaseUrl(string resource)
{
    string url = "";
    url.append(PROTOCOL);
    url.append(BASE_URL);
    url.append(RESOURCE_BASE);
    url.append(resource);
    return url;
}

string App42Service::buildQueryString(map<string, string> queryParamsMap)
{
    string queryString = "";
    
    std::map<string,string>::iterator it;
    
    for(it=queryParamsMap.begin(); it!=queryParamsMap.end(); ++it)
    {
        queryString.append(it->first);
        queryString.append("=");
        queryString.append(it->second);
        queryString.append("&");
    }
    return queryString;
}

void App42Service::setQuery(string _dbName, string _collectionName,Query *_metaInfoQuery)
{
    dbName = _dbName;
    collectionName = _collectionName;
    query = _metaInfoQuery->getString();
}

void App42Service::setQuery(string _dbName, string _collectionName)
{
    dbName = _dbName;
    collectionName = _collectionName;
}
void App42Service::setOtherMetaHeaders(std::map<std::string, std::string> l_otherMetaHearders)
{
    otherMetaHearders = l_otherMetaHearders;
}