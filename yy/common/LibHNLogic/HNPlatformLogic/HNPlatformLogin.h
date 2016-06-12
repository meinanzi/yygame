#ifndef __HNPlatformLogin_H__
#define __HNPlatformLogin_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"
#include <string>

namespace HN
{
	/*
	 *	redbird login interface.
	 */
	class IHNPlatformLogin: public IHNPlatformLogicBase
	{
	public:
		virtual void onPlatformLoginCallback(bool success, const std::string& message,const std::string& name, const std::string& pwd) {}
	};


	/*
	 * redbird login logic.
	 */
	class HNPlatformLogin: public HNPlatformLogicBase
	{
	public:
		bool requestLogin(const std::string& name, const std::string& pwd);
	public:		
		// 平台连接
		virtual void I_P_M_Connect(bool result) override;
		
		// 平台登录
		virtual void I_P_M_Login(bool result, UINT dwErrorCode) override;
	public:
		HNPlatformLogin(IHNPlatformLogin* callback);
		virtual ~ HNPlatformLogin();
	protected:
		std::string _name;
		std::string _pwd;
		IHNPlatformLogin* _callback;
		void platformLogin();
	};
}

#endif