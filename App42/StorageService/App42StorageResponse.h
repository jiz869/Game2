//
//  App42StorageResponse.h
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 21/10/13.
//
//

#ifndef __App42CPPSDK__App42StorageResponse__
#define __App42CPPSDK__App42StorageResponse__

#include <iostream>
#include "cJSON.h"
#include "App42Response.h"
#include "JSONDocument.h"

using namespace cocos2d::extension;
using namespace std;

struct App42Storage;

class App42StorageResponse : public App42Response
{
public:
    
    vector<App42Storage> storages;
    vector<JSONDocument> documents;
    
    App42StorageResponse(cocos2d::CCObject *pTarget, cocos2d::SEL_CallFuncND pSelector);
	~App42StorageResponse();
    void onComplete(cocos2d::CCNode *sender, void *data);
private:
    void init();
    
};

//typedef struct JSONDocument
//{
//	string docId;
//	string jsonDoc;
//}JSONDocument;

typedef struct App42Storage
{
    string dbName;
    string collectionName;
}App42Storage;

#endif /* defined(__App42CPPSDK__App42StorageResponse__) */
