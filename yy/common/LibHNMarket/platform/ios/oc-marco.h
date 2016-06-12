#ifndef _OC_MARRO_H__
#define _OC_MARRO_H__

#define OC_SAFE_RELEASE(object)     \
{                               \
    if (object != nil)          \
{                           \
    [object release];               \
    object = nil;               \
}                               \
}


#define SYNTHESIZE_SINGLETON_FOR_DESCRIPTION(classname)   \
+ (classname *)shared##classname;


#define SYNTHESIZE_SINGLETON_FOR_IMPL(classname)   \
\
static classname *shared##classname = nil;          \
+ (classname *)shared##classname                        \
{                                                                    \
@synchronized(self)                                                     \
{                                                                           \
if (shared##classname == nil)                                                   \
{\
shared##classname = [[self alloc] init];                \
}\
}           \
return shared##classname;\
}\
\
+ (id)allocWithZone:(NSZone *)zone\
{\
@synchronized(self)\
{\
if (shared##classname == nil)\
{\
shared##classname = [super allocWithZone:zone];\
return shared##classname;\
}\
}\
return nil;\
}\
\
- (id)copyWithZone:(NSZone *)zone\
{                                           \
return self;                             \
}                                         \
\
- (id)retain                            \
{                                       \
return self;                        \
}                                       \
\
- (NSUInteger)retainCount               \
{                                       \
return NSUIntegerMax;               \
}                                       \
\
- (oneway void)release                         \
{                                       \
\
}                                       \
\
- (id)autorelease               \
{                               \
return self;                \
}

#define stringToNSString(args)   [NSString stringWithUTF8String:args.c_str()]
#define NSStringTostring(args)   [args UTF8String]


#ifdef DEBUG
#define HNNSLog(...) NSLog(__VA_ARGS__)
#else
#define HNNSLog(...) {}
#endif

#endif  //_OC_MARCO_H__

