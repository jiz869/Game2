/**********************************************************************
 *  App42Exception.h
 *  App42CPPSDK
 *
 *  Created by shephertz technologies on 20/02/14.
 *  Copyright (c) 2013 ShephertzTechnology PVT LTD. All rights reserved.
 **********************************************************************/


#ifndef __App42CPPSDK__App42Exception__
#define __App42CPPSDK__App42Exception__

#include <iostream>
#include <exception>
#include <string>
#include "cJSON.h"
#include <cstdio>

class App42Exception : public std::exception
{
public:

    App42Exception(std::string exception, int _httpErrorCode, int _appErrorCode);
    //~App42Exception();


    int setHttpErrorCode(int _httpErrorCode);
    int setAppErrorCode(int _appErrorCode);

    int getHttpErrorCode(){return httpErrorCode;};
    int getAppErrorCode(){return appErrorCode;};

    //std::string getExceptionReason();

    virtual const char* what() const throw()
    {
        const char *str = cJSON_PrintUnformatted(exceptionString);
        std::printf("\nApp42Exception : %s\n",str);
        return str;
    }

private:
    int httpErrorCode;
    int appErrorCode;
    cJSON *exceptionString;
};





#endif /* defined(__App42CPPSDK__App42Exception__) */
