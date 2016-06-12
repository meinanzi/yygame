//
//  DinUIViewController.m
//  LibHNMarket
//
//  Created by hn78 on 17/9/15.
//  Copyright (c) 2015年 redbird. All rights reserved.
//

#import "DinUIViewController.h"

@implementation DinViewController

- (id)getAppController {
    id controller = [[UIApplication sharedApplication] delegate];
    return controller;
}

- (void)DinPayPluginResult:(NSString *)result
{
    NSString* str = nil;
    if ([result isEqualToString:@"success"]) {
        str = @"支付成功";
        [self HandlePayResult: true];
    }
    if ([result isEqualToString:@"cancel"]) {
        str = @"用户取消支付";
        [self HandlePayResult: false]; //for test
    }
    if ([result isEqualToString:@"fail"]) {
        str = @"支付失败";
        [self HandlePayResult: false];
    }
    UIAlertView* alertview = [[UIAlertView alloc]initWithTitle:@"提示" message:str delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
    [alertview show];
    
    [[self getAppController] setSupportPortrait:false];
}


- (void)HandlePayResult:(bool)success
{
    NSString *result = [NSString stringWithFormat:@"{\"status\":%d,\"platform\":%d}",success,1];
    HN::Market::sharedMarket()->responseChannel(&__callback, result.UTF8String);
}


@end

