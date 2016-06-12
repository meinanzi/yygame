#import "paycb.h"
#import "oc-marco.h"

@interface IapAdapter_IMPL : PayBase
{

}

@property (strong, nonatomic) NSDictionary* ProcductDic;

SYNTHESIZE_SINGLETON_FOR_DESCRIPTION(IapAdapter_IMPL)

- (void) setVerify:(bool)verify;

- (void) restore;

@end
