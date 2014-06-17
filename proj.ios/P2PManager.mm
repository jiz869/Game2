//
//  P2PManager.m
//  crossRoad
//
//  Created by Min Li on 2014-06-13.
//
//

#import "P2PManager.h"

static P2PCPPHelper * helper;

P2PCPPHelper::P2PCPPHelper(){
    
}

P2PCPPHelper::~P2PCPPHelper(){
    
}

P2PCPPHelper * P2PCPPHelper::sharedHelper(){
    if (helper==NULL) {
        helper = new P2PCPPHelper();
    }
    return helper;
}

void P2PCPPHelper::connect(P2PManagerDelegate *delegate){
    P2PManager * manager = [P2PManager sharedManager];
    manager->delegate = delegate;
    [manager connect];
}

void P2PCPPHelper::sendData(const char *data){
    P2PManager * manager = [P2PManager sharedManager];
    NSString * command = [NSString stringWithUTF8String:data];
    [manager sendData:command];
}

static P2PManager * manager;

@implementation P2PManager

- (id) init
{
    if (self = [super init]) {
        _session = NULL;
        _picker = NULL;
        delegate = NULL;
    }
    
    return self;
}

+ (P2PManager *) sharedManager{
    if(manager == NULL){
        manager = [[P2PManager alloc] init];
    }
    
    return manager;
}

-(void) connect
{
    _picker = [[GKPeerPickerController alloc] init];
    _picker.delegate = self;
    _picker.connectionTypesMask = GKPeerPickerConnectionTypeNearby;
    [_picker show];
}

-(void) disconnect
{
    [self.session disconnectFromAllPeers];
    [self.session release];
    [self.peerID release];
    _session = nil;
    _peerID = nil;
}

-(void) peerPickerController:(GKPeerPickerController *)picker didConnectPeer:(NSString *)peerID toSession:(GKSession *)session
{
    NSLog(@"peerPickerController %@", peerID);
    self.session = session;
    self.peerID = peerID;
    _session.delegate = self;
    [_session setDataReceiveHandler:self withContext:nil];
    _picker.delegate = nil;
    [_picker dismiss];
    [self.picker release];
    _picker=nil;
    if (delegate) {
        delegate->onConnect([_session.peerID UTF8String], [self.peerID UTF8String]);
    }
}

- (void)peerPickerControllerDidCancel:(GKPeerPickerController *)picker{
    
    _picker.delegate = nil;
    [self.picker release];
    _picker=nil;
}


- (void)session:(GKSession *)session peer:(NSString *)peerID didChangeState:(GKPeerConnectionState)state {
    switch (state){
        case GKPeerStateConnected:
            NSLog(@"connected");
            break;
            
        case GKPeerStateDisconnected:
            NSLog(@"disconnected");
            [self.session release];
            [self.peerID release];
            _session = nil;
            _peerID = nil;
            if (delegate) {
                delegate->onDisconnect();
            }
            break;
        default:
            break;
    }
}

- (void) sendData:(NSString *)data
{
    NSData * _data = [data dataUsingEncoding:NSUTF8StringEncoding];
    
    if (_session) {
        [_session sendData:_data toPeers:[NSArray arrayWithObject:_peerID] withDataMode:GKSendDataReliable error:nil];
    }
}

-(void)receiveData:(NSData *)data fromPeer:(NSString *)peer inSession:(GKSession *)session context:(void *)context {
    NSString* str;
    str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    const char * command = [str UTF8String];
    if (delegate) {
        delegate->onDataReceived(command);
    }
}

@end
