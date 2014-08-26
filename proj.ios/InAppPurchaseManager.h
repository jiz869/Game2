#import <StoreKit/StoreKit.h>
#import <Foundation/Foundation.h>
#include "P2PCPPHelper.h"

#define PRODUCT_ID "ca.welcomelm.continue.ads"

@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate , SKPaymentTransactionObserver>
{
    SKProduct * product;
    SKProductsRequest *productRequest;
@public
    IAPManagerDelegate * delegate;
}

// public methods
- (void)loadStore;
@end


@interface SKProduct (LocalizedPrice)

@property (nonatomic, readonly) NSString *localizedPrice;

@end