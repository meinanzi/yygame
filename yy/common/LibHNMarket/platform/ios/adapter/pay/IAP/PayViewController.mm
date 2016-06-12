#import "PayViewController.h"

@interface PayViewController()
{
    UIViewController*    mUIViewController;
    MultiPayDelegate*    mMultiPayDelegate;
}
- (void) close;
@end

@implementation PayViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    //self.modalPresentationStyle = UIModalPresentationFormSheet;
    //[self setModalTransitionStyle: UIModalTransitionStyleCrossDissolve]; //渐变
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_ButtonPay1 release];
    [_ButtonPay2 release];
    [_ButtonClose release];
    [super dealloc];
}

- (void)viewDidUnload {
    [self setButtonPay1:nil];
    [self setButtonPay2:nil];
    [self setButtonClose:nil];
    [super viewDidUnload];
}

#if __IPHONE_OS_VERSION_MIN_REQUIRED < 60000

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return ((interfaceOrientation == UIInterfaceOrientationLandscapeRight)
                || (interfaceOrientation == UIInterfaceOrientationLandscapeLeft));
}

#else

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return YES;
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscape;
}

#endif

- (IBAction) ButtonPay1Click:(id)sender {
    UIButton* btn = sender;
    mMultiPayDelegate->handleMultiPayStart((HN::PayModule*)btn.tag);
    [self close];
}

- (IBAction) ButtonPay2Click:(id)sender {
    UIButton* btn = sender;
    mMultiPayDelegate->handleMultiPayStart((HN::PayModule*)btn.tag);
    [self close];
}

- (IBAction)ButtonCloseClick:(id)sender {
    mMultiPayDelegate->handleMultiPayCancel();
    [self close];
}

- (void) start:(UIViewController*) viewController MultiPayDelegate:(MultiPayDelegate*)multiPayDelegate
{
    NSAssert(nil != viewController, @"viewController is nil");
    NSAssert(nil != multiPayDelegate, @"payCallBack is nil");
    
    mUIViewController = viewController;
    
    mMultiPayDelegate = multiPayDelegate;
    
    float width = mUIViewController.view.bounds.size.width;
    float height = mUIViewController.view.bounds.size.height;
//    NSString* modal = [UIDevice currentDevice].model;
    
    UIImageView *imgView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"背景"]];
    [self.view insertSubview:imgView atIndex:0];
    imgView.contentMode = UIViewContentModeCenter;
    [self.view setBackgroundColor:[UIColor clearColor]];// clearColor   blackColor
    self.view.frame = imgView.frame;//CGRectMake(0, 0, 380, 250);////

    //iPod touch
//    if ([modal isEqualToString:@"iPhone"]) {
//        self.view.frame = CGRectMake(0, 0, 500, 400);
//    } else
//    {
//        self.view.frame = CGRectMake(0, 0, 500, 400);
//    }
    self.view.center = CGPointMake(width / 2, height / 2);
    self.view.layer.cornerRadius = 8;
    //[self.view autoContentAccessingProxy ];
    [self.view setAlpha:1];
//    cocos2d::CCTouchDispatcher m_pTouchDispatcher = *new cocos2d::CCTouchDispatcher();
//    m_pTouchDispatcher.addTargetedDelegate((cocos2d::CCTouchDelegate)self,-10000,YES);
}

- (void) showPay
{
    [mUIViewController.view addSubview:self.view];
    
    //    [mUIViewController presentViewController:self animated:YES completion: nil];
}

- (void) close
{
    [self.view removeFromSuperview];
//    [self dismissViewControllerAnimated:YES completion: nil];
}

- (void) setPayPriority:(HN::PayModule*)first firstName:(NSString*)firstName
                 second:(HN::PayModule*)second secondName:(NSString*)secondName
{
    NSAssert(nil != first, @"first is null");
    if (first == nil) {
        return;
    }
    NSString* picture = @".png";
    NSString* picName1 = [firstName stringByAppendingString:picture];
   // [_ButtonPay1 setTitle:firstName forState:UIControlStateNormal];
    
    [_ButtonPay1 setBackgroundImage:[UIImage imageNamed:picName1] forState:UIControlStateNormal];
    _ButtonPay1.tag = (long)first;
    NSAssert(nil != second, @"second is null");
    if (second == nil) {
        return;
    }
    NSString* picName2 = [secondName stringByAppendingString:picture];
    [_ButtonPay2 setBackgroundImage:[UIImage imageNamed:picName2] forState:UIControlStateNormal];
   // [_ButtonPay2 setTitle:secondName forState:UIControlStateNormal];
    _ButtonPay2.tag = (long)second;
}

@end

