#ifndef __HN_HNPlatformLogic_H__
#define __HN_HNPlatformLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNSocket/HNSocketMessageDelegate.h"
#include "HNSocket/HNSocketMessage.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

namespace HN
{
	class IPlatformMessageDelegate;
	class HNSocketLogic;

	typedef std::function<bool (IPlatformMessageDelegate *)> PlatformNotifyFun;

	class HNPlatformLogic : public Ref, ISocketMessageDelegate 
	{

	public:
		// 登陆结果
		MSG_GP_R_LogonResult		loginResult;

		// 获取单例
		static HNPlatformLogic* getInstance();

		// 初始化
		bool init();

	public:
		// 连接平台
		bool connect();

		// 关闭平台
		bool close();
		
		// 是否连接
		bool isConnect() const;

		// 是否登陆
		bool isLogin() const;
		
		// 发送数据
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

		// 发送数据（带回掉）
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);

		// 添加指定消息监听
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// 移除指定消息监听
		void removeEventSelector(UINT MainID, UINT AssistantID);

		// 添加所有消息监听
		void addObserver(IPlatformMessageDelegate* delegate);

		// 移除所有消息监听
		void removeObserver(IPlatformMessageDelegate* delegate);

		// 获取房间列表
		void getRoomList(UINT uKindID, UINT uNameID);

		// 登陆
		void login(const std::string& name, const std::string& password, UINT uNameID);

		// 注册
		void regist(BYTE byFastRegister, UINT uNameID, const std::string& usn, const std::string& name = "", const std::string& password = "", const std::string &agency = "");

	public:
		// 连接消息
		virtual void onConnected(bool connect, emSocketStatus status) override;
		
		// 断线消息
		virtual void onDisConnect() override;
		
		// TCP消息转发
		virtual void onSocketMessage(HNSocketMessage* socketMessage) override;

	// 消息处理
	protected:
		// 平台连接
		void H_P_M_Connect(const NetMessageHead* messageHead, void* object, INT objectSize);
		
		// 平台登录
		void H_P_M_Login(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 列表
		void H_P_M_List(const NetMessageHead* messageHead, void* object, INT objectSize);
		
		// 游戏类型列表
		void H_P_M_ListKind(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏名称列表
		void H_P_M_ListName(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 房间列表
		void H_P_M_ListRoom(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 房间人数
		void H_P_M_ListRoomUserCount(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏人数
		void H_P_M_ListGameUserCount(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 房间密码
		void H_P_M_ListRoomPassword(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 登陆玩家
		void H_P_M_LoginUsers(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 注册
		void H_P_M_Regist(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 道具
		void H_P_M_Prop(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 消息
		void H_P_M_Message(const NetMessageHead* messageHead, void* object, INT objectSize);

	private:
		// 心跳包
		void heartBeat(float dt);

		// 消息分发
		bool dispatchMessage(const PlatformNotifyFun& func);

	private:
		// 添加监听缓冲队列
		HNUIDelegateQueue<IPlatformMessageDelegate*>* _addQueue;

		// 移除监听缓冲队列
		HNUIDelegateQueue<IPlatformMessageDelegate*>* _removeQueue;

		// 消息通知队列
		HNUIDelegateQueue<IPlatformMessageDelegate*>* _notifyQueue;

		// Socket逻辑
		HNSocketLogic*	_socketLogic;

		// 平台是否连接标志
		bool			_connected;

		// 平台是否登陆标志
		bool			_logined;

		// 心跳包频率
		INT				_heartBeatFrequency;

		// 服务器地址（ip或者域名）
		std::string		_serverAddress;

		// 服务器端口
		INT				_serverPort;

	private:
		// 构造函数
		HNPlatformLogic(void);

		// 析构函数
		virtual ~HNPlatformLogic(void);

	};

	#define  PlatformLogic()	HNPlatformLogic::getInstance()
}

#endif	//__HN_HNPlatformLogic_H__

