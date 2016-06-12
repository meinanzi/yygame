package com.umeng.social;



import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;

import com.umeng.socialize.bean.CustomPlatform;
import com.umeng.socialize.bean.RequestType;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.bean.SocializeConfig;
import com.umeng.socialize.bean.SocializeEntity;
import com.umeng.socialize.bean.StatusCode;
import com.umeng.socialize.common.ResContainer;
import com.umeng.socialize.common.ResContainer.ResType;
import com.umeng.socialize.common.SocializeConstants;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.controller.listener.SocializeListeners.SnsPostListener;
import com.umeng.socialize.controller.listener.SocializeListeners.SocializeClientListener;
import com.umeng.socialize.controller.listener.SocializeListeners.UMAuthListener;
import com.umeng.socialize.exception.SocializeException;
import com.umeng.socialize.media.BaseShareContent;
import com.umeng.socialize.media.SimpleShareContent;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.net.utils.SocializeNetUtils;
import com.umeng.socialize.sso.UMSsoHandler;
import com.umeng.socialize.utils.Log;
import com.umeng.socialize.utils.OauthHelper;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 友盟Social Android SDK的控制器, cocos2d-x sdk 通过jni代码调用相关的静态函数实现对应的功能.
 * 该sdk目前提供的功能有授权、分享(打开分享面板来分享、底层api分享)、删除授权、判断某个平台是否授权、设置文字分享内容、设置图片分享内容的功能,
 * 并且将结果回调给cocos2d-x sdk.
 * 
 * @author mrsimple
 */
public class CCUMSocialController {

    /**
     * 友盟社会化组件控制器
     */
    public static UMSocialService mController;
    /**
     * Social SDK 配置类, 单例
     */
    private static SocializeConfig mSocializeConfig = SocializeConfig
            .getSocializeConfig();
    // Cocos2dxActivity对象
    private static Cocos2dxActivity mActivity;
    /**
     * log tag
     */
    private static final String TAG = CCUMSocialController.class
            .getSimpleName();

    /**
     * 主线程的消息Handler
     */
    private static Handler mSDKHandler = new Handler(Looper.getMainLooper());;
    private static final int DELAY_MS = 50;
    private static String DESCRIPTOR;

    // ******* 以下字段的调用都在supportPlatfrom函数中 *********
    /**
     * QQ和QQ空间app id
     */
    private static String QQ_QZONE_APP_ID = "";
    /**
     * QQ和QQ空间app key
     */
    private static String QQ_QZONE_APP_KEY = "";

    /**
     * 微信或者微信朋友圈 app id
     */
    private static String WEIXIN_APP_ID = "";

    /**
     * 微信或者微信朋友圈 app secret
     */
    private static String WEIXIN_APP_SECRET = "";

    /**
     * facebook app id
     */
    private static String FACEBOOK_APP_ID = "";
    /**
     * 易信或者易信朋友圈app id, 需要添加易信或者易信朋友圈平台的支持, 请参考线上文档
     */
    private static String YIXIN_APPKEY = "";
    /**
     * 来往和来往动态的app id, 需要添加来往或者来往动态平台的支持, 请参考线上文档
     */
    private static String LAIWANG_APPID = "";
    /**
     * 来往和来往动态的app key, 需要添加来往或者来往动态平台的支持, 请参考线上文档
     */
    private static String LAIWANG_APPKEY = "";
    /**
     * 当前应用名称,用于显示在来往分享来源上
     */
    private static String LAIWANG_APP_NAME = "";

    /**
     * 人人网SSO的app id
     */
    private static String RENREN_APP_ID = "";

    /**
     * 人人网SSO的app key
     */
    private static String RENREN_APP_KEY = "";

    /**
     * 人人网SSO的app secret
     */
    private static String RENREN_APP_SECRET = "";
    /**
     * 在某些平台的分享中， 希望用户点击该分享内容跳转到的目标平台, 一般为app的官网或者下载地址
     */
    private static String TARGET_URL = "http://www.umeng.com/social";

    /**
     * 
     */
    private static volatile String mShareText = "";

    /**
     * 初始化SDK
     * 
     * @param activity 游戏的Cocos2dxActivity类型
     * @param descriptor SDK的字符串描述符
     */
    public static void initSocialSDK(Activity activity, String descriptor) {

        if (mController == null) {
            synchronized (CCUMSocialController.class) {
                if (mController == null) {
                    DESCRIPTOR = descriptor;
                    mController = UMServiceFactory.getUMSocialService(
                            DESCRIPTOR, RequestType.SOCIAL);
                    mSocializeConfig = mController.getConfig();
                }
            }
        }

        if (activity instanceof Cocos2dxActivity) {
            mActivity = (Cocos2dxActivity) activity;
        } else {
            throw new IllegalArgumentException(
                    "initSocialSDK函数的activity参数必须设置为Cocos2dxActivity类型, 且不为null. ");
        }
        // mController.getConfig().closeToast();
    }

    /**
     * 授权回调
     * 
     * @param requestCode
     * @param resultCode
     * @param data
     */
    public static void onActivityResult(int requestCode, int resultCode,
            Intent data) {
        UMSsoHandler ssoHandler = mSocializeConfig.getSsoHandler(requestCode);
        if (ssoHandler != null) {
            ssoHandler.authorizeCallBack(requestCode, resultCode, data);
        }
    }

