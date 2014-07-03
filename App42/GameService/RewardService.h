//
//  RewardService.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 18/10/13.
//
//

#ifndef __App42CPPSDK__RewardService__
#define __App42CPPSDK__RewardService__

#include <iostream>
#include "App42Service.h"
#include "cocos2d.h"

using namespace std;
class RewardService: public App42Service, public cocos2d::CCNode
{
private:
	static RewardService* _instance;
	RewardService();
public:
    /**
     * This is a constructor that takes
     *
     * @param apiKey
     * @param secretKey
     * @param server
     *
     */
	
    static RewardService* Initialize(string apikey, string secretkey);
    
	static void Terminate();
    
	/**
     * Builds the instance of RewardService.
     *
     * @return RewardService - RewardService Object
     */
	static RewardService* getInstance();
    
    /**
     * Creates Reward. Reward can be Sword, Energy etc. When Reward Points have
     * to be added the Reward name created using this method has to be
     * specified.
     *
     * @param rewardName - The reward that has to be created
     * @param description - The description of the reward to be created.
     * @return app42Result - The result of the request.
     *
     */
	void CreateReward(string rewardName,string description, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Fetches all the Rewards
     *
     * @return app42Result - The result of the request.
     *
     */
	void GetAllRewards(cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Adds the reward points to an users account. Reward Points can be earned
     * by the user which can be redeemed later.
     *
     * @param gameName - Name of the game for which reward points have to be added
     * @param userName - The user for whom reward points have to be added
     * @param rewardName - The rewards for which reward points have to be added
     * @param rewardPoints - The points that have to be added
     * @return app42Result - The result of the request.
     *
     */
	void EarnRewards(string gameName, string userName, string rewardName, double rewardPoints,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Deducts the reward points from the earned rewards by a user.
     *
     * @param gameName - Name of the game for which reward points have to be deducted
     * @param userName - The user for whom reward points have to be deducted
     * @param rewardName - The rewards for which reward points have to be deducted
     * @param rewardPoints - The points that have to be deducted
     * @return app42Result - The result of the request.
     *
     */
	void RedeemRewards(string gameName, string userName, string rewardName, double rewardPoints,cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
	/**
     * Fetches the reward points for a particular user
     *
     * @param gameName - Name of the game for which reward points have to be fetched
     * @param userName - The user for whom reward points have to be fetched
     * @return app42Result - The result of the request.
     * 
     */
	void GetGameRewardPointsForUser(string gameName, string userName, cocos2d::CCObject* pTarget, cocos2d::SEL_CallFuncND pSelector);
};
#endif /* defined(__App42CPPSDK__RewardService__) */
