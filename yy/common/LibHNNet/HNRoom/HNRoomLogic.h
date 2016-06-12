#ifndef __HN_HNRoomLogic_H__
#define __HN_HNRoomLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNSocket/HNSocketMessageDelegate.h"
#include "HNSocket/HNSocketMessage.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "cocos2d.h"

USING_NS_CC;

namespace HN
{
	class IGameMessageDelegate;
	class IRoomMessageDelegate;
	class HNSocketLogic;
	
	// room message distribution statement a callback function
	typedef std::function<bool (IRoomMessageDelegate*)> RoomFrameMessageFun;

	// game message distribution statement a callback function
	typedef std::function<bool (IGameMessageDelegate*)> GameMessageFun;

	// the room network processing logic class
	class HNRoomLogic : public Ref, ISocketMessageDelegate 
	{
	public:
		// 获取实例
		static HNRoomLogic* getInstance();

		// 牌桌状态
		MSG_GR_DeskStation	deskStation;

		// 登录结果
		MSG_GR_R_LogonResult loginResult;

		// 初始化
		bool init();

		// 连接房间
		bool connect(const CHAR* ipOrHost, INT port);
		
		// 关闭房间
		bool close();

		// 房间是否连接
		bool isConnect() const;

		// 是否登录
		bool isLogin() const;

		// 获取进入的房间信息
		inline ComRoomInfo* getSelectedRoom() const
		{
			return _selectedRoom;
		}

		// 设置进入的房间信息
		inline void setSelectedRoom(ComRoomInfo* room)
		{
			_selectedRoom = room;
		}

		// 获取房间规则
		UINT getRoomRule() const
		{
			return _gameRoomRule;
		}

		// 设置房间规则
		void setRoomRule(UINT roomRule)
		{
			_gameRoomRule = roomRule;
		}

		// 房间登录
		void login(UINT uGameID);

		// 发送数据
		INT sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);
		
		// 发送数据（带回调）
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);

		// 添加特定消息监听
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// 移除特定消息监听
		void removeEventSelector(UINT MainID, UINT AssistantID);

		// 添加游戏内消息监听
		void addObserver(IGameMessageDelegate* delegate);

		// 移除游戏内消息监听
		void removeObserver(IGameMessageDelegate* delegate);

		// 添加房间内消息监听
		void addRoomObserver(IRoomMessageDelegate* delegate);

		// 移除房间内消息监听
		void removeRoomObserver(IRoomMessageDelegate* delegate);

		// 消息回调
		virtual void onSocketMessage(HNSocketMessage* socketMessage) override;

		// 网络连接
		virtual void onConnected(bool connect, emSocketStatus status) override;

		// 网络断开
		virtual void onDisConnect() override;

	// 消息处理
	protected:
		// 连接
		void H_R_M_Connect(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 登录
		void H_R_M_Login(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户列表
		void H_R_M_UserList(const NetMessageHead* messageHead, void* object, INT objectSize);
		
		// 用户动作
		void H_R_M_UserAction(const NetMessageHead* messageHead, void* object, INT objectSize);
		
		// 排队用户坐下
		void H_R_M_QueueUserSit(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户进入房间
		void H_R_M_UserCome(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户离开房间
		void H_R_M_UserLeft(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户断线
		void H_R_M_UserCut(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户站起（包含旁观站起）
		void H_R_M_UserUp(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户坐下错误
		void H_R_M_SitError(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户坐下（包含旁观坐下）
		void H_R_M_UserSit(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 房间消息
		void H_R_M_Room(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏开始
		void H_R_M_GameBegin(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏结算
		void H_R_M_UserPoint(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏结束
		void H_R_M_GameEnd(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户同意
		void H_R_M_UserAgree(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 及时更新金币和积分
		void H_R_M_InstantUpdate(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户比赛信息
		void H_R_M_UserContest(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户比赛信息
		void H_R_M_ContestNotic(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 比赛结束
		void H_R_M_ContestOver(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 用户被淘汰
		void H_R_M_ContestKick(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 比赛等待结束
		void H_R_M_ContestWaitOver(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 比赛初始化
		void H_R_M_ContestInit(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 获取报名数量
		void H_R_M_ContestPeople(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 个人参赛记录
		void H_R_M_ContestRecord(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 比赛奖励
		void H_R_M_ContestAward(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏消息
		void H_R_M_GameNotify(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 封桌
		void H_R_M_Manage(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 框架消息
		void H_R_M_GameFrame(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏信息
		void H_R_M_GameInfo(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 游戏状态
		void H_R_M_GameStation(const NetMessageHead* messageHead, void* object, INT objectSize);

		// 普通聊天（表情公用）
		void H_R_M_NormalTalk(const NetMessageHead* messageHead, void* object, INT objectSize);

	private:
		// 当前选中的房间
		ComRoomInfo*	_selectedRoom;
		
		// Tcp逻辑
		HNSocketLogic*	_socketLogic;
		
		// 服务器地址
		std::string		_serverAddress;
		
		// 服务器端口
		INT				_serverPort;

		// 是否连接
		bool			_connected;

		// 是否登录
		bool			_logined;

		// 房间规则
		UINT			_gameRoomRule;	

		// 游戏消息通知队列
		HNUIDelegateQueue<IGameMessageDelegate*> *	_gameNotifyQueue;

		// 游戏消息通知增加缓冲队列
		HNUIDelegateQueue<IGameMessageDelegate*> *	_addGameNotifyQueue;
		
		// 游戏消息通知移除缓冲队列
		HNUIDelegateQueue<IGameMessageDelegate*> *	_removeGameNotifyQueue;

		// 房间消息通知队列
		HNUIDelegateQueue<IRoomMessageDelegate*> *	_roomNotifyQueue;
		
		// 房间消息增加缓冲队列
		HNUIDelegateQueue<IRoomMessageDelegate*> *	_addRoomNotifyQueue;

		// 房间消息移除缓冲队列
		HNUIDelegateQueue<IRoomMessageDelegate*> *	_removeRoomNotifyQueue;

		// 构造函数
		HNRoomLogic(void);

		// 析构函数
		virtual ~HNRoomLogic(void);

		// 转发房间消息
		bool dispatchFrameMessage(const RoomFrameMessageFun& func);
		
		// 转发游戏消息
		bool dispatchGameMessage(const GameMessageFun& func);

		// 获取坐下失败原因
		std::string getSitErrorReason(int handleCode);
	};

#define RoomLogic() HNRoomLogic::getInstance()
}

#endif	//__HN_HNPlatformLogic_H__

