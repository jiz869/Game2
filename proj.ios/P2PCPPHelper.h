//
//  P2PCPPHelper.h
//  crossRoad
//
//  Created by Min Li on 2014-06-16.
//
//

#ifndef crossRoad_P2PCPPHelper_h
#define crossRoad_P2PCPPHelper_h

class P2PManagerDelegate {
public:
    virtual void onConnect(const char * myPeerId , const char * enemyPeerId)=0;
    virtual void onDisconnect()=0;
    virtual void onDataReceived(const char * command)=0;
};

class P2PCPPHelper {
    
public:
    static P2PCPPHelper * sharedHelper();
    void connect(P2PManagerDelegate * delegate);
    void sendData(const char * data);
private:
    P2PCPPHelper();
    ~P2PCPPHelper();
};

#endif
