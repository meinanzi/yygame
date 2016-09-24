//
//  IPSUpmpViewController.h
//  UPMP
//
//  Created by IH1246 on 15/10/16.
//  Copyright © 2015年 IH1246. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>
#import "GTMBase64.h"
@protocol IPSUPMPDelegate;
@interface IPSUpmpViewController : UIViewController
@property(nonatomic,strong)NSString* merCode;
@property(nonatomic,strong)NSString* merBillNo;
@property(nonatomic,strong)NSString* tranAmt;
@property(nonatomic,strong)NSString* sign;
@property(nonatomic,strong)NSString* orderDesc;
@property (weak, nonatomic) id<IPSUPMPDelegate> delegate;
@property(nonatomic,strong) UIColor * originalNavigationBarcolor;
@property(nonatomic,strong) NSString * banktn;

+(void)IPSStartPayWithMerCode:(NSString*)merCode
                         sign:(NSString*)sign
               merRequestInfo:(NSString*)merRequestInfo
                     delegate:(id<IPSUPMPDelegate>)delegate
               viewController:(UIViewController*)selfViewController;

+(NSString *) md5: (NSString *) inPutText;
+(NSString*)TripleDES:(NSString*)plainText encryptOrDecrypt:(CCOperation)encryptOrDecrypt key:(NSString*)key desIv:(NSString*)desIv;
+(NSString*)deviceVersion;
@end


@protocol IPSUPMPDelegate <NSObject>
@optional
-(void)orderCompletedStatus:(NSString*)status
                    merCode:(NSString*)merCode
                  merBillNo:(NSString*)merBillNo
                  orderDesc:(NSString*)orderDesc
                    tranAmt:(NSString*)tranAmt;

@end