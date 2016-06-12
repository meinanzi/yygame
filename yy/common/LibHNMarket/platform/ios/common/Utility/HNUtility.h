#ifndef __HNUtility_h__
#define __HNUtility_h__

#include <UIKit/UIKit.h>

#include <Foundation/NSString.h>
#include <Foundation/NSException.h>
#import <MessageUI/MFMailComposeViewController.h>

@interface HNUtility : NSObject

// UUID 字符串
+ (NSString*) createUUIDString;

/**
 *  获取build版本
 *
 *  @return NSString
 */
+ (NSString*) getBuildVersion;

//获取横竖屏配置
+ (NSString*) getOrientation;

// 获取版本
+ (NSString*) getVersion;

// 获取系统版本
+ (NSString*) getOSVersion;

// 获取设备类型
+ (NSString*) getDevice;

// 获取IMEI
+ (NSString*) getIMEI;

// 获取游戏ID
+ (NSString*) getGameID;

// 获取渠道ID
+ (NSString*) getChannelID;

//联网判断
+ (bool) connectedToNetwork;

// 获取AppID
+ (NSString*) getAppID;

// base64加密
+ (NSString*) encodeBase64:(NSString*)text;

// 获取字符串哈希码
+ (unsigned int) hashCode:(NSString*) hashText;

+ (NSString*) getBundleIdentifier;

+ (void) messageBox:(NSString*)title msg:(NSString*)message;

+ (NSString*) getIDFA;

+ (NSString*) getMac;

+ (NSString*) getIPAdress;

+ (NSString*) getUDID;

+ (void) dealPhone:(NSString*)number;

+ (void) dealMsg:(NSString*)number;

+ (void) sendEmail:(NSString*)to text:(NSString*)text;

@end

#endif // __HNUtility_h__