//
//  Exceptions.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 20/02/14.
//
//

#ifndef App42CPPSDK_Exceptions_h
#define App42CPPSDK_Exceptions_h

#include "App42Exception.h"
#include "cocos2d.h"
#include "Query.h"
#include <algorithm>

using namespace std;


namespace Util
{
    static string removeSpaces(string input)
    {
        input.erase(std::remove(input.begin(),input.end(),' '),input.end());
        return input;
    }
    
    static void throwExceptionIfStringNullOrBlank(string obj, string name)
    {
		if(obj.c_str()==NULL)
        {
			throw new App42Exception(name + ": parameter can not be null ",0,0);
		}
        string temp = removeSpaces(obj);
		int len = temp.length();
        if(len==0)
        {
            throw new App42Exception(name + ": parameter can not be blank ",0,0);
        }
	}
    
    static void throwExceptionIfTargetIsNull(cocos2d::CCObject *obj, string name)
    {
		if(obj==NULL)
        {
			throw new App42Exception(name + ": parameter can not be null ",0,0);
		}
	}
    
    static void throwExceptionIfCallBackIsNull(cocos2d::SEL_CallFuncND pSelector, string name)
    {
		if(pSelector==NULL)
        {
			throw new App42Exception(name + ": parameter can not be null ",0,0);
		}
	}
    
    static void throwExceptionIfMapIsNullOrBlank(std::map<std::string, std::string>map, string name)
    {
		if(map.empty())
        {
			throw new App42Exception(name + ": parameter can not be null ",0,0);
		}
	}
    
    static void throwExceptionIfMaxIsNotValid(int max,string name)
    {
		if(max<1)
        {
			throw new App42Exception(name + ": Max must be greater than zero ",500,1500);
		}
	}
    
    static void throwExceptionIfObjectIsNull(void *obj, string name)
    {
		if(obj==NULL)
        {
			throw new App42Exception(name + ": parameter can not be null ",0,0);
		}
	}
}

#endif
