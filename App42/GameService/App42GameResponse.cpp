//
//  App42GameResponse.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 17/10/13.
//
//

#include "App42GameResponse.h"
#include "Common.h"
#include "JSONDocument.h"

App42GameResponse::App42GameResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector)
:App42Response(pTarget,pSelector)
{
    
}

App42GameResponse::~App42GameResponse()
{
    games.clear();
}

void App42GameResponse::onComplete(cocos2d::CCNode *sender, void *data)
{
    App42Response::onComplete(sender, data);
    init();
    
    
    if (_pTarget && _pSelector)
    {
        (_pTarget->*_pSelector)((cocos2d::CCNode *)_pTarget, this);
    }
}

void App42GameResponse::init()
{
    if(_result != 200)
    {
        Util::app42Trace("App42Game failed result is %d", _result);
        buildErrorMessage();
        return;
    }
    cJSON *ptrBody = cJSON_Parse(_body.c_str());
    cJSON* ptrApp42 = Util::getJSONChild("app42", ptrBody);
    cJSON* ptrResponse = Util::getJSONChild("response", ptrApp42);
    cJSON* ptrGames = Util::getJSONChild("games", ptrResponse);
    cJSON* ptrGame = Util::getJSONChild("game", ptrGames);
    
    cJSON* child = ptrGame;
    if(child->type == cJSON_Array)
    {
        child = child->child;
    }
    while(child != NULL && child->type == cJSON_Object)
    {
        App42Game app42Game;
        app42Game.name = Util::getJSONString("name", child);
        app42Game.description = Util::getJSONString("description", child);
        games.push_back(app42Game);
        child = child->next;
        if(Util::getJSONChild("scores",ptrGame))
        {
			cJSON* ptrScores = Util::getJSONChild("scores",ptrGame);
			cJSON* ptrScore = Util::getJSONChild("score",ptrScores);
			cJSON* child = ptrScore;
			if(child->type == cJSON_Array)
            {
				child = child->child;
			}
			while(child != NULL && child->type == cJSON_Object)
            {
				App42Score app42Score;
                app42Score.setUserName(Util::getJSONString("userName", child));
                app42Score.setRank(Util::getJSONInt("rank", child));
                app42Score.setScoreId(Util::getJSONString("scoreId", child));
                app42Score.setScoreValue(Util::getJSONDouble("value", child));
                app42Score.setCreatedOn(Util::getJSONString("createdOn", child));
                
                cJSON *ptrJsonDoc = Util::getJSONChild("jsonDoc", child);
                if (ptrJsonDoc != NULL)
                {
                    vector<JSONDocument> docArray;
                    if (ptrJsonDoc->type == cJSON_Array)
                    {
                        ptrJsonDoc = ptrJsonDoc->child;
                    }
                    while(ptrJsonDoc != NULL && ptrJsonDoc->type == cJSON_Object)
                    {
                        JSONDocument jsonDOC;
                        buildJsonDocument(ptrJsonDoc, &jsonDOC);
                        ptrJsonDoc = ptrJsonDoc->next;
                        docArray.push_back(jsonDOC);
                    }
                    app42Score.setJsonDocList(docArray);
                }
				scores.push_back(app42Score);
				child = child->next;
			}
		}
    }
    cJSON_Delete(ptrBody);
}


