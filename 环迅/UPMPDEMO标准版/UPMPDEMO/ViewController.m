//
//  ViewController.m
//  UPMPDEMO
//
//  Created by IH1246 on 15/12/1.
//  Copyright © 2015年 IH1246. All rights reserved.
//

#import "ViewController.h"
#import <IPSUPMPSDK/IPSUpmpViewController.h>

@interface ViewController ()<IPSUPMPDelegate,UIAlertViewDelegate>
{
    NSString* md5Cert;//md5证书
    NSString* desKey;//des加密的key
    NSString* desIv;//des加密的向量
}
- (IBAction)click:(UIButton *)sender;
@property (strong, nonatomic) IBOutlet UITextField *merCodeTextFile;
@property (strong, nonatomic) IBOutlet UITextField *accCodeTextFile;
@property (strong, nonatomic) IBOutlet UITextField *merBillNOTextFile;
@property (strong, nonatomic) IBOutlet UITextField *tranAmtTextFile;
@property (strong, nonatomic) IBOutlet UITextField *ordPerValTextFile;
@property (strong, nonatomic) IBOutlet UITextField *orderDescTextFile;
@property (strong, nonatomic) IBOutlet UITextField *noticeURLtextField;
@property (strong, nonatomic) IBOutlet UITextField *bankCardTextFiled;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    md5Cert = @"Wnhf4JAaGCR0z5tTLPJSE088YjWbdR8kgrAGtL51aH8JHJXoyZS7NgOtGcuUTzzLru9ddw1CnmJjCQTYmBPrQ5475AoKU610lPKDfR84TTCjfP8vnbr7eUuPNMkEdHA2";//按商户号来修改md5证书
    desKey = @"5oj9zL0QpBr7jcSrgTAORLwk";//设置des加密的key
    desIv = @"A4QkDknv";//设置des加密的向量
    
    self.merCodeTextFile.text= @"118506";
    self.accCodeTextFile.text=@"1185060017";
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    NSDate * date = [NSDate date];
    NSDateFormatter * dateformatter = [[NSDateFormatter alloc] init];
    dateformatter.dateFormat = @"yyyyMMddHHmmss";
    NSString* dateStr = [NSString stringWithFormat:@"%@", [dateformatter stringFromDate:date]];
    self.merBillNOTextFile.text = [NSString stringWithFormat:@"Mer%@",dateStr];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)click:(UIButton *)sender {

    NSDate * date = [NSDate date];
    NSDateFormatter * dateformatter = [[NSDateFormatter alloc] init];
    dateformatter.dateFormat = @"yyyyMMddHHmmss";
    NSString* dateStr = [NSString stringWithFormat:@"%@", [dateformatter stringFromDate:date]];
    
    self.merCode = self.merCodeTextFile.text;
    self.accCode = self.accCodeTextFile.text;
    self.merBillNo =self.merBillNOTextFile.text;
    self.tranAmt = self.tranAmtTextFile.text;
    self.ordPerVal = self.ordPerValTextFile.text;
    self.requestTime = dateStr;
    self.orderDesc = self.orderDescTextFile.text;
    self.merNoticeUrl =self.noticeURLtextField.text;
    self.bankCard = self.bankCardTextFiled.text;
    
    if (self.merCode.length == 0||self.accCode.length == 0 ||self.merBillNo.length == 0 || self.tranAmt.length == 0|| self.orderDesc == 0|| self.merNoticeUrl.length == 0 ) {//判断参数是不是填满，请自行改写
        UIAlertView* alertView = [[UIAlertView alloc]initWithTitle:@"提示" message:@"请填满参数！" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alertView show];

    }else{
    
        NSString* bankCardStr = [IPSUpmpViewController TripleDES:self.bankCard encryptOrDecrypt:kCCEncrypt key:desKey desIv:desIv];
        
        NSMutableDictionary* merRequestDict = [[NSMutableDictionary alloc]init];
        [merRequestDict setValue:self.accCode forKey:@"accCode"];
        [merRequestDict setValue:self.merBillNo forKey:@"merBillNo"];
        [merRequestDict setValue:@"156" forKey:@"ccyCode"];
        [merRequestDict setValue:@"2301" forKey:@"prdCode"];
        [merRequestDict setValue:self.tranAmt forKey:@"tranAmt"];
        [merRequestDict setValue:self.requestTime forKey:@"requestTime"];
        [merRequestDict setValue:self.ordPerVal forKey:@"ordPerVal"];
        [merRequestDict setValue:self.merNoticeUrl forKey:@"merNoticeUrl"];
        [merRequestDict setValue:self.orderDesc forKey:@"orderDesc"];
        [merRequestDict setValue:bankCardStr forKey:@"bankCard"];
        
        NSLog(@"merRequestDict:%@",merRequestDict);
        NSError *error;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:merRequestDict options:NSJSONWritingPrettyPrinted error:&error];
        NSString *requestJason =[[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        NSString* merRequestDes = [IPSUpmpViewController TripleDES:requestJason encryptOrDecrypt:kCCEncrypt key:desKey desIv:desIv];
        NSLog(@"加密数据:%@",merRequestDes);
        
        NSString* merRequest = [NSString stringWithFormat:@"%@%@%@",self.merCode,merRequestDes,md5Cert];
        NSLog(@"md5字段：%@",merRequest);
        
        NSString* merRequestMd5 = [IPSUpmpViewController md5:merRequest];
        NSLog(@"md5:%@",merRequestMd5);
        
        
        [IPSUpmpViewController IPSStartPayWithMerCode:self.merCode sign:merRequestMd5 merRequestInfo:merRequestDes delegate:self viewController:self];
    
    }
    
    
    
}
@end
