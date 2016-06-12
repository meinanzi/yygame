#ifndef __HNPlatformLogicBase_H__
#define __HNPlatformLogicBase_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"

namespace HN
{
	/*
	 * common interface for platform logic.
	 */
	class IHNPlatformLogicBase 
	{
	public:
		virtual void onPlatformDisConnectCallback(const std::string& message){}
		virtual void onPlatformNewsCallback(const std::string& message){};
	};

	/*
	 * platform logic base.
	 */
	class HNPlatformLogicBase: public HNLogicBase, public IPlatformMessageDelegate
	{
	public:
		virtual void start() override;
		virtual void stop() override;
		HNPlatformLogicBase(IHNPlatformLogicBase* callback);
		virtual ~HNPlatformLogicBase();
	public:
		// 网络断开
		virtual void I_P_M_DisConnect() override;

		// 新闻系统消息
		virtual void I_P_M_NewsSys(MSG_GR_RS_NormalTalk* pData) override;
	protected:
		IHNPlatformLogicBase* _callback;
	};
}


#endif