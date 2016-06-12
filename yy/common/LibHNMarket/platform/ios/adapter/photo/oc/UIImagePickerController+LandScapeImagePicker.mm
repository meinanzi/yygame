#import "UIImagePickerController+LandScapeImagePicker.h"


@implementation UIImagePickerController (LandScapeImagePicker)

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskLandscape;
}

@end




