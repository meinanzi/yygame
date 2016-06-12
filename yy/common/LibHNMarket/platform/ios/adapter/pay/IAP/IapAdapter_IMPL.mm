#import "IapAdapter_IMPL.h"

#import <StoreKit/StoreKit.h>
#import <StoreKit/SKPaymentTransaction.h>
#import <UIKit/UIAlert.h>
#import "JSONKit.h"
#import "HNUtility.h"

#define HTTP_ERROR_STATUSCODE    -10000000

#define HTTP_SUCCESS_STATUSCODE   200


#define SANDBOX_VERIFY      @"https://sandbox.itunes.apple.com/verifyReceipt"

#define PRODUCTION_VERIFY   @"https://buy.itunes.apple.com/verifyReceipt"

typedef NS_ENUM(NSInteger, IPAPayState)
{
    emPaymentStart = 0,
    emPaymentResponse,
    emPaymentChecking,
    emPaymentSuccess,
    emPaymentCancelled,
    emPaymentInvalid,
    emPaymentNotAllowed,
    emPaymentTimeOut,
    emCannotPayment,
    emClientInvalid,
    emProductNotAvailable,
    emErrorUnknown,
    emPaymentCheckFailed
};

@protocol PayIAPObserverDelegate <NSObject>
@required
-(void) transactionPayEvent:(IPAPayState)payState data:(NSString*) data;
@end

@interface PayStoreObserver : NSObject<SKPaymentTransactionObserver, NSURLConnectionDataDelegate>
{
    SKPaymentTransaction*   _paymentTransaction;
    BOOL                    _payVerify;
}

@property (nonatomic, strong) id<PayIAPObserverDelegate> iapObserverDelegate;

- (void) completeTransaction:(SKPaymentTransaction *)transaction;
- (void) failedTransaction:(SKPaymentTransaction *)transaction;
- (void) restoreTransaction:(SKPaymentTransaction *)transaction;
- (void) recordTransaction:(SKPaymentTransaction *)transaction;
- (void) provideContent:(NSString *)productId;
- (void) finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful;
- (void) sendAyncVerify:(NSString*) url;

- (NSDictionary*) sendSyncVerify:(NSString*) url repeatCount:(unsigned int) repeatCount;

- (void) setVerify:(bool)verify;

@end

@implementation PayStoreObserver

@synthesize iapObserverDelegate;

- (void) setVerify:(bool)verify
{
    _payVerify = verify;
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction* transaction in transactions)
	{
		switch (transaction.transactionState)
		{
            case SKPaymentTransactionStatePurchasing:
                [self completingTransaction:transaction];
                break;
			case SKPaymentTransactionStatePurchased:        //交易完成
                NSLog(@"［订单号］%@", transaction.transactionIdentifier);
				[self completeTransaction:transaction];
				break;
			case SKPaymentTransactionStateFailed:           //交易失败
				[self failedTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:         //已经购买过该商品
				[self restoreTransaction:transaction];
				break;
			default:
				break;
		}
	}
}

// Sent when transactions are removed from the queue (via finishTransaction:).
- (void) paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions
{
    HNNSLog(@"paymentQueue removedTransactions");
}

