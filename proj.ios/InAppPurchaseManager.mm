//
//  InAppPurchaseManager.m
//  crossRoad
//
//  Created by Min Li on 2014-06-02.
//
//

#import "InAppPurchaseManager.h"

static IAPCPPHelper * helper;

IAPCPPHelper::IAPCPPHelper(){
    
}

IAPCPPHelper::~IAPCPPHelper(){
    
}

IAPCPPHelper * IAPCPPHelper::sharedHelper(){
    if (helper==NULL) {
        helper = new IAPCPPHelper();
    }
    return helper;
}

void IAPCPPHelper::request(IAPManagerDelegate *delegate){
    InAppPurchaseManager * manager = [InAppPurchaseManager sharedManager];
    manager->delegate = delegate;
    [manager loadStore];
}

void IAPCPPHelper::purchase(){
    InAppPurchaseManager * manager = [InAppPurchaseManager sharedManager];
    [manager purchase];
}

bool IAPCPPHelper::canMakePayments(){
    InAppPurchaseManager * manager = [InAppPurchaseManager sharedManager];
    return [manager canMakePayments];
}

@implementation SKProduct(LocalizedPrice)
- (NSString *)localizedPrice
{
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:self.priceLocale];
    NSString *formattedString = [numberFormatter stringFromNumber:self.price];
    [numberFormatter release];
    return formattedString;
}
@end

static InAppPurchaseManager * manager;

@implementation InAppPurchaseManager

- (id) init
{
    if (self = [super init]) {
        product = NULL;
        productRequest = NULL;
    }
    
    return self;
}

+ (InAppPurchaseManager *) sharedManager{
    if(manager == NULL){
        manager = [[InAppPurchaseManager alloc] init];
    }
    
    return manager;
}

- (void)requestProductData
{
    NSSet *productIdentifiers = [NSSet setWithObject:[NSString stringWithUTF8String:PRODUCT_ID]];
    productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productRequest.delegate = self;
    [productRequest start];
    
    // we will release the request object in the delegate callback
}

#pragma mark -
#pragma mark SKProductsRequestDelegate methods

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *products = response.products;
    product = [products count] == 1 ? [[products firstObject] retain] : nil;
    if (product)
    {
        NSLog(@"Product title: %@" , product.localizedTitle);
        NSLog(@"Product description: %@" , product.localizedDescription);
        NSLog(@"Product price: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
        delegate->onRequestFinished(true);
    }
    
    for (NSString *invalidProductId in response.invalidProductIdentifiers)
    {
        NSLog(@"Invalid product id: %@" , invalidProductId);
        delegate->onRequestFinished(false);
    }
}

-(void) requestDidFinish:(SKRequest *)request
{
    // finally release the reqest we alloc/init’ed in requestProUpgradeProductData
    [productRequest release];
}

-(void) request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"%@", error);
    delegate->onRequestFinished(false);
}

#pragma -
#pragma Public methods

//
// call this method once on startup
//
- (void)loadStore
{
    // restarts any purchases if they were interrupted last time the app was open
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    // get the product description (defined in early sections)
    [self requestProductData];
}

//
// call this before making a purchase
//
- (BOOL)canMakePayments
{
    return [SKPaymentQueue canMakePayments];
}

//
// kick off the upgrade transaction
//
- (void)purchase
{
    SKPayment *payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

//
// removes the transaction from the queue and posts a notification with the transaction result
//
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
    // remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    if (wasSuccessful)
    {
        delegate->onPaymentFinished(true);
    }
    else
    {
        delegate->onPaymentFinished(false);
    }
}

#pragma mark -
#pragma mark SKPaymentTransactionObserver methods

//
// called when the transaction status is updated
//
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            case SKPaymentTransactionStateRestored:
                [self finishTransaction:transaction wasSuccessful:TRUE];
                break;
            case SKPaymentTransactionStateFailed:
                [self finishTransaction:transaction wasSuccessful:FALSE];
                break;
            default:
                break;
        }
    }
}

@end