    /**
     * 设置平台独立的分享内容，例如你想使得新浪微博和QQ两个平台的分享内容不一样，那么则需要通过这种方式实现.
     * 
     * @param platformCode
     * @param text
     * @param imagePath
     * @param title
     * @param targetUrl
     */
    public static void setPlatformShareContent(int platformCode, String text, String imagePath,
            String title, String targetUrl) {

        //
        SHARE_MEDIA platform = getPlatform(platformCode);
        //
        UMImage shareImage = parseShareImage(imagePath);

        //
        Object shareObject = ReflectUtils.newHandlerInstance(
                mShareContentClzPathMap.get(platform), null, null);

        Log.d(TAG, "平台独立的分享内容 : " + shareObject);
        //
        if (shareObject instanceof BaseShareContent) {
            BaseShareContent shareContent = (BaseShareContent) shareObject;
            // 设置文本内容
            shareContent.setShareContent(text);
            // 设置图片
            shareContent.setShareMedia(shareImage);
            // 设置title
            shareContent.setTitle(title);
            // 设置target url
            shareContent.setTargetUrl(targetUrl);
            // 设置分平台的分享内容
            mController.setShareMedia(shareContent);
        } else if (shareObject instanceof SimpleShareContent) {
            SimpleShareContent shareContent = (SimpleShareContent) shareObject;
            shareContent.setShareContent(text);
            shareContent.setShareImage(shareImage);
            mController.setShareMedia(shareContent);
        }

    }

    /**
     * 设置要分享的文字内容
     * 
     * @param text
     */
    public static void setShareContent(final String text) {
        mShareText = text;
        mController.setShareContent(mShareText);
    }

    /**
     * 设置要分享的图片路径或者url,或者资源名
     * 
     * @param imgName 图片的本地路径或者url,
     *            如果是url则必须以http://或者https://开头.assets下的图片资源需要传递assets
     *            /图片名，资源图片则需要传递res/图片名
     */
    public static void setShareImagePath(final String imgName) {
        Log.d(TAG, "#### 设置图片路径 :" + imgName);

        // 解析图片
        UMImage shareImage = parseShareImage(imgName);
        // 将图片设置为通用分享内容
        mController.setShareMedia(shareImage);

    }

