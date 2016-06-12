#import "HNUtility.h"
#import "OpenUDID.h"
#import <SystemConfiguration/SCNetworkReachability.H>
#import <netinet/in.h>
#import <CoreFoundation/CFUUID.h>
//#import <AdSupport/ASIdentifierManager.h>
#import <unistd.h>
#import <sys/socket.h>
#import <sys/sysctl.h>
#import <net/if.h>
#import <net/if_dl.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <sys/ioctl.h>
#import <sys/types.h>
#import <sys/socket.h>
#import <sys/sockio.h>
#import <netdb.h>
#import <arpa/inet.h>
#import <net/if.h>
#import <errno.h>
#import <net/if_dl.h>

#define MAXADDRS     128
#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

int char2Int(char a)
{
	if (a >= '0' && a <= '9') {
		return a - '0';
	}
	else if (a >= 'a' && a <= 'f') {
		return a - 'a' + 10;
	}
	else if (a >= 'A' && a <= 'F') {
		return a - 'A' + 10;
	}
	else {
		return 0;
	}
}

const char *base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char* Base64Encode(const char* input, unsigned long inplen)
{
	int a = 0, b = 0, c = 0;
	int d, e, f, g;
	int outlen = 4;
	static char* output = NULL;
	for (int i = 0; i < inplen; i += 3)
	{
		a = input[i];
		b= i + 1 < inplen ? input[i+1]:0;
		c= i + 2 < inplen ? input[i+2]:0;
		
		d = base64_alphabet[a >> 2];
		e = base64_alphabet[((a & 3) << 4) | (b >> 4)];
		f = base64_alphabet[((b & 15) << 2) | (c >> 6)];
		g = base64_alphabet[c & 63];
		if (i + 1 >= inplen) f = '=';
		if (i + 2 >= inplen) g = '=';
		output = (char*)realloc(output, outlen);
		output[outlen-4] = d;
		output[outlen-3] = e;
		output[outlen-2] = f;
		output[outlen-1] = g;
		outlen += 4;
	}
	output = (char*)realloc(output, outlen-3);
	output[outlen-4] = '\0';
	return output;
	/* this is a statically allocated string.  Make sure and dup it
	 * if you need to save it between calls.  This entire algorithm
	 * seems memory-kludy though */
}

unsigned int HashCode(const char *str)
{
    //BKDRHash
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    
    return (hash & 0x7FFFFFFF);
}

bool GetMacAddress(char * pMacBuffer)
{
    assert(pMacBuffer != NULL);
    if (pMacBuffer == NULL)
    {
        return false;
    }
    
    int                  mib[6];
    size_t               len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0)
    {
        printf("Error: if_nametoindex error/n");
        return false;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        printf("Error: sysctl, take 1/n");
        return false;
    }
    
    if ((buf = (char*)malloc(len)) == NULL)
    {
        printf("Could not allocate memory. error!/n");
        return false;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        printf("Error: sysctl, take 2");
        return false;
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    
    // NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    sprintf(pMacBuffer, "%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
    free(buf);
    return true;
}

#define BUFFERSIZE  4000

char *if_names[MAXADDRS];
char *ip_names[MAXADDRS];
char *hw_addrs[MAXADDRS];
unsigned long ip_addrs[MAXADDRS];

static int   nextAddr = 0;

void InitAddresses()
{
    int i;
    for (i=0; i<MAXADDRS; ++i)
    {
        if_names[i] = ip_names[i] = hw_addrs[i] = NULL;
        ip_addrs[i] = 0;
    }
}

void FreeAddresses()
{
    int i;
    for (i=0; i<MAXADDRS; ++i)
    {
        if (if_names[i] != 0) free(if_names[i]);
        if (ip_names[i] != 0) free(ip_names[i]);
        if (hw_addrs[i] != 0) free(hw_addrs[i]);
        ip_addrs[i] = 0;
    }
    InitAddresses();
}

