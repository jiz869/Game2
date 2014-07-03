/**********************************************************************
 *  QueryBuilder.cpp
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 20/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/

#include "QueryBuilder.h"
#include "Exceptions.h"
#include "App42Exception.h"
#include "BodyBuilder.h"
QueryBuilder::QueryBuilder()
{
   
}

QueryBuilder::~QueryBuilder()
{
    
}


Query* QueryBuilder::BuildQuery(std::string key,std::string value,std::string op)
{
    try
    {
        Util::throwExceptionIfStringNullOrBlank(key, "Key");
        Util::throwExceptionIfStringNullOrBlank(value, "value");
        Util::throwExceptionIfStringNullOrBlank(op, "Operator");
    }
    catch (App42Exception *e)
    {
        throw e;
    }
    
    Query *query = NULL;
    
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "key", key.c_str());
    cJSON_AddStringToObject(jsonObject, "value", value.c_str());
    cJSON_AddStringToObject(jsonObject, "operator", op.c_str());
    
    query = new Query(jsonObject);
    return query;
}

Query* QueryBuilder::BuildQuery(std::string key,std::vector<std::string> value,std::string op)
{
    try
    {
        Util::throwExceptionIfStringNullOrBlank(key, "Key");
        Util::throwExceptionIfStringNullOrBlank(op, "Operator");
    }
    catch (App42Exception *e)
    {
        throw e;
    }
    
    Query *query = NULL;
    
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "key", key.c_str());
    string valueString = "[";
    valueString.append(Util::GetStringFromVector(value));
    valueString.append("]");
    cJSON_AddStringToObject(jsonObject, "value", valueString.c_str());
    cJSON_AddStringToObject(jsonObject, "operator", op.c_str());
    
    query = new Query(jsonObject);
    return query;
    
}


Query* QueryBuilder::CompoundOperator(Query *q1,std::string op,Query *q2)
{
    try
    {
        Util::throwExceptionIfStringNullOrBlank(op, "Operator");
        Util::throwExceptionIfObjectIsNull(q1, "Query");
        Util::throwExceptionIfObjectIsNull(q2, "Query");

    }
    catch (App42Exception *e)
    {
        throw e;
    }
    
    Query *query = NULL;
    
    cJSON *jsonArray = cJSON_CreateArray();
    if (q1->jsonObject)
    {
        cJSON_AddItemToArray(jsonArray, q1->jsonObject);
    }
    else
    {
        cJSON_AddItemToArray(jsonArray, q1->jsonArray);
    }
    
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "compoundOpt", op.c_str());
    cJSON_AddItemToArray(jsonArray, jsonObject);

    if (q2->jsonObject)
    {
        cJSON_AddItemToArray(jsonArray, q2->jsonObject);
    }
    else
    {
        cJSON_AddItemToArray(jsonArray, q2->jsonArray);
    }
    query = new Query(jsonArray);
    
    return query;
}

Query* QueryBuilder::SetLoggedInUser(std::string logged)
{
    try
    {
        Util::throwExceptionIfStringNullOrBlank(logged, "logged");
    }
    catch (App42Exception *e)
    {
        throw e;
    }
    
    Query *query = NULL;
    
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "key", "_$owner.owner");
    cJSON_AddStringToObject(jsonObject, "value", logged.c_str());
    cJSON_AddStringToObject(jsonObject, "operator", APP42_OP_EQUALS);
    
    query = new Query(jsonObject);
    return query;
}