//
//  Connector.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 16/10/13.
//
//

#ifndef App42CPPSDK_Connector_h
#define App42CPPSDK_Connector_h

#include "cocos-ext.h"
#include "cocos2d.h"
#include "App42Response.h"
#include "curl/curl.h"
#include "Common.h"


using namespace std;
using namespace cocos2d;

namespace Util
{
    
    static char* url_encode(string &url)
    {
        CURL *curl = curl_easy_init();
        char *str = curl_easy_escape(curl, url.c_str(), url.size());
        
        //app42Trace("URL == %s",str);
        return str;
    }
    
    static void printMap(map<std::string, std::string>map)
    {
        std::map<string,string>::iterator it;
        
        for(it=map.begin(); it!=map.end(); ++it)
        {
            printf("\n%s=%s",it->first.c_str(),it->second.c_str());
        }
    }
    
    static void printVector(vector<std::string>vector)
    {
        std::vector<string>::iterator it;
        
        for(it=vector.begin(); it!=vector.end(); ++it)
        {
            printf("\n%s",it->c_str());
        }
    }
    
    static void executePost(string baseUrl,std::vector<std::string> headers, const char* postData,CCObject* pTarget, SEL_CallFuncND pSelector)
    {
        
        cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
        request->setUrl(baseUrl.c_str());
        request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
        
        headers.push_back("Accept: application/json");
        headers.push_back("Content-Type: application/json");
        headers.push_back("SDKName: Cocos2d-X");
        
        request->setHeaders(headers);
        //printVector(headers);
        request->setResponseCallback(pTarget, pSelector);
        
        request->setRequestData(postData, strlen(postData));
        
        cocos2d::extension::CCHttpClient::getInstance()->send(request);
        request->release();
        return;
    }
    
    static void executePut(string baseUrl,std::vector<std::string> headers, const char* postData,CCObject* pTarget, SEL_CallFuncND pSelector)
    {
        cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
        request->setUrl(baseUrl.c_str());
        request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);
        
        headers.push_back("Accept: application/json");
        headers.push_back("Content-Type: application/json");
        headers.push_back("SDKName: Cocos2d-X");
        headers.push_back("Custom-Headers: PUT");
        request->setHeaders(headers);
        
        request->setResponseCallback(pTarget, pSelector);
        
        request->setRequestData(postData, strlen(postData));
        
        cocos2d::extension::CCHttpClient::getInstance()->send(request);
        request->release();
        return;
    }

    static void executeGet(string baseUrl,std::vector<std::string> headers,CCObject* pTarget, SEL_CallFuncND pSelector)
    {
        cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
        request->setUrl(baseUrl.c_str());
        request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
        
        headers.push_back("Accept: application/json");
        headers.push_back("Content-Type: application/json");
        headers.push_back("SDKName: Cocos2d-X");
        
        request->setHeaders(headers);
        //printVector(headers);
        //printf("\nBaseUrl=%s",baseUrl.c_str());
        request->setResponseCallback(pTarget, pSelector);
        
        cocos2d::extension::CCHttpClient::getInstance()->send(request);
        request->release();
        return;
    }
    
    static void executeDelete(string baseUrl,std::vector<std::string> headers,CCObject* pTarget, SEL_CallFuncND pSelector)
    {
        cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
        request->setUrl(baseUrl.c_str());
        request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
        
        headers.push_back("Accept: application/json");
        headers.push_back("Content-Type: application/json");
        headers.push_back("SDKName: Cocos2d-X");
        headers.push_back("Custom-Headers:DELETE");
        
        request->setHeaders(headers);
        
        request->setResponseCallback(pTarget, pSelector);
        
        cocos2d::extension::CCHttpClient::getInstance()->send(request);
        request->release();
        return;
    }
    
    static void handleException()
    {
        
    }
    
    
}



#endif
