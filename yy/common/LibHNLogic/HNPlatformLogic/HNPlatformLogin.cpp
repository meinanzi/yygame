#include "HNPlatformLogin.h"
#include "HNNetExport.h"
#include "cocos2d.h"

USING_NS_CC;

namespace HN
{
	static const std::string Word_Wrong_Name  = GBKToUtf8("µÇÂ½Ãû×Ö´íÎó"); 
	static const std::string Word_Wrong_Pass  = GBKToUtf8("ÓÃ»§ÃÜÂë´íÎó"); 
	static const std::string Word_Logined     = GBKToUtf8("ÕÊºÅÒÑ¾­µÇÂ¼"); 

	bool HNPlatformLogin::requestLogin(const std::string& name, const std::string& pwd)
	{
		if(name.empty() || pwd.empty())
		{
			_callback->onPlatformLoginCallback(false, GBKToUtf8("Ãû³Æ»òÕßÃÜÂëÎª¿Õ"), name, pwd);
			return false;
		}
		_name = name;
		_pwd  = pwd;

		platformLogin();

		return true;
	}

	void HNPlatformLogin::I_P_M_Connect(bool result)
	{
		if(!result)
		{
			_callback->onPlatformLoginCallback(false, GBKToUtf8("Æ½Ì¨ÍøÂçÁ¬½ÓÊ§°Ü"), _name, _pwd);
			return;
		}
		platformLogin();
	}

	void HNPlatformLogin::I_P_M_Login(bool result, UINT dwErrorCode)
	{
		if(!result)
		{
			char message[50] = {0};
			switch (dwErrorCode)
			{
			case ERR_GP_USER_NO_FIND:
				sprintf(message, "%s(%u)", Word_Wrong_Name.c_str(), dwErrorCode);
				break;
			case ERR_GP_USER_PASS_ERROR:
				sprintf(message, "%s(%u)", Word_Wrong_Pass.c_str(), dwErrorCode);
				break;
			case ERR_GP_USER_LOGON:
				sprintf(message, "%s(%u)", Word_Logined.c_str(), dwErrorCode);
				break;
			default:
				break;
			}
			_callback->onPlatformLoginCallback(false, message, _name, _pwd);
			return;
		}
		_callback->onPlatformLoginCallback(true, GBKToUtf8("µÇÂ¼³É¹¦"), _name, _pwd);
	}

	HNPlatformLogin::HNPlatformLogin(IHNPlatformLogin* callback)
		: HNPlatformLogicBase(callback)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
	}

	HNPlatformLogin::~HNPlatformLogin()
	{
		_callback = nullptr;
	}

	void HNPlatformLogin::platformLogin()
	{
		if(PlatformLogic()->isConnect())
		{
			PlatformLogic()->login(_name, _pwd, GameCreator()->getCurrentGameNameID());
		}
		else
		{
			PlatformLogic()->connect();
		}
	}
}

