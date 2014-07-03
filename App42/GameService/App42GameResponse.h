//
//  App42GameResponse.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 17/10/13.
//
//

#ifndef __App42CPPSDK__App42GameResponse__
#define __App42CPPSDK__App42GameResponse__

#include <iostream>
#include "cJSON.h"
#include "App42Response.h"
#include "App42Score.h"

using namespace cocos2d::extension;
using namespace std;

struct App42Game;
//struct App42Score;

class App42GameResponse : public App42Response
{
public:
    
    vector<App42Game> games;
    vector<App42Score> scores;
    
    App42GameResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);
	~App42GameResponse();
    void onComplete(cocos2d::CCNode *sender, void *data);
private:
    void init();
        
};


typedef struct App42Game
{
    string name;
    string description;
}App42Game;

#endif /* defined(__App42CPPSDK__App42GameResponse__) */
