//
//  P2PManager.h
//  crossRoad
//
//  Created by Min Li on 2014-06-13.
//
//

#import <Foundation/Foundation.h>
#import "UIKit/UIKit.h"
#import "GameKit/GameKit.h"
#include "P2PCPPHelper.h"

@interface P2PManager : NSObject <GKSessionDelegate , GKPeerPickerControllerDelegate>
{
@public
    P2PManagerDelegate * delegate;
}

@property (nonatomic , retain) GKSession * session;
@property (nonatomic , retain) GKPeerPickerController * picker;
@property (nonatomic , retain) NSString * peerID;

-(void) connect;
-(void) disconnect;
-(void) sendData:(NSString *)data;

@end
