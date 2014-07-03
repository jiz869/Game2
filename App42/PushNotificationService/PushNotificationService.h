//
//  PushNotificationService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#ifndef __App42CPPSDK__PushNotificationService__
#define __App42CPPSDK__PushNotificationService__

#include <iostream>
#include "App42Service.h"
#include "cocos2d.h"

using namespace std;

class PushNotificationService : public App42Service, public cocos2d::CCNode
{
    
private:
    PushNotificationService();
	static PushNotificationService* _instance;
public:
    
	/**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static PushNotificationService* Initialize(string apikey, string secretkey);
    
	static void Terminate();
    
	/**
     * Builds the instance of UserService.
     *
     * @return UserService - UserService Object
     */
	static PushNotificationService* getInstance();
    
    void registerDeviceToken(string devoceToken, string userName, string deviceType, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void sendPushMessageToUser(string username,  string message, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void sendPushMessageToUser(string userName, map<string, string>messageMap, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void subscribeToChannel(string channel, string userName, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void sendPushMessageToChannel(string channel, string message, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    void sendPushMessageToChannel(string channel, map<string, string>messageMap, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
};
#endif /* defined(__App42CPPSDK__PushNotificationService__) */