// Sent when all transactions from the user's purchase history have successfully been added back to the queue.
- (void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction{
    HNNSLog(@"paymentQueueRestoreCompletedTransactionsFinished");
    UIAlertView *uA = [[UIAlertView alloc] initWithTitle:nil message:@"内购恢复成功"
                                                delegate:nil
                                       cancelButtonTitle:@"确定"
                                       otherButtonTitles:nil, nil];
    [uA show];
    [uA release];
}

// Sent when an error is encountered while adding transactions from the user's purchase history back to the queue.
- (void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    HNNSLog(@"paymentQueue restoreCompletedTransactionsFailedWithError --> %@", error);
    
    UIAlertView *uA = [[UIAlertView alloc] initWithTitle:nil message:@"内购恢复失败"
                                                delegate:nil
                                       cancelButtonTitle:@"确定"
                                       otherButtonTitles:nil, nil];
    [uA show];
    [uA release];
}

- (void) completingTransaction: (SKPaymentTransaction *)transaction
{
    HNNSLog(@"completingTransaction");
    
    [iapObserverDelegate transactionPayEvent:emPaymentResponse data:nil];
}

- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
    HNNSLog(@"compileteTransaction");
    
    NSString* receipt = [[NSString alloc] initWithData:transaction.transactionReceipt
                                              encoding:NSUTF8StringEncoding];
    
    NSString *encodeReceipt = [HNUtility encodeBase64:receipt];
    
    NSString* ext = [NSString stringWithFormat:@"{\"receipt-data\":\"%@\",\"order-id\":\"%@\"}", encodeReceipt, transaction.transactionIdentifier];
    NSLog(@"[ext]%@", ext);
    
    _paymentTransaction = transaction;
    if (_payVerify)
    {
        [iapObserverDelegate transactionPayEvent:emPaymentChecking data:nil];
        //[self sendAyncVerify:SANDBOX_VERIFY];
        [self sendAyncVerify:PRODUCTION_VERIFY];
    }
    else
    {
        [iapObserverDelegate transactionPayEvent:emPaymentSuccess data:ext];
        //[iapObserverDelegate transactionPayEvent:emPaymentSuccess data:nil];
        [[SKPaymentQueue defaultQueue] finishTransaction: _paymentTransaction];
    }
    
//    NSDictionary* resultDic = [self sendSyncVerify:PRODUCTION_VERIFY repeatCount:2];
//    
//    int status = [[resultDic objectForKey:@"status"] intValue];
//
//    HNNSLog(@"status = %d", status);
//
//    if (21007 == status)
//    {
//        resultDic = [self sendSyncVerify:SANDBOX_VERIFY repeatCount:2];
//    }
//    
//    status = [[resultDic objectForKey:@"status"] intValue];
//
//    HNNSLog(@"status = %d", status);
//    
//    bool ret = (0 == status);
//    if (ret) {
//        [iapObserverDelegate TransactionPayEvent:PaymentSuccess data:nil];
//        [[SKPaymentQueue defaultQueue] finishTransaction: _paymentTransaction];
//    } else {
//        [iapObserverDelegate TransactionPayEvent:PaymentCheckFailed data:[NSString stringWithFormat:@"%d", status]];
//    }
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{

    NSLog(@"failedTransaction");

	switch (transaction.error.code)
    {
		case SKErrorPaymentCancelled:
			[iapObserverDelegate transactionPayEvent:emPaymentCancelled data:nil];
			break;
		case SKErrorPaymentInvalid:
			[iapObserverDelegate transactionPayEvent:emPaymentInvalid data:nil];
			break;
		case SKErrorPaymentNotAllowed:
			[iapObserverDelegate transactionPayEvent:emPaymentNotAllowed data:nil];
			break;
        case SKErrorClientInvalid:
            [iapObserverDelegate transactionPayEvent:emClientInvalid data:nil];
			break;
        case SKErrorStoreProductNotAvailable:
            [iapObserverDelegate transactionPayEvent:emProductNotAvailable data:nil];
			break;
		default:
			[iapObserverDelegate transactionPayEvent:emErrorUnknown data:nil];
			break;
	}
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    HNNSLog(@"restoreTransaction---");
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

//
// saves a record of the transaction by storing the receipt to disk
//
- (void) recordTransaction:(SKPaymentTransaction *)transaction
{
    HNNSLog(@"recordTransaction");
}

//
// enable pro features
//
- (void) provideContent:(NSString *)productId
{
    HNNSLog(@"provideContent");
}

- (void) finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
    HNNSLog(@"finishTransaction [%d]", wasSuccessful);
}

- (void) sendAyncVerify:(NSString*) url
{
    NSString* receipt = [[NSString alloc] initWithData:_paymentTransaction.transactionReceipt
                                                         encoding:NSUTF8StringEncoding];
    
	NSString *encodeReceipt = [HNUtility encodeBase64:receipt];
    
	NSString *data = [NSString stringWithFormat:@"{\"receipt-data\":\"%@\"}", encodeReceipt];
    
    NSURL* nsurl = [NSURL URLWithString:
                       [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
	NSData *postData = [NSData dataWithBytes:[data UTF8String] length:[data length]];
    
	NSMutableURLRequest *connectionRequest = [NSMutableURLRequest requestWithURL:nsurl];
	[connectionRequest setHTTPMethod:@"POST"];
    [connectionRequest addValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
	[connectionRequest setTimeoutInterval:360];
	[connectionRequest setCachePolicy:NSURLRequestUseProtocolCachePolicy];
	[connectionRequest setHTTPBody:postData];
    
    HNNSLog(@"request outtime = %f", [connectionRequest timeoutInterval]);
    
	[NSURLConnection connectionWithRequest:connectionRequest delegate:self];
    
    [receipt release];
}

- (NSDictionary*) sendSyncVerify:(NSString*) url repeatCount:(unsigned int) repeatCount
{
    NSString* receipt = [[NSString alloc] initWithData:_paymentTransaction.transactionReceipt
                                              encoding:NSUTF8StringEncoding];
	
    NSString *encodeReceipt = [HNUtility encodeBase64:receipt];
    
	NSString *value = [NSString stringWithFormat:@"{\"receipt-data\":\"%@\"}", encodeReceipt];
    
	NSData *data = [NSData dataWithBytes:[value UTF8String] length:[value length]];
    
    NSURL* storeURL = [NSURL URLWithString: [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    NSDictionary* resultDic = nil;
    unsigned int repeat = 1;
    int statusCode = HTTP_ERROR_STATUSCODE;
    
    do {
        // 请求成功，则推出循环
        if (statusCode == HTTP_SUCCESS_STATUSCODE) {
            break;
        }
        
        // 超出请求次数，则推出循环
        if (repeat > repeatCount) {
            break;
        }
        
        ++repeat;
        
        NSMutableURLRequest *connectionRequest = [NSMutableURLRequest requestWithURL:storeURL];
        [connectionRequest setHTTPMethod:@"POST"];
        [connectionRequest addValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
        [connectionRequest setTimeoutInterval:240];
        [connectionRequest setCachePolicy:NSURLRequestUseProtocolCachePolicy];
        [connectionRequest setHTTPBody:data];
        
        HNNSLog(@"request outtime = %f", [connectionRequest timeoutInterval]);
        
        NSError* error = nil;
        NSHTTPURLResponse * response = nil;
        NSData *responseData = [NSURLConnection sendSynchronousRequest:connectionRequest
                                                     returningResponse: &response error:&error];
        
        statusCode = (int)[response statusCode];
        
        HNNSLog(@"sendSyncHttp - statusCode = %d, error = %@", statusCode, error);
        
        OC_SAFE_RELEASE(error);
        
        if (!responseData)
        {
            continue;
        }
        
        resultDic = [NSJSONSerialization JSONObjectWithData:responseData
                                                                  options:NSJSONReadingAllowFragments
                                                                    error:nil];
    } while (true);
    
    [receipt release];
    
    return resultDic;
}

#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSString *receiveData = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSDictionary *result = [receiveData objectFromJSONString];
    int status = [[result objectForKey:@"status"] intValue];

    HNNSLog(@"status = %d", status);
    
    do {
        if (21007 == status)
        {
            [self sendAyncVerify:SANDBOX_VERIFY];
            break;
        }
        
        // 验证成功
        if (0 == status)
        {
            [[SKPaymentQueue defaultQueue] finishTransaction: _paymentTransaction];
            _paymentTransaction = nil;
            [iapObserverDelegate transactionPayEvent:emPaymentSuccess data:nil];
        }
        
    } while (0);
	
    [receiveData release];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    int statusCode = (int)[(NSHTTPURLResponse *)response statusCode];
	switch(statusCode)
    {
		case 200:
		case 206:
			break;
		case 304:
			break;
		case 400:
			break;
		case 404:
			break;
		case 416:
			break;
		case 403:
			break;
		case 401:
		case 500:
			[iapObserverDelegate transactionPayEvent:emPaymentCheckFailed data:[NSString stringWithFormat:@"%d", statusCode]];
			break;
		default:
			break;
	}
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    [iapObserverDelegate transactionPayEvent:emPaymentCheckFailed data:[NSString stringWithFormat:@"%ld", (long)error.code]];
}

@end

////////////////////////////////////////////////////////////////////////////////

@interface IapAdapter_IMPL()<PayIAPObserverDelegate, SKProductsRequestDelegate>
{
    UIViewController*               mViewController;
    NSTimer*                        mWaitingTimer;
    UIView*                         mWaitingUiView;
    UIActivityIndicatorView*        mActivityIndicator;
    UILabel*                        mLabel;
    BOOL                            mWaitFlag;
    PayStoreObserver*               mObserver;
}
- (void) payTimeOut;
- (void) alertWaitDialog:(BOOL)showFlag Title:(NSString*) title;
- (void) waitDialogCallback;
- (BOOL) checkNetworkStatus;
@end

@implementation IapAdapter_IMPL

SYNTHESIZE_SINGLETON_FOR_IMPL(IapAdapter_IMPL)

- (void) dealloc
{
    OC_SAFE_RELEASE(mObserver);
    OC_SAFE_RELEASE(mWaitingTimer);
    OC_SAFE_RELEASE(mWaitingUiView);
    OC_SAFE_RELEASE(mActivityIndicator);
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:mObserver];
    [super dealloc];
}

- (void) start:(UIViewController*) ViewController appId:(NSString*)appId appKey:(NSString*)appKey
{
    NSAssert(ViewController != nil, @"ViewController = nil");
    mViewController = ViewController;
    
    mObserver = [[PayStoreObserver alloc] init];
    mObserver.iapObserverDelegate = self;
    [[SKPaymentQueue defaultQueue] addTransactionObserver:mObserver];
}

- (void) setVerify:(bool)verify
{
    NSAssert(mObserver != nil, @"mObserver  is null");
    [mObserver setVerify:verify];
}

- (void) pay:(NSString*)productId
{
    NSAssert(productId != nil, @"productId = nil");
    if (productId == nil)
    {
        return;
    }
    
    if ([SKPaymentQueue canMakePayments])
    {
		SKProductsRequest *request = [[SKProductsRequest alloc]
                                      initWithProductIdentifiers:[NSSet setWithObject:productId]];
		request.delegate = self;
		[request start];
        
		[self transactionPayEvent:emPaymentStart data:nil];
	}
	else
    {
		[self transactionPayEvent:emCannotPayment data:nil];
	}
}

/**
 *  恢复内购
 */
- (void) restore
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

/*
    收到产品的消息
 */
- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray* products = response.products;
    int count = (int)[products count];
    
    HNNSLog(@"产品数量: %d", count);
    
    if (count > 0)
    {
        for(SKProduct *product in products)
        {
            procuctPrice = [product.price doubleValue];
            
            HNNSLog(@"描述信息 >> %@", [product description]);
            HNNSLog(@"产品标题 >> %@", product.localizedTitle);
            HNNSLog(@"产品描述 >> %@", product.localizedDescription);
            HNNSLog(@"产品价格 >> %@", product.price);
            HNNSLog(@"产品 ID >> %@", product.productIdentifier);
            
            SKPayment* payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
        }
    }
    else
    {
        [self transactionPayEvent:emProductNotAvailable data:nil];
    }
    [request autorelease];
}

/*
    请求完成
 */
- (void) requestDidFinish:(SKRequest *)request
{
    HNNSLog(@"iap requestDidFinish");
}

/*
    失败
 */
- (void) request:(SKRequest *)request didFailWithError:(NSError *)error
{
	UIAlertView *alerView = [[UIAlertView alloc]
                             initWithTitle:@"Alert"
                             message:[error localizedDescription]
                             delegate:nil
                             cancelButtonTitle: NSLocalizedString(@"Close", nil)
                             otherButtonTitles:nil];
	[alerView show];
	[alerView release];
    [self alertWaitDialog:NO Title:nil];
    
    payDelegate->handlePayResult(0, "");
}

- (void) transactionPayEvent:(IPAPayState)payState data:(NSString*)data;
{
    if (emPaymentStart == payState)
    {
        [self alertWaitDialog:YES Title:@"正在支付"];
    }
    else if (emPaymentResponse == payState)
    {
        [self alertWaitDialog:YES Title:@"等待支付结果"];
    }
    else if (emPaymentChecking == payState)
    {
        [self alertWaitDialog:YES Title:@"确认结果"];
    }
    else
    {
        int status = 0;
        
        switch (payState)
        {
            case emPaymentSuccess:
                status = 1;
                [HNUtility messageBox:@"" msg:@"支付成功。"];
                break;
            case emCannotPayment:
                [HNUtility messageBox:@"" msg:@"无支付权限，请到“设置”中打开限制。"];
                break;
            case emPaymentCancelled:
                [HNUtility messageBox:@"" msg:@"支付被取消。"];
                break;
            case emPaymentInvalid:
                [HNUtility messageBox:@"" msg:@"支付无效。"];
                break;
            case emPaymentNotAllowed:
                [HNUtility messageBox:@"" msg:@"未获得支付许可。"];
                break;
            case emPaymentTimeOut:
                [HNUtility messageBox:@"" msg:@"支付连接超时，请稍后重试。"];
                break;
            case emErrorUnknown:
#ifdef DEBUG
                [HNUtility messageBox:@"" msg:@"未知错误(账号有问题)，请稍后重试。"];
#endif
                break;
            case emPaymentCheckFailed:
                [HNUtility messageBox:@"" msg:[NSString stringWithFormat:@"%@ [%@]", @"支付验证失败，请稍后重试。", data]];
                break;
            case emProductNotAvailable:
                [HNUtility messageBox:@"" msg:@"没有产品信息。"];
                break;
            case emClientInvalid:
                [HNUtility messageBox:@"" msg:@"客户端无效。"];
                break;
            default:
                break;
        }
        [self alertWaitDialog:NO Title:nil];
        if (data == nil)
        {
            data = @"";
        }
        payDelegate->handlePayResult(status, [data UTF8String]);
    }
}

- (void) payTimeOut
{
    [self transactionPayEvent:emPaymentTimeOut data:nil];
}

- (void) alertWaitDialog:(BOOL)showFlag Title:(NSString*) title
{
    if (mLabel != nil)
    {
        mLabel.text = title;
    }
    
    if (!mWaitFlag && showFlag)
    {
        mWaitingTimer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(waitDialogCallback) userInfo:nil repeats:YES];
        mWaitingUiView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 200, 150)];
        mWaitingUiView.center = CGPointMake(mViewController.view.bounds.size.width / 2, mViewController.view.bounds.size.height / 2);
        mWaitingUiView.layer.cornerRadius = 8;
        [mWaitingUiView setAlpha:0.6];
        [mWaitingUiView setBackgroundColor:[UIColor blackColor]];
        
        [mViewController.view addSubview:mWaitingUiView];
        
        mActivityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
        mActivityIndicator.center = CGPointMake(mWaitingUiView.bounds.size.width / 2, mWaitingUiView.bounds.size.height / 3);
        
        mLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 200, 30)];
        mLabel.autoresizingMask = mWaitingUiView.autoresizingMask;
        mLabel.textColor = [UIColor whiteColor];
        mLabel.textAlignment = NSTextAlignmentCenter;
        mLabel.text = title;
        mLabel.center = CGPointMake(mWaitingUiView.bounds.size.width / 2, mWaitingUiView.bounds.size.height - 50);
        
        [mWaitingUiView addSubview:mLabel];
        
        [mWaitingUiView addSubview:mActivityIndicator];
        
        [mActivityIndicator startAnimating];
    }
    else if (mWaitFlag && !showFlag)
    {
        [mWaitingTimer invalidate];
        [mActivityIndicator stopAnimating];
        [mActivityIndicator removeFromSuperview];
        [mWaitingUiView removeFromSuperview];
        [mActivityIndicator release];
        mActivityIndicator = nil;
        [mWaitingUiView release];
        mWaitingUiView = nil;
        [mLabel release];
        mLabel = nil;
    }
    mWaitFlag = showFlag;
}

- (BOOL) checkNetworkStatus
{
    if ([HNUtility connectedToNetwork]) return YES;
    
    [HNUtility messageBox:@"温馨提示" msg:@"当前网络异常，请检查网络后再试"];
    return NO;
}

- (void) waitDialogCallback
{
    if (![self checkNetworkStatus])
    {
        [self alertWaitDialog:NO Title:nil];
    }
}

@end
