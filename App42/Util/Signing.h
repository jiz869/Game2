//
//  Signing.h
//  testTool
//
//  Created by Dhruv Chopra on 4/1/13.
//  Copyright (c) 2013 Dhruv Chopra. All rights reserved.
//

#ifndef Signing_h
#define Signing_h

#include <string.h>
#include <vector>
#include "app42base64.h"
#include <sys/time.h>
#include <algorithm>
#include <map>
#include "HMAC_SHA1.h"
#include "urlencode.h"
#include <sstream>
#include "Common.h"

using namespace std;

namespace Util
{

    static string signString(string key, string data){        
        unsigned char hmac_digest[20];
        memset(hmac_digest, 0, 20);
        CHMAC_SHA1 hmac_sha1;
        hmac_sha1.HMAC_SHA1((unsigned char *)data.c_str(),data.length(), (unsigned char *)key.c_str(), key.length(),hmac_digest);
        std::string hmac = urlencode(base64_encode(hmac_digest, 20));
        
        return hmac;
    }
    
    static string signMap(string secretkey, std::map<string, string> map){
        // First we need to sort the members
        vector<string> keys;
        
        for( std::map<string,string>::iterator it=map.begin(); it!=map.end(); ++it)
        {
            keys.push_back(it->first);
        }

        std::sort(keys.begin(), keys.end());

        // Now we need to build the data string to be signed
        string data;
        for(unsigned int i=0; i<keys.size(); i++){
            string name = keys[i];
            data.append(name);
            data.append(map[name]);
        }
        //printf("signData=%s",data.c_str());
        // ok now we can sign this data with the given secret key
        return signString(secretkey, data);
        
    };
    
	static string ItoA(int num)
	{
		std::stringstream str;
		str << num;
		std::string res(str.str());
		return res;
	}

    static string getTimeStamp(){
		time_t localTime;
		struct tm *utcTime_tm;

		localTime = time(NULL);
		utcTime_tm = gmtime(&localTime);

		std::string date(ItoA(1900+utcTime_tm->tm_year));
		date.append("-");
		if((utcTime_tm->tm_mon+1) < 10)
		{
			date.append("0");
			date.append(ItoA(utcTime_tm->tm_mon+1));
		}
		else
			date.append(ItoA(utcTime_tm->tm_mon+1));
		date.append("-");
		if((utcTime_tm->tm_mday) < 10)
		{
			date.append("0");
			date.append(ItoA(utcTime_tm->tm_mday));
		}
		else
			date.append(ItoA(utcTime_tm->tm_mday));
		date.append("T");
		date.append(ItoA(utcTime_tm->tm_hour));
		date.append(":");
		date.append(ItoA(utcTime_tm->tm_min));
		date.append(":");
		date.append(ItoA(utcTime_tm->tm_sec));
		date.append(".000Z");

		return date;
    }
}

#endif
