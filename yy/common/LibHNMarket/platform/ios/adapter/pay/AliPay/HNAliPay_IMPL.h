//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#import <Foundation/Foundation.h>
#import "../HNPayDelegate.h"

@interface HNAliPay_IMPL : NSObject
- (id)initWithAppID:(NSString *)appID
         PrivateKey:(NSString *)privateKey
           Delegate:(HNPayDelegate *)delegate
          HandleURL:(NSString *)handleUrl;
- (void)pay:(NSString  *)orderID Price:(float)price Description:(NSString  *)description;
- (bool)handleURL:(NSURL *)url;
@end
