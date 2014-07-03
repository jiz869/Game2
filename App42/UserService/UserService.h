//
//  UserService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 08/10/13.
//
//

#ifndef __App42CPPSDK__UserService__
#define __App42CPPSDK__UserService__

#include <iostream>
#include "App42Service.h"
#include "cocos2d.h"

using namespace std;

class UserService : public App42Service, public cocos2d::CCNode
{
    
private:
    UserService();
    string buildCreateUserBody(string username, string password, string email,string roleList);
	string buildCompletePostUrl(string baseUrl, string body);
	static UserService* _instance;
public:
    
	/**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static UserService* Initialize(string apikey, string secretkey);

	static void Terminate();
    
	/**
     * Builds the instance of UserService.
     *
     * @return UserService - UserService Object
     */
	static UserService* getInstance();
    
	/**
     * Create a User with userName, password & emailAddress
     *
     * @param userName - Name of the User
     * @param password - Password for the User
     * @param email - Email address of the user
     * @return app42Result - The result of the request
     *
     */
	void CreateUser(string username, string password, string email,CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Authenticate user based on userName and password
     *
     * @param userName  - UserName which should be unique for the App
     * @param password - Password for the User
     * @return app42Result - The result of the request
     */
	void Authenticate(string userName, string password, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
	/**
     * Gets the User details based on userName
     *
     * @param userName - Name of the User to retrieve.
     * @return app42Result - The result of the request.
     */
	void GetUser(string username, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Gets the details of all users
     * 
     * @return app42Result - The result of the request.
     */
	void GetAllUsers(CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
};
#endif /* defined(__App42CPPSDK__UserService__) */