void GetIPAddresses()
{
    int                 i, len, flags;
    char                buffer[BUFFERSIZE], *ptr, lastname[IFNAMSIZ], *cptr;
    struct ifconf       ifc;
    struct ifreq        *ifr, ifrcopy;
    struct sockaddr_in  *sin;
    
    char temp[80];
    
    int sockfd;
    
    for (i = 0; i < MAXADDRS; ++i)
    {
        if_names[i] = ip_names[i] = NULL;
        ip_addrs[i] = 0;
    }
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        return;
    }
    
    ifc.ifc_len = BUFFERSIZE;
    ifc.ifc_buf = buffer;
    
    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl error");
        return;
    }
    
    lastname[0] = 0;
    
    for (ptr = buffer; ptr < buffer + ifc.ifc_len; )
    {
        ifr = (struct ifreq *)ptr;
        len = max(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
        ptr += sizeof(ifr->ifr_name) + len;  // for next one in buffer
        
        if (ifr->ifr_addr.sa_family != AF_INET)
        {
            continue;   // ignore if not desired address family
        }
        
        if ((cptr = (char *)strchr(ifr->ifr_name, ':')) != NULL)
        {
            *cptr = 0;      // replace colon will null
        }
        
        if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0)
        {
            continue;   /* already processed this interface */
        }
        
        memcpy(lastname, ifr->ifr_name, IFNAMSIZ);
        
        ifrcopy = *ifr;
        ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
        flags = ifrcopy.ifr_flags;
        if ((flags & IFF_UP) == 0)
        {
            continue;   // ignore if interface not up
        }
        
        if_names[nextAddr] = (char *)malloc(strlen(ifr->ifr_name)+1);
        if (if_names[nextAddr] == NULL)
        {
            return;
        }
        strcpy(if_names[nextAddr], ifr->ifr_name);
        
        sin = (struct sockaddr_in *)&ifr->ifr_addr;
        strcpy(temp, inet_ntoa(sin->sin_addr));
        
        ip_names[nextAddr] = (char *)malloc(strlen(temp)+1);
        if (ip_names[nextAddr] == NULL)
        {
            return;
        }
        strcpy(ip_names[nextAddr], temp);
        
        ip_addrs[nextAddr] = sin->sin_addr.s_addr;
        
        ++nextAddr;
    }
    
    close(sockfd);
}

@implementation HNUtility

+ (NSString*) encodeBase64:(NSString*)text
{
    unsigned long len = [text length];
	const char* output = Base64Encode([text UTF8String], (int)len);
	return [NSString stringWithUTF8String:output];
}

+ (unsigned int) hashCode:(NSString*) hashText
{
    return HashCode([hashText UTF8String]);
}

+ (NSString*) createUUIDString
{
    CFUUIDRef puuid = CFUUIDCreate(nil);
    CFStringRef uuidString = CFUUIDCreateString(nil, puuid);
    NSString * result = (NSString *)CFStringCreateCopy(NULL, uuidString);
    CFRelease(puuid);
    CFRelease(uuidString);
    return [[result uppercaseString] autorelease];
}

// 获取版本
+ (NSString*) getVersion
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString* GameVersion = [infoDict objectForKey:@"CFBundleShortVersionString"];
    //NSString* GameVersion = [infoDict objectForKey:@"CFBundleVersion"];
    return GameVersion;
}

//获取横竖屏配置
+ (NSString*) getOrientation
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString* OrientationValue = [infoDict objectForKey:@"UIInterfaceOrientation"];
    return OrientationValue;//value: UIInterfaceOrientationPortrait or ..
    
}


// 获取Build版本
+ (NSString*) getBuildVersion
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString* GameVersion = [infoDict objectForKey:@"CFBundleVersion"];
    return GameVersion;
}

+ (NSString*) getOSVersion
{
    NSString *strSysVersion = [[UIDevice currentDevice] systemVersion];
    return strSysVersion;
}

+ (NSString*) getDevice
{
    NSString *strModel = [[UIDevice currentDevice] model];
    return strModel;
}

+ (NSString*) getIMEI
{
    UIDevice *myDevice = [UIDevice currentDevice];
    NSString *deviceModel = [myDevice model];
    if ([deviceModel isEqualToString:@"iPhone Simulator"]) {
        return @"105001234567890";
    }
    
    NSString *deviceID = [OpenUDID value];
    const char* deviceid = [deviceID UTF8String];
    //shen+ may have problem,need modify
    char szImei[16] = {0};
    int index = 0;
    for (int i = 0; i < strlen(deviceid) && index < 15; i += 2) {
        int temp = char2Int(deviceid[i]) + char2Int(deviceid[i+1]);
        if (temp >= 30) {
            szImei[index++] = temp - 30 + '0';
        }
        else if (temp >= 20) {
            szImei[index++] = temp - 20 + '0';
        }
        else if (temp >= 10) {
            szImei[index++] = temp - 10+'0';
        }
        else {
            szImei[index++] = temp + '0';
        }
    }
    return [NSString stringWithUTF8String:szImei];
}

