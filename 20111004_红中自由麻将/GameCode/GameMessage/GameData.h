#pragma once
#include "../ChangeFile/UpgradeMessageEx.h"

///游戏数据管理类:配置数据，游戏基本数据等，对数据进行插入修改等动作
#pragma pack(1)
class GameData
{
public:
	GameData(void);
	~GameData(void);
	
public:	   

	BYTE					m_byNtStation;	//	庄家位置	DWJ

	int						m_iHuangZhuangCount;	//荒庄次数	DWJ

	BYTE					ApplyThingID;	///要申请的事件	DWJ
	
	BYTE					m_byThingDoing;	///正在发生的事件号  
	
	BYTE					m_byWatingThing;//当前等待的事件ID	DWJ

	MjRuleSet				m_mjRule;		/// 麻将常规属性	DWJ

	bool					m_bGangState[PLAY_COUNT];//记录杠牌状态，拥于杠开和杠后炮

	bool					m_bShowTax;//是否显示台费

	int						m_iRemaindTime;	//倒计时剩余时间(客户端用)




	/// 麻将动作属性
	ActionRuleSet			m_mjAction;               
	/// 麻将算分属性
	CountFenRuleSet			m_mjFenRule;				
	/// 麻将糊牌属性
	HuPaiRuleSet			m_mjHuRule;				
	/// 麻将糊牌类型
	HuPaiTypeSet			m_mjHuType;				
	///记录游戏所有的流程事件（抓牌，出牌，吃碰杠糊，补花等，最后一个元素299记录事件总数）
	BYTE					m_byThingRecord[300];

	/// 每个事件的后接事件（最大100个事件）
	BYTE					m_byThingNext[100];



	////////////基本数据定义/////////////////////////////////////////////////////////
	
	

	///待执行事件
	BYTE					m_byNextAction;
				
	///房间的倍率
	int						basepoint;	
	
	

	///吃牌牌事件,临时数据
	tagChiPaiEx				temp_ChiPai;
	///碰牌牌事件,临时数据
	tagPengPaiEx			temp_PengPai;
	///杠牌牌事件,临时数据
	tagGangPaiEx			temp_GangPai;
	////糊牌牌事件,临时数据

	
public:////////////事件数据对象定义//////////////////////
	///游戏开始事件	DWJ
	tagBeginEx				T_Begin;
	///以东为庄事件	DWJ
	tagDongNtEx				T_DongNt;
	///掷2颗色子确定起牌位置事件	DWJ
	tagTwoSeziDirEx			T_TwoSeziDir;
	///掷2颗色子确定起牌位置（点数和）和起牌点（最小点）事件	DWJ
	tagTwoSeziDirAndGetPaiEx	T_TwoSeziDirAndGetPai;
	///发牌事件	DWJ
	tagSendPaiEx			T_SendPai;
	///所有玩家补花事件	DWJ
	tagAllBuHuaEx			T_AllBuHua;
	///开始出牌通知事件	DWJ
	tagBeginOutPaiEx		T_BeginOutPai;
	///出牌事件	DWJ
	tagOutPaiEx				T_OutPai;
	///吃碰杠糊牌通知事件牌事件	DWJ
	tagCPGNotifyEx			T_CPGNotify[PLAY_COUNT];
	///抓牌牌事件	DWJ	
	tagZhuaPaiEx			T_ZhuaPai;
	///单个玩家补花事件	DWJ
	tagOneBuHuaEx			T_OneBuHua;



	///吃牌牌事件	DWJ
	tagChiPaiEx				T_ChiPai;
	///碰牌牌事件	DWJ
	tagPengPaiEx			T_PengPai;
	///杠牌牌事件	DWJ
	tagGangPaiEx			T_GangPai;
	///听牌牌事件	DWJ
	tagTingPaiEx			T_TingPai;
	///糊牌牌事件	DWJ
	tagHuPaiEx				T_HuPai;
	///算分事件		DWJ
	tagCountFenEx			T_CountFen;













	///掷2颗色子的点数和为庄事件
	tagTwoSeziNtEx			T_TwoSeziNt;
	///掷2颗色子确定庄家和起牌点位置事件
	tagTwoSeziNtAndGetPaiEx	T_TwoSeziNtAndGetPai;
	///轮流掷2颗色子确定庄家
	tagTurnSeziNtEx			T_TurnSeziNt;
	
	
	///掷2颗色子确定起牌敦数事件
	tagTwoSeziGetPaiEx		T_TwoSeziGetPai;
	
	///跳牌事件
	tagTiaoPaiEx			T_TiaoPai;
	///掷色子2颗色子定精牌事件
	tagTwoSeziJingEx		T_TwoSeziJing;
	///掷色子1颗色子定金牌事件
	tagOneSeziJinEx			T_OneSeziJin;
	
	
	
	
	///游戏结束处理事件
	tagEndHandleEx			T_EndHandle;
	///非正常结束处理事件
	tagUnnormalEndHandleEx   T_UnnormalEndHandle;


	

public:

	///设置所有事件的后接事件
	virtual void SetThingNext();
	///设置某个事件的后接事件
	virtual void SetThingNext(BYTE id);
	//virtual 初始化数据
	virtual void InitData();
	///初始化数据
	virtual void LoadIni();

};
#pragma pack()