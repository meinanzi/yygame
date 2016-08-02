#include "HNPlatformRegist.h"
#include "HNMarketExport.h"

namespace HN
{
    bool HNPlatformRegist::requestRegist(const std::string &name, const std::string pwd, const std::string& agentid, bool fastRegist)
	{
		if(!fastRegist)
		{
			if(name.empty() || pwd.empty())
			{
				_callback->onPlatformRegistCallback(false, fastRegist, GBKToUtf8("名称或密码为空"), name, pwd, "", 0);
				return false;
			}
		}
		
		_fastRegist = fastRegist;
		_name       = name;
		_pwd        = pwd;
        _agentid = agentid;
		platformRegist();
		return true;
	}
	
	void HNPlatformRegist::I_P_M_Connect(bool result)
	{
		if(!result)
		{
			_callback->onPlatformRegistCallback(false, _fastRegist, GBKToUtf8("平台网络连接失败"), _name, _pwd, "", 0);
			return;
		}
		platformRegist();
	}
	
	void HNPlatformRegist::I_P_M_Regist(MSG_GP_S_Register* registerStruct, UINT ErrorCode)
	{
		_name = registerStruct->szName;
		_pwd  = registerStruct->szPswd;

		switch (ErrorCode)
		{
		case eFAILE:
			_callback->onPlatformRegistCallback(false, _fastRegist, GBKToUtf8("注册失败"), _name, _pwd, "", registerStruct->LogonTimes);
			break;
		case eSUCCESS:
			_callback->onPlatformRegistCallback(true, _fastRegist, GBKToUtf8("注册成功"), _name, _pwd, "", registerStruct->LogonTimes);
			break;
		case eEXIST:
			_callback->onPlatformRegistCallback(false, _fastRegist, GBKToUtf8("用户名已存在"), _name, _pwd, "", registerStruct->LogonTimes);
			break;
		case eSENSITIVE:
			_callback->onPlatformRegistCallback(false, _fastRegist, GBKToUtf8("包含敏感词汇"), _name, _pwd, "", registerStruct->LogonTimes);
			break;
        case eNOEGENCY:
            _callback->onPlatformRegistCallback(false, _fastRegist, GBKToUtf8("代理商不存在"), _name, _pwd, "", registerStruct->LogonTimes);
            break;
		default:
			break;
		}
	}

	HNPlatformRegist::HNPlatformRegist(IHNPlatformRegist* callback)
		: HNPlatformLogicBase(callback)
		, _fastRegist(true)
	{
		CCAssert(callback != nullptr, "callback is null.");
		_callback = callback;
	}

	HNPlatformRegist::~HNPlatformRegist()
	{
		_callback = nullptr;
	}

	void HNPlatformRegist::platformRegist()
	{
		if(PlatformLogic()->isConnect())
		{
			if(_fastRegist)
			{
				std::string onlyString = Operator::requestChannel("sysmodule", "GetSerialNumber");
				PlatformLogic()->regist(0, GameCreator()->getCurrentGameNameID(), onlyString);
			}
			else
			{
				PlatformLogic()->regist(1, GameCreator()->getCurrentGameNameID(), _name, _pwd, _agentid);
			}
		}
		else
		{
			PlatformLogic()->connect();
		}
	}
}