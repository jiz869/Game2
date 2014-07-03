//
//  GameService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 17/10/13.
//
//

#ifndef __App42CPPSDK__GameService__
#define __App42CPPSDK__GameService__

#include <iostream>
#include "App42Service.h"
#include "cocos2d.h"
#include "App42GameResponse.h"

using namespace std;

class GameService : public App42Service, public cocos2d::CCNode
{
    
private:
    GameService();
	static GameService* _instance;
public:
    
	/**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static GameService* Initialize(string apikey, string secretkey);
    
	static void Terminate();
    
	/**
     * Builds the instance of UserService.
     *
     * @return UserService - UserService Object
     */
	static GameService* getInstance();
    
    /**
     * Creates a game on the cloud
     *
     * @param gameName - Name of the game that has to be created.
     * @param description - Description of the game to be created.
     * @return app42Result - The result of the request.
     *
     */
	void CreateGame(string gameName,string description, CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Retrieves the game by the specified name
     *
     * @param gameName - Name of the game that has to be fetched
     * @return app42Result - The result of the request.
     *
     */
	void GetGamebyName(string gameName,CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
	/**
     * Fetches all games for the App
     *
     * @return app42Result - The result of the request.
     *
     */
	void GetAllGames(CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
};
#endif /* defined(__App42CPPSDK__GameService__) */
