#ifndef LibHNMarket_UIImagePickerController_h
#define LibHNMarket_UIImagePickerController_h

#import <Foundation/Foundation.h>
#import "UIKit/UIKit.h"

@interface UIImagePickerController (LandScapeImagePicker)

- (BOOL)shouldAutorotate;
- (NSUInteger)supportedInterfaceOrientations;

@end

#endif
