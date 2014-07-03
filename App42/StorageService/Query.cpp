/**********************************************************************
 *  Query.cpp
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 19/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/


#include "Query.h"

Query::Query(cJSON *_jsonObject)
{
    if (_jsonObject->type == cJSON_Array)
    {
        jsonArray = _jsonObject;
        jsonObject = NULL;
    }
    else
    {
        jsonArray  = NULL;
        jsonObject = _jsonObject;
    }
}

Query::~Query()
{
    
}

void Query::setJsonObject(cJSON *_jsonObject)
{
    jsonObject = _jsonObject;
}
void Query::setJsonArray(cJSON *_jsonArray)
{
    jsonArray = _jsonArray;
}

cJSON* Query::getJsonObject()
{
   return jsonObject;
}
cJSON* Query::getJsonArray()
{
   return jsonArray;
}

std::string Query::getString()
{
    if (jsonObject)
    {
        std::string queryString;
        queryString.append("[");
        queryString.append(cJSON_PrintUnformatted(jsonObject));
        queryString.append("]");
        return queryString;
    }
    else
    {
        std::string queryString;
        queryString.append(cJSON_PrintUnformatted(jsonArray));
        return queryString;
    }
}

