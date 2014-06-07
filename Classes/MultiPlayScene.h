/*
 * MultiPlayScene.h
 *
 *  Created on: Jun 4, 2014
 *      Author: mli
 */

#ifndef MULTIPLAYSCENE_H_
#define MULTIPLAYSCENE_H_

#include "cocos2d.h"
#include "appwarp.h"
#include "PlayScene.h"

USING_NS_CC;
using namespace AppWarp;
using namespace std;

#define APPWARP_APP_KEY     "6a62bddeb3c503c6ad766390357a403f2ea778316726be19c29d309064d37862"
#define APPWARP_SECRET_KEY  "f87c2719986446f3d07816a58b5e89e7a7b8af1b04795e5cac8cb5e88afe0e16"
#define ROOM_ID             "2103900253"

typedef enum{
    FIRST=0,
    SECOND,
    ORDER_MAX,
}ORDER;

#define SYNC_TIMES 6

class MultiPlayScene: public PlayScene , public ConnectionRequestListener , public RoomRequestListener,
                    public NotificationListener,public ZoneRequestListener,public ChatRequestListener
{
public:
    MultiPlayScene();
    virtual ~MultiPlayScene();

    virtual bool init();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(MultiPlayScene);

    virtual void onConnectDone(int res);
    virtual void onJoinRoomDone(AppWarp::room revent);
    virtual void onUserJoinedRoom(AppWarp::room event, std::string username);
    void returnOnConnectionFailed();
    virtual void update(float dt);
    virtual void onSubscribeRoomDone(AppWarp::room revent);
    virtual void onPrivateChatReceived(std::string sender, std::string message);
    virtual void onSendPrivateChatDone(int res);

protected:
    void connectToAppWarp();
    bool isFirstLaunch;
    void recover();
    void connectionFailed();
    CCLabelTTF * infoLabel;
    PlayerObj * enemy;
    string enemyName;

    virtual void initMisc();
    virtual void initPlayer();
    void prepareToStart();
    ORDER order;
    unsigned long latency;
    int syncCount;

    virtual void sendStart();
    virtual void sendSync();
    unsigned long getCurrentTime();

    void startGame();

    MultiPlaySceneData * multiPlayScenedata;
};

#endif /* MULTIPLAYSCENE_H_ */
