/*
 * FileName : CCUMScoialSDK.h
 * Author   : hehonghui
 * Company  : umeng.com
 *
 */

#ifndef __CCUMSOCIALSDK__
#define __CCUMSOCIALSDK__

#include "cocos2d.h"
#include "CCUMTypeDef.h"
#include <vector>

USING_NS_CC;
using namespace std;

namespace umeng {
/**
 * Umeng Social命名空间, 使用该SDK必须加上USING_NS_UM_SOCIAL
 */
namespace social {
/**
 * 该类为友盟社会化组件的Cocos2d-x版本，在友盟社会化组件Android、iOS原生SDK上进行适配，支持打开分享面板分享、直接分享、授权某平台、删除某平台授权、判断某平台是否授权这几个核心功能，
 * 	如果有进一步的需求请参考本SDK进行实现。
 *
 * Android 平台实现的代码参考UmengSocial/Android目录下的实现, IOS平台的参考UmengSocial/IOS目录下的实现。
 *
 */
class CCUMSocialSDK {
private:
	CCUMSocialSDK(const char* umAppKey);
	void initSDK();
public:

	/**
	 * 构建CCUMSocialSDK对象
	 * @param appKey 友盟app key
	 */
	static CCUMSocialSDK* create(const char* appKey);

	/*
	 * 设置友盟app key
	 * @param appKey 友盟app key
	 */
	void setAppKey(const char* appkey);

	/**
	 * 对某平台进行授权
	 * @param platfrom 要授权的平台, 参考CCUMTypeDef.h中的Platform枚举定义
	 * @param  callback 授权回调, 具体参考CCUMTypeDef.h中的定义
	 */
	void authorize(int platform, AuthEventHandler callback);

	/**
	 * 删除某平台的授权信息
	 * @param platform 要删除授权的平台
	 * @param   callback 删除授权的回调, 具体参考CCUMTypeDef.h中的定义
	 */
	void deleteAuthorization(int platform, AuthEventHandler callback);

	/**
	 * 判断某平台是否已经授权
	 *@param    platform 判断某平台是否已经授权
	 */
	bool isAuthorized(int platform);

	/**
	 * 设置SDK中的所有平台
	 *@param    platforms SDK中包含的所有平台
	 */
	void setPlatforms(vector<int>* platforms);

	/**
	 * 返回SDK中设置的所有平台
	 * @return SDK中设置的所有平台
	 */
	vector<int>* getPlatforms();

	/**
	 * 打开友盟分享面板, 在调用该方法前请先调用setPlatforms函数来设置分享SDK支持的所有平台.
	 * @param text 要分享的文字内容
	 * @param imgName
	 // *******************************************
	 要分享的图片支持url图片、assets目录下的图片、资源图片和存放在sd卡目录下的图片。这四种图片对于前缀都有一定的要求，要求如下:
	 //
	 1、url图片必须以"http://"或者"https://"开头,例如 : http://www.umeng.com/images/pic/home/feedback/banner.png；
	 2、assets目录下的图片必须以"assets/"开头,cocos2d-x的资源图片默认会添加到该目录只, 例如 : assets/CloseNornal.png;
	 3、资源图片即放在工程中的res/drawable中的图片, 必须以"res/"开头，例如 : res/myimage.png;
	 4、sd卡目录下的图片即存放在本地目录的图片，此时传递绝对路径即可，例如 : /sdcard/myimage.jpg;
	 // *******************************************
	 * @param callback 分享回调,具体参考CCUMTypeDef.h中的定义
	 */
	void openShare(const char* text, const char* imgName,
			ShareEventHandler callback);

	/**
	 * 直接分享到某个平台，不打开分享面板和内容编辑页面
	 * @param platform 要分享到的目标平台，参考CCUMTypeDef.h中的Platform枚举定义
	 * @param text 要分享的文字内容
	 * @param imgName
	 // *******************************************
	 要分享的图片支持url图片、assets目录下的图片、资源图片和存放在sd卡目录下的图片。这四种图片对于前缀都有一定的要求，要求如下:
	 //
	 1、url图片必须以"http://"或者"https://"开头,例如 : http://www.umeng.com/images/pic/home/feedback/banner.png；
	 2、assets目录下的图片必须以"assets/"开头,cocos2d-x的资源图片默认会添加到该目录只, 例如 : assets/CloseNornal.png;
	 3、资源图片即放在工程中的res/drawable中的图片, 必须以"res/"开头，例如 : res/myimage.png;
	 4、sd卡目录下的图片即存放在本地目录的图片，此时传递绝对路径即可，例如 : /sdcard/myimage.jpg;
	 // *******************************************
	 * @param callback 分享回调，具体参考CCUMTypeDef.h中的定义
	 */
	void directShare(int platform, const char* text, const char* imgName,
			ShareEventHandler callback);

