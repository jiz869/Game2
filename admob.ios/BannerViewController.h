//
//  BannerViewController.h
//  crossRoad
//
//  Created by Min Li on 2014-05-26.
//
//

#import <UIKit/UIKit.h>
#import "GADInterstitial.h"

extern NSString * const BannerViewIsClicked;

@interface BannerViewController : UIViewController
{
@public
    GADInterstitial *_bannerView;
}

- (instancetype)initWithContentViewController:(UIViewController *)contentController;

@end
