//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#import "HNAliPay_IMPL.h"
#include <UIKit/UIKit.h>
#import <AlipaySDK/AlipaySDK.h>
#import "Util/DataSigner.h"
#import "Order.h"

@interface HNAliPay_IMPL()
{
    NSString *_appID;
    NSString *_privateKey;
    NSString *_handleUrl;
    HNPayDelegate *_delegate;
}
@end

@implementation HNAliPay_IMPL

- (void)dealloc
{
    [super dealloc];
    [_appID release];
    [_privateKey release];
    [_handleUrl release];
    _appID = nil;
    _privateKey = nil;
    _handleUrl = nil;
}

- (id)initWithAppID:(NSString *)appID
         PrivateKey:(NSString *)privateKey
           Delegate:(HNPayDelegate *)delegate
          HandleURL:(NSString *)handleUrl {
    if (self = [super init]) {
        _appID = [appID retain];
        _privateKey = [privateKey retain];
        _handleUrl = [handleUrl retain];
        _delegate = delegate;
    }
    return self;
}

- (void)pay:(NSString *)orderID Price:(float)price Description:(NSString *)description {
    /*
     *商户的唯一的parnter和seller。
     *签约后，支付宝会为每个商户分配一个唯一的 parnter 和 seller。
     *生成订单信息及签名
     */
    //将商品信息赋予AlixPayOrder的成员变量
    Order_Alipay *order = [[[Order_Alipay alloc] init] autorelease];
    order.partner = _appID;
    order.seller = _appID;
    if (orderID.length == 0)
    {
        //客户端生成订单
        order.tradeNO = [self generateTradeNO];
    }
    else
    {
        //服务端生成订单
        order.tradeNO = orderID;
    }
    order.notifyURL = _handleUrl;//回调地址
    order.productName = description; //商品标题
    order.productDescription = description; //商品描述
    order.amount = [NSString stringWithFormat:@"%.2f",price]; //商品价格
    
    order.service = @"mobile.securitypay.pay";
    order.paymentType = @"1";
    order.inputCharset = @"utf-8";
    order.itBPay = @"30m";
    order.showUrl = @"m.alipay.com";
    
    //应用注册scheme,在AlixPayDemo-Info.plist定义URL types
    NSString *appScheme = @"RedBirdAlipay";
    
    //将商品信息拼接成字符串
    NSString *orderSpec = [order description];
    NSLog(@"orderSpec = %@",orderSpec);
    
    //获取私钥并将商户信息签名,外部商户可以根据情况存放私钥和签名,只需要遵循RSA签名规范,并将签名字符串base64编码和UrlEncode
    //id<DataSigner> signer = [RSACreater CreateRSADataSigner:_privateKey];
    id<DataSigner> signer = CreateRSADataSigner(_privateKey);
    NSString *signedString = [signer signString:orderSpec];
    
    //将签名成功字符串格式化为订单字符串,请严格按照该格式
    NSString *orderString = nil;
    if (signedString != nil) {
        orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                       orderSpec, signedString, @"RSA"];
        
        [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
            NSString* resultCode = [resultDic objectForKey:@"resultStatus"];
            bool status = false;
            if ([resultCode isEqualToString:@"9000"])
            {
                status = true;
            }
            _delegate->HandlePayResult(status, 2);
        }];
    }
}

- (NSString *)generateTradeNO
{
    NSDateFormatter *formatter = [[[NSDateFormatter alloc] init] autorelease];
    [formatter setDateFormat:@"MMddHHmmss"];
    //时间戳
    NSString *sourceStr = [formatter stringFromDate:[NSDate date]];
    //15位订单号
    NSString *result = [NSString stringWithFormat:@"%@%d",sourceStr,rand() + 10000];
    
    return result;
}

- (bool)handleURL:(NSURL *)url
{
    if ([url.host isEqualToString:@"safepay"]) {
        //支付结果url，传入后由SDK解析，统一在支付时的pay方法的callback中回调
        [[AlipaySDK defaultService] processOrderWithPaymentResult:url standbyCallback:^(NSDictionary *resultDic) {}];
        return true;
    }
    return false;
}
@end
