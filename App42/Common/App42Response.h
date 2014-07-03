//
//  App42Response.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 14/10/13.
//
//

#ifndef __App42CPPSDK__App42Response__
#define __App42CPPSDK__App42Response__

#include <iostream>
#include "cocos-ext.h"
#include "cocos2d.h"
#include "JSONDocument.h"
#include "cJSON.h"
#include "Exceptions.h"

class App42Response: public cocos2d::CCObject
{
protected:
    int _result;
    std::string _body;
    cocos2d::extension::CCHttpRequest* _httpRequest;
    cocos2d::CCObject*          _pTarget;
    cocos2d::SEL_CallFuncND     _pSelector;
    
public:
    
    App42Response(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);//(int code, std::string body, cocos2d::extension::CCHttpRequest* request);
    
    virtual ~App42Response();
    
    bool isSuccess;
    // The HTTP response code.
    int getCode();
    int httpErrorCode;
    // The App Error Code.
    int appErrorCode;
    std::string errorDetails;
    std::string errorMessage;
    
    // The JSON body of the HTTP response containing details
    std::string getBody();
    virtual void onComplete(cocos2d::CCNode *sender, void *data);

    void buildJsonDocument(cJSON *json, JSONDocument *jsonDocumnet);
    void buildErrorMessage();
};

#endif /* defined(__App42CPPSDK__App42Response__) */
