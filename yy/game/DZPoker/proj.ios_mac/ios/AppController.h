#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    bool _supportPortrait;
}

@property(nonatomic, readonly) RootViewController* viewController;

- (void)setSupportPortrait:(bool)support;

-(void) changeScreen:(bool) changed;

@end

