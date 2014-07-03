/**********************************************************************
 *  Query.h
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 19/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/

#ifndef __App42CPPSDK__Query__
#define __App42CPPSDK__Query__

#include <iostream>
#include "cJSON.h"



#define APP42_OP_EQUALS                  "$eq"
#define APP42_OP_NOT_EQUALS              "$ne"
#define APP42_OP_GREATER_THAN            "$gt"
#define APP42_OP_LESS_THAN               "$lt"
#define APP42_OP_GREATER_THAN_EQUALTO    "$gte"
#define APP42_OP_LESS_THAN_EQUALTO       "$lte"
#define APP42_OP_LIKE                    "$lk"
#define APP42_OP_AND                     "$and"
#define APP42_OP_OR                      "$or"
#define APP42_OP_INLIST                  "$in"

#define APP42_ORDER_ASCENDING            "ASCENDING"
#define APP42_ORDER_DESCENDING           "DESCENDING"

class Query
{
public:
        
    Query(cJSON *_jsonObject);
    
    ~Query();
    
    cJSON *jsonObject;
    cJSON *jsonArray;

    void setJsonObject(cJSON *_jsonObject);
    void setJsonArray(cJSON *_jsonArray);
    cJSON* getJsonObject();
    cJSON* getJsonArray();

    std::string getString();
protected:
    
private:
    
};

#endif /* defined(__App42CPPSDK__Query__) */
