#import <UIKit/UIKit.h>
#import <StartApp/StartApp.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate , STADelegateProtocol> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

