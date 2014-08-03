#import <UIKit/UIKit.h>
#import "../admob.ios/GADInterstitial.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate , GADInterstitialDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

