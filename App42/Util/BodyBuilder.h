//
//  BodyBuilder.h
//  testTool
//
//  Created by Dhruv Chopra on 4/1/13.
//  Copyright (c) 2013 Dhruv Chopra. All rights reserved.
//

#ifndef BodyBuilder_h
#define BodyBuilder_h

#include "Signing.h"


using namespace std;

namespace Util {
    
    static void BuildGetSigningMap(string apikey, string timestamp, string version, map<string, string>& getMap){
        
        getMap["apiKey"] = apikey;
        getMap["timeStamp"] = timestamp;
        getMap["version"] = version;
        
        return;
    }

    static void BuildPostSigningMap(string apikey, string timestamp, string version, string body, map<string, string>& postMap){
        
        postMap["apiKey"] = apikey;
        postMap["timeStamp"] = timestamp;
        postMap["version"] = version;
        postMap["body"] = body;
        
        return;
    }
    
    static string BuildQueryString(string apikey, string timestamp, string version, string signature){
        string queryString = "";
        queryString.append("apiKey=").append(apikey);
        queryString.append("&timeStamp=").append(timestamp);
        queryString.append("&version=").append(version);
        queryString.append("&signature=").append(signature);
        
        return queryString;
    }
    
    static void BuildHeaders(string apikey, string timestamp, string version, string signature, std::vector<std::string> &headers)
    {
        string stringValue = "";
        stringValue.append("apiKey:").append(apikey);
        headers.push_back(stringValue);
        
        stringValue = "";
        stringValue.append("timeStamp:").append(timestamp);
        headers.push_back(stringValue);
        
        stringValue = "";
        stringValue.append("signature:").append(signature);
        headers.push_back(stringValue);
        
        stringValue = "";
        stringValue.append("version:").append(version);
        headers.push_back(stringValue);
        
    }
    
    static void BuildHeaders(std::map<std::string, std::string>metaHeaders,std::vector<std::string> &headers)
    {
        std::map<string,string>::iterator it;
        
        for(it=metaHeaders.begin(); it!=metaHeaders.end(); ++it)
        {
            string stringValue = it->first;
            stringValue.append(":");
            stringValue.append(it->second);
            headers.push_back(stringValue);
        }
    }
    
    static string GetStringFromVector(std::vector<std::string>vec)
    {
        std::vector<string>::iterator it;
        string stringValue;
        for(it=vec.begin(); it!=vec.end(); ++it)
        {
            if (it!=vec.begin() && it!=vec.end())
            {
                stringValue.append(",");
            }
            stringValue.append(it->c_str());
        }
        return stringValue;
    }
}

#endif