// 获取游戏ID
+ (NSString*) getGameID;
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    
    NSMutableDictionary* _config = [infoDict objectForKey:@"hn_config"];
    
    NSAssert(_config != nil, @"In the info.plist file, hn_config is null");
    if (_config == nil) {
        return @"";
    }
    
    NSString* gameID = [_config objectForKey:@"game_id"];
    NSAssert(gameID != nil, @"In the info.plist file, the koogame_config configuration gameid is null");
    if (gameID == nil) {
        return @"";
    }
    return [gameID stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

// 获取渠道ID
+ (NSString*) getChannelID;
{
    NSDictionary *infoDict = [[NSBundle mainBundle] infoDictionary];
    
    NSMutableDictionary* _config = [infoDict objectForKey:@"hn_config"];
    
    NSAssert(_config != nil, @"In the info.plist file, hn_config is null");
    if (_config == nil) {
        return @"";
    }
    
    NSString *channelID = [_config objectForKey:@"channel_id"];
    
    NSAssert(channelID != nil, @"In the info.plist file, the koogame_config configuration channel_id is null");
    if (channelID == nil) {
        return @"";
    }
    
    return [channelID stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

+ (bool) connectedToNetwork
{
    // Create zero addy
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    // Recover reachability flags
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    bool didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags)
    {
        return false;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? true : false;
    //return YES;
}

// 获取AppID
+ (NSString*) getAppID
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    
    NSMutableDictionary* _config = [infoDict objectForKey:@"hn_config"];
    
    NSAssert(_config != nil, @"In the info.plist file, _config is null");
    if (_config == nil) {
        return @"";
    }
    
    NSString* appid = [_config objectForKey:@"app_id"];
    
    NSAssert(appid != nil, @"In the info.plist file, the koogame_config configuration appid is null");
    if (appid == nil) {
        return @"";
    }
    return [appid stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

+ (NSString*) getBundleIdentifier
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    NSString* bundleid = [infoDict objectForKey:@"CFBundleIdentifier"];
    return bundleid;
}

+ (void) messageBox:(NSString*)title msg:(NSString*)message
{
    UIAlertView *uA = [[UIAlertView alloc] initWithTitle:title
                                               message:message
                                              delegate:nil
                                     cancelButtonTitle:@"确定"
                                     otherButtonTitles:nil, nil];
    [uA show];
    [uA release];
}

+ (NSString*) getIDFA
{
    //NSString *IDFA = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    //HNNSLog(@"device IDFA: %@", IDFA);
    //return IDFA;
    return @"";
}

+ (NSString*) getMac
{
    char mac[64] = {0};
    NSString * macString = NULL;
    if (GetMacAddress(mac))
    {
        macString = [[NSString stringWithUTF8String:mac] uppercaseString];
    }
    return macString;
}

+ (NSString*) getIPAdress
{
    InitAddresses();
    GetIPAddresses();
    return [NSString stringWithFormat:@"%s", ip_names[1]];
}

+ (NSString*) getUDID
{
    NSString* deviceID = [OpenUDID value];
    return [deviceID uppercaseString];
}

+ (void) dealPhone:(NSString*)number
{
    //NSString* telNumber = [NSString stringWithFormat:@"tel://%@", number];
    //[[UIApplication sharedApplication] openURL:[NSURL URLWithString:telNumber]];
    
    UIWebView* callView = [[UIWebView alloc] init];
    NSString* telNumber = [NSString stringWithFormat:@"tel://%@", number];
    [callView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:telNumber]]];
    UIViewController* control = NULL;
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        NSArray* windows = [[UIApplication sharedApplication] windows];
        UIWindow* win = [windows objectAtIndex:0];
        UIView* ui = [[win subviews] objectAtIndex:0];
        control = (UIViewController*)[ui nextResponder];
    }
    else
    {
        control = [UIApplication sharedApplication].keyWindow.rootViewController;
    }
    [control.view addSubview:callView];
}

+ (void) dealMsg:(NSString*)number
{
    NSString* telNumber = [NSString stringWithFormat:@"sms://%@", number];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:telNumber]];
}

+ (void) sendEmail:(NSString*)to text:(NSString*)text
{
    MFMailComposeViewController *mailPicker = [[MFMailComposeViewController alloc] init];
    if(mailPicker == nil){
        /*UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: @"提示"
                                                              message: @"用户没有设置邮件账户"
                                                             delegate: nil
                                                    cancelButtonTitle: @"OK"
                                                    otherButtonTitles: nil];
        [messageBox autorelease];
        [messageBox show];*/
        return;
    }
    
    mailPicker.mailComposeDelegate = (id)[UIApplication sharedApplication].keyWindow.rootViewController;//self;
    
    //设置主题
    [mailPicker setSubject: @"[游戏问题反馈]"];
    
    //添加收件人
    NSArray *toRecipients = [NSArray arrayWithObject: to];
    [mailPicker setToRecipients: toRecipients];
    [mailPicker setMessageBody:text isHTML:NO];
    
    [[UIApplication sharedApplication].keyWindow.rootViewController  presentViewController: mailPicker animated: YES completion:nil];
    
    [mailPicker release];
}

//- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
//{
//    [self dismissViewControllerAnimated:NO completion:nil];
//}

@end
