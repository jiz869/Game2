//
//  BannerViewController.h
//  crossRoad
//
//  Created by Min Li on 2014-05-26.
//
//

#import <UIKit/UIKit.h>
#import "GADBannerView.h"

extern NSString * const BannerViewIsClicked;

@interface BannerViewController : UIViewController
{
@public
    GADBannerView *_bannerView;
}

- (instancetype)initWithContentViewController:(UIViewController *)contentController;

@end
