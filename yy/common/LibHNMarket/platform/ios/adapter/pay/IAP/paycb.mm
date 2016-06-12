
#import "paycb.h"

@implementation PRODUCT_INFO
@synthesize _productId, _productPrice, _description;

- (NSString *)description
{
    NSString * str = [NSString stringWithFormat:@"productId = %@,              \
                      productPrice = %f, description = %@",
                      _productId, _productPrice, _description];
    return str;
}
@end


////////////////////////////////////////////////////////////////////////////////

@interface PayBase()
{

}
@end

@implementation PayBase

- (instancetype) init
{
    if (self = [super init])
    {
        procuctPrice = 0;
    }
    return self;
}

- (void) start:(UIViewController *) viewController appId:(NSString*)appId
        appKey:(NSString*)appKey
{
    NSAssert(false, @"subclasses should override this method!");
}

- (void) setDelegate:(PayDelegate*) delegate
{
    NSAssert(delegate != nil, @"payDelegate = nil");
    if (delegate == nil) return;

    payDelegate = delegate;
}

- (void) pay:(NSString*)productId
{
    NSAssert(false, @"subclasses should override this method!");
}

- (BOOL) isLogin
{
    return NO;
}

- (void) login
{
    
}

- (void) logout
{
    
}

@end
