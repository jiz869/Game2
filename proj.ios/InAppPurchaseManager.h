#import <StoreKit/StoreKit.h>
#import <Foundation/Foundation.h>

#define PRODUCT_ID "ca.welcomelm.ugp"

@interface InAppPurchaseManager : NSObject <SKProductsRequestDelegate , SKPaymentTransactionObserver>
{
    SKProduct * product;
    SKProductsRequest *productRequest;
}

// public methods
- (void)loadStore;
- (BOOL)canMakePurchases;
- (void)purchaseProUpgrade;
@end


@interface SKProduct (LocalizedPrice)

@property (nonatomic, readonly) NSString *localizedPrice;

@end