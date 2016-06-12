//
//  DinPayPluginDelegate.h
//  UnionPayTest
//
//  Created by yangliang on 14-3-27.
//  Copyright (c) 2014年 yangliang. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol DinPayPluginDelegate <NSObject>
-(void)DinPayPluginResult:(NSString*)result;
@end




