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

#define USE_UDP

typedef enum{
    FIRST=0,
    SECOND,
    ORDER_MAX,
}ORDER;

#define SYNC_TIMES 6
#define NULL_ROOM_ID "0"

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
    virtual void onUserLeftRoom(AppWarp::room rData, std::string user);
    virtual void onRoomCreated(AppWarp::room rData);
    virtual void onChatReceived(AppWarp::chat chatevent);
    void sendScore();
    void sendOver();
    void gameOver();
    //virtual void sendPlayPos();
    virtual void upHandler(int tag);
    virtual void downHandler(int tag);
    virtual void touchendHandler(int tag);
    virtual void BeginContact(b2Contact* contact);
    void resetEnemy();
    void onInitUDPDone(int result);
    void onUpdatePeersReceived(AppWarp::byte update[], int len, bool isUDP);

protected:
    void connectToAppWarp();
    void recover();
    void connectionFailed(const char * message);
    CCLabelTTF * infoLabel;
    PlayerObj * enemy;
    std::string enemyName;
    std::string userName;
    std::string roomId;

    virtual void initMisc();
    virtual void initPlayer();
    virtual void initControlMenu();
    void prepareToStart();
    ORDER order;
    unsigned long latency;
    int syncCount;

    virtual void sendStart();
    virtual void sendSync();
    virtual void sendUp();
    virtual void sendDown();
    virtual void sendWait();
    virtual void sendHit();

    void startGame();

    virtual void processContact(float dt);

    string getUserName();

    void sendUpdate(const char * command , bool useUdp);

    void processMsg();
};

#endif /* MULTIPLAYSCENE_H_ */
