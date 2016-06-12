#ifndef __HNSDK__PayBase__
#define __HNSDK__PayBase__

//#include <UIKit/UIKit.h>
#include <UIKit/UIViewController.h>
#include <Foundation/NSString.h>
#include <Foundation/NSException.h>

#include <string>

#define createProduct(object_type, object,pointId, productId, price, description)    \
object_type* object = [[[object_type alloc] init] autorelease];    \
object._productId = productId;              \
object._pointId = pointId;              \
object._productPrice = price;                   \
object._description = description;

/*
 产品信息
 */
@interface PRODUCT_INFO : NSObject

@property (nonatomic, retain) NSString *_productId;                  /**< 商品ID */
@property (nonatomic, assign) double    _productPrice;               /**< 商品价格，两位小数 */
@property (nonatomic, retain) NSString* _description;                /**< 购买描述，可选，没有为空 */
@property (nonatomic, retain) NSString *_pointId;

@end

class PayDelegate
{
public:
    virtual void handlePayResult(int status, const std::string& ext) = 0;
    virtual void handleLoginResult(int status) = 0;
    virtual void handleLogoutResult() = 0;
};


@interface PayBase : NSObject
{
@protected
    PayDelegate*    payDelegate;
    double          procuctPrice;
}

/**
 *  Description
 *
 *  @param viewController viewController description
 *  @param appId          appId description
 *  @param appKey         appKey description
 */
- (void) start:(UIViewController *) viewController appId:(NSString*)appId
                                appKey:(NSString*)appKey;
/**
 *  Description
 *
 *  @param delegate delegate description
 */
- (void) setDelegate:(PayDelegate*) delegate;

/**
 *  Description
 *
 *  @param productId productId description
 */
- (void) pay:(NSString*)productId;

/**
 *  Description
 *
 *  @return return value description
 */
- (BOOL) isLogin;

/**
 *  Description
 */
- (void) login;

/**
 *  Description
 */
- (void) logout;

@end


#endif      // __HNSDK__PayBase__