	/**
	 * 设置SDK的target url, 即用户点击某条分享时跳转到的目标页面
	 * @param url 用户点击某条分享时跳转到的目标页面通常为app的主页或者下载链接等
	 */
	void setTargetUrl(const char* targetUrl);

	/**
	 * 设置QQ和QQ空间的app id, 两个平台共用套app id, appKey
	 *
	 * @param appid	QQ空间或者QQ的app id
	 * @param appKey QQ空间或者QQ的app key
	 */
	void setQQAppIdAndAppKey(const char* appid, const char* appkey);

	/**
	 * 设置微信和微信朋友圈的app id
	 *
	 * @param appid 微信或者微信朋友圈的appid
	 * @param appsecret 微信或者微信朋友圈的appsecret
	 */
	void setWeiXinAppInfo(const char* appid, const char* appsecret);

	/**
	 * 设置人人网的app id,app key,app secret信息
	 *
	 * @param appid 人人网的appid
	 * @param appKey 人人网的app key
	 * @param appsecret 人人网的appsecret
	 */
	void setRenrenAppInfo(const char* appid, const char* appkey,
			const char* appsecret);

	/**
	 * 设置易信和易信朋友圈的app id
	 *
	 * @param appid 易信或者易信朋友圈的app id
	 */
	void setYiXinAppKey(const char* appid);

	/**
	 * 设置来往和来往动态的app id
	 *
	 * @param appid	来往的app id
	 * @param appKey  来往的app key
	 * @param appid	应用名, 一般为你的应用名
	 */
	void setLaiwangAppInfo(const char* appid, const char* appkey,
			const char* appName);

	/**
	 * 设置Facebook的appid
	 *
	 * @param appid   Facebook应用的app id
	 */
	void setFacebookAppId(const char *appid);

	/** Deprecated，使用'setPlatforms'方法传入相应平台参数进行控制
	 * 打开Twitter开关，只对iOS平台生效
	 *
	 */
	void openTwitterForiOS();

	/** Deprecated，使用'setPlatforms'方法传入相应平台参数进行控制
	 * 打开Instagram开关，只对iOS平台生效
	 *
	 */
	void openInstagramForiOS();

	/**
	 *  是否开启log
	 * @param flag 如果为true则开启log, 否则关闭.
	 */
	void setLogEnable(bool flag);

	/**
	 * 分平台设置分享内容
	 * @param platform 平台的整形枚举
	 * @param text 该平台分享内容的文本
	 * @param imagePath 该平台分享内容的图片url链接、或者本地路径，或者资源名或者是asset中的图片.具体参考setShareImageName方法
	 * @param title 分享时的标题, 默认为空字符串
	 * @param targetUrl 分享消息被点击时跳转到的目标url ( 不是所有平台都支持此项功能 ) , 默认为空字符串
	 */
	void setPlatformShareContent(int platform, const char* text,
			const char* imagePath, const char* title = NULL,
			const char* targetUrl = NULL);

	/**
	 * 设置平台的sso授权，目前支持的平台有新浪微博、人人网、腾讯微博三个平台. 在设置SSO时请确保您在友盟官方绑定了这些平台的app id, app key等信息.iOS仅支持新浪微博、腾讯微博，不支持人人网平台。
	 * 且人人网的app id, app key必须在本地先通过setRenrenAppInfo来设置.
	 * @param  platform 要支持SSO授权的平台
     * @param redirectURL 该授权平台的回调URL，该URL需要和开放平台应用管理上设置的回调URL一致
	 */
	void setSsoAuthorization(int platform, const char *redirectURL);

private:
	static CCUMSocialSDK *_instance;
	vector<int>* mPlatforms;
	char* _wrapperType;
	char* _wrapperVersion;

};
}
}

#endif