    /**
     * 解析图片, 支持url图片、存在asserts下的图片、存放在sd卡路径下的图片以及资源图片。在用户传递参数时以不用的前缀传递即可.
     * 
     * @param imgName 图片的路径或者url地址
     * @return
     */
    private static UMImage parseShareImage(String imgName) {

        if (TextUtils.isEmpty(imgName)) {
            Log.e(TAG, "您传递的分享图片路径为空");
            return null;
        }
        UMImage shareImage = null;
        // 网络图片, 必须以tttp://或者https://开头
        if (SocializeNetUtils.startWithHttp(imgName)) {
            shareImage = new UMImage(mActivity, imgName);
        } else if (imgName.startsWith("assets/")) { // 在assets中的图片,
                                                    // 即Cocos2d-x中的资源图片会放在这里
            AssetManager am = mActivity.getResources().getAssets();
            String imageName = getFileName(imgName);
            InputStream is = null;
            if (!TextUtils.isEmpty(imageName)) {
                try {
                    is = am.open(imageName);
                    shareImage = new UMImage(mActivity,
                            BitmapFactory.decodeStream(is));
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (is != null) {
                        try {
                            is.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }

        } else if (imgName.startsWith("res/")) { // 资源中的图片
            String imgNameWithFormat = getFileName(imgName);
            if (!TextUtils.isEmpty(imgNameWithFormat)) {
                int index = imgNameWithFormat.indexOf(".");
                if (index > 0) {
                    String imgNameString = imgNameWithFormat
                            .substring(0, index);
                    int imgId = ResContainer.getResourceId(mActivity,
                            ResType.DRAWABLE, imgNameString);
                    shareImage = new UMImage(mActivity, imgId);
                } else {
                    Log.e(TAG, "### 请检查你传递的图片路径 : " + imgName);
                }
            }

        } else { // 本地图片,即sdcard中的图片.
            // 本地图片
            File imgFile = new File(imgName);
            if (!imgFile.exists()) {
                Log.e(TAG, "### 要分享的本地图片不存在");
            } else {
                shareImage = new UMImage(mActivity, imgFile);
            }
        }

        return shareImage;
    }

    /**
     * @param platform
     */
    public static void supportSsoAuthorization(int platform, String redirectURL) {
        if (!isPlatformValid(platform)) {
            Log.d(TAG, "### 设置SSO授权时传入的平台参数有误，请检查~");
            return;
        }
        SHARE_MEDIA share_platform = getPlatform(platform);
        if (!isPlatformConfiged(share_platform)) {
            Log.d(TAG, "### 设置SSO授权时传入的平台没有在SDK中配置，请先通过CCUMSocialSDK的setPlatforms函数添加该平台.");
            return;
        }

        if (share_platform == SHARE_MEDIA.SINA) {
            // 如果要添加新浪微博SSO授权，则需要拷贝/com/sina/sso文件件到你的src根目录下
            // mController.getConfig().setSsoHandler(new SinaSsoHandler());
            //
            addPlatformToSDK(SHARE_MEDIA.SINA, null, null, false);
            mController.getConfig().setSinaCallbackUrl(redirectURL);
            Log.d(TAG,
                    "### 设置新浪微博SSO : "
                            + mSocializeConfig.getSsoHandler(SHARE_MEDIA.SINA.getReqCode()));
        } else if (share_platform == SHARE_MEDIA.RENREN) {
            // 如果要添加人人网SSO授权，则需要拷贝以SocialSDK_renren开始两个jar包到libs目录
            // mController.getConfig().setSsoHandler(
            // new RenrenSsoHandler(mActivity, RENREN_APP_ID, RENREN_APP_KEY,
            // RENREN_APP_SECRET));
            addRenrenSso();

            Log.d(TAG,
                    "### 设置人人网 SSO : "
                            + mSocializeConfig.getSsoHandler(SHARE_MEDIA.RENREN.getReqCode()));
        }

        if (share_platform == SHARE_MEDIA.TENCENT) {
            // 添加腾讯微博SSO授权，则需要拷贝以SocialSDK_tencentWB开始两个jar包到libs目录
            // mController.getConfig().setSsoHandler(new TencentWBSsoHandler());
            //

            addPlatformToSDK(SHARE_MEDIA.TENCENT, null, null, false);
        }

    }

    /**
     * 授权某个平台
     * 
     * @param platform 授权平台的字符串表示
     */
    public static void doAuthorize(final int platform) {
        // 检测平台的有效性
        if (!isPlatformValid(platform)) {
            return;
        }

        checkActivity();

        // 在UI线程执行授权操作
        runOnMainThread(new Runnable() {

            @Override
            public void run() {
                mController.doOauthVerify(mActivity, getPlatform(platform),
                        mAuthListener);
            }
        });

    }

    /**
     * 删除平台授权
     * 
     * @param platform 授权平台的字符串表示
     */
    public static void deleteAuthorization(final int platform) {

        // 检测平台的有效性
        if (!isPlatformValid(platform)) {
            return;
        }

        checkActivity();

        // 在UI线程执行授权操作
        runOnMainThread(new Runnable() {

            @Override
            public void run() {
                // 删除授权
                mController.deleteOauth(mActivity, getPlatform(platform),
                        new SocializeClientListener() {

                            @Override
                            public void onStart() {

                            }

                            @Override
                            public void onComplete(final int status,
                                    SocializeEntity entity) {
                                // 运行在gl线程
                                runOnOpenGLThread(new Runnable() {

                                    @Override
                                    public void run() {
                                        // 删除授权的回调, 开发者可以通过字符串来判断
                                        OnAuthorizeComplete(platform, status,
                                                new String[] {
                                                    "deleteOauth"
                                                });
                                    }
                                });

                            }
                        });
            }
        });

        Log.d(TAG, "@@@@ deleteAuthorization");

    }

    /**
     * 打开分享面板
     * 
     * @param registerCallback 是否注册回调接口
     */
    public static void openShare() {
        // 注册回调接口, 默认将分享回调注册要sdk
        mController.registerListener(mSocialShareListener);

        checkActivity();

        // 在UI线程执行打开分享面板操作
        runOnMainThread(new Runnable() {
            @Override
            public void run() {
                // 打开分享面板
                mController.openShare(mActivity, false);
            }
        });

        Log.d(TAG, "@@@@ openShare");

    }

    /**
     * 直接分享到某个平台, 不打开分享面板, 直接底层分享
     * 
     * @param platform 平台对应的字符串
     */
    public static void directShare(final int platform) {

        // 检测平台的有效性
        if (!isPlatformValid(platform)) {
            return;
        }

        checkActivity();

        // 在UI线程执行底层分享操作
        runOnMainThread(new Runnable() {

            @Override
            public void run() {
                mController.directShare(mActivity, getPlatform(platform),
                        mSocialShareListener);
            }
        });

        Log.d(TAG, "@@@@ directShare");
    }

    /**
     * 判断某个平台是否授权
     * 
     * @param platform 平台的字符串表示
     * @return 返回true则表示该平台已经授权, 否则为未授权.
     */
    public static boolean isAuthorized(int platform) {
        SHARE_MEDIA target = getPlatform(platform);
        if (target != null && target != SHARE_MEDIA.GENERIC) {
            return OauthHelper.isAuthenticated(mActivity, target);
        }

        Log.d(TAG, "@@@@ isAuthorized");
        return false;

    }

    /**
     * 设置友盟的app key, 也可以通过manifest指定
     * 
     * @param text
     */
    public static void setUmengAppkey(String appkey) {
        Log.d(TAG, "#### 设置umeng appkey :" + appkey);
        if (!TextUtils.isEmpty(appkey)) {
            SocializeConstants.APPKEY = appkey;
        }
    }

    /**
     * @param fullname
     * @return
     */
    private static String getFileName(String fullname) {
        if (fullname.startsWith("assets/") || fullname.startsWith("res/")) {
            return fullname.split("/")[1];
        }
        return "";
    }

    /**
     * 设置是否开启log
     * 
     * @param flag
     */
    protected static void setAndroidLogEnable(boolean flag) {
        Log.LOG = flag;
        Log.e(TAG, "### 是否开启log : " + Log.LOG);
    }

    /**
     * 初始化Cocos2d-x SDK版本信息
     */
    protected static void initCocos2dxSDKInfo(String type, String sdkVersion) {
        Log.d(TAG, "### initCocos2dxSDKInfo, type = " + type + ", version = "
                + sdkVersion);
        // 设置cocos2dx sdk版本号
        mController.getEntity().setAdapterSDKInfo(type, sdkVersion);
    }

    /**
     * 设置用户点击某条分享时跳转到的目标页面
     * 
     * @param targetUrl
     */
    public static void setTargetUrl(String targetUrl) {
        if (!TextUtils.isEmpty(targetUrl)
                && SocializeNetUtils.startWithHttp(targetUrl)) {
            TARGET_URL = targetUrl;
        }
        Log.d(TAG, "### target url : " + TARGET_URL);
    }

    /**
     * 要支持的新平台, 可以添加的平台主要有微信,微信朋友圈,QQ,易信,来往等. 具体参考线上文档
     * http://dev.umeng.com/social/android /share/specific-integration
     * 
     * @param platform 平台的字符串表示
     * @param appkey 平台的appkey或者称为appid
     * @param targetUrl 用户点击分享内容时跳转的页面, 一般为APP主页或者下载页
     */
    public static void supportPlatfrom(SHARE_MEDIA target) {
        // 判断target
        checkActivity();

        try {
            if (target == SHARE_MEDIA.QQ) {
                // 添加QQ平台支持
                // UMQQSsoHandler qqssoSsoHandler = new
                // UMQQSsoHandler(mActivity, QQ_QZONE_APP_ID,
                // QQ_QZONE_APP_KEY);
                // qqssoSsoHandler.setTargetUrl(TARGET_URL);
                // qqssoSsoHandler.addToSocialSDK();

                addTencentPlatform(SHARE_MEDIA.QQ);

            } else if (target == SHARE_MEDIA.QZONE) {

                // 添加QQ空间平台的支持
                // QZoneSsoHandler qZoneSsoHandler = new
                // QZoneSsoHandler(mActivity,
                // QQ_QZONE_APP_ID, QQ_QZONE_APP_KEY);
                // qZoneSsoHandler.setTargetUrl(TARGET_URL);
                // qZoneSsoHandler.addToSocialSDK();

                addTencentPlatform(SHARE_MEDIA.QZONE);

            } else if (target == SHARE_MEDIA.WEIXIN) {

                // 添加微信
                // UMWXHandler wxHandler = new UMWXHandler(mActivity,
                // WEIXIN_APP_ID, WEIXIN_APP_SECRET);
                // wxHandler.setTargetUrl(TARGET_URL);
                // wxHandler.addToSocialSDK();

                addWeiXinPlatform(false);

            } else if (target == SHARE_MEDIA.WEIXIN_CIRCLE) {

                // 需要添加微信朋友圈平台
                // UMWXHandler circleHandler = new UMWXHandler(mActivity,
                // WEIXIN_APP_ID,
                // WEIXIN_APP_SECRET);
                // circleHandler.setTargetUrl(TARGET_URL);
                // // 设置为朋友圈平台
                // circleHandler.setToCircle(true);
                // circleHandler.addToSocialSDK();

                addWeiXinPlatform(true);

            } else if (target == SHARE_MEDIA.YIXIN) {
                // 创建易信的handler, 参数2为你的app id, 参数3为是否是易信朋友圈平台, false为易信,
                // true为易信朋友圈,
                // UMYXHandler yxHandler = new UMYXHandler(mActivity,
                // YIXIN_APPKEY);
                // yxHandler.setTargetUrl(TARGET_URL);
                // // 添加易信平台到SDK
                // yxHandler.addToSocialSDK();

                addYiXinPlatform(false);

            } else if (target == SHARE_MEDIA.YIXIN_CIRCLE) {
                // 创建易信的handler, 参数2为你的app id, 参数3为是否是易信朋友圈平台, false为易信,
                // true为易信朋友圈,
                // UMYXHandler yxHandler = new UMYXHandler(mActivity,
                // YIXIN_APPKEY);
                // yxHandler.setToCircle(true);
                // yxHandler.setTargetUrl(TARGET_URL);
                // // 添加易信朋友圈平台到SDK
                // yxHandler.addToSocialSDK();

                addYiXinPlatform(true);

            } else if (target == SHARE_MEDIA.LAIWANG) {
                // 添加来往平台的支持
                // UMLWHandler umLWHandler = new UMLWHandler(mActivity,
                // LAIWANG_APPID, LAIWANG_APPKEY);
                // umLWHandler.setTargetUrl(TARGET_URL);
                // umLWHandler.setTitle("分享到来往");
                // umLWHandler.setMessageFrom(LAIWANG_APP_NAME);
                // umLWHandler.addToSocialSDK();

                addLaiwangPlatform(false);

            } else if (target == SHARE_MEDIA.LAIWANG_DYNAMIC) {
                // 添加来往动态平台的支持
                // UMLWHandler umlwDynamicHandler = new UMLWHandler(mActivity,
                // LAIWANG_APPID,
                // LAIWANG_APPKEY);
                // // 设置为易信朋友圈平台
                // umlwDynamicHandler.setToCircle(true);
                // umlwDynamicHandler.setTitle("分享到来往动态");
                // umlwDynamicHandler.setTargetUrl(TARGET_URL);
                // // 设置消息来源
                // umlwDynamicHandler.setMessageFrom(LAIWANG_APP_NAME);
                // umlwDynamicHandler.addToSocialSDK();

                addLaiwangPlatform(true);

            } else if (target == SHARE_MEDIA.FACEBOOK) {
                // facebook的支持
                // UMFacebookHandler mFacebookHandler = new UMFacebookHandler(
                // mActivity, FACEBOOK_APP_ID, PostType.FEED);
                // mFacebookHandler.setTargetUrl(TARGET_URL);
                // mFacebookHandler.addToSocialSDK();

                addFacebookPlatform();

            } else if (target == SHARE_MEDIA.INSTAGRAM) {
                // 构建Instagram的Handler
                // UMInstagramHandler instagramHandler = new UMInstagramHandler(
                // mActivity);
                // instagramHandler.addToSocialSDK();

                addInstagramPlatform();

            } else if (target == SHARE_MEDIA.TWITTER) {
                mSocializeConfig.supportAppPlatform(mActivity, target, DESCRIPTOR,
                        true);
            } else if (target == SHARE_MEDIA.GOOGLEPLUS) {
                mSocializeConfig.supportAppPlatform(mActivity, target, DESCRIPTOR,
                        true);
            } else if (target == SHARE_MEDIA.SMS) { // 短信平台分享
                addPlatformToSDK(target, null, null, false);
            }
            else if (target == SHARE_MEDIA.EMAIL) { // 邮件分享
                //
                addPlatformToSDK(target, null, null, false);
            } else {
                Log.e(TAG,
                        target
                                + "平台暂不支持该集成方式, 请参考 : http://dev.umeng.com/social/cocos2d-x/integration");
            }
        } catch (NoClassDefFoundError e) {
            Log.e(TAG,
                    "### 没有找到对应的类, 请确保您添加了对应的jar文件, 更多请参考线上文档 : http://dev.umeng.com/social/cocos2d-x/integration");
        }

        Log.d(TAG, "@@@@ supportPlatfrom");

    }

    /**
     * @param classes
     * @return
     */
    private static Class<?>[] convertToClassArray(Class<?>... classes) {
        Class<?>[] clzArray = new Class<?>[classes.length];
        for (int i = 0; i < clzArray.length; i++) {
            clzArray[i] = classes[i];
        }

        return clzArray;
    }

    /**
     * @param args
     * @return
     */
    private static Object[] convertToArgsArray(Object... args) {
        Object[] argsArray = new Object[args.length];
        for (int i = 0; i < argsArray.length; i++) {
            argsArray[i] = args[i];
            Log.d(TAG, "### 参数 : " + args[i]);
        }

        return argsArray;
    }

    /**
     * 
     */
    private static void addWeiXinPlatform(boolean toCircle) {

        SHARE_MEDIA platform = toCircle ? SHARE_MEDIA.WEIXIN_CIRCLE : SHARE_MEDIA.WEIXIN;
        //
        Class<?>[] parameterTypes = convertToClassArray(Context.class, String.class, String.class);
        //
        Object[] args = convertToArgsArray(mActivity, WEIXIN_APP_ID, WEIXIN_APP_SECRET);
        // 添加微信或者微信朋友圈平台
        addPlatformToSDK(platform, parameterTypes, args, toCircle);

    }

    /**
     * 添加QQ或者QQ空间平台
     */
    private static void addTencentPlatform(SHARE_MEDIA platform) {

        if (platform == SHARE_MEDIA.QQ || platform == SHARE_MEDIA.QZONE) {
            //
            Class<?>[] parameterTypes = convertToClassArray(Activity.class, String.class,
                    String.class);
            //
            Object[] args = convertToArgsArray(mActivity, QQ_QZONE_APP_ID, QQ_QZONE_APP_KEY);
            //
            addPlatformToSDK(platform, parameterTypes, args, false);

        }
    }

    /**
     * 添加易信平台
     */
    private static void addYiXinPlatform(boolean toCircle) {
        //
        SHARE_MEDIA platform = toCircle ? SHARE_MEDIA.YIXIN_CIRCLE : SHARE_MEDIA.YIXIN;

        //
        Class<?>[] parameterTypes = convertToClassArray(Context.class, String.class);
        //
        Object[] args = convertToArgsArray(mActivity, YIXIN_APPKEY);
        // 添加易信或者易信朋友圈平台
        addPlatformToSDK(platform, parameterTypes, args, toCircle);

    }

    /**
     * 添加来往或者来往动态平台
     */
    private static void addLaiwangPlatform(boolean toCircle) {
        //
        SHARE_MEDIA platform = toCircle ? SHARE_MEDIA.LAIWANG_DYNAMIC : SHARE_MEDIA.LAIWANG;
        //
        Class<?>[] parameterTypes = convertToClassArray(Context.class, String.class, String.class);
        //
        Object[] args = convertToArgsArray(mActivity, LAIWANG_APPID, LAIWANG_APPKEY);
        // 添加易信或者易信朋友圈平台
        Object object = addPlatformToSDK(platform, parameterTypes, args, toCircle);
        // 设置应用名称
        ReflectUtils.invokeMethod(object, SET_LW_APP_NAME_METHOD,
                convertToClassArray(String.class), convertToArgsArray(LAIWANG_APP_NAME));

    }

    /**
     * 添加facebook平台
     */
    private static void addFacebookPlatform() {
        //
        Class<?>[] parameterTypes = convertToClassArray(Activity.class, String.class);
        //
        Object[] args = convertToArgsArray(mActivity, FACEBOOK_APP_ID);
        // 添加FACEBOOK
        addPlatformToSDK(SHARE_MEDIA.FACEBOOK,
                parameterTypes, args, false);
    }

    /**
     * 
     */
    private static void addRenrenSso() {
        //
        Class<?>[] parameterTypes = convertToClassArray(Context.class, String.class, String.class,
                String.class);
        //
        Object[] args = convertToArgsArray(mActivity, RENREN_APP_ID, RENREN_APP_KEY,
                RENREN_APP_SECRET);
        // 添加renren
        addPlatformToSDK(SHARE_MEDIA.RENREN,
                parameterTypes, args, false);
    }

    /**
     * 添加instagram平台
     */
    private static void addInstagramPlatform() {
        //
        Class<?>[] parameterTypes = convertToClassArray(Context.class);
        //
        Object[] args = convertToArgsArray(mActivity);
        // 添加INSTAGRAM
        addPlatformToSDK(SHARE_MEDIA.INSTAGRAM,
                parameterTypes, args, false);

    }

    /**
     * 
     */
    private static final String ADD_TO_SDK_METHOD = "addToSocialSDK";
    /**
     * 
     */
    private static final String SET_TARGET_URL_METHOD = "setTargetUrl";

    /**
     * 
     */
    private static final String SET_TO_CIRCLE_METHOD = "setToCircle";

    /**
     * 
     */
    private static final String SET_LW_APP_NAME_METHOD = "setMessageFrom";

    /**
     * @param platform
     * @param ssoHandleClassPath
     * @param paramTypes
     * @param args
     * @return
     */
    private static Object addPlatformToSDK(SHARE_MEDIA platform,
            Class<?>[] paramTypes, Object[] args, boolean toCircle) {
        if (platform != null) {
            Object objectHandler = ReflectUtils.newHandlerInstance(
                    mHandlerClzPathMap.get(platform),
                    paramTypes, args);

            Log.d(TAG, "zzz 新添加的平台类型 : " + objectHandler + ", platform : " + platform);
            // 设置target url
            ReflectUtils.invokeMethod(objectHandler, SET_TARGET_URL_METHOD, new Class<?>[] {
                    String.class
            }, new String[] {
                    TARGET_URL
            });

            if (isNeedSetToCircle(platform)) {
                // setToCircle
                ReflectUtils.invokeMethod(objectHandler, SET_TO_CIRCLE_METHOD,
                        convertToClassArray(boolean.class),
                        convertToArgsArray(toCircle));
            }

            // 将平台添加到SDK中
            ReflectUtils.invokeMethod(objectHandler, ADD_TO_SDK_METHOD, null, null);
            Log.d("", "zzz SsoHandler 是否添加成功 ? "
                    + mController.getConfig().getSsoHandler(platform.getReqCode()));
            return objectHandler;
        }

        return new Object();
    }

    private static boolean isNeedSetToCircle(SHARE_MEDIA platform) {
        return platform == SHARE_MEDIA.WEIXIN_CIRCLE || platform == SHARE_MEDIA.LAIWANG_DYNAMIC
                || platform == SHARE_MEDIA.YIXIN_CIRCLE;
    }

    /**
     * 设置人人网的app id等信息，必须在本地设置，且需要在调用setSsoAuthorization之前调用
     * 
     * @param appid
     * @param appkey
     * @param appSecret
     */
    public static void setRenrenAppInfo(String appid, String appkey, String appSecret) {
        RENREN_APP_ID = appid;
        RENREN_APP_KEY = appkey;
        RENREN_APP_SECRET = appSecret;

        Log.d(TAG, "### 人人网SSO, app id = " + RENREN_APP_ID + ", app key = " + RENREN_APP_KEY
                + ", app secret = " + RENREN_APP_SECRET);
    }

    /**
     * 设置平台顺序, 没有的平台则添加, 内置的但是没有给出的则移除.
     * 
     * @param platforms 平台的顺序数组
     */
    public static void setPlatforms(final int[] platforms) {

        // 运行在主线程
        runOnMainThread(new Runnable() {

            @Override
            public void run() {
                if (platforms != null && platforms.length > 0) {
                    int length = platforms.length;
                    //
                    List<SHARE_MEDIA> cacheList = new ArrayList<SHARE_MEDIA>();
                    // 迭代
                    for (int i = 0; i < length; i++) {
                        int index = platforms[i];
                        SHARE_MEDIA target = getPlatform(index);
                        Log.d(TAG, "### 添加平台 --> " + target);
                        if (target != null && target != SHARE_MEDIA.GENERIC) {
                            supportPlatfrom(target);
                            cacheList.add(target);
                        }
                    }

                    SHARE_MEDIA[] platformsMedias = new SHARE_MEDIA[cacheList
                            .size()];
                    cacheList.toArray(platformsMedias);

                    // 设置平台
                    mSocializeConfig.setPlatforms(platformsMedias);
                    // 设置显示顺序
                    mSocializeConfig.setPlatformOrder(platformsMedias);
                }
            }
        });

    }

    /**
     * 平台是否配置在SDK了
     * 
     * @param target
     * @return
     */
    private static boolean isPlatformConfiged(SHARE_MEDIA target) {
        // 内置平台
        List<SHARE_MEDIA> internalList = mSocializeConfig.getPlatforms();
        // 如果是内置平台, 找到以后直接返回
        if (internalList.contains(target)) {
            return true;
        }

        // 自定义平台
        List<CustomPlatform> customPlatforms = mSocializeConfig
                .getCustomPlatforms();
        // 在自定义平台查找
        for (CustomPlatform cs : customPlatforms) {
            if (cs != null && cs.mKeyword.equals(target.toString())) {
                return true;
            }
        }
        return false;
    }

    /**
     * 使得runnable中的代码运行在opengl线程, 即cocos2d-x的opengl线程
     * 
     * @param runnable
     */
    private static void runOnOpenGLThread(Runnable runnable) {
        if (mActivity != null) {
            mActivity.runOnGLThread(runnable);
        }
    }

    /**
     * 将操作封装在Runnable中, 使之运行在Android主线程
     * 
     * @param runnable
     */
    private static void runOnMainThread(Runnable runnable) {
        mSDKHandler.postDelayed(runnable, DELAY_MS);
    }

    /**
     * @param platform 要检测的平台
     * @return
     */
    private static boolean isPlatformValid(int platform) {
        final SHARE_MEDIA target = getPlatform(platform);
        if (target == null || target == SHARE_MEDIA.GENERIC) {
            Log.e(TAG, "### 设置的目标平台无效.   platform = " + target);
            return false;
        }
        return true;
    }

    /**
     * 设置QQ和QQ空间的app id
     * 
     * @param appid
     */
    public static void setQQAndQzoneAppIdWithAppKey(String appid, String appKey) {
        QQ_QZONE_APP_ID = appid;
        QQ_QZONE_APP_KEY = appKey;
        Log.d(TAG, "### QQ or qzone app id = " + appid + ", app key = "
                + appKey);
    }

    /**
     * 设置微信和微信朋友圈的app id
     * 
     * @param appid
     */
    public static void setWeiXinAppInfo(String appid, String appsecret) {
        WEIXIN_APP_ID = appid;
        WEIXIN_APP_SECRET = appsecret;
        Log.d(TAG, "### 微信 app id = " + WEIXIN_APP_ID + ", secret = " + WEIXIN_APP_SECRET);
    }

    /**
     * 设置facebook app id
     * 
     * @param appid
     */
    public static void setFacebookAppId(String appid) {
        FACEBOOK_APP_ID = appid;
        Log.d(TAG, "### facebook appid = " + appid);
    }

    /**
     * 设置易信和易信朋友圈的app id
     * 
     * @param appid
     */
    public static void setYiXinAppKey(String appid) {
        YIXIN_APPKEY = appid;
        Log.d(TAG, "### 易信 app id = " + appid);
    }

    /**
     * 设置来往和来往动态的app id
     * 
     * @param appid
     */
    public static void setLaiwangAppId(String appid) {
        LAIWANG_APPID = appid;
        Log.d(TAG, "### 来往 app id = " + appid);
    }

    /**
     * 设置来往和来往动态的app key
     * 
     * @param appid
     */
    public static void setLaiwangAppKey(String appkey) {
        LAIWANG_APPKEY = appkey;
        Log.d(TAG, "### 来往 app key = " + appkey);
    }

    /**
     * 设置来往和来往动态的app name
     * 
     * @param appName
     */
    public static void setLaiwangAppName(String appName) {
        LAIWANG_APP_NAME = appName;
        Log.d(TAG, "### 来往 app name = " + appName);
    }

    /**
     * 检查mActivity的合法性
     */
    private static void checkActivity() {
        assert mActivity != null : "在CCUMSocialController类中, mActivity为null.";
    }

    /**
     * 清空SDK
     */
    public static void cleanup() {
        mController = null;
        mSDKHandler = null;
        mActivity = null;
        Log.d(TAG, "@@@@ cleanup");
    }

    /*******************************************************************************
     * 授权回调接口, 会调用native层对应的回调方法, 开发者可以在Java或者native层进行相应的处理
     ******************************************************************************/
    private static UMAuthListener mAuthListener = new UMAuthListener() {

        @Override
        public void onStart(final SHARE_MEDIA platform) {
            // 运行在gl线程
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnAuthorizeStart(getPlatformInt(platform));
                }
            });

        }

        @Override
        public void onError(final SocializeException e,
                final SHARE_MEDIA platform) {

            // 运行在gl线程
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnAuthorizeComplete(getPlatformInt(platform), 0,
                            new String[] {
                                e.getLocalizedMessage()
                            });
                }
            });
        }

        @Override
        public void onComplete(final Bundle value, final SHARE_MEDIA platform) {

            // 运行在gl线程
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnAuthorizeComplete(getPlatformInt(platform),
                            StatusCode.ST_CODE_SUCCESSED, getAuthData(value));
                }
            });
        }

        @Override
        public void onCancel(final SHARE_MEDIA platform) {
            // 运行在gl线程
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnAuthorizeComplete(getPlatformInt(platform), -1,
                            new String[] {
                                "cancel"
                            });
                }
            });

        }

        /**
         * @param data 授权的数据
         * @return 如果含有access_token则获取access_token和过期时间, 传递到native层
         */
        private String[] getAuthData(Bundle data) {
            if (data != null
                    && (data.containsKey("access_token") || data
                            .containsKey("access_secret"))) {
                String[] authData = new String[3];
                // 有的字段为secret
                if (data.containsKey("access_secret")) {
                    authData[0] = data.getString("access_secret");
                } else {
                    authData[0] = data.getString("access_token");
                }
                if (data.containsKey("uid")) {
                    authData[1] = data.getString("uid");
                } else {
                    authData[1] = "";
                }

                return authData;
            } else {
                return new String[] {};
            }
        }
    };

    /**
     * 回调授权的OnStart方法到native层
     */
    private native static void OnAuthorizeStart(int platform);

    /**
     * 回调授权的onComplete方法到native层
     * 
     * @param value 授权信息, 包含token等, 包含access_token, 过期时间expires_in
     * @param platform 平台
     */
    private native static void OnAuthorizeComplete(int platform, int stCode,
            String[] value);

    /******************************************************************************
     * 分享回调接口,会调用native层对应的回调方法, 开发者可以在Java或者native层进行相应的处理
     *****************************************************************************/
    private static SnsPostListener mSocialShareListener = new SnsPostListener() {

        /*
         * 分享开始
         */
        @Override
        public void onStart() {
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnShareStart();
                }
            });

        }

        /*
         * 分享完成
         */
        @Override
        public void onComplete(final SHARE_MEDIA platform, final int eCode,
                final SocializeEntity entity) {
            runOnOpenGLThread(new Runnable() {

                @Override
                public void run() {
                    OnShareComplete(getPlatformInt(platform), eCode,
                            entity.mDescriptor);
                }
            });

        }
    };

    /**
     * 回调分享的OnStart方法到native层
     */
    private native static void OnShareStart();

    /**
     * 回调分享的onComplete方法到native层
     * 
     * @param platform 平台
     * @param eCode 错误码, 200为授权成功
     * @param entity UMSocialService的属性容器
     */
    private native static void OnShareComplete(int platform, int eCode,
            String descriptor);

    /**
     * 通过整型获取对应的平台, C++中使用enum常量来代表平台
     * 
     * @param location
     * @return
     */
    private static SHARE_MEDIA getPlatform(int location) {
        int length = mPlatformsList.size();
        if (location < 0 || location >= length) {
            return null;
        }
        return mPlatformsList.get(location);
    }

    /**
     * 获取平台对应的整型号码
     * 
     * @param platform 对应的平台
     * @return the index of the first occurrence of the object or -1 if the
     *         object was not found.
     */
    private static int getPlatformInt(SHARE_MEDIA platform) {
        return mPlatformsList.indexOf(platform);
    }

    /**
     * 
     */
    private static Map<SHARE_MEDIA, String> mHandlerClzPathMap = new HashMap<SHARE_MEDIA, String>();

    /**
     * 
     */
    private static Map<SHARE_MEDIA, String> mShareContentClzPathMap = new HashMap<SHARE_MEDIA, String>();
    /*
	 * 
	 */
    private static List<SHARE_MEDIA> mPlatformsList = new ArrayList<SHARE_MEDIA>();

    /**
     * 
     */
    private static void initPlatformList() {
        mPlatformsList.add(0, SHARE_MEDIA.SINA);
        mPlatformsList.add(1, SHARE_MEDIA.WEIXIN);
        mPlatformsList.add(2, SHARE_MEDIA.WEIXIN_CIRCLE);
        mPlatformsList.add(3, SHARE_MEDIA.QQ);
        mPlatformsList.add(4, SHARE_MEDIA.QZONE);
        mPlatformsList.add(5, SHARE_MEDIA.RENREN);
        mPlatformsList.add(6, SHARE_MEDIA.DOUBAN);
        mPlatformsList.add(7, SHARE_MEDIA.LAIWANG);
        mPlatformsList.add(8, SHARE_MEDIA.LAIWANG_DYNAMIC);
        mPlatformsList.add(9, SHARE_MEDIA.YIXIN);
        mPlatformsList.add(10, SHARE_MEDIA.YIXIN_CIRCLE);
        mPlatformsList.add(11, SHARE_MEDIA.FACEBOOK);
        mPlatformsList.add(12, SHARE_MEDIA.TWITTER);
        mPlatformsList.add(13, SHARE_MEDIA.INSTAGRAM);
        mPlatformsList.add(14, SHARE_MEDIA.SMS);
        mPlatformsList.add(15, SHARE_MEDIA.EMAIL);
        mPlatformsList.add(16, SHARE_MEDIA.TENCENT);
    }

    /**
     * 
     */
    private static void initHandlerClzsMap() {
        mHandlerClzPathMap.put(SHARE_MEDIA.WEIXIN,
                "com.umeng.socialize.weixin.controller.UMWXHandler");
        mHandlerClzPathMap.put(SHARE_MEDIA.WEIXIN_CIRCLE,
                "com.umeng.socialize.weixin.controller.UMWXHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.QQ, "com.umeng.socialize.sso.UMQQSsoHandler");
        mHandlerClzPathMap.put(SHARE_MEDIA.QZONE, "com.umeng.socialize.sso.QZoneSsoHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.SINA, "com.umeng.socialize.sso.SinaSsoHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.RENREN, "com.umeng.socialize.sso.RenrenSsoHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.LAIWANG,
                "com.umeng.socialize.laiwang.controller.UMLWHandler");
        mHandlerClzPathMap.put(SHARE_MEDIA.LAIWANG_DYNAMIC,
                "com.umeng.socialize.laiwang.controller.UMLWHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.YIXIN,
                "com.umeng.socialize.yixin.controller.UMYXHandler");
        mHandlerClzPathMap.put(SHARE_MEDIA.YIXIN_CIRCLE,
                "com.umeng.socialize.yixin.controller.UMYXHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.FACEBOOK,
                "com.umeng.socialize.facebook.controller.UMFacebookHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.INSTAGRAM,
                "com.umeng.socialize.instagram.controller.UMInstagramHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.SMS, "com.umeng.socialize.sso.SmsHandler");
        //
        mHandlerClzPathMap.put(SHARE_MEDIA.EMAIL, "com.umeng.socialize.sso.EmailHandler");

        //
        mHandlerClzPathMap.put(SHARE_MEDIA.TENCENT, "com.umeng.socialize.sso.TencentWBSsoHandler");
    }

    /**
     * 
     */
    private static void initShareContentClzsMap() {
        mShareContentClzPathMap.put(SHARE_MEDIA.WEIXIN,
                "com.umeng.socialize.weixin.media.WeiXinShareContent");
        mShareContentClzPathMap.put(SHARE_MEDIA.WEIXIN_CIRCLE,
                "com.umeng.socialize.weixin.media.CircleShareContent");
        //
        mShareContentClzPathMap.put(SHARE_MEDIA.QQ, "com.umeng.socialize.media.QQShareContent");
        mShareContentClzPathMap.put(SHARE_MEDIA.QZONE,
                "com.umeng.socialize.media.QZoneShareContent");
        //
        mShareContentClzPathMap.put(SHARE_MEDIA.LAIWANG,
                "com.umeng.socialize.laiwang.media.LWShareContent");
        mShareContentClzPathMap.put(SHARE_MEDIA.LAIWANG_DYNAMIC,
                "com.umeng.socialize.laiwang.media.LWDynamicShareContent");
        //
        mShareContentClzPathMap.put(SHARE_MEDIA.YIXIN,
                "com.umeng.socialize.yixin.media.YiXinShareContent");
        mShareContentClzPathMap.put(SHARE_MEDIA.YIXIN_CIRCLE,
                "com.umeng.socialize.yixin.media.YiXinCircleShareContent");
        //
        mShareContentClzPathMap.put(SHARE_MEDIA.FACEBOOK,
                "com.umeng.socialize.facebook.media.FaceBookShareContent");
        //
        mShareContentClzPathMap.put(SHARE_MEDIA.INSTAGRAM,
                "com.umeng.socialize.instagram.media.InstagramShareContent");
        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.DOUBAN, "com.umeng.socialize.media.DoubanShareContent");
        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.GOOGLEPLUS, "com.umeng.socialize.media.GooglePlusShareContent");
        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.RENREN, "com.umeng.socialize.media.RenrenShareContent");
        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.SINA, "com.umeng.socialize.media.SinaShareContent");

        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.TENCENT, "com.umeng.socialize.media.TencentWbShareContent");

        mShareContentClzPathMap
                .put(SHARE_MEDIA.TWITTER, "com.umeng.socialize.media.TwitterShareContent");

        mShareContentClzPathMap
                .put(SHARE_MEDIA.SMS, "com.umeng.socialize.media.SmsShareContent");

        //
        mShareContentClzPathMap
                .put(SHARE_MEDIA.EMAIL, "com.umeng.socialize.media.MailShareContent");
    }

    /*
	 * 
	 */
    static {
        // 添加平台到索引中
        initPlatformList();

        // 添加各个Sso handler类的绝对路径
        initHandlerClzsMap();

        //
        initShareContentClzsMap();
    }

}
