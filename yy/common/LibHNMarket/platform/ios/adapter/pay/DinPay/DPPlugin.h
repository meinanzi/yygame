//
//  DPPlugin.h
//  UnionPayTest
//
//  Created by yangliang on 14-3-29.
//  Copyright (c) 2014年 yangliang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "DinPayPluginDelegate.h"

@interface Order : NSObject

//商家号，商户签约时，智付支付平台分配的唯一商家号，必选参数
@property(nonatomic, strong)NSString* merchant;

//接口版本，固定值：V3.0，必选参数
@property(nonatomic, strong)NSString* interface_version;

//默认值：MD5，目前仅支持MD5，不参与签名，必选参数
@property(nonatomic, strong)NSString* sign_type;

//服务器异步通知地址 ,支付成功后，智付支付平台会主动通知商家系统，商家系统必须指定接收通知的地址，必选参数
@property(nonatomic, strong)NSString* notify_url;

//商户网站唯一订单号,由商户系统保证唯一性，最长64位字母、数字组成，必选参数
@property(nonatomic, strong)NSString* order_no;

//商户订单时间, 格式：yyyy-MM-dd HH:mm:ss，必选参数
@property(nonatomic, strong)NSString* order_time;

//商户订单总金额,该笔订单的总金额，以元为单位，精确到小数点后两位，必选参数
@property(nonatomic, strong)NSString* oreder_amount;

//商品名称，不超过100个字符，必选参数
@property(nonatomic, strong)NSString* product_name;

//商品编号，不超过60个字符，可选参数
@property(nonatomic, strong)NSString* product_code;

//商品数量，可选参数
@property(nonatomic, strong)NSString* product_num;

//商品描述,不超过300个字符，可选参数
@property(nonatomic, strong)NSString* product_desc;

//如果支付请求时传递该参数，则通知商户支付成功时回传该参数，可选参数
@property(nonatomic, strong)NSString* extra_return_param;

//签名数据, 以上所有非空参数经MD5签名后的数据，必选参数
@property(nonatomic, strong)NSString* sign;

//1 订单号不允许重复  0 订单号允许重复,可选参数
@property(nonatomic, strong)NSString* redo_flag;

/**
 *  把需要签名的参数排序后输出
 *
 *  @return 排序后的签名参数
 */
-(NSMutableString*)getSortStrFromOrder;

/**
 *  md5签名
 *
 *  @param inPutText 需要签名的参数
 *
 *  @return 签名后的字符串
 */
+(NSString *) md5: (NSString *) inPutText;
@end

@interface DPPlugin : NSObject


/**
 *  启动智付的支付插件
 *
 *  @param order          传入的order对象
 *  @param mode           接入模式，“00”代表正式环境，“01”代表测试环境
 *  @param viewController 当前的viewCotroller
 *  @param delegate       DinPayPluginDelegate
 */
+ (void)startDinPay:(Order*)order
               mode:(NSString*)mode
     viewController:(UIViewController*)viewController
           delegate:(id<DinPayPluginDelegate>)delegate;

@end
