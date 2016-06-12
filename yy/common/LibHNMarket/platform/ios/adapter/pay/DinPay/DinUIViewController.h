//
//  DinUIViewController.h
//  LibHNMarket
//
//  Created by hn78 on 17/9/15.
//  Copyright (c) 2015年 redbird. All rights reserved.
//

#ifndef LibHNMarket_DinUIViewController_h
#define LibHNMarket_DinUIViewController_h

#include <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "HNDinPayModule.h"
#import "cocos2d.h"
#import "HNMarket.h"

@interface DinViewController : UIViewController <DinPayPluginDelegate>
{
    bool _supportPortrait;
}

- (void)setSupportPortrait:(bool)support;

@property (nonatomic)CALLBACK_PRAGMA _callback;

@end


#endif
