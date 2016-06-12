#import "UIViewController+PotraitViewController.h"


@implementation UIViewController (PotraitViewController)

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskAll;
}

@end
