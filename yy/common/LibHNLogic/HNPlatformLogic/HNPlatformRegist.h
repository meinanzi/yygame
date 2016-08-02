#ifndef __HNPlatformRegist_H__
#define __HNPlatformRegist_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"

namespace HN
{
	/*
	 * 注册接口
	 */
	class IHNPlatformRegist: public IHNPlatformLogicBase
	{
	public:
		virtual void onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message,
                                              const std::string&name, const std::string& pwd, const std::string& agentid, int loginTimes){}
	};

	/*
	 * 注册逻辑
	 */
	class HNPlatformRegist: public HNPlatformLogicBase
	{
	public:
		// 注册请求
        bool requestRegist(const std::string &name, const std::string pwd, const std::string& agentid, bool fastRegist);

		// 平台连接
		virtual void I_P_M_Connect(bool result) override;
		
		// 平台注册
		virtual void I_P_M_Regist(MSG_GP_S_Register* registerStruct, UINT ErrorCode) override;
		
		// 构造函数
		HNPlatformRegist(IHNPlatformRegist* callback);

		// 析构函数
		virtual ~HNPlatformRegist();
	protected:
		// 是否快速注册
		bool _fastRegist;

		// 注册用户名
		std::string _name;

		// 注册密码
		std::string _pwd;
        
        // 代理商账号
        std::string _agentid;

		// 回调
		IHNPlatformRegist* _callback;

		// 注册逻辑
		void platformRegist();

		// 注册返回码
		enum REGIST_CODE
		{
			eFAILE     = 0, // 注册失败
			eSUCCESS   = 1, // 注册成功
			eEXIST     = 2, // 相同名称
			eSENSITIVE = 3, // 敏感词汇
		};
	};
}


#endif // !__HNPlatformRegist_H__
