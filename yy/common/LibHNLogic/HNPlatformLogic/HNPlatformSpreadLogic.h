//Copyright (c) 2012—2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#ifndef __SpreadLogic_h__
#define __SpreadLogic_h__

#include "HNPlatformLogic/HNPlatformSpreadManager.h"
#include "HNPlatformLogicBase.h"
#include "HNNetExport.h"

USING_NS_CC;
using namespace network;


namespace HN
{
	struct spread_layerInfo
	{
		spread_layerInfo()
		:minMoney(0)
		,spreadIncome(0)
		,trueName("")
		,userName("")
		{}

		LLONG minMoney;
		LLONG spreadIncome;
		std::string trueName;
		std::string userName;
	};

	struct Spread_PayInfo
	{
		Spread_PayInfo()
		:PayUserID(0)
		,PayMoney(0)
		,GetMoney(0)
		,PayType("")
		,AddTime("")
		,UserName("")
		{}

		int PayUserID;
		LLONG PayMoney;
		LLONG GetMoney;
		std::string PayType;
		std::string AddTime;
		std::string UserName;
	};
	typedef std::vector<Spread_PayInfo> vecSpreadPayList;

	struct SpreadedUserInfo
	{
		SpreadedUserInfo()
		:UserID(0)
		,GameTimeCount(0)
		,BillStatus(false)
		,IsRegAward(0)
		,GetMoney(0)
		,UserName("")
		,FinishPercent("")
		{}

		int UserID;
		int GameTimeCount;
		bool BillStatus;
		LLONG IsRegAward;
		LLONG GetMoney;
		std::string UserName;
		std::string FinishPercent;
	};
	typedef std::vector<SpreadedUserInfo> vecSpreadList;

	class HNSpreadLogic : public IHNPlatformLogicBase, public HNHttpDelegate
	{
	public:
		class Delegate
		{
		public:
			virtual ~Delegate(){}
		public:
			virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg){};
			virtual void onBeSpreader(bool result, const std::string& msg){};
			virtual void onGetBillLayerInfo(bool result, const spread_layerInfo& info, const std::string& msg){};
			virtual void onGetMoney(bool result, const std::string& msg){};
			virtual void onSpreadedPayList(bool result, const vecSpreadPayList& vecInfo, const std::string& msg){};
			virtual void onSpreadedList(bool result, const vecSpreadList& vecInfo, const std::string& msg){};
			virtual void onSingleSpreadedUserBillResult(bool result, const std::string& msg){};
		};

	private:
		Delegate* _delegate;

	public:
		HNSpreadLogic();
		virtual ~HNSpreadLogic();

	public:
		void init(Delegate* delegate);

	public:
		bool isSpreader(){return HNSpreadManager::getInstance()->isSpreader();}
		SpreaderInfo getSpreadInfo(){return HNSpreadManager::getInstance()->getSpreadInfo();}

	public:
		virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;

		//请求推广员信息
		void requestSpreadInfo(std::string url, std::string data = "");
		void onSpreadInfoCallback(const std::string& data);

		//申请推广员
		void requestBeSpreader(std::string url, std::string data = "");
		void onBeSpreaderCallback(const std::string& data);

		//推广员结算界面信息
		void requestSpreadBillLayerInfo(std::string url, std::string data = "");
		void onSpreadBillLayerInfoCallback(const std::string& data);

		//推广员结账提现
		void requestSpreaderBill(std::string url, std::string data = "");
		void onSpreaderBillCallback(const std::string& data);

		//获取被推广玩家充值列表
		void requestSpreadedPayList(std::string url, std::string data = "");
		void onSpreaderPayListCallback(const std::string& data);

		//获取推广列表
		void requestSpreadedList(std::string url, std::string data = "");
		void onSpreaderListCallback(const std::string& data);

		//单个玩家有效推广结算请求
		void requestSingleSpreadedUserBill(std::string url, std::string data = "");
		void onSingleSpreaderUserBillCallback(const std::string& data);

		//void test(network::HttpClient* client, network::HttpResponse* response);
	private:
	};

}

#endif // __SpreadLogic_h__

