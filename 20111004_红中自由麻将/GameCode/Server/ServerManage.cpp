#include "StdAfx.h"
#include "ServerManage.h"
#include <shlobj.h>


int CServerGameDesk::m_iBaseMoney = 0;
/*
* 打印日志文件
*/
void DebugPrintf(int iRoomID, const char *p, ...)
{
	return ;
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d_%d-%d%d%d-log.txt", NAME_ID, iRoomID, time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start(arg, p);
	vfprintf(fp, p, arg);
	fprintf(fp,"\n");
	fclose(fp);
}
/*------------------------------------------------------------------------------*/
/**
* 构造函数	DWJ
*/
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{	
	m_uSocketID		= 0;
	m_bGameStation	= GS_WAIT_ARGEE;
	m_byAgreeLeaveNum = 0;//同意结束游戏的玩家数量
	

    sCPGAction.SetDeskPoint(this);	//吃碰杠检测方法(获取父类指针)

    sCheckHuPai.SetDeskPoint(this);	//糊牌检测方法(获取父类指针)

    m_sThingHandle.SetDeskPoint(this);//事件处理(获取父类指针)
	m_iMaiMaCount = 4;

}
/*------------------------------------------------------------------------------*/
/**
* 析构函数
*/
CServerGameDesk::~CServerGameDesk(void)
{
}
/*------------------------------------------------------------------------------*/
/**
* 定时器消息
* @param [in] uTimerID 定时器ID
* @return 返回布尔类型
*/
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if (m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT)
	{
		m_sThingHandle.OnGameTimer(uTimerID);
	}
	return __super::OnTimer(uTimerID);
}
/*------------------------------------------------------------------------------*/
/**
* 获取游戏状态信息
* @param [in] bDeskStation 玩家位置
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser 旁观
* @return 返回布尔类型
*/
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	//保存断线重连时的socket
	m_uSocketID = uSocketID;
	//房间
	m_RoomId = m_pDataManage->m_InitData.uRoomID;
	
	CString cs;
	cs.Format("llj:server::m_bGameStation = %d",m_bGameStation);
	OutputDebugString(cs);

	m_sThingHandle.GetGameStaton(bDeskStation, uSocketID, bWatchUser, m_bGameStation);	

	return true;
}
/*-------------------------------------------------------------------------------------------------*/
/**
* 框架游戏数据包处理函数
* @param [in] bDeskStation 玩家位置
* @param [in] pNetHead 消息头
* @param [in] pData 消息体
* @param [in] uSize 消息包的大小
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser 旁观
* @return 返回布尔类型
*/
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			//不是强退托管 那么就是强退结束游戏
			if(sGameData.m_mjRule.bForceTuoGuan)
			{
				//sUserData.m_bTuoGuan[bDeskStation] = true;
				return true ; 
			}
			else
			{
				GameFinish(bDeskStation,GFF_FORCE_FINISH);//强退结束游戏
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
/*-------------------------------------------------------------------------------------------------*/
/**
* 游戏数据包处理函数
* @param [in] bDeskStation 玩家位置
* @param [in] pNetHead 消息头
* @param [in] pData 消息体
* @param [in] uSize 消息包的大小
* @param [in] uSoketID Socket ID
* @param [in] bWatchUser 旁观
* @return 返回布尔类型
*/
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	KillTimer(TIME_CHECK_GAME_MSG);
	SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//重新设置超时计时器

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:		//用户同意游戏
		{
			if (bWatchUser)
			{
				return FALSE;
			}
			if(NULL != m_pUserInfo[bDeskStation])
			{
				m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;  //玩家置为同意状态
			}
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		      }
	}
	if(pNetHead->bMainID == MDM_GM_GAME_NOTIFY)//接收游戏消息
	{
		m_sThingHandle.HandleNotifyMessage(bDeskStation,pNetHead->bAssistantID,(BYTE*)pData,uSize,bWatchUser);
	}
	if(pNetHead->bMainID == 180)
	{
		return true;
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/**
* 重置游戏状态
* @param [in] bLastStation 游戏结束标志
* @return 返回布尔类型
*/
bool	CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	return TRUE;
}
/*-------------------------------------------------------------------------------------------------*/
/**
* 初始化游戏
* @return 返回布尔类型
*/
bool	CServerGameDesk::InitDeskGameStation()
{
	//加载房间设置

	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f(s + SKIN_FOLDER  + _T("_s.ini"));

	CString key;
	key = TEXT("game");
	m_iBaseMoney = f.GetKeyVal(key,"BaseMoney",10); //底分设置

	m_iMaiMaCount = f.GetKeyVal(key,"MaiMaCount",2); //买码个数

	if (m_iMaiMaCount <= 0)
	{
		m_iMaiMaCount = 1;
	}
	else if (m_iMaiMaCount >= 4)
	{
		m_iMaiMaCount = 4;
	}

	m_i23ZhongCount = f.GetKeyVal(key,"23ZhongCount",8); //买码2、3、中出现的个数


	key = TEXT("SuperSet");
	int iCount = f.GetKeyVal(key,"SuperCount",0);
	long int iSuperId = 0;
	CString sText;

	for(int i=0; i<iCount; i++)
	{
		sText.Format("SuperID_%d",i);
		iSuperId = f.GetKeyVal(key,sText,0);
		m_vlSuperUserID.push_back(iSuperId);
	}

	LoadiniByRoom(m_pDataManage->m_InitData.uRoomID);
	return true;
}
/*-------------------------------------------------------------------------------------------------*/
/**
* 游戏开始
* @param [in] bBeginFlasg 开始模式标志
*/
bool	CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag) == false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}

	m_bGameStation = GS_SEND_CARD;
    sGameData.ApplyThingID = THING_GAME_BEGIN;
    //请求事件
    m_sThingHandle.ApplyAction(THING_GAME_BEGIN,200);
	return true;
}
void CServerGameDesk::SetParamaterStation(BYTE byStation)
{
	m_bGameStation = byStation;
}
/*-------------------------------------------------------------------------------------------------*/
/**
* 游戏结束
* @param [in] bDeskStation 玩家位置
* @param [in] bCloseFlag 游戏结束标志
* @return 返回布尔类型
*/
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//游戏正常结束
		{
			m_bGameStation = GS_WAIT_NEXT;			
    		//计算胜负分数
			for (int i = 0;i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
				{
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
				}
			}

			__int64   i_ChangePoint[PLAY_COUNT];     //积分
			__int64   i_ChangeMoney[PLAY_COUNT];     //金币
			__int64   i_ReturnMoney[PLAY_COUNT];     //返回金币			

			memset(i_ChangePoint, 0, sizeof(i_ChangePoint));
			memset(i_ChangeMoney, 0, sizeof(i_ChangeMoney));
			memset(i_ReturnMoney, 0, sizeof(i_ReturnMoney));

			// 服务器写入数据库
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut, 0, sizeof(temp_cut));
	
			for(int i=0;i<PLAY_COUNT;++i)
			{
				i_ChangePoint[i] = sGameData.T_CountFen.iZongFen[i];
			}

			// 扣所有赢家税
			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(i_ChangeMoney);

			//扣税后获得的金币
			for(int i=0;i<PLAY_COUNT;++i)
			{
				sGameData.T_CountFen.i64Money[i] = i_ChangeMoney[i];
			}

			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//是否显示服务费
			sGameData.T_CountFen.iBasePoint = m_pDataManage->m_InitData.uBasePoint;//倍数

			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//
			KillTimer(TIME_CHECK_GAME_MSG);
				
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;
			//发送数据
			for (int i=0; i<PLAY_COUNT; i++)     
			{
				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_HANDLE, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_HANDLE, 0); 
			}

			bCloseFlag = GF_NORMAL;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_SAFE_FINISH:
		{
			if(bDeskStation==255)
			{
				bDeskStation=0;
			}
			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//
			KillTimer(TIME_CHECK_GAME_MSG);

			int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//倍数
			for (int i=0; i<m_bMaxPeople; i++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}				
			bCloseFlag	= GFF_FORCE_FINISH;

			//写入数据库				
			__int64 i_ChangePoint[PLAY_COUNT];
			__int64 iWinMoney[8] ={0};
			bool temp_cut[PLAY_COUNT];
			memset(i_ChangePoint,0,sizeof(i_ChangePoint));
			memset(temp_cut,0,sizeof(temp_cut));
	
			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(iWinMoney);
			for (int i=0;i<PLAY_COUNT;i++)
			{
				sGameData.T_CountFen.i64Money[i] = iWinMoney[i]; 
			}
			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//是否显示服务费
			
			///拷贝手牌数据
			sUserData.CopyHandPai(sGameData.T_CountFen.m_byArHandPai,0,true);
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;
			///发送数据
			for (int i=0; i<PLAY_COUNT; i++)     
			{
				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);
			}
			break;
		}
	case GFF_FORCE_FINISH:
		{
			// 为方便地让用户离开而新添加的代码
			// 判断该玩家是否压了注，是否庄家
			if(bDeskStation==255 || m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_NEXT)
			{
				return true;
			}
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			KillTimer(TIME_WAIT_MSG);
			KillTimer(10);//
			KillTimer(TIME_CHECK_GAME_MSG);
			bCloseFlag	= GFF_FORCE_FINISH;
			//int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//倍数

			//写入数据库				
			__int64 i_ChangePoint[PLAY_COUNT]={0};
			__int64 iWinMoney[8] ={0};
			bool temp_cut[PLAY_COUNT] = {0};
			//乘以基础分
			i_ChangePoint[bDeskStation] = -(__int64)m_pDataManage->m_InitData.uRunPublish;
			temp_cut[bDeskStation] = true;

			ChangeUserPointint64(i_ChangePoint, temp_cut);
			__super::RecoderGameInfo(iWinMoney);
			for (int i=0;i<PLAY_COUNT;i++)
			{
				sGameData.T_CountFen.i64Money[i] = iWinMoney[i]; 
			}
			sGameData.T_CountFen.bIsShowTax = sGameData.m_bShowTax;//是否显示服务费



			///拷贝手牌数据
			sUserData.CopyHandPai(sGameData.T_CountFen.m_byArHandPai,0,true);
			sGameData.T_CountFen.byCloseFlag = bCloseFlag;

			///发送数据
			for (int i=0; i<PLAY_COUNT; i++)     
			{
				SendGameData(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);   
				SendWatchDataEx(i, &sGameData.T_CountFen, sizeof(sGameData.T_CountFen), MDM_GM_GAME_NOTIFY, THING_ENG_UN_NORMAL, 0);
			}
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	default:
		{
			break;
		}
	}
	// 重置数据
	ReSetGameState(bCloseFlag);
	return __super::GameFinish(bDeskStation,bCloseFlag);
}
/*-----------------------------------------------------------------------------------*/
/**
* 判断是否正在游戏
* @param [in] bDeskStation 玩家位置
* @return 返回当前游戏状态
*/
bool	CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT)
	{
		return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------------*/
/**
* 用户离开游戏桌
* @return 返回用户离开状态
*/
BYTE	CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_sThingHandle.UserLeftDesk(bDeskStation);
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
/*-----------------------------------------------------------------------------------*/
//用来改变用户断线条件的函数
bool	CServerGameDesk::CanNetCut(BYTE bDeskStation)
{
	return true;
}
/*-----------------------------------------------------------------------------------*/
//用户断线离开
bool	CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	//在游戏中，有断线托管就进行托管
	if (m_bGameStation	>= GS_SEND_CARD && m_bGameStation	< GS_WAIT_NEXT )  
	{

		//sUserData.m_bTuoGuan[bDeskStation] = true;
		//m_sThingHandle.UserNetCut(bDeskStation);

		//sUserData.m_byFoceLeavCount = 0;
		//for(int i=0;i<4;++i)
		//{
		//	if(sUserData.m_bFoceLeave[i])//强退
		//	{
		//		sUserData.m_byFoceLeavCount++;
		//	}
		//}
  //      if (sUserData.m_byFoceLeavCount >= PLAY_COUNT && sUserData.m_byFoceLeavCount != 255) //四家都强行离开了
  //      {
  //          GameFinish(0,GF_NORMAL); //用户强行离开
  //          return true;
  //      }
	}
	//记录断线玩家（在重连前部给该玩家发送消息）
	return __super::UserNetCut(bDeskStation, pLostUserInfo);
}
/*-----------------------------------------------------------------------------------*/
/**
* 玩家坐下
* @return 返回用户坐下状态
*/
BYTE	CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	BYTE result = __super::UserSitDesk(pUserSit, pUserInfo);
	return result;
}
/*-----------------------------------------------------------------------------------*/
///设置游戏定时器
void CServerGameDesk::SetGameTimer(UINT iId,int iTime)
{
	SetTimer(iId,iTime);
}
/*-----------------------------------------------------------------------------------*/
//删除指定计时器
void CServerGameDesk::KillGameTimer(UINT iId)
{
	KillTimer(iId);
}
/*-----------------------------------------------------------------------------------*/
///发送游戏状态
void	CServerGameDesk::SendGameStationEx(BYTE nDeskStation, bool bWatchUser, void * pStationData, UINT uSize)
{
	// 发送数据
	SendGameStation(nDeskStation,m_uSocketID,bWatchUser,pStationData,uSize);
}
///发送数据函数 （发送消息给游戏者）
void	CServerGameDesk::SendGameDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendGameData(nDeskStation,MDM_GM_GAME_NOTIFY,bAssID,0) ; 
}
///发送旁观数据 （发送消息给旁观者）
void	CServerGameDesk::SendWatchDataEx(BYTE nDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendWatchData(nDeskStation,MDM_GM_GAME_NOTIFY,bAssID,0) ; 
}
///发送数据函数 （发送消息给游戏者）
void	CServerGameDesk::SendGameDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendGameData(nDeskStation, pData, uSize, MDM_GM_GAME_NOTIFY, bAssID, 0);
}
///发送旁观数据 （发送消息给旁观者）
void	CServerGameDesk::SendWatchDataEx(BYTE nDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode)
{
	SendWatchData(nDeskStation, pData, uSize, MDM_GM_GAME_NOTIFY, bAssID, 0);
}
/*-----------------------------------------------------------------------------------*/
//加载房间设置
void	CServerGameDesk::LoadiniByRoom(int iRoomId)
{
	char key[MAX_PATH];
	CString s = CINIFile::GetAppPath ();/////本地路径    
	CINIFile f(s +_T("\\")+SKIN_FOLDER  + _T("_s.ini"));
	sprintf(key,"%d_%d",NAME_ID,iRoomId);

	sGameData.m_mjRule.byAutoOutTime	= f.GetKeyVal(key,TEXT("byAutoOutTime"),sGameData.m_mjRule.byAutoOutTime);	//自动出牌时间	DWJ
	sGameData.m_mjRule.byOutTime		= f.GetKeyVal(key,TEXT("byOutTime"),sGameData.m_mjRule.byOutTime);			//出牌时间		DWJ
	sGameData.m_mjRule.byBlockTime		= f.GetKeyVal(key,TEXT("byBlockTime"),sGameData.m_mjRule.byBlockTime);		//拦牌思考时间	DWJ
	sGameData.m_mjRule.byBeginTime		= f.GetKeyVal(key,TEXT("byBeginTime"),sGameData.m_mjRule.byBeginTime);		//开始等待时间	DWJ
	sGameData.m_mjRule.bAutoBegin		= f.GetKeyVal(key,TEXT("bAutoBegin"),sGameData.m_mjRule.bAutoBegin);		//时间到了是否自动开始

	sGameData.m_mjRule.bNetCutTuoGuan	= f.GetKeyVal(key,TEXT("bNetCutTuoGuan"),sGameData.m_mjRule.bNetCutTuoGuan);	//是否断线托管
	sGameData.m_mjRule.byGamePassNum	= f.GetKeyVal(key,TEXT("GamePassNum"),sGameData.m_mjRule.byGamePassNum);;		//流局牌数
	//sprintf(key,"game");
	//m_iBaseMoney = f.GetKeyVal(key,TEXT("BaseMoney"),10); //底分设置
	//
	//sprintf(key,"SuperSet");
	//int iCount = f.GetKeyVal(key,TEXT("SuperCount"),0);
	//long int iSuperId = 0;
	//CString sText;

	//for(int i=0; i<iCount; i++)
	//{
	//	sText.Format("SuperID_%d",i);
	//	iSuperId = f.GetKeyVal(key,sText,0);
	//	m_vlSuperUserID.push_back(iSuperId);
	//}

	///查找上层路径
	bool  bForceQuitAsAuto = true ; 
	CString szAppPath = CINIFile::GetAppPath();

	CString strBCFFile  ; 
	CString strKeyName  ; 
	strKeyName.Format("%d" ,NAME_ID) ; 
	strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

	CBcfFile File(strBCFFile) ;

	if (File.IsFileExist(strBCFFile))
	{
		bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 
	}
	sGameData.m_mjRule.bForceTuoGuan	= bForceQuitAsAuto;		//是否强退托管
}
/*-----------------------------------------------------------------------------*/
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//修改奖池数据1(平台->游戏)
/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
/// @return 是否成功
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return false;
}

//修改奖池数据2(平台->游戏)
/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return false;
}

//修改奖池数据3(平台->游戏)
/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return false;
}

//判断
BOOL CServerGameDesk::Judge()
{
	if(JudgeWiner())		//先处理胜者
	{
		return TRUE;
	}

	if (JudgeLoser())		//处理输者
	{
		return TRUE;
	}		

	return FALSE;
}
//================================================
//判断胜者
//================================================
BOOL CServerGameDesk::JudgeLoser()
{
	return FALSE;
}
//================================================
//判断胜者
//================================================
BOOL CServerGameDesk::JudgeWiner()
{
	return FALSE;
}
//GRM函数
//更新奖池数据(游戏->平台)
/// @return 机器人赢钱数
void CServerGameDesk::UpDataRoomPond(__int64 iAIHaveWinMoney)
{

}
