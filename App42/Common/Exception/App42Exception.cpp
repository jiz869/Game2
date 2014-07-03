/**********************************************************************
 *  App42Exception.cpp
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 20/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/

#include "App42Exception.h"

App42Exception::App42Exception(std::string exception, int _httpErrorCode, int _appErrorCode)
{
    httpErrorCode = _httpErrorCode;
    appErrorCode  = _appErrorCode;
    exceptionString = cJSON_CreateObject();
    cJSON_AddStringToObject(exceptionString, "reason", exception.c_str());
    cJSON_AddNumberToObject(exceptionString, "httpErrorCode", _httpErrorCode);
    cJSON_AddNumberToObject(exceptionString, "appErrorCode", _appErrorCode);
}