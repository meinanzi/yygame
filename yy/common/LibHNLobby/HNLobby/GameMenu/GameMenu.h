#ifndef __GameMenu_H__
#define __GameMenu_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class GameMenu : public HNLayer, public HN::IHNPlatformLogin, public HN::IHNPlatformRegist
{
public:
	// 进入登陆界面
	static void createMenu();

	// 构造函数
	GameMenu();

	// 析构函数
	virtual ~GameMenu();

	// 初始化
	virtual bool init() override;

	// 显示完成
	virtual void onEnterTransitionDidFinish() override;

	// 更新检查
	void checkUpdate();

	// 更新检测
	void updateCheckFinishCallback(bool updated, cocos2d::Node* pSender, const std::string& message, const std::string& installPath);

	// 游客登陆按钮回调函数
	void guestLoginEventCallback(Ref* pSender, Widget::TouchEventType type);

	// 注册登陆按钮回调函数
	void accountLoginEventCallback(Ref* pSender, Widget::TouchEventType type);

	// 账号注册按钮回调函数
	void RegisterEventCallback(Ref* pSender, Widget::TouchEventType type);

	// 进入游戏
	void enterGame(const std::string& userName, const std::string& userPswd);

	// 创建
	CREATE_FUNC(GameMenu);

	// 加载Icon
	void loadIcon();

private:
	//注册按钮回调
	void registerActionCallBack();

	//保存用户账号
	void saveUserInfo(const std::string& userName, const std::string& userPswd);

public:
	//手机返回键监听回调函数
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) override;

public:
	//注册回调
	virtual void onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message,
		const std::string&name, const std::string& pwd, int loginTimes)  override;

	//登陆回调
	virtual void onPlatformLoginCallback(bool success, const std::string& message,
		const std::string& name, const std::string& pwd)  override;

public:
	HNPlatformLogin*	_gameLogin;
	HNPlatformRegist*	_gameRegist;
	Sprite*				_logo;
	Sprite*				_logoBg;
	Sprite*				_logoTitle;
	Sprite*				_backGround;
	Button*				_buttonGuestLogin;
	Button*				_buttonAccountLogin;
	Button*				_buttonRegister;
	bool				_isAccountLogin;
	std::string			_userName;
	std::string			_userPsd;

	// 是否是游客
	static bool			isVisitor;
};

#endif // __GameMenu_H__