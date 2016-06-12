//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved



#include "../HNPhotoDelegate.h"
#import "UIKit/UIKit.h"

@interface HNPhotoAdapter_IMPL : NSObject<UIActionSheetDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    bool _supportPortrait;
}

- (void)setSupportPortrait:(bool)support;

-(id)initWithViewController:(UIViewController *)viewController Delegate:(HNPhotoDelegate *)delegate;

-(void)getPhoto:(int)x :(int)y;
@end
