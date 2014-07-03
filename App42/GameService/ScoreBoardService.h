//
//  ScoreBoardService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#ifndef __App42CPPSDK__ScoreBoardService__
#define __App42CPPSDK__ScoreBoardService__

#include <iostream>
#include "App42Service.h"
#include "cocos2d.h"

using namespace std;
class ScoreBoardService: public App42Service, public cocos2d::CCNode
{
private:
	static ScoreBoardService* _instance;
	ScoreBoardService();
public:
    /**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static ScoreBoardService* Initialize(string apikey, string secretkey);
    
	static void Terminate();
    
	/**
     * Builds the instance of UserService.
     *
     * @return UserService - UserService Object
     */
	static ScoreBoardService* getInstance();
    
    /**
     * Saves the User score for a game
     *
     * @param gameName - Name of the game for which score has to be saved.
     * @param userName - The user for which score has to be saved.
     * @param score - The score that has to be saved.
     * @return app42Result - The result of the request.
     *
     */
	void SaveUserScore(string gameName,string userName, double score, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Edits the User score for a game
     *
     * @param scoreId - The id for score that has to be edited.
     * @param gameScore - The new score that has to be saved.
     * @return app42Result - The result of the request.
     *
     */
    void EditScoreValueById(string scoreId, double gameScore, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Retrieves the average game score for the specified user
     *
     * @param gameName - Name of the game for which average score has to be fetched
     * @param userName - The user for which average score has to be fetched
     * @return app42Result - The result of the request.
     *
     */
	void GetAverageScoreByUser(string gameName, string userName, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Retrieves the highest game score for the specified user
     *
     * @param gameName - Name of the game for which highest score has to be fetched
     * @param userName - The user for which highest score has to be fetched
     * @return app42Result - The result of the request.
     *
     */
	void GetHighestScoreByUser(string gameName,string userName , cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * This function returns the top score attained by the specified user in the game.
     *
     * @param gameName - Name of the game
     * @param userName - Name of the user for which score has to retrieve
     * @return app42Result - The result of the request.
     *
     */
	void GetLastScoreByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Retrieves the Top Rankings for the specified game
     *
     * @param gameName - Name of the game for which ranks have to be fetched
     * @return app42Result - The result of the request.
     *
     */
	void GetTopRankings(string gameName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Retrieves the User Ranking for the specified game
     *
     * @param gameName - Name of the game for which ranks have to be fetched.
     * @param userName - Name of the user for which ranks have to be fetched.
     * @return app42Result - The result of the request.
     *
     */
	void GetUserRanking(string gameName, string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Retrieves the Top Rankers for the specified game
     *
     * @param gameName - Name of the game for which ranks have to be fetched
     * @param max - maximum number of result need to be fetched
     * @return app42Result - The result of the request.
     *
     */
    void GetTopNRankers(string gameName,int max,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    /**
     * Retrieves the scores for a game for the specified name
     *
     * @param gameName
     *            - Name of the game for which score has to be fetched
     * @param userName
     *            - The user for which score has to be fetched
     *
     * @return the game score for the specified user
     *
     */

    void GetScoresByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);

    /**
     * Retrieves the lowest game score for the specified user
     *
     * @param gameName
     *            - Name of the game for which lowest score has to be fetched
     * @param userName
     *            - The user for which lowest score has to be fetched
     *
     * @return the lowest game score for the specified user
     *
     */
    void GetLowestScoreByUser(string gameName,string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);

    /**
     * Retrieves the Top Rankers/Scorers for the specified game within the
     * given group
     * @param gameName
     *            - Name of the game for which ranks have to be fetched
     * @param group
     *            - array of usernames in the group
     *
     * @return the Top rankers for a game
     *
     */
    void GetTopRankersByGroup(string gameName,std::vector<std::string>group,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);

    /**
     * Retrieves the last score made by the user in all games
     *
     * @param userName
     *            - Name of the user for which the last score has to be fetched
     *
     * @return the Top rankers for a game
     *
     */

    void GetLastGameScore(string userName,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
    
    void GetTopNTargetRankers(string gameName,int max,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);

};

#endif /* defined(__App42CPPSDK__ScoreBoardService__) */
