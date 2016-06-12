//
//  UIViewController+PotraitViewController.h
//  LibHNMarket
//
//  Created by hn78 on 21/9/15.
//  Copyright (c) 2015年 redbird. All rights reserved.
//

#ifndef LibHNMarket_UIViewController_PotraitViewController_h
#define LibHNMarket_UIViewController_PotraitViewController_h

#import <Foundation/Foundation.h>
#import "UIKit/UIKit.h"

@interface UIViewController (PotraitViewController)

- (BOOL)shouldAutorotate;
- (NSUInteger)supportedInterfaceOrientations;

@end

#endif
