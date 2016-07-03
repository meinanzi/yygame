#include "stdafx.h"
#include "ThingHandleEx.h"
#include "../server/ServerManage.h"
#include "../server/DebugPrintf.h"

//全局对象
//ThingHandleEx g_ThingHandle;
	
ThingHandleEx::ThingHandleEx(void)
{
}
ThingHandleEx::~ThingHandleEx(void)
{
}
/*-------------------------------------------------------------------------------------------------------------*/
//断线重连事件
bool ThingHandleEx::GetGameStaton(BYTE bDeskStation, UINT uSocketID, bool bWatchUser,BYTE GameStation)
{
	//GameStationDataEx GameStationData;
	
	switch (GameStation)
	{
	case GS_WAIT_SETGAME:	//游戏没有开始状态
	case GS_WAIT_ARGEE:		//等待玩家开始状态
	case GS_WAIT_NEXT:		//等待下一盘游戏开始
		{
			GameStation_Wait_Agree TGameStation;

			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;

			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//开始等待时间
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//出牌时间
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//思考时间
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//强退是否托管

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.dwUserID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}

			for (int i=0; i<PLAY_COUNT; i++)
			{
				if(NULL != pDesk->m_pUserInfo[i])
				{
					if(pDesk->m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)  //玩家置为同意状态
					{
						TGameStation.bUserAgree[i] = true;
					}
				}
			}

			//发送数据
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	
	case GS_SEND_CARD:		//发牌状态(还未发牌)
		{
			//发送数据
			//pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			GameStationData TGameStation;
			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;
			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//开始等待时间
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//出牌时间
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//思考时间
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//强退是否托管

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.bLogoID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}
			TGameStation.byNtStation = pDesk->sGameData.m_byNtStation;
			//TGameStation.m_byNowOutStation = pDesk->sUserData.m_byNowOutStation;
			//pDesk->sUserData.m_bTuoGuan[bDeskStation] = false;
			memcpy(TGameStation.m_bistuoguan,pDesk->sUserData.m_bTuoGuan,sizeof(TGameStation.m_bistuoguan));
			memcpy(TGameStation.m_byArHandPai,pDesk->sUserData.m_byArHandPai,sizeof(TGameStation.m_byArHandPai));
			memcpy(TGameStation.m_iArMenPai,pDesk->sUserData.m_iArMenPai,sizeof(TGameStation.m_iArMenPai));

			//发送数据
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	case GS_PLAY_GAME:	//游戏中状态
		{
			//if(pDesk->sUserData.m_bIsHu[bDeskStation])//玩家已经胡牌了
			//{
			//	
			//}
			//for(int i=0;i<4;++i)//玩家是否已经胡牌
			//{
			//	if(pDesk->sUserData.m_bIsHu[i])//玩家已经胡牌了
			//	{
			//		GameStationData.m_bIsHu[i] = true;
			//		for(int j=0;j<17;j++)
			//		{
			//			GameStationData.m_byArHandPai[i][j] = pDesk->sUserData.m_byArHandPai[i][j];
			//		}
			//	}
			//}

			GameStationDataEx TGameStation;
			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;
			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//开始等待时间
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//出牌时间
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//思考时间
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//强退是否托管

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.bLogoID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}
			TGameStation.byNtStation = pDesk->sGameData.m_byNtStation;
			TGameStation.m_byNowOutStation = pDesk->sUserData.m_byNowOutStation;
			//pDesk->sUserData.m_bTuoGuan[bDeskStation] = false;
			memcpy(TGameStation.m_bistuoguan,pDesk->sUserData.m_bTuoGuan,sizeof(TGameStation.m_bistuoguan));
			memcpy(TGameStation.m_byArHandPai,pDesk->sUserData.m_byArHandPai,sizeof(TGameStation.m_byArHandPai));
			memcpy(TGameStation.m_iArMenPai,pDesk->sUserData.m_iArMenPai,sizeof(TGameStation.m_iArMenPai));
			memcpy(TGameStation.m_byArOutPai,pDesk->sUserData.m_byArOutPai,sizeof(TGameStation.m_byArOutPai));
			memcpy(TGameStation.m_UserGCPData,pDesk->sUserData.m_UserGCPData,sizeof(TGameStation.m_UserGCPData));
			//memcpy(TGameStation.m_byHandPaiCount,pDesk->sUserData.m_byArHandPaiCount,sizeof(TGameStation.m_byHandPaiCount));
			//memcpy(TGameStation.m_byOutPaiCount,pDesk->sUserData.m_byArOutPaiCount,sizeof(TGameStation.m_byOutPaiCount));

			/*for(int i = 0; i < PLAY_COUNT; i++)
			{
			pDesk->sUserData.CopyHandPai(TGameStation.m_byArHandPai,i,true);
			pDesk->sUserData.CopyMenPai(i,TGameStation.m_iArMenPai[i]);
			}*/
			//发送数据
			//pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			//if(!pDesk->sUserData.m_bIsHu[bDeskStation] && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing > ACTION_NO && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing <= ACTION_HU)//如果有吃碰杠就进行处理
			//{
			//	pDesk->SendGameDataEx(bDeskStation, &pDesk->sGameData.T_CPGNotify[bDeskStation], sizeof(pDesk->sGameData.T_CPGNotify[bDeskStation]), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
			//}
			//发送数据
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	
	}
    return true;
}
/*---------------------------------------------------------------------------------------------------------*/
///接收并处理消息
int ThingHandleEx::HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser)
{
	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	pDesk->SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//重新设置超时计时器

	if(bWatchUser)
	{
		return 0;
	}

    switch (code)
    {
	case ASS_GM_AGREE_GAME:		//	收到用户同意游戏消息	DWJ
		{
			return	ReceiveUserAgreeGame(bDeskStation, pData, uSize,	bWatchUser);
			break;
		}
	case ASS_AUTOCARD:		//托管		DWJ
		{	
			return ReceiveUserTuoGuan(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case ASS_CHANGE_PAI:	//超端玩家换牌 手机可以考虑
		{
			return	ReceiveUserChangeCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_OUT_PAI:		//	用户出牌	DWJ
		{
			return	ReceiveUserOutCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_GUO_PAI:		//	放弃拦牌	DWJ
		{
			return	ReceiveUserPassCheck(bDeskStation, pData, uSize,	bWatchUser);
			break;
		}
	case THING_CHI_PAI:		//	玩家请求吃牌   DWJ
		{
			return	ReceiveUserChiCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_PENG_PAI:	//	用户碰牌	DWJ
		{
			return	ReceiveUserPengCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_GANG_PAI:	//	用户杠牌	DWJ
		{
			return	ReceiveUserGangCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_TING_PAI:	// 用户听牌		DWJ
		{
			return	ReceiveUserTingCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_HU_PAI:		// 用户糊牌		DWJ
		{
			return	ReceiveUserHuCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
#ifdef MJ_CAN_SETPAI//超级客户端抓牌，发牌
    case ASS_SUPER://超级客户端,进行牌设置
        {
			ReceiveSuperChangePai(bDeskStation,pData, uSize, bWatchUser);
        }
        break;
#endif // MJ_CAN_SETPAI
    }
    return 0;
}





/*---------------------------------------------------------------------------------------------------------*/
/*--------------------处理接收消息-------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
//收到用户同意游戏消息
int		ThingHandleEx::ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserAgreeGame(bDeskStation, pData, uSize, bWatchUser);
}
//是否超端用户
bool	ThingHandleEx::IsSuperUser(BYTE bDeskStation)
{
	if (bDeskStation < 0 || bDeskStation >= PLAY_COUNT)
	{
		return false;
	}
	if (NULL == pDesk->m_pUserInfo[bDeskStation])
	{
		return false;
	}
	for(int i=0; i<pDesk->m_vlSuperUserID.size(); i++)
	{
		if (pDesk->m_pUserInfo[bDeskStation]->m_UserData.dwUserID == pDesk->m_vlSuperUserID.at(i))
		{
			return true;
		}
	}
	return false;
}
//玩家换牌
int		ThingHandleEx::ReceiveUserChangeCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagSuperChangeCard))
	{
		return 0;
	}

	tagSuperChangeCard * pChangeCard = (tagSuperChangeCard *)pData;

	if(pChangeCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if (pChangeCard->byChangeCard == 0 || pChangeCard->byChangeCard == 255 || pChangeCard->byNewCard == 0 || pChangeCard->byNewCard == 255)
	{
		return 0;
	}
	//是否超端玩家
	if (!IsSuperUser(bDeskStation) && 0)
	{
		return 0;
	}
	tagSuperChangeCard TSuperChangeCard;
	//手牌中有这张牌&&牌墙中有这张牌才能换牌
	if (pDesk->sUserData.IsHaveAHandPai(bDeskStation,pChangeCard->byChangeCard) && pDesk->sUserData.IsHaveAMenPai(pChangeCard->byNewCard))
	{

		if (pDesk->sUserData.ChangeAHandPai(bDeskStation,pChangeCard->byChangeCard,pChangeCard->byNewCard) && pDesk->sUserData.ChangeAMenPai(pChangeCard->byNewCard,pChangeCard->byChangeCard))
		{

			TSuperChangeCard.bSuccese = true;
			TSuperChangeCard.byArHandPaiCount = pDesk->sUserData.CopyOneUserHandPai(TSuperChangeCard.byArHandPai,bDeskStation);
		}
	}

	pDesk->SendGameDataEx(bDeskStation, &TSuperChangeCard, sizeof(TSuperChangeCard), MDM_GM_GAME_NOTIFY, ASS_CHANGE_PAI_RESULT, 0);
	pDesk->SendWatchDataEx(bDeskStation, &TSuperChangeCard, sizeof(TSuperChangeCard), MDM_GM_GAME_NOTIFY, ASS_CHANGE_PAI_RESULT, 0);

	return 0;

}
/*---------------------------------------------------------------------------------------------------------*/
// 收到托管消息
int		ThingHandleEx::ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(UserTouGuan))
	{
		return 0;
	}
	if(bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)//不在游戏中不能托管
	{
		return 0;
	}

	UserTouGuan *pUserTouGuan = (UserTouGuan *)pData;
	if(pUserTouGuan == NULL)
	{
		return 0;
	}

	pDesk->sUserData.m_bTuoGuan[bDeskStation] = pUserTouGuan->bIsTuoGuan;

	pDesk->m_byAutoTime[bDeskStation] = 0;

	// 各个玩家托管状态
	UserTouGuan	TUserTouGuan;
	TUserTouGuan.byStation	= pUserTouGuan->byStation;
	TUserTouGuan.bIsTuoGuan	= pUserTouGuan->bIsTuoGuan;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		TUserTouGuan.bTuoGuan[i] = pDesk->sUserData.m_bTuoGuan[i];		
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
		pDesk->SendWatchDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
	}

	//是否正在等待自己的事件 && 是托管事件
	if(pDesk->sGameData.m_MjWait.byCurChair == pUserTouGuan->byStation && pUserTouGuan->bIsTuoGuan && pDesk->sGameData.m_MjWait.byTime > 1)
	{
		//1秒后立马出牌
		pDesk->sGameData.m_MjWait.byTime = 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户出牌
int		ThingHandleEx::ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagOutPaiEx))
	{
		return 0;
	}
	tagOutPaiEx * pOutCard = (tagOutPaiEx *)pData;

	if(pOutCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		DebugPrintf("用户 %d 出牌失败，非游戏状态", bDeskStation);
		return 0;
	}


	if(pOutCard->byPs == 255 || pDesk->sUserData.GetAHandPaiCount(pOutCard->byUser,pOutCard->byPs)==0 || !pDesk->sUserData.IsOutPaiPeople(pOutCard->byUser))
	{
		DebugPrintf("用户 %d 出牌失败，没有此牌", bDeskStation);
		return 0;
	}

	//记录正在执行的事件
	pDesk->sGameData.m_byThingDoing = THING_OUT_PAI;
//	pDesk->sGameData.T_HuPai.Clear();//清理浒牌记录 2012-08-15

	//记录杠牌状态，拥于杠开
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//if(i != bDeskStation)
		{
			pDesk->sGameData.m_bGangState[i] = false;
		}
	}


	//记录已经出过牌了(用于天胡地胡检测)
	pDesk->sUserData.m_bHaveOut[bDeskStation] = true;

	//先清理出牌数据
	pDesk->sGameData.T_OutPai.Clear();

	pDesk->sGameData.T_OutPai = * pOutCard;
	

	//删除一张手牌
	pDesk->sUserData.DelleteAHandPai(pOutCard->byUser,pOutCard->byPs);
	//添加一张出牌
	pDesk->sUserData.AddToOutPai(pOutCard->byUser,pOutCard->byPs);
	//清理临时数据
	ClearTempData();

	pDesk->sUserData.m_byBuGangPai = 255;//补杠得分的牌（用在补杠，抓到能补杠的牌后马上补杠才有分，否则无分）

	//玩家点击了听牌
	if(pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] && !pDesk->sUserData.m_bTing[pOutCard->byUser])//玩家点击了听牌,
	{
		if(CLogicBase::IsHaveInArr(pDesk->sGameData.T_CPGNotify[pOutCard->byUser].m_byTingCanOut,pOutCard->byPs,HAND_CARD_NUM))
		{
			pDesk->sUserData.m_bTing[pOutCard->byUser] = true;
			pDesk->sGameData.T_OutPai.bTing = true;
		}
		else
		{
			pDesk->sUserData.m_bTing[pOutCard->byUser] = false;
		}
		pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] = false;
	}

	//出完牌后 排序该玩家的手牌
	pDesk->sUserData.SortHandPai(pOutCard->byUser,false);

	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//出牌数据
		pDesk->sGameData.T_OutPai.m_byArOutPaiCount[i] = pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_OutPai.m_byArOutPai[i]);
		//玩家的手牌数量
		pDesk->sGameData.T_OutPai.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		//获取听牌状态
		pDesk->sGameData.T_OutPai.bUserTing[i] = pDesk->sUserData.m_bTing[i];
	}

	//发送出牌数据
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_OutPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_OutPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OutPai.m_byArHandPai,pOutCard->byUser,true);
		//手牌数据
		//pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OutPai.m_byArHandPai,i,pDesk->sUserData.m_bTing[i]);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
	}

	//动作检测：没有人能响应就直接抓牌	
	if(!CheckAction(pOutCard->byUser,pOutCard->byPs,2))
	{
		//没有动作就直接下一个玩家抓牌
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(THING_ZHUA_PAI,100);	//申请下一个事件
	}
	else
	{
		//发送了拦牌给客户端玩家 
		if(HaveUserCanDoAction(bDeskStation))
		{

		}
		else
		{
			//么有发送了拦牌给客户端玩家 就启动抓牌事件
			//没有动作就直接下一个玩家抓牌
			pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
			ApplyAction(THING_ZHUA_PAI,100);	//申请下一个事件
		}
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 收到用户pass 消息
int		ThingHandleEx::ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser,bool bAuto)
{
	if(pDesk->sGameData.ApplyThingID != 255 || bWatchUser)//有申请事件就不响应pass消息
	{
		return 0;
	}
	//玩家已经响应了此动作 
	if (pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn || !pDesk->sGameData.T_CPGNotify[bDeskStation].bCanAction)
	{
		return 0;
	}

	//当前不是等待该玩家
	if(pDesk->sGameData.m_MjWait.byCurChair != bDeskStation)
	{
		return 0;
	}

/*	if(bDeskStation != pDesk->sUserData.m_byNowOutStation && pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)//玩家能糊没有糊牌,记录弃糊
	{
		pDesk->sUserData.m_bQiHu[bDeskStation] = true;//玩家是否弃糊状态
	}
*/

	pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn = true;//记录玩家回应
	pDesk->sGameData.T_CPGNotify[bDeskStation].Clear();

	//返回过牌结果消息给客户端 THING_GUO_PAI
	pDesk->SendGameDataEx(bDeskStation, MDM_GM_GAME_NOTIFY, THING_GUO_PAI, 0);
	pDesk->SendWatchDataEx(bDeskStation, MDM_GM_GAME_NOTIFY, THING_GUO_PAI, 0);

	//其他人是否有可以做的动作
	if(HaveUserCanDoAction(bDeskStation))
	{
		OutputDebugString("dwjsdmj::其他人还有动作");
	}
	else
	{
		//是否有人胡牌了
		bool	bHaveHuPai = false;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			if(pDesk->sGameData.T_HuPai.bHaveHu[i])
			{
				bHaveHuPai = true;
				break;
			}
		}

		//有人胡牌了 就结束
		if(bHaveHuPai)
		{
			//游戏结束
			pDesk->sGameData.ApplyThingID	= THING_COUNT_FEN;
			ApplyAction(pDesk->sGameData.ApplyThingID,200);
		}
		else
		{
			//是否玩家出牌
			bool bIsOutPai = false;	
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(pDesk->sUserData.IsOutPaiPeople(i))//出牌玩家
				{
					bIsOutPai = true;
					break;
				}
			}

			//如果是出牌 那么就出牌
			if(bIsOutPai)
			{
				//如果是系统处理 那么直接就出牌了
				if(bAuto)
				{
					tagOutPaiEx outpai;
					outpai.Clear();
					outpai.byUser = bDeskStation;
					outpai.byPs = pDesk->sUserData.GetLastHandPai(bDeskStation);
					ReceiveUserOutCard(bDeskStation,&outpai,sizeof(outpai),false);
				}
				else
				{
					OutputDebugString("dwjsdmj::玩家主动取消拦牌后要出牌--时间为当前的剩余时间");
					//玩家主动取消拦牌后 这里重新启动出牌计时器 因为前面把拦牌数据清空了 否则服务端计时器到了 不会自动出牌的
					if(pDesk->sGameData.m_MjWait.byTime > 1)
					{
						//启动自动处理出牌数据
						SetDoingThing(bDeskStation,THING_OUT_PAI,pDesk->sGameData.m_MjWait.byTime-1);
					}
					else
					{
						//启动自动处理出牌数据
						SetDoingThing(bDeskStation,THING_OUT_PAI,1);
					}
				}
			}
			else	//不是出牌 就是抓牌
			{
				//下一个玩家抓牌
				pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
				ApplyAction(THING_ZHUA_PAI,100);	//申请下一个事件

			}
		}
	}
	
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户吃牌消息
int		ThingHandleEx::ReceiveUserChiCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}
	if(uSize != sizeof(tagChiPaiEx))
	{
		return 0;
	}
	tagChiPaiEx * pChiCard = (tagChiPaiEx *)pData;
	if(pChiCard == NULL )
	{
		return 0;
	}
	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bChi)
	{
		return 0;
	}

	if(pChiCard->byPs == 255 )
	{
		return 0;
	}

	if(!pDesk->sGameData.m_mjAction.bChi)
	{
		return 0;
	}

	//判断是否有其他玩家可以碰、杠、胡 什么的
	bool bCanChi = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bPeng || pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanChi = false;
			break;
		}
	}

	if(!bCanChi)
	{
		return 0;
	}

	
	//初始化吃牌数据
	pDesk->sGameData.T_ChiPai.Clear();
	memcpy(&pDesk->sGameData.T_ChiPai, pChiCard, sizeof(pDesk->sGameData.T_ChiPai));

	BYTE NextID=255,StarTime=255;
	HandleChiPai(NextID,StarTime);
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户碰牌消息
int		ThingHandleEx::ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagPengPaiEx))
	{
		return 0;
	}

	//判断是否有其他玩家可以胡 什么的
	bool bCanPeng = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanPeng = false;
			break;
		}
	}

	if(!bCanPeng)
	{
		return 0;
	}


	tagPengPaiEx * pPengPai = (tagPengPaiEx *)pData;
	if(pPengPai == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bPeng || pPengPai->byPs == 255 || !pDesk->sGameData.m_mjAction.bPeng)
	{
		return 0;
	}

	//初始化吃牌数据
	pDesk->sGameData.T_PengPai.Clear();
	memcpy(&pDesk->sGameData.T_PengPai, pPengPai, sizeof(pDesk->sGameData.T_PengPai));

	BYTE NextID=255,StarTime=255;
	HandlePengPai(NextID,StarTime);

	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户杠牌消息
int		ThingHandleEx::ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagGangPaiEx))
	{
		return 0;
	}

	//判断是否有其他玩家可以胡 什么的
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		return 0;
	}

	tagGangPaiEx * pGangPai = (tagGangPaiEx *)pData;
	if(pGangPai == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bGang || pGangPai->byPs == 255 || !pDesk->sGameData.m_mjAction.bGang)
	{
		return 0;
	}

	//初始化吃牌数据
	pDesk->sGameData.T_GangPai.Clear();
	memcpy(&pDesk->sGameData.T_GangPai, pGangPai, sizeof(pDesk->sGameData.T_GangPai));

	BYTE NextID=255,StarTime=255;
	HandleGangPai(NextID,StarTime);

	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户听牌消息
int		ThingHandleEx::ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return 0;//没有听牌
	if(uSize != sizeof(tagTingPaiEx))
	{
		return 0;
	}

	tagTingPaiEx * pTingCard = (tagTingPaiEx *)pData;
	if(pTingCard == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}
	
	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bTing || !pDesk->sGameData.m_mjAction.bTing)
	{
		return 0;
	}
	//玩家已经听牌了 
	if(pDesk->sUserData.m_bTing[pTingCard->byUser])
	{
		return 0;
	}

	//清除该玩家的所有其他动作
//	pDesk->sGameData.T_CPGNotify[pTingCard->byUser].Clear();
	//记录玩家已经点击了听牌
	pDesk->sGameData.T_TingPai.bHitTing[pTingCard->byUser] = pTingCard->bTing;
	//通知所有玩家
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TingPai, sizeof(pDesk->sGameData.T_TingPai), MDM_GM_GAME_NOTIFY, THING_TING_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_TingPai, sizeof(pDesk->sGameData.T_TingPai), MDM_GM_GAME_NOTIFY, THING_TING_PAI, 0);
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// 用户糊牌消息
int		ThingHandleEx::ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagHuPaiEx))
	{
		return 0;
	}

	tagHuPaiEx * pHuCard = (tagHuPaiEx *)pData;
	if(pHuCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)
	{
		return 0;
	}

	//别人点炮
	if(pDesk->sGameData.m_byThingDoing == THING_OUT_PAI && pDesk->sGameData.T_OutPai.byUser != 255)
	{
		pDesk->sGameData.T_HuPai.byDianPao	= pDesk->sGameData.T_OutPai.byUser;
		pDesk->sGameData.T_HuPai.byPs		= pDesk->sGameData.T_OutPai.byPs;
	}
	else if(pDesk->sGameData.m_byThingDoing == THING_GANG_PAI && pDesk->sGameData.T_GangPai.byUser != 255)	//抢杠胡
	{
		pDesk->sGameData.T_HuPai.bQiangGang	= true;
		pDesk->sGameData.T_HuPai.byDianPao	= pDesk->sGameData.T_GangPai.byUser;
		pDesk->sGameData.T_HuPai.byPs		= pDesk->sGameData.T_GangPai.byPs;
	}
	else if((pDesk->sGameData.m_byThingDoing == THING_ZHUA_PAI) || (pDesk->sGameData.m_byThingDoing == THING_ONE_BUHUA) || (pDesk->sGameData.m_byThingDoing == THING_BEGIN_OUT))	//自摸
	{
		pDesk->sGameData.T_HuPai.bZimo = true;
		pDesk->sGameData.T_HuPai.byDianPao = 255;
		pDesk->sGameData.T_HuPai.byPs = 255;
		//是否杠开
		if(pDesk->sGameData.m_bGangState[pHuCard->byUser])
		{
			pDesk->sGameData.T_HuPai.bGangKai = true;
			//杠开算 放杠玩家点炮
			pDesk->sGameData.T_HuPai.byDianPao = pDesk->sGameData.T_OutPai.byUser;
		}
	}
	//胡牌玩家的位置
	pDesk->sGameData.T_HuPai.byUser = pHuCard->byUser;

	//标识该玩家胡牌了
	pDesk->sGameData.T_HuPai.bHaveHu[pHuCard->byUser] = true;
	pDesk->sGameData.T_HuPai.byUserNum++;
	//获取胡牌类型
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		pDesk->sGameData.T_HuPai.byHuType[bDeskStation][i] = pDesk->sGameData.T_CPGNotify[bDeskStation].byHuType[i];
	}
	//胡牌玩家的手牌数据
	for(int j = 0; j < PLAY_COUNT; j++)
	{
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			pDesk->sGameData.T_HuPai.m_byArHandPai[j][i] = pDesk->sUserData.m_byArHandPai[j][i];
		}
		//胡牌玩家的手牌张数
		pDesk->sGameData.T_HuPai.m_byArHandPaiCount[j] = pDesk->sUserData.GetHandPaiCount(j);
	}

	//清除该玩家的所有其他动作
	pDesk->sGameData.T_CPGNotify[pHuCard->byUser].Clear();
	//记录玩家回应
	pDesk->sGameData.T_CPGNotify[pHuCard->byUser].bHaveReturn = true;

	//通知玩家糊牌了
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		/*memset(pDesk->sGameData.T_HuPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_HuPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPai.m_byArHandPai,i,false);*/

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_HuPai, sizeof(pDesk->sGameData.T_HuPai), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_HuPai, sizeof(pDesk->sGameData.T_HuPai), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
	}
	//有人胡牌了 就不是流局
	pDesk->sGameData.T_HuPai.bIsLiuJu = false;

	bool bHaveFourZhong(false);
	for(int i = 0; i < MAX_HUPAI_TYPE; i++)
	{
		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
			break;
		else
		{
			if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_FOUR_ZHONG)
			{
				bHaveFourZhong = true;
				break;
			}
		}
	}

	//如果是自摸 只有一个人胡牌 
	if(pDesk->sGameData.T_HuPai.bZimo && !bHaveFourZhong)
	{
		ApplyAction(THING_MAI_MA,1*1000);
		
		//申请下一个事件
		//ApplyAction(THING_COUNT_FEN,2*100);
		//ApplyAction(THING_ZHONG_NIAO,1*1000);
	}
	else
	{
		bool bHaveSend = false;	//是否已经发送了消息
		BYTE byCanActionDesk = 255;	//可以做动作的玩家
		//检测是否还有其他玩家可以胡牌
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == bDeskStation)
			{
				continue;
			}

			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
					pDesk->SendWatchDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}

				bHaveSend = true;		
				byCanActionDesk = i;
				break;
			}
		}

		//有人还有胡牌 //没人可以胡牌了 申请算分事件
		if(bHaveSend && byCanActionDesk != 255)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);
		}
		else	
		{
			ApplyAction(THING_COUNT_FEN,2*100);
		}
	}

	return 0;
}












































///名称：CheckNextAction
///描述：检测下一步动作
void ThingHandleEx::CheckNextAction()
{
    bool bNotify = false;
    switch(pDesk->sGameData.m_byNextAction)//确定了下一步的响应事件：吃，碰，杠，胡
    {
    case ACTION_HU:					//糊动作
        {
			//如果是抢杠要把玩家手中的牌删掉
			if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].bQiangGang) //抢杠糊
			{
				pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_byDianPao,pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_iHuPs);
			}
           

 //           HandleHuPai(NextID,StarTime);
			///获取各个玩家的分数数据
			BYTE st = pDesk->sGameData.T_HuPai.byUser;
            ///通知玩家糊牌了




         

          

        }
        break;
    }
}

BYTE ThingHandleEx::GetCanHuUser(BYTE outst,bool hu[],BYTE hust)//获取离糊牌玩家近的玩家
{

	return ThingHandle::GetCanHuUser(outst,hu, hust);
}

///获取下家位置
BYTE ThingHandleEx::GetNextStation(BYTE station,bool shun)
{
    BYTE re = 255;
    if(shun)//顺时针
    {
        int nLoop = 8;
        re = (station + 1)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re + 1)%PLAY_COUNT;
        }
    }
    else
    {
        int nLoop = 8;
        re = (station + 3)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re+3)%PLAY_COUNT;
        }
    }
    return re;
}

///清理临时数据
void ThingHandleEx::ClearTempData()
{
	ThingHandle::ClearTempData();
}


///////////流程事件处理///////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------*/
//请求事件,type要申请的事件	DWJ
void ThingHandleEx::ApplyAction(int type,int time)
{
	if(type >= THING_GAME_BEGIN && type <= THING_ENG_UN_NORMAL)
	{
		pDesk->sGameData.ApplyThingID = type;
	}

    if(pDesk->sGameData.ApplyThingID != 255)
    {
        pDesk->SetTimer(10,time);
    }
}
/*------------------------------------------------------------------------------*/
///名称：GameCPU
///描述：游戏事件处理中心，
void	ThingHandleEx::GameCPU(int type)
{
    BYTE nextID = 255,time = 255;    
    switch(type)
    {
    case THING_GAME_BEGIN:	//游戏开始事件	DWJ
        {
            ExecuteGameBegin(nextID,time);
			break;
        }
    case THING_DING_NT:		//首局以东为庄事件	DWJ
        {
            ExecuteDongNt(nextID,time);
			break;
        }
	case THING_2SEZI_NT:  //首局以掷两色子定庄家事件
		{
			ExecuteTwoSeziNt(nextID,time);
			break;
		}
	case THING_2SEZI_DIR:	//掷2颗色子事件定起牌方向事件	DWJ
		{
			ExecuteTwoSeziDir(nextID,time);
			break;
		}
	case THING_2SEZI_GP: ////掷2颗色子事件定起牌点事件
		{
			ExecuteTwoSeziGp(nextID,time);
			break;
		}
	case THING_2SEZI_DIR_GP:	//掷2颗色子事件定起牌方向和起牌点事件	DWJ
		{
			ExecuteTwoSeziDirGp(nextID,time);
			break;
		}
	case THING_SEND_PAI:	//发牌事件	DWJ
		{
			ExecuteSendPai(nextID,time);
			break;
		}
	case THING_ALL_BUHUA:	//为所有玩家补花事件	DWJ
		{
			ExecuteAllBuHua(nextID,time);
			break;
		}
	case THING_BEGIN_OUT:	//庄家开始出牌通知事件	DWJ
		{
			pDesk->SetParamaterStation(GS_PLAY_GAME);
			pDesk->m_byGameStation = GS_PLAY_GAME;
			ExecuteBeginOut(nextID,time);
			break;
		}
	case THING_ZHUA_PAI:	//玩家抓牌	DWJ
		{
			ExecuteZhuaPai(nextID,time);
			break;
		}
	case THING_ONE_BUHUA:	//单个玩家抓到花牌后补花事件
		{
			ExecuteOneBuHua(nextID,time);
			break;
		}
	case THING_MAI_MA:	//买码
		{
			ExecuteMaiMa(nextID,time);
			break;
		}
	case THING_ZHONG_NIAO:  //中鸟事件
		{
			ExecuteZhongNiao(nextID,time);
			break;
		}
	case THING_COUNT_FEN:	//算分事件
		{
			ExecuteCountFen(nextID,time);
			break;
		}
	case THING_ENG_HANDLE://结束处理事件
		{
			ExecuteNormalEnd(nextID,time);
			break;
		}
    case THING_ENG_UN_NORMAL://异常结束处理事件
        {
            ExecuteUnNormalEnd(nextID,time);
			break;
        }
    }

    pDesk->sGameData.ApplyThingID = nextID;
    ApplyAction(nextID,time*100);//申请下一个事件

}
void ThingHandleEx::ExecuteMaiMa(BYTE &nextID,BYTE &time)
{
	pDesk->sGameData.m_byThingDoing = THING_MAI_MA;
	pDesk->sGameData.m_TMaiMa.clear();
	//自摸胡牌玩家买码
	pDesk->sGameData.m_TMaiMa.byUser = pDesk->sGameData.T_HuPai.byUser;
	pDesk->sGameData.m_TMaiMa.byCount = pDesk->m_iMaiMaCount;
	
	//直接先确定好数据
	BYTE byPaiData[300];
	memset(&byPaiData,255,sizeof(byPaiData));
	DisPatchCardForMaiMa(byPaiData);

	for(int i = 0; i < pDesk->sGameData.m_TMaiMa.byCount; i++)
	{
		BYTE byCard = 255;
		do 
		{
			int iRand = rand()%24;
			byCard = byPaiData[iRand];
			byPaiData[iRand] = 255;
		} while (255 == byCard);
		
		pDesk->sGameData.m_TMaiMa.byCard[i] = byCard;
		pDesk->sGameData.m_ZhongNiao.byCard[i] = byCard;
	}
	//如果是超端玩家 买码全中红中
	if (IsSuperUser(pDesk->sGameData.m_TMaiMa.byUser))
	{
		for(int i = 0; i < pDesk->sGameData.m_TMaiMa.byCount; i++)
		{
			pDesk->sGameData.m_TMaiMa.byCard[i] = 35;
			pDesk->sGameData.m_ZhongNiao.byCard[i] = 35;
		}
	}

	//通知客户端
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->SendGameData(i,&pDesk->sGameData.m_TMaiMa,sizeof(pDesk->sGameData.m_TMaiMa),MDM_GM_GAME_NOTIFY,THING_MAI_MA,0);
	}
	//设定买码时间
	SetDoingThing(pDesk->sGameData.m_TMaiMa.byUser,THING_MAI_MA,pDesk->sGameData.m_mjRule.byOutTime);

}
/*-------------------------------------------------------------------------------------------------*/
void ThingHandleEx::ExecuteZhongNiao(BYTE &nextID,BYTE &time)
{
	pDesk->sGameData.m_byThingDoing = THING_ZHONG_NIAO;

	nextID = THING_COUNT_FEN;
	time = 100;

	for(int i = 0; i < 4; i++)
	{

		BYTE card = pDesk->sGameData.m_ZhongNiao.byCard[i];

		if(card == 255)
		{
			break;
		}
		//2  3 红中才中鸟
		if((card % 10)== 2)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 6;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 2;
			}
		}
		else if((card % 10)== 3)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 9;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 3;
			}
		}
		else if((card)== 35)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 15;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 5;
			}
		}
	}
	
	memcpy(pDesk->sGameData.m_ZhongNiao.byMenPai,pDesk->sUserData.m_iArMenPai,sizeof(pDesk->sGameData.m_ZhongNiao.byMenPai));
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->SendGameData(i,&pDesk->sGameData.m_ZhongNiao,sizeof(pDesk->sGameData.m_ZhongNiao),MDM_GM_GAME_NOTIFY,THING_ZHONG_NIAO,0);
	}
}
///开始事件处理	DWJ
void	ThingHandleEx::ExecuteGameBegin(BYTE &nextID,BYTE &time)
{   
	//初始化游戏数据和玩家数据
    pDesk->sGameData.InitData();

    pDesk->sUserData.InitData();       

	memset(pDesk->m_byAutoTime,0,sizeof(pDesk->m_byAutoTime));

	pDesk->KillTimer(TIME_WAIT_MSG);
	pDesk->SetTimer(TIME_WAIT_MSG,1000);//重新设置超时计时器

	pDesk->sGameData.m_byThingDoing = THING_GAME_BEGIN;

	//庄家不存在 就执行随机一个庄家事件
	if(pDesk->sGameData.m_byNtStation == 255)
	{
		pDesk->sGameData.m_iHuangZhuangCount = 0;
		//随机一个玩家坐庄
		srand(GetTickCount());
		//pDesk->sGameData.m_byNtStation = rand()%PLAY_COUNT;
		nextID = THING_2SEZI_NT;//THING_DING_NT;
	}
	else 
	{
		nextID = THING_2SEZI_DIR;//THING_2SEZI_DIR_GP;//掷色子发牌
	}
	time	= 1;

	// 庄家位置
    pDesk->sGameData.T_Begin.byNt = pDesk->sGameData.m_byNtStation;	
	pDesk->sGameData.T_Begin.iHuangZhuangCount = pDesk->sGameData.m_iHuangZhuangCount;	

	//门牌数据
    memset(pDesk->sGameData.T_Begin.byMenPai,0,sizeof(pDesk->sGameData.T_Begin.byMenPai));

	//发送开始消息
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_GAME_BEGIN,0);
		pDesk->SendWatchDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_GAME_BEGIN,0);
	}
//    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameDatam_byNtStation;
    ///当前抓牌方向，true 顺时针，false 逆时针
//    pDesk->sUserData.m_bZhuaPaiDir = true;

//    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameData.m_byNtStation;//起牌方向(每局初始化为庄家门前起牌)
}
/*-------------------------------------------------------------------------------------------------*/
///以第一个玩家为庄家（以东为庄事件）	DWJ
void	ThingHandleEx::ExecuteDongNt(BYTE &nextID,BYTE &time)
{	
	pDesk->sGameData.m_byThingDoing = THING_DING_NT;

	//设置下一个事件为 掷筛子确定起牌方位事件
	nextID = THING_2SEZI_DIR;
	time = 2;	//200毫秒间隔

	//先清空数据
	pDesk->sGameData.T_DongNt.Clear();
	// 庄家位置	
	pDesk->sGameData.T_DongNt.byNt = pDesk->sGameData.m_byNtStation;	
	pDesk->sGameData.T_DongNt.iHuangZhuangCount = pDesk->sGameData.m_iHuangZhuangCount;

	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&pDesk->sGameData.T_DongNt,sizeof(pDesk->sGameData.T_DongNt),MDM_GM_GAME_NOTIFY,THING_DING_NT,0);
		pDesk->SendWatchDataEx(i,&pDesk->sGameData.T_DongNt,sizeof(pDesk->sGameData.T_DongNt),MDM_GM_GAME_NOTIFY,THING_DING_NT,0);
	}
}
/*-------------------------------------------------------------------------------------------------*/
///掷2颗色子确定起牌方向（点数和确定方向）	DWJ
void	ThingHandleEx::ExecuteTwoSeziDir(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_2SEZI_DIR;
	///掷2颗色子确定起牌位置事件
	nextID	= THING_2SEZI_GP;//THING_2SEZI_DIR_GP;//准备执行掷第二次色子确定棋牌顿数事件
	time	= 30; //3000ms = 3S

	//先清空数据
	pDesk->sGameData.T_TwoSeziDir.Clear();
	//掷筛子玩家的位置 = 庄家的位置
	pDesk->sGameData.T_TwoSeziDir.byUser = pDesk->sGameData.m_byNtStation;
	pDesk->sGameData.T_TwoSeziDir.byNt = pDesk->sGameData.m_byNtStation;
	CLogicBase::GetSeziData(&pDesk->sGameData.T_TwoSeziDir.bySezi0,&pDesk->sGameData.T_TwoSeziDir.bySezi1);

	//记录起牌的方向(1为自己 逆时针数)
	pDesk->sUserData.m_MenPai.byGetPaiDir = (pDesk->sGameData.T_TwoSeziDir.bySezi1 + pDesk->sGameData.T_TwoSeziDir.bySezi0 +  pDesk->sGameData.T_TwoSeziDir.byUser - 1)%4;
	//起牌方向
	pDesk->sGameData.T_TwoSeziDir.byDirection = pDesk->sUserData.m_MenPai.byGetPaiDir;

	//发送数据至客户端
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TwoSeziDir, sizeof(pDesk->sGameData.T_TwoSeziDir), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_TwoSeziDir, sizeof(pDesk->sGameData.T_TwoSeziDir), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR, 0);
	
}
/*-------------------------------------------------------------------------------------------------*/
///掷2颗色子确定起牌方向和起牌敦数（点数和方向，小点敦数）DWJ
void	ThingHandleEx::ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time)	
{    
	pDesk->sGameData.m_byThingDoing = THING_2SEZI_DIR_GP;

	//先清空数据
	pDesk->sGameData.T_TwoSeziDirAndGetPai.Clear();
	///掷2颗色子确定起牌位置（点数和）和起牌点（最小点）事件	
	pDesk->sGameData.T_TwoSeziDirAndGetPai.byUser = pDesk->sGameData.m_byNtStation;
	//摇骰子
	CLogicBase::GetSeziData(&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0,&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1);

	//起牌点数 = (第一次筛子的总和+这次两个筛子点数和)	DWJTEST (四个筛子点数>18怎么办)
	pDesk->sUserData.m_MenPai.byGetPai = (pDesk->sGameData.T_TwoSeziDir.bySezi1 + pDesk->sGameData.T_TwoSeziDir.bySezi0 + pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0 + pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1)%18;
	
	//通知客户端
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);

	//下一个发牌事件
	nextID = THING_SEND_PAI;
	time = 30;

}
/*-------------------------------------------------------------------------------------------------*/
///发牌	
void	ThingHandleEx::ExecuteSendPai(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_SEND_PAI;//记录正在发生的事件

	///当前抓牌方向，true 顺时针，false 逆时针
	pDesk->sUserData.m_bZhuaPaiDir = true;    
	///发牌事件
	pDesk->sGameData.T_SendPai.byDo = 1;	 
	//打乱牌
	DisPatchCard();

	//下发所有玩家的13张牌
	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<MAX_HAND_PAI-1;++j)
		{
			GetPai(i,true,1);
		}
	}

	//庄家再多抓一张牌
	BYTE card = 255;
	card=GetPai(pDesk->sGameData.m_byNtStation,true,1);

	//分配门牌
	ChangeMenPai();
	pDesk->sGameData.T_SendPai.Clear();
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_SendPai.byMenPai[i]);//拷贝门牌

		//玩家牌张数
		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_SendPai.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_SendPai.m_byArHandPaiCount[i] = 13;
		}
	}

	//dwjtesttest
	/*pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][0] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][1] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][2] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][3] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][4] = 15;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][5] = 6;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][6] = 11;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][7] = 12;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][8] = 13;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][9] = 14;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][10] = 15;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][11] = 16;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][12] = 27;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][13] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][12] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][12] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][12] = 28;*/

	

	//给四个玩家分发所有的牌
	for(int i=0;i<PLAY_COUNT;i++)
	{	
		//拷贝手牌
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_SendPai.m_byArHandPai,i,false);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
	}


	//检查玩家手上是否有花牌 
	bool bNeedBuHua = false;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(pDesk->sUserData.IsHuaPaiInHand(i))
		{
			bNeedBuHua = true;
			break;
		}
	}
	nextID = THING_BEGIN_OUT;//THING_ALL_BUHUA;
	time = 50;
}
/*-------------------------------------------------------------------------------------------------*/
///获取牌数据	DWJ
void	ThingHandleEx::DisPatchCard()
{
	//发牌
	BYTE index=0;
	memset(pDesk->sUserData.m_MenPai.byMenPai,255,sizeof(pDesk->sUserData.m_MenPai.byMenPai));//
	

	for(int i=0;i<4;i++)
	{
		if(pDesk->sGameData.m_mjRule.bHaveWan)//有万
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_W[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBing)//有柄
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_B[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveTiao)//有条
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_T[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBaiBan)//有白板
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[2];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFaCai)//有发财
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[1];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveHongZhong)//有红中
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[0];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFengPai)//有东南西北
		{
			for(int j=0;j<4;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_FENG[j];
				index++;
			}
		}
	}

	if(pDesk->sGameData.m_mjRule.bHaveFlower)//有花牌
	{
		for(int i=0;i<8;i++)
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}


	///打乱牌
	srand(GetTickCount());
	BYTE temp=255,data=0;
	for(int i=0;i<index;i++)
	{
		temp = rand()%(index-i)+i;
		data = pDesk->sUserData.m_MenPai.byMenPai[temp];
		pDesk->sUserData.m_MenPai.byMenPai[temp] = pDesk->sUserData.m_MenPai.byMenPai[i];
		pDesk->sUserData.m_MenPai.byMenPai[i] = data;
	}


	///////////////	DWJ???
//	pDesk->sUserData.m_MenPai.byGetPai = pDesk->sUserData.m_MenPai.byGetPai%13;
	//pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;
	//if(pDesk->sUserData.m_MenPai.byGetPaiDir == 255)
	//{
	//	pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameData.m_byNtStation;
	//}
	//pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;

	int dun = 0,num=0;
	dun = index/8;
	num = (dun*pDesk->sUserData.m_MenPai.byGetPaiDir + pDesk->sUserData.m_MenPai.byGetPai)*2;
	if(num == 0)
	{
		num=2;
	}
	pDesk->sUserData.m_MenPai.byStartIndex = num;//门牌起始索引
	pDesk->sUserData.m_MenPai.byEndIndex = num-1;	//门牌结束索引
	pDesk->sUserData.m_MenPai.byAllPaiNum = index;//所有牌的数量
}
/*----------------------------------------------------------------------------------------------------*/
//为了买码而打乱的牌
void	ThingHandleEx::DisPatchCardForMaiMa(BYTE byPai[300])
{
	//发牌
	BYTE	index=0;
	BYTE    byMenPai[300];
	memset(byMenPai,255,sizeof(byMenPai));//

	for(int i=0;i<4;i++)
	{
		if(pDesk->sGameData.m_mjRule.bHaveWan)//有万
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_W[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBing)//有柄
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_B[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveTiao)//有条
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_T[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBaiBan)//有白板
		{
			byMenPai[index] = stcArr_A_Mj_ZI[2];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFaCai)//有发财
		{
			byMenPai[index] = stcArr_A_Mj_ZI[1];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveHongZhong)//有红中
		{
			byMenPai[index] = stcArr_A_Mj_ZI[0];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFengPai)//有东南西北
		{
			for(int j=0;j<4;j++)
			{
				byMenPai[index] = stcArr_A_Mj_FENG[j];
				index++;
			}
		}
	}

	if(pDesk->sGameData.m_mjRule.bHaveFlower)//有花牌
	{
		for(int i=0;i<8;i++)
		{
			byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}


	///打乱牌
	srand(GetTickCount());
	BYTE temp=255,data=0;
	for(int i=0;i<index;i++)
	{
		temp = rand()%(index-i)+i;
		data = byMenPai[temp];
		byMenPai[temp] = byMenPai[i];
		byMenPai[i] = data;

		byPai[i] = byMenPai[i];
	}

	//至少出现红2、3、红中的个数
	int iTmp23zhongCount = 0;
FIND23ZHONG:
	iTmp23zhongCount = 0;
	for(int i=0; i<24; i++)
	{
		if ((byPai[i] % 10 == 2) || (byPai[i] % 10 == 3) || (byPai[i] == 35))
		{
			iTmp23zhongCount++;
		}
	}

	if (iTmp23zhongCount < pDesk->m_i23ZhongCount)
	{
		//先去除掉一个
		for(int i=0; i<24; i++)
		{
			if ((byPai[i] % 10 != 2) && (byPai[i] % 10 != 3) && (byPai[i] != 35))
			{
				byPai[i] = 255;
				break;
			}
		}
		//再用另外的 2、3、红中的牌替换
		for(int i=0; i<24; i++)
		{
			if (255 == byPai[i])
			{
				for(int j=24; j<300; j++)
				{
					if ( 255 == byPai[j] )
					{
						continue;
					}
					if ((byPai[j] % 10 == 2) || (byPai[j] % 10 == 3) || (byPai[j] == 35))
					{
						byPai[i] = byPai[j];
						byPai[j] = 255;
						break;
					}
				}
			}
		}
		goto FIND23ZHONG;
	}

	
}
/*----------------------------------------------------------------------------------------------------*/
///描述：抓一张牌
///@param station 抓牌玩家的位置 , head  是否从前面抓,type 抓牌类型 0 正常抓牌，1开始前发牌，2开前补花抓牌，3补花
BYTE ThingHandleEx::GetPai(int station,bool head,BYTE type,BYTE *pIndex)
{
	return ThingHandle::GetPai(station, head, type,pIndex);
}
/*----------------------------------------------------------------------------------------------------*/
///描述：转换门牌数据,将总门牌数组中的数据转换到每个玩家的门牌数组中
void ThingHandleEx::ChangeMenPai()
{
	for(int i=0;i<MEN_CARD_NUM;++i)//0位置的门牌
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 255;
		}
	}
	for(int i=MEN_CARD_NUM;i<(2*MEN_CARD_NUM);++i)//1位置的门牌
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[1][i-MEN_CARD_NUM] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[1][i-MEN_CARD_NUM] = 255;
		}
	}
	for(int i=(2*MEN_CARD_NUM);i<(3*MEN_CARD_NUM);++i)//2位置的门牌
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[2][i-(2*MEN_CARD_NUM)] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[2][i-(2*MEN_CARD_NUM)] = 255;
		}
	}
	for(int i=(3*MEN_CARD_NUM);i<(4*MEN_CARD_NUM);++i)//3位置的门牌
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[3][i-(3*MEN_CARD_NUM)] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[3][i-(3*MEN_CARD_NUM)] = 255;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
///所有玩家补花事件	DWJ
void	ThingHandleEx::ExecuteAllBuHua(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_ALL_BUHUA;

	//先清空补花数据
	pDesk->sGameData.T_AllBuHua.Clear();
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!pDesk->sUserData.IsHuaPaiInHand(i))
		{
			continue;
		}
		while(pDesk->sUserData.IsHuaPaiInHand(i))
		{
			int num = pDesk->sUserData.MoveHuaPaiFormHand(i);
			for(int j=0;j<num;j++)
			{
				GetPai(i,false,1);//开始前补花抓牌
			}
		}

		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_AllBuHua.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_AllBuHua.m_byArHandPaiCount[i] = 13;
		}
	}

	//取完牌后 及时转换门牌
	ChangeMenPai();
	//拷贝花牌
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->sUserData.CopyHuaPai(i,pDesk->sGameData.T_AllBuHua.m_byArHuaPai[i]);
	}


	///发送牌数据
	for (int i=0;i<PLAY_COUNT;i++)
	{		
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_AllBuHua, sizeof(pDesk->sGameData.T_AllBuHua), MDM_GM_GAME_NOTIFY, THING_ALL_BUHUA, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_AllBuHua, sizeof(pDesk->sGameData.T_AllBuHua), MDM_GM_GAME_NOTIFY, THING_ALL_BUHUA, 0);
	}
	///下一个时间就是 开始出牌事件
	nextID = THING_BEGIN_OUT;
	time = 10;
	//ApplyAction(nextID,1500);
}
/*----------------------------------------------------------------------------------------------------*/
///开始出牌通知事件	DWJ
void	ThingHandleEx::ExecuteBeginOut(BYTE &nextID,BYTE &time)
{    
	pDesk->sGameData.m_byThingDoing = THING_BEGIN_OUT;//记录正在发生的事件


	///开始出牌通知事件 
	pDesk->sGameData.T_BeginOutPai.byNt		= pDesk->sGameData.m_byNtStation;//庄家位置
	pDesk->sGameData.T_BeginOutPai.byUser	= pDesk->sGameData.m_byNtStation;//庄家位置
	pDesk->sUserData.m_byNowOutStation		= pDesk->sGameData.m_byNtStation;//出牌位置

	//转换门牌
	ChangeMenPai();
	//拷贝门牌
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_BeginOutPai.byMenPai[i]);//拷贝门牌
		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_BeginOutPai.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_BeginOutPai.m_byArHandPaiCount[i] = 13;
		}
		//排序所有玩家的手牌
		pDesk->sUserData.SortHandPai(i,false);
	}

	
	//发送通知
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_BeginOutPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_BeginOutPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_BeginOutPai.m_byArHandPai,i,false);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);


	//检测动作
	if(CheckAction(pDesk->sGameData.m_byNtStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),0))
	{
		//标记是抓牌后有动作(客户端用于显示倒计时时间的)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		//pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		//pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);

		HaveAction action;
		action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
		memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
			pDesk->SendWatchDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
		}
		//如果有胡牌动作那么就启动胡牌自动处理计时器
		if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.m_byNtStation].bHu)
		{
			//没有动作 设置执行事件 自动启动超时处理计时器
			SetDoingThing(pDesk->sGameData.m_byNtStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
		}
		else
		{
			//没有动作 设置执行事件 自动启动超时处理计时器
			SetDoingThing(pDesk->sGameData.m_byNtStation,THING_BEGIN_OUT);
		}
	}
	else
	{
		//没有动作 设置执行事件 自动启动超时处理计时器
		SetDoingThing(pDesk->sGameData.m_byNtStation,THING_BEGIN_OUT);
	}
}

/*--------------------------------------------------------------------------------------------------*/
///检测玩家station当前能做哪些动作	DWJ
///描述：station 检测位置,pai 别的玩家打出的牌，type 类型 出牌 0 抓牌，1杠开，2 点炮，3抢杠 4吃牌后检测
bool ThingHandleEx::CheckAction(int station,BYTE pai,BYTE type)
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->sGameData.T_CPGNotify[i].Clear();//清零
	}

	bool action = false;
	switch(type)
	{
	case 0://自摸
	case 1://杠开
		{
			//糊
			if(pDesk->sCheckHuPai.CanHu(station,pai,pDesk->sGameData.T_CPGNotify[station].byHuType,true))
			{
				pDesk->sGameData.T_CPGNotify[station].bHu = true;
				pDesk->sGameData.T_CPGNotify[station].m_iHuPs = pDesk->sUserData.GetLastHandPai(station);
				pDesk->sGameData.T_CPGNotify[station].m_byDianPao = station;
			}
			//杠(听牌了就不能杠了)
			if(pai != 35)
			{
				if(!pDesk->sUserData.m_bTing[station] && pDesk->sCPGAction.CanGang(station,pai,pDesk->sGameData.T_CPGNotify[station].m_iGangData ,true))
				{
					pDesk->sGameData.T_CPGNotify[station].bGang = true;
				}
			}
			////听
			//if(!pDesk->sUserData.m_bTing[station] && CanTingDWJ(station))
			//{
			//	pDesk->sGameData.T_CPGNotify[station].bTing = true;
			//}
			if(pDesk->sGameData.T_CPGNotify[station].bHu || pDesk->sGameData.T_CPGNotify[station].bGang || pDesk->sGameData.T_CPGNotify[station].bTing)
			{
				action=true;
				pDesk->sGameData.T_CPGNotify[station].bCanAction = true;
			}
		}
		break;
	case 2://点炮
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if (pDesk->sUserData.m_bIsHu[i] || pDesk->sUserData.m_bFoceLeave[i])        // 已胡牌玩家,或者断线状态
				{
					continue;
				}

				if(i==station)
				{
					continue;
				}
				//检测下家是否能吃
				//吃
				/*if(!pDesk->sUserData.m_bTing[i] && GetNextStation(station)==i)
				{
				if(pDesk->sCPGAction.CanChi(i,pai,pDesk->sGameData.T_CPGNotify[i].m_iChi))
				{					
				pDesk->sGameData.T_CPGNotify[i].bChi = true;
				pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_CHI;
				}
				}*/
				//碰
				if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanPeng(i,pai))
				{	
					pDesk->sGameData.T_CPGNotify[i].bPeng = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_PENG;
				}
				//杠
				if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanGang(i,pai,pDesk->sGameData.T_CPGNotify[i].m_iGangData,false))
				{
					pDesk->sGameData.T_CPGNotify[i].bGang = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_MING_GANG;

				}
				//糊
				//玩家弃糊 不能胡   玩家听牌了 只能自摸
				/*if(!pDesk->sUserData.m_bQiHu[i] && !pDesk->sUserData.m_bTing[i] &&pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;
				}*/
				if(pDesk->sGameData.T_CPGNotify[i].bChi || pDesk->sGameData.T_CPGNotify[i].bHu || pDesk->sGameData.T_CPGNotify[i].bPeng || pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bTing)
				{
					action=true;
					pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 3://抢杠
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if (pDesk->sUserData.m_bIsHu[i]|| pDesk->sUserData.m_bFoceLeave[i])        // 已胡牌玩家,或者断线状态
				{
					continue;
				}
				if(i==station)
				{
					continue;
				}
				if(pDesk->sUserData.m_bQiHu[i] )
				{
					OutputDebugString("抢杠检测：弃糊，不能抢杠糊");
				}
				//糊 听牌了 不能抢杠胡
				if(!pDesk->sUserData.m_bQiHu[i] && !pDesk->sUserData.m_bTing[i] && pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					//设置抢杠牌型
					pDesk->sCheckHuPai.SetAHupaiType(HUPAI_TYPE_QIANG_GANG, pDesk->sGameData.T_CPGNotify[i].byHuType);
		
					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;//记录胡的牌
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;		
					pDesk->sGameData.T_CPGNotify[i].bQiangGang = true; //抢杠糊

					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
					action=true;
					pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 4:	//拦牌后检测(拦牌后只能听和杠 不能胡)
		{
			//杠(听牌了就不能杠了)
			if(!pDesk->sUserData.m_bTing[station] && pDesk->sCPGAction.CanGang(station,pai,pDesk->sGameData.T_CPGNotify[station].m_iGangData ,true))
			{
				pDesk->sGameData.T_CPGNotify[station].bGang = true;
			}
			if(!pDesk->sUserData.m_bTing[station])
			{
				if(CanTingDWJ(station))
				{
					pDesk->sGameData.T_CPGNotify[station].bTing = true;
				}
			}
			
			if(pDesk->sGameData.T_CPGNotify[station].bGang || pDesk->sGameData.T_CPGNotify[station].bTing)
			{
				action=true;
				pDesk->sGameData.T_CPGNotify[station].bCanAction = true;
			}
			break;
		}
	}
	if(action)
	{
		return true;
	}
	return false;
}
/*--------------------------------------------------------------------------------------------------*/
//玩家能否听牌	DWJ
bool	ThingHandleEx::CanTingDWJ(BYTE byDeskStation)
{
	if(byDeskStation < 0 || byDeskStation >= PLAY_COUNT)
	{
		return false;
	}
	//先拷贝保存数据
	BYTE	byUserHandCards[HAND_CARD_NUM] = {0};
	memcpy(byUserHandCards, &pDesk->sUserData.m_byArHandPai[byDeskStation],sizeof(byUserHandCards));

	int iCardCount = pDesk->sUserData.GetHandPaiCount(byDeskStation);

	BYTE	byTingCanOut[HAND_CARD_NUM];//听牌能打的牌
	memset(byTingCanOut,255,sizeof(byTingCanOut));

	BYTE	byDelValue = 255;
	for(int i = 0; i< iCardCount; i++)
	{
		memcpy(&pDesk->sUserData.m_byArHandPai[byDeskStation], byUserHandCards, sizeof(pDesk->sUserData.m_byArHandPai[byDeskStation]));
		//先去掉一个牌 判断能否听牌 
		byDelValue	= pDesk->sUserData.m_byArHandPai[byDeskStation][i];
		pDesk->sUserData.m_byArHandPai[byDeskStation][i] = 255;
		//将玩家的牌排序
		pDesk->sUserData.SortHandPai(i,false);

		//这里判断能否听牌
		if(pDesk->sGameData.m_mjAction.bTing && !pDesk->sUserData.m_bTing[byDeskStation])//玩家没有听牌
		{
			//pDesk->sGameData.T_CPGNotify[byDeskStation].bTing = pDesk->sCheckHuPai.CanTing(byDeskStation,pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut);
			if(pDesk->sCheckHuPai.CanTing(byDeskStation,pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut))
			{
				for(int x=0; x<HAND_CARD_NUM;x++)
				{
					if(byTingCanOut[x] == 255)
					{
						byTingCanOut[x] = byDelValue;
						break;
					}
				}
				//break;
			}
			else
			{

			}
		}
	}

	//拷贝听牌能打的牌的数据
	memcpy(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut, byTingCanOut, sizeof(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut));
	//最后还原玩家的原始数据牌
	memcpy(&pDesk->sUserData.m_byArHandPai[byDeskStation], byUserHandCards, sizeof(pDesk->sUserData.m_byArHandPai[byDeskStation]));

	//判断能否听牌
	for(int i=0; i<HAND_CARD_NUM;i++)
	{
		if(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut[i] != 255)
		{
			pDesk->sGameData.T_CPGNotify[byDeskStation].bCanAction = true;
			pDesk->sGameData.T_CPGNotify[byDeskStation].bTing = true;
			return true;
		}
	}
	return false;
}
/*--------------------------------------------------------------------------------------------------*/
///发送拦牌信息给玩家	DWJ
bool	ThingHandleEx::HaveUserCanDoAction(BYTE byIgnoreStation)
{
	bool bHaveSend = false;	//是否已经发送了消息
	BYTE byCanActionDesk = 255;	//可以做动作的玩家
	if(!bHaveSend)
	{
		//动作优先级大的玩家先发
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}

			//优先给能胡牌的玩家发送动作消息 
			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/
				HaveAction action;
				action.byDeskStation = byIgnoreStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
					pDesk->SendWatchDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
				}
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);//设置执行事件
		}
		
	}
	//没有发送胡的消息 那么再优先发送杠 或者碰的消息
	if(!bHaveSend)
	{
		//动作优先级大的玩家先发
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}
			//再优先给能杠、碰牌的玩家发送动作消息 
			if(pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bPeng)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/
				//通知道其他玩家某玩家有动作
				HaveAction action;
				action.byDeskStation = byIgnoreStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
					pDesk->SendWatchDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
				}
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_GANG_PAI);//设置执行事件
		}
	}

	//没有发送胡、杠、碰消息 再检查发送吃牌消息
	if(!bHaveSend)
	{
		//动作优先级大的玩家先发
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}
			//再优先给能杠、碰牌的玩家发送动作消息 
			if(pDesk->sGameData.T_CPGNotify[i].bChi)
			{
				pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_CHI_PAI);//设置执行事件
		}
	}

	return bHaveSend;
}
/*--------------------------------------------------------------------------------------------------*/
///抓牌事件
void	ThingHandleEx::ExecuteZhuaPai(BYTE &nextID,BYTE &time)	
{
	///抓牌牌事件
	nextID = 255;
	BYTE card = 255;
	if(pDesk->sUserData.m_MenPai.GetMenPaiNum()<=pDesk->sGameData.m_mjRule.byGamePassNum)
	{
		OutputDebugString("dwjsdmj::------牌 抓完了 游戏结束");
		//流局清空胡牌信息
		pDesk->sGameData.T_HuPai.Clear();
		//抓到空牌 算分结束游戏
		nextID = THING_COUNT_FEN;
		time = 2;
	}
	else
	{
		//出牌事件 就获取出牌玩家的下一个玩家抓牌
		if( pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)
		{
			pDesk->sUserData.m_byNowOutStation = GetNextStation(pDesk->sGameData.T_OutPai.byUser);//抓牌位置
			//从前面抓
			pDesk->sUserData.m_bZhuaPaiDir = true;
		}
		else if(pDesk->sGameData.m_byThingDoing == THING_GANG_PAI)	//如果是杠牌事件  那么就是杠牌玩家抓牌	
		{
			//抓牌位置
			pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_GangPai.byUser;
			//从后面抓
			pDesk->sUserData.m_bZhuaPaiDir = false;
		}

//		pDesk->sGameData.T_HuPai.Clear();//清理浒牌记录

		pDesk->sGameData.m_byThingDoing = THING_ZHUA_PAI;//记录正在发生的事件

		//清理抓牌数据
		pDesk->sGameData.T_ZhuaPai.Clear();

		pDesk->sGameData.T_ZhuaPai.byUser = pDesk->sUserData.m_byNowOutStation;
		card = GetPai(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.m_bZhuaPaiDir,0,&pDesk->sGameData.T_ZhuaPai.byGetPaiIndex);//正常抓牌
		pDesk->sGameData.T_ZhuaPai.byPs = card;		//抓牌的值

//		pDesk->sUserData.m_bQiHu[pDesk->sUserData.m_byNowOutStation] = false;//清理玩家弃糊状态


		//抓到空牌强制结束
		if(card==255 && (pDesk->sUserData.m_MenPai.GetMenPaiNum())<=pDesk->sGameData.m_mjRule.byGamePassNum)
		{
			//流局清空胡牌信息
			pDesk->sGameData.T_HuPai.Clear();

			//抓到空牌 算分结束游戏
			nextID = THING_COUNT_FEN;
			time = 2;
			
			return;
		}


		ChangeMenPai();//转换门牌

		for(int i=0;i<PLAY_COUNT;i++)
		{
			//拷贝门牌
			pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_ZhuaPai.byMenPai[i]);
			//玩家手牌总数
			pDesk->sGameData.T_ZhuaPai.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		}

		//发送抓牌数据
		for (int i=0;i<PLAY_COUNT;i++)
		{			
			//拷贝手牌
			memset(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_ZhuaPai.m_byArHandPai));
			pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,i,true);
			
			pDesk->SendGameDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
			pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
		}


		//如果是花牌 那么就立马补花
		if(pDesk->sUserData.CheckIsHuaPai(card))
		{
			nextID	= THING_ONE_BUHUA;
			time	= 5;
		}
		else
		{
			//动作检测
			if(!CheckAction(pDesk->sUserData.m_byNowOutStation,card,0))
			{
				//没有动作 那么就启动出牌计时器
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
			}
			else
			{
				//标记是抓牌后有动作(客户端用于显示倒计时时间的)
				pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
				/*pDesk->SendGameDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}

				//如果是胡牌 那么启动 自动胡牌响应计时器
				if(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bHu)
				{
					SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
				}
				else //其他动作 都启动 自动出牌计时器
				{
					SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI,pDesk->sGameData.m_mjRule.byOutTime);
				}
			}
		}	


	}

	//20131218
//	测试抓牌和补花后有动作倒计时的情况--客户端显示情况和服务端处理情况

	//记录杠牌状态，拥于杠开和杠后炮
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(i != pDesk->sUserData.m_byNowOutStation)
		{
			pDesk->sGameData.m_bGangState[i] = false;
		}
	}
}
/*--------------------------------------------------------------------------------------------------*/
///单个玩家补花事件
void	ThingHandleEx::ExecuteOneBuHua(BYTE &nextID,BYTE &time)
{	
	nextID = 255;
	pDesk->sGameData.m_byThingDoing = THING_ONE_BUHUA;
	
	//清空补花数据
	pDesk->sGameData.T_OneBuHua.Clear();
	///单个玩家补花的位置即抓牌玩家的位置
	pDesk->sGameData.T_OneBuHua.byUser = pDesk->sGameData.T_ZhuaPai.byUser;

	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_OneBuHua.byUser;

	//开始补花
	if(pDesk->sUserData.IsHuaPaiInHand(pDesk->sUserData.m_byNowOutStation))
	{
		int num = pDesk->sUserData.MoveHuaPaiFormHand(pDesk->sUserData.m_byNowOutStation);
		for(int i=0;i<num;i++)
		{
			pDesk->sGameData.T_OneBuHua.byPs = GetPai(pDesk->sUserData.m_byNowOutStation,false,3);//正常抓牌
		}
	}

	//如果补花补到了空牌 就游戏结束
	if(pDesk->sGameData.T_OneBuHua.byPs == 255 || (pDesk->sUserData.m_MenPai.GetMenPaiNum())<=pDesk->sGameData.m_mjRule.byGamePassNum)
	{
		//流局清空胡牌信息
		pDesk->sGameData.T_HuPai.Clear();
		//抓到空牌 算分结束游戏
		nextID = THING_COUNT_FEN;
		time = 2;
		return;
	}

	//取完牌后 及时转换门牌
	ChangeMenPai();
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//拷贝花牌
		pDesk->sUserData.CopyHuaPai(i,pDesk->sGameData.T_OneBuHua.m_byArHuaPai[i]);
		//拷贝门牌
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_OneBuHua.byMenPai[i]);

		//手牌张数
		pDesk->sGameData.T_OneBuHua.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		
	}
	///发送牌数据
	for (int i=0;i<PLAY_COUNT;i++)
	{			
		memset(pDesk->sGameData.T_OneBuHua.m_byArHandPai,255,sizeof(pDesk->sGameData.T_OneBuHua.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OneBuHua.m_byArHandPai,i,true);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_OneBuHua, sizeof(pDesk->sGameData.T_OneBuHua), MDM_GM_GAME_NOTIFY, THING_ONE_BUHUA, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_OneBuHua, sizeof(pDesk->sGameData.T_OneBuHua), MDM_GM_GAME_NOTIFY, THING_ONE_BUHUA, 0);
	}

	//如果是花牌 那么就补花
	if(pDesk->sUserData.CheckIsHuaPai(pDesk->sGameData.T_OneBuHua.byPs))
	{
		nextID	= THING_ONE_BUHUA;
		time	= 5;
	}
	else
	{
		//动作检测
		if(!CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sGameData.T_OneBuHua.byPs,0))
		{
			//没有动作 那么就启动出牌计时器
			SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
		}
		else
		{
			//因为是抓牌有动作 所以拦牌思考时间是出牌时间
			//pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].byBlockTime = pDesk->sGameData.m_mjRule.byOutTime;
			//标记是抓牌后有动作(客户端用于显示倒计时时间的)
			pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
			pDesk->SendGameDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
			pDesk->SendWatchDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);


			//如果是胡牌 那么启动 自动胡牌响应计时器
			if(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bHu)
			{
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
			}
			else //其他动作 都启动 自动出牌计时器
			{
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI,pDesk->sGameData.m_mjRule.byOutTime);
			}
		}
	}	
}
/*--------------------------------------------------------------------------------------------------*/
///算分事件
void	ThingHandleEx::ExecuteCountFen(BYTE &nextID,BYTE &time)
{	
	OutputDebugString("20160615::ExecuteCountFen-0");
	//记录正在执行的事件
	pDesk->sGameData.m_byThingDoing = THING_COUNT_FEN;

	pDesk->sGameData.T_CountFen.bIsLiuJu	= pDesk->sGameData.T_HuPai.bIsLiuJu;		//是否流局
	pDesk->sGameData.T_CountFen.bZiMo		= pDesk->sGameData.T_HuPai.bZimo;			//是否自摸
	pDesk->sGameData.T_CountFen.byUser		= pDesk->sGameData.T_HuPai.byUser;			//糊牌玩家位置
	pDesk->sGameData.T_CountFen.byUserNum	= pDesk->sGameData.T_HuPai.byUserNum;		//糊牌玩家的个数	
	pDesk->sGameData.T_CountFen.byDianPao	= pDesk->sGameData.T_HuPai.byDianPao;		//点炮玩家的位置
	pDesk->sGameData.T_CountFen.bQiangGang	= pDesk->sGameData.T_HuPai.bQiangGang;		//是否抢杠
	pDesk->sGameData.T_CountFen.byHuUserNum	= pDesk->sGameData.T_HuPai.byUserNum;		//胡牌玩家的个数
	pDesk->sGameData.T_CountFen.bGangKai	= pDesk->sGameData.T_HuPai.bGangKai;		//是杠开
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//明楼情况
		pDesk->sGameData.T_CountFen.bMingPai[i] = pDesk->sUserData.m_bTing[i];
		//胡牌的玩家
		pDesk->sGameData.T_CountFen.m_bIsHu[i] = pDesk->sGameData.T_HuPai.bHaveHu[i];
		//胡牌类型
		for(int j=0;j<MAX_HUPAI_TYPE;j++)
		{
			pDesk->sGameData.T_CountFen.byHuType[i][j] = pDesk->sGameData.T_HuPai.byHuType[i][j];
		}
	}
	OutputDebugString("20160615::ExecuteCountFen-1");
	//底分
	//int iBase = 5;
	pDesk->sGameData.T_CountFen.iBase = pDesk->m_iBaseMoney;
	//计算各玩杠牌所得番数
	CalculateGangFen();
	OutputDebugString("20160615::ExecuteCountFen-2");
	//荒庄倍数(最高32倍)
	pDesk->sGameData.T_CountFen.iHuangZhuangBase = ::pow(2.0,pDesk->sGameData.m_iHuangZhuangCount);
	if(pDesk->sGameData.T_CountFen.iHuangZhuangBase >= 32)
	{
		pDesk->sGameData.T_CountFen.iHuangZhuangBase = 32;
	}
	OutputDebugString("20160615::ExecuteCountFen-3");
	//流局
	if(pDesk->sGameData.T_HuPai.bIsLiuJu)
	{
		/*for(int i=0; i<PLAY_COUNT; i++)
		{
		pDesk->sGameData.T_CountFen.iZongFen[i] = 0;
		}*/
		//荒庄次数+1
		pDesk->sGameData.m_iHuangZhuangCount++;
		if(pDesk->sGameData.m_iHuangZhuangCount >= 5)
		{
			pDesk->sGameData.m_iHuangZhuangCount = 5;
		}
		//所有玩家都不扣分 只收取 台费
		//for(int i=0; i<PLAY_COUNT; i++)
		//{
			//pDesk->sGameData.T_CountFen.iZongFen[i] = 0;
			//pDesk->sGameData.T_CountFen.iZongFen[i] = pDesk->sGameData.T_CountFen.iFanCount[i] * pDesk->m_iBaseMoney;
		//}

		//下一个事件，游戏结束事件
		nextID = THING_ENG_HANDLE;
		time = 5;
	}
	else
	{
		OutputDebugString("20160615::ExecuteCountFen-4");
		//下一个事件，游戏结束事件
		nextID = THING_ENG_HANDLE;
		time = 5;
		//荒庄次数清0
		pDesk->sGameData.m_iHuangZhuangCount = 0;
		int iHuaCount	= 0;	//花数字
		//int iFanCount	= 1;	//番数
		int iZongFen	= 0;	//总分

		//只有一个人胡牌
		if(1 == pDesk->sGameData.T_HuPai.byUserNum)
		{
			OutputDebugString("20160615::ExecuteCountFen-5");
			bool bHaveFourZhong(false);
			for(int i = 0; i < MAX_HUPAI_TYPE; i++)
			{
				//if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].byHuType[i] == 255)
				if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
					break;
				else
				{
					if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_FOUR_ZHONG)
					{
						bHaveFourZhong = true;
						break;
					}
				}
			}
			OutputDebugString("20160615::ExecuteCountFen-6");
			if(bHaveFourZhong)
			{
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					if(i == pDesk->sGameData.T_HuPai.byUser)
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 18;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] -= 6;
					}
				}
			}
			else
			{
				OutputDebugString("20160615::ExecuteCountFen-7");
				//是否自摸
				CString dwjlog;
				dwjlog.Format("20160615::pDesk->sGameData.T_HuPai.byUser=%d",pDesk->sGameData.T_HuPai.byUser);
				OutputDebugString(dwjlog);
				bool bHaveZhong = pDesk->sUserData.IsHaveAHandPai(pDesk->sGameData.T_HuPai.byUser,35);
				OutputDebugString("20160615::ExecuteCountFen-8");
				if(pDesk->sGameData.T_CountFen.bZiMo)
				{
					for(int i=0; i<PLAY_COUNT; i++)
					{

						if(i == pDesk->sGameData.T_HuPai.byUser)
						{

							if(bHaveZhong)
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] += 3;
							}
							else
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] += 6;
							}
						}
						else
						{
							if(bHaveZhong)
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] -= 1;
							}
							else
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] -= 2;
							}
						}
					}
				}
				else	//不是自摸
				{
					OutputDebugString("20160615::ExecuteCountFen-9");
					for(int i = 0; i < PLAY_COUNT; i++)
					{
						if(i == pDesk->sGameData.T_HuPai.byDianPao)
						{
							pDesk->sGameData.T_CountFen.iGangFan[i] -= 3;
						}
						else
						{
							pDesk->sGameData.T_CountFen.iGangFan[i] += 1;
						}
					}

					if(bHaveZhong)
					{
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byUser] += 3;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byUser] += 6;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 6;
					}
				}
			}
			//胡牌玩家 下局当庄
			pDesk->sGameData.m_byNtStation = pDesk->sGameData.T_HuPai.byUser;
			OutputDebugString("20160615::ExecuteCountFen-10");
		}
		else	//一炮多响
		{
			for(int i=0; i<PLAY_COUNT; i++)
			{
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					if(i == pDesk->sGameData.T_HuPai.byDianPao)
					{
						pDesk->sGameData.T_CountFen.iGangFan[i] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iGangFan[i] += 1;
					}
				}

				if(pDesk->sGameData.T_HuPai.bHaveHu[i])
				{
					bool bHaveZhong = pDesk->sUserData.IsHaveAHandPai(i,35);

					if(bHaveZhong)
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 3;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 6;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 6;
					}
				}
			}
			//一炮多想 点炮玩家当庄
			pDesk->sGameData.m_byNtStation = pDesk->sGameData.T_HuPai.byDianPao;
		}
	}
	OutputDebugString("20160615::ExecuteCountFen-11");
	//计算各玩家总分
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->sGameData.T_CountFen.iZongFen[i] = pDesk->m_iBaseMoney * (pDesk->sGameData.T_CountFen.iGangFan[i] + pDesk->sGameData.T_CountFen.iHuFan[i] + pDesk->sGameData.T_CountFen.iZhongFan[i]);
	}
	OutputDebugString("20160615::ExecuteCountFen-12");
}
/*--------------------------------------------------------------------------------------------------*/
//计算番数和花数
void	ThingHandleEx::CountFanAndHua(BYTE byDeskStation,int &iHuaCount, int &iFanCount)
{
	//计算番数
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
		{
			continue;
		}

		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_PENG_PENG_HU)		//碰碰胡
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_HUN_YI_SE)	//混一色
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_QING_LONG)	//清龙(一条龙)
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_GANG_KAI)// 杠开
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_AN_QI_DUI)	//暗七对
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_QING_YI_SE)	//清一色
		{
			iFanCount += 4;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_JIANG_YI_SE)	//将一色
		{
			iFanCount += 5;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_DI_HU)	// 地胡
		{
			iFanCount += 5;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_TIAN_HU)// 天胡
		{
			iFanCount += 6;
		}
	}

	//计算花数
	for(int i=0;i<HUA_CARD_NUM;i++)
	{
		if(pDesk->sUserData.m_byArHuaPai[pDesk->sGameData.T_HuPai.byUser][i] != 255)
		{
			iHuaCount +=1;
		}
	}
	//计算拦牌中的花牌数
	for(int i=0;i<5;i++)
	{
		//如果是碰
		if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_TRIPLET)
		{
			//如果是 风牌 或者 箭牌 
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=1;
			}
		}
		else if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET || pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET_REVEALED)
		{//如果是补杠||明杠 
			//风牌||箭牌 +2
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=2;
			}
			else	//万条筒 +1
			{
				iHuaCount +=1;
			}
		}
		else if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET_CONCEALED)	
		{//暗杠
			//风牌||箭牌 +2
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=3;
			}
			else	//万条筒 +1
			{
				iHuaCount +=2;
			}
		}
	}
}
/*--------------------------------------------------------------------------------------------------*/
///正常游戏结束事件
void ThingHandleEx::ExecuteNormalEnd(BYTE &nextID,BYTE &time)
{    
	//当前执行事件
	pDesk->sGameData.m_byThingDoing = THING_ENG_HANDLE;

	pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_CountFen.m_byArHandPai,0,true);
	
	//手牌张数
	for(int i=0; i<PLAY_COUNT; i++)
	{
		pDesk->sGameData.T_CountFen.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
	}

	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	//调用平台结束游戏接口
	pDesk->GameFinish(pDesk->sGameData.T_HuPai.byUser,GF_NORMAL);

	pDesk->KillTimer(TIME_WAIT_MSG);
	this->ReSetOutTime();
}




































///异常游戏结束事件事件
void ThingHandleEx::ExecuteUnNormalEnd(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteUnNormalEnd(nextID,time);
}



///断线自动处理函数
void ThingHandleEx::NetCutAutoHandle()
{
	//ThingHandle::NetCutAutoHandle();
	switch(pDesk->sGameData.m_byThingDoing)
	{			        				         
	case THING_2SEZI_NT://首局掷2颗色子的点数和为庄事件		   
	case THING_2SEZI_NT_GP://首局掷2颗色子定庄家和起牌点事件	
	case THING_TURN_2SEZI_NT://首局轮流掷2颗色子定庄家事件		
	case THING_2SEZI_DIR://掷2颗色子事件定起牌方向事件		
	case THING_2SEZI_DIR_GP://掷2颗色子事件定起牌方向和起牌点事件
	case THING_2SEZI_GP://掷2颗色子事件定起牌点事件												
	case THING_2SEZI_JING://掷2颗色子定精牌事件			
	case THING_1SEZI_JIN://掷1颗色子定金牌事件
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(!pDesk->sUserData.m_bSiziFinish[i] && pDesk->sUserData.m_bFoceLeave[i])//该玩家没有发结束消息
				{
					ReceiveSeziFinish(i,NULL,0,false);
				}
			}
		}break;
	case THING_ALL_BUHUA://为所有玩家补花事件
		{
		}
		break;
	case THING_ONE_BUHUA://单个玩家补花事件	
	case THING_CHI_PAI://吃牌事件
	case THING_PENG_PAI://碰牌事件
	case THING_GANG_PAI://杠牌事件
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//如果出牌玩家断线
			{
				AutoOutOrGet();//代替玩家出牌
			}
		}
		break;
	case THING_BEGIN_OUT://庄家开始发牌通知事件	
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sGameData.m_byNtStation])//如果庄家断线
			{
				AutoOutOrGet();
			}
		}
		break;
	case THING_OUT_PAI://出牌事件
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(pDesk->sUserData.m_bFoceLeave[i] && pDesk->sUserData.m_byNowOutStation != i)//玩家玩家断线
				{
					ReceiveUserPassCheck(i,NULL,0,false);//发送放弃消息
				}
			}
		}
		break;
	case THING_ZHUA_PAI://抓牌事件
		{
			OutputDebugString("超时处理：NetCutAutoHandle 000");
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//如果出牌玩家断线
			{
				AutoOutOrGet();
			}
		}
		
	}
}






















//-------------------------------------------动作处理------------------------------------------------------//
/*---------------------------------------------------------------------------------------------------------*/
///吃牌事件
void	ThingHandleEx::HandleChiPai(BYTE &nextID,BYTE &time)
{	

	pDesk->sGameData.m_byThingDoing = THING_CHI_PAI;//记录正在执行的事件
	///吃牌牌事件
//	pDesk->sGameData.T_ChiPai = pDesk->sGameData.temp_ChiPai;//获取数据
//	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.temp_ChiPai.byUser;

	//添加吃牌
	GCPStructEx chi;
	chi.Init();
	chi.byType		= AT_COLLECT;	//拦牌类型
	chi.iBeStation	= pDesk->sGameData.T_OutPai.byUser;	//被吃的玩家 就是之前出牌的玩家
	chi.iOutpai		= pDesk->sGameData.T_OutPai.byPs;	//被吃的牌数据
	chi.iStation	= pDesk->sGameData.T_ChiPai.byUser;	//吃牌的玩家
	//吃牌数据
	for(int i=0;i<3;i++)
	{	
		chi.byData[i] = pDesk->sGameData.T_ChiPai.byChiPs[i];
		if(pDesk->sGameData.T_ChiPai.byChiPs[i] == pDesk->sGameData.T_ChiPai.byPs)
		{
			continue;
		}
		//删除手牌
		pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.T_ChiPai.byUser,pDesk->sGameData.T_ChiPai.byChiPs[i]);
	}

	//删除出牌玩家的最后一张牌
	pDesk->sUserData.DelleteLastOutPai(pDesk->sGameData.T_OutPai.byUser);
	//添加拦牌数据
	pDesk->sUserData.AddToGCP(chi.iStation,&chi);

	//拷贝拦牌数据
	pDesk->sUserData.CopyGCPData(chi.iStation,pDesk->sGameData.T_ChiPai.m_UserGCPData[chi.iStation]);
	//拷贝被吃玩家的出牌数据
	pDesk->sGameData.T_ChiPai.m_byArOutPaiCount[chi.iBeStation] = pDesk->sUserData.CopyOutPai(chi.iBeStation,pDesk->sGameData.T_ChiPai.m_byArOutPai[chi.iBeStation]);

	//吃牌玩家的手牌张数
	pDesk->sGameData.T_ChiPai.m_byArHandPaiCount[chi.iStation] = pDesk->sUserData.GetHandPaiCount(chi.iStation);

	//通知各个玩家
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_ChiPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_PengPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_ChiPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_ChiPai, sizeof(pDesk->sGameData.T_ChiPai), MDM_GM_GAME_NOTIFY, THING_CHI_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_ChiPai, sizeof(pDesk->sGameData.T_ChiPai), MDM_GM_GAME_NOTIFY, THING_CHI_PAI, 0);
	}

	//清除该玩家的所有其他动作
	pDesk->sGameData.T_CPGNotify[chi.iStation].Clear();
	//记录玩家回应
	pDesk->sGameData.T_CPGNotify[chi.iStation].bHaveReturn = true;

	//当前出牌的玩家
	pDesk->sUserData.m_byNowOutStation = chi.iStation;

	//吃完牌后检测动作
	if(CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),4))
	{
		//标记是抓牌后有动作(客户端用于显示倒计时时间的)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
	}
	

	//吃完牌后直接启动超时出牌计时器(因为吃完牌后不可能有胡牌的情况)
	SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);

}
/*---------------------------------------------------------------------------------------------------------*/
///碰牌事件
void	ThingHandleEx::HandlePengPai(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_PENG_PAI;//记录正在执行的事件
	
	
	//碰牌牌事件
	GCPStructEx peng;
	peng.Init();
	peng.byType		= AT_TRIPLET;	//碰类型
	peng.iStation	= pDesk->sGameData.T_PengPai.byUser;	//吃碰杠玩家位置
	peng.iBeStation	= pDesk->sGameData.T_OutPai.byUser;		//被碰的玩家 就是之前出牌的玩家
	peng.iOutpai	= pDesk->sGameData.T_OutPai.byPs;		//被碰的牌数据

	//保存三张碰的牌
	for(int i=0;i<3;i++)
	{
		peng.byData[i] = pDesk->sGameData.T_OutPai.byPs;
	}

	//出牌玩家删除最后一张出牌
	pDesk->sUserData.DelleteLastOutPai(peng.iBeStation);
	//删除碰牌玩家的手上两张牌
	for(int i=0;i<2;i++)
	{	
		pDesk->sUserData.DelleteAHandPai(peng.iStation, peng.iOutpai);
	}
	
	//添加碰牌
	pDesk->sUserData.AddToGCP(peng.iStation,&peng);

	//拷贝拦牌数据
	pDesk->sUserData.CopyGCPData(peng.iStation,pDesk->sGameData.T_PengPai.m_UserGCPData[peng.iStation]);
	//拷贝被碰玩家的出牌数据
	pDesk->sGameData.T_PengPai.m_byArOutPaiCount[peng.iBeStation] = pDesk->sUserData.CopyOutPai(peng.iBeStation,pDesk->sGameData.T_PengPai.m_byArOutPai[peng.iBeStation]);

	//碰牌玩家的手牌张数
	pDesk->sGameData.T_PengPai.m_byArHandPaiCount[peng.iStation] = pDesk->sUserData.GetHandPaiCount(peng.iStation);

	//通知各个玩家
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_PengPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_PengPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_PengPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
	}
	//清除该玩家的所有其他动作
	pDesk->sGameData.T_CPGNotify[peng.iStation].Clear();
	//记录玩家回应
	pDesk->sGameData.T_CPGNotify[peng.iStation].bHaveReturn = true;

	//当前出牌的玩家
	pDesk->sUserData.m_byNowOutStation = peng.iStation;

	//吃完牌后检测动作
	if(CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),4))
	{
		//标记是抓牌后有动作(客户端用于显示倒计时时间的)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
	}

	//吃完牌后直接启动超时出牌计时器(因为吃完牌后不可能有胡牌的情况)
	SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
}

//算分事件(主要是针对杠牌情况)
void    ThingHandleEx::CalculateGangFen(void)
{
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			GCPStructEx gang;
			gang = pDesk->sUserData.m_UserGCPData[i][j];
			switch(gang.byType)
			{
			case AT_QUADRUPLET_CONCEALED:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 6;
					for(int k = 0; k < PLAY_COUNT; k++)
					{
						if(i == k)
							continue;
						pDesk->sGameData.T_CountFen.iGangFan[k] -= 2;
					}
					break;
				}
			case AT_QUADRUPLET:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 3;
					for(int k = 0; k < PLAY_COUNT; k++)
					{
						if(i == k)
							continue;
						pDesk->sGameData.T_CountFen.iGangFan[k] -= 1;
					}
					break;
				}
			case AT_QUADRUPLET_REVEALED:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 3;
					BYTE fangGang = pDesk->sUserData.m_UserGCPData[i][j].iBeStation;
					pDesk->sGameData.T_CountFen.iGangFan[fangGang] -= 3;
					break;
				}
			default:
				break;
			}
		}
	}
}
/*---------------------------------------------------------------------------------------------------------*/
///杠事件
void	ThingHandleEx::HandleGangPai(BYTE &nextID,BYTE &time)	
{
	/*先判断是什么类型的杠*/
	//如果当前是出牌时间 那么就是明杠
	BYTE byGangType = 255;
	if(pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)
	{
		byGangType = AT_QUADRUPLET_REVEALED;
	}
	else if(pDesk->sGameData.m_byThingDoing == THING_ZHUA_PAI || THING_ONE_BUHUA == pDesk->sGameData.m_byThingDoing || THING_BEGIN_OUT == pDesk->sGameData.m_byThingDoing)	
	{
		//如果是抓牌状态 || 补花状态 || 开始出牌状态 那么就去判断是暗杠 还是补杠
		for(int i=0; i<5; i++)
		{
			//拦牌数据中有该数据 && 类型是碰
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_GangPai.byUser][i].byData[0] == pDesk->sGameData.T_GangPai.byPs && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_GangPai.byUser][i].byType == ACTION_PENG)
			{
				byGangType = AT_QUADRUPLET;
				break;
			}
		}

		if(byGangType != AT_QUADRUPLET)
		{
			byGangType = AT_QUADRUPLET_CONCEALED;
		}
	}

	pDesk->sGameData.m_byThingDoing = THING_GANG_PAI;//记录正在执行的事件

	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_GangPai.byUser;

	//杠牌数据
	GCPStructEx gang;
	gang.Init();
	gang.byType		= byGangType;	//杠类型
	gang.iStation	= pDesk->sGameData.T_GangPai.byUser;	//吃碰杠玩家位置
	gang.iBeStation	= pDesk->sGameData.T_GangPai.byBeGang;	//被杠的玩家 就是之前出牌的玩家
	gang.iOutpai	= pDesk->sGameData.T_GangPai.byPs;		//被杠的牌数据
	//杠牌数据
	for(int i=0; i<4; i++)
	{
		gang.byData[i] = pDesk->sGameData.T_GangPai.byPs;
	}

	//如果是明杠
	if(byGangType == AT_QUADRUPLET_REVEALED)
	{
		//删除碰牌玩家的手上3张牌
		for(int i=0;i<3;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}

		//删除出牌玩家最后一张出牌
		pDesk->sUserData.DelleteLastOutPai(gang.iBeStation);
		//拷贝被杠玩家的出牌数据
		pDesk->sGameData.T_GangPai.m_byArOutPaiCount[gang.iBeStation] = pDesk->sUserData.CopyOutPai(gang.iBeStation,pDesk->sGameData.T_GangPai.m_byArOutPai[gang.iBeStation]);

		//记录玩家正处于杠牌状态(用于判断是否杠开) 只有明杠是 才算杠开
		pDesk->sGameData.m_bGangState[gang.iStation] = true;
	}
	else if(byGangType == AT_QUADRUPLET)	//补杠
	{
		//删除碰牌玩家的手上1张牌
		for(int i=0;i<1;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}
		//删除相应碰牌
		pDesk->sUserData.DelleteAGCPData(gang.iStation,ACTION_PENG,gang.iOutpai);
	}
	else if(byGangType == AT_QUADRUPLET_CONCEALED)	//暗杠
	{
		//删除碰牌玩家的手上4张牌
		for(int i=0;i<4;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}
	}
	
	//添加杠牌数据
	pDesk->sUserData.AddToGCP(gang.iStation,&gang);

	//拷贝拦牌数据
	pDesk->sUserData.CopyGCPData(gang.iStation,pDesk->sGameData.T_GangPai.m_UserGCPData[gang.iStation]);

	//杠牌玩家的手牌张数
	pDesk->sGameData.T_GangPai.m_byArHandPaiCount[gang.iStation] = pDesk->sUserData.GetHandPaiCount(gang.iStation);
//	pDesk->sUserData.m_bIsGangPao[pDesk->sGameData.temp_GangPai.byUser] = true;//正在杠后炮的状态
	pDesk->sGameData.T_GangPai.byType = byGangType;
	//通知各个玩家
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_GangPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_GangPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_GangPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
	}


	//清除该玩家的所有其他动作
	pDesk->sGameData.T_CPGNotify[gang.iStation].Clear();
	//记录玩家回应
	pDesk->sGameData.T_CPGNotify[gang.iStation].bHaveReturn = true;

	//当前出牌的玩家
	pDesk->sUserData.m_byNowOutStation = gang.iStation;

	////如果是补杠就要检测是否有人抢杠胡 
	if(byGangType == AT_QUADRUPLET && CheckAction(gang.iStation,gang.iOutpai,3))
	{
		bool bHaveSend = false;	//是否已经发送了消息
		BYTE byCanActionDesk = 255;	//可以做动作的玩家
		//有人抢杠胡 那么就发送拦牌消息
		for(int i=0; i<PLAY_COUNT; i++)
		{
			if(i == gang.iStation)
			{
				continue;
			}
			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}
				bHaveSend		= true;	//是否已经发送了消息
				byCanActionDesk = i;	//可以做动作的玩家
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);
		}
		else
		{
			//抓牌事件
			pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
			ApplyAction(THING_ZHUA_PAI,100);
		}
	}
	else
	{
		//抓牌事件
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(THING_ZHUA_PAI,100);
	}
}

/*--------------------------------------------------------------------------------------*/
/*	计时器函数群
/*--------------------------------------------------------------------------------------*/
/// 处理计时器消息	DWJ
void	ThingHandleEx::OnGameTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case 10://事件计时器id
		{
			pDesk->KillTimer(uTimerID);
			GameCPU(pDesk->sGameData.ApplyThingID);
			break;  
		}
	case TIME_WAIT_MSG: // 超过事件无响应，是超时自动处理，系统自动处理
		{
			OutTimeHandle();
			break;
		}





	//case TIME_CHECK_GAME_MSG://消息超时，自动处理
	//	{
	//		//pDesk->KillTimer(uTimerID);
	//		//AutoHandle();
	//		ThingHandle::OnGameTimer( uTimerID);
	//	}
	//	break;

	}
}

/*--------------------------------------------------------------------------------------*/
//	设置当前正在发生的事件	DWJ
void	ThingHandleEx::SetDoingThing(BYTE byStation,BYTE thing, BYTE byTime)
{
	pDesk->sGameData.m_byWatingThing = thing;
	// 恢复超时数据
	ReSetOutTime();
	switch(thing)
	{		
	case THING_BEGIN_OUT://庄家开始发牌通知事件		
		{
			if(255 == byTime)
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byOutTime,thing,true);
			}
			else
			{
				SetOutTime(byStation,byTime,thing,true);
			}
			
			break;
		}
	case THING_OUT_PAI://出牌事件
		{
			//听牌了 或者托管了 1秒出牌
			if(pDesk->sUserData.m_bTing[byStation] || pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byOutTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_CHI_PAI:		//吃牌事件				DWJ
	case THING_PENG_PAI:	//碰牌事件				DWJ
	case THING_GANG_PAI:	//杠牌事件				DWJ
		{
			//听牌了 或者托管了 1秒处理
			if(pDesk->sUserData.m_bTing[byStation] || pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byBlockTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_HU_PAI:		//胡牌事件				DWJ
		{
			//托管了 1秒出牌
			if(pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byBlockTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_MAI_MA:	//买码事件
		{
			SetOutTime(byStation,byTime,thing,true);
			break;
		}
	}
}
/*--------------------------------------------------------------------------------------*/
/// 恢复超时数据	DWJ
void	ThingHandleEx::ReSetOutTime()
{
	pDesk->sGameData.m_MjWait.Clear();
}
/*--------------------------------------------------------------------------------------*/
/// 设置超时数据
void	ThingHandleEx::SetOutTime(BYTE station,BYTE time,BYTE thing,bool effect)
{
	pDesk->sGameData.m_MjWait.byWaitThing	= thing;
	pDesk->sGameData.m_MjWait.byCurChair	= station;
	pDesk->sGameData.m_MjWait.byTime		= time + 1;
	pDesk->sGameData.m_MjWait.bEffect		= effect;
}
/*--------------------------------------------------------------------------------------*/
/// 超时处理
void	ThingHandleEx::OutTimeHandle()
{
	if(!pDesk->sGameData.m_MjWait.bEffect || pDesk->sGameData.m_MjWait.byCurChair == 255 || pDesk->sGameData.m_MjWait.byTime == 255)
	{
		return;
	}
	if(pDesk->sGameData.m_MjWait.byWaitThing != pDesk->sGameData.m_byWatingThing)
	{
		return;
	}
	if(pDesk->sGameData.m_MjWait.byTime == 255)
	{
		return;
	}
	pDesk->sGameData.m_MjWait.byTime -= 1;
	if(pDesk->sGameData.m_MjWait.byTime == 0)
	{
		if(!pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair])
			pDesk->m_byAutoTime[pDesk->sGameData.m_MjWait.byCurChair]++;
		if(pDesk->m_byAutoTime[pDesk->sGameData.m_MjWait.byCurChair] > 1 && !pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair])
		{
			pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair] = true;
			UserTouGuan	TUserTouGuan;
			TUserTouGuan.byStation	= pDesk->sGameData.m_MjWait.byCurChair;
			TUserTouGuan.bIsTuoGuan	= true;
			for(int i=0;i<PLAY_COUNT;++i)
			{
				TUserTouGuan.bTuoGuan[i] = pDesk->sUserData.m_bTuoGuan[i];		
			}
			for(int i=0;i<PLAY_COUNT;i++)
			{
				pDesk->SendGameDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
				pDesk->SendWatchDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
			}
		}
		this->AutoHandle();
	}
}
/*--------------------------------------------------------------------------------------*/
//玩家处理超时，系统自动处理	DWJ
void	ThingHandleEx::AutoHandle()
{
	switch(pDesk->sGameData.m_byWatingThing)
	{	
	case THING_BEGIN_OUT:	//庄家开始发牌自动出牌响应
		{
			//系统代替玩家出牌
			AutoOutOrGet();
			break;
		}
	case THING_OUT_PAI:		//玩家自动出牌		DWJ
		{
			//系统代替玩家出牌
			AutoOutOrGet();
			break;
		}
	case THING_CHI_PAI:		//自动吃牌响应		DWJ
	case THING_PENG_PAI:	//自动碰牌响应		DWJ
	case THING_GANG_PAI:	//自动杠牌响应		DWJ
		{
			//系统代替玩家做出动作
			AutoDoAction();
			break;
		}
	case THING_HU_PAI:		//自动胡牌响应		DWJ
		{
			AutoHuPai();
			break;
		}
	case THING_MAI_MA:	//自动买码处理
		{
			ApplyAction(THING_ZHONG_NIAO,10);
			break;
		}
	}
}
/*--------------------------------------------------------------------------------------*/
///自动处理出牌或者抓牌
void	ThingHandleEx::AutoOutOrGet()
{
	bool haveout = false;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!pDesk->sUserData.IsOutPaiPeople(i))//出牌玩家
		{
			continue;
		}
		haveout = true;
		tagOutPaiEx outpai;
		outpai.Clear();
		outpai.byUser = i;
		outpai.byPs = pDesk->sUserData.GetLastHandPai(i);

		ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
		break;

	}
	//没有出牌的玩家，启动抓牌事件
	if(!haveout)
	{
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(pDesk->sGameData.ApplyThingID,200);//申请下一个事件
	}
}
/*--------------------------------------------------------------------------------------*/
//	自动处理动作	DWJ
void	ThingHandleEx::AutoDoAction()
{
	//这里要将结果发送给客户端 告诉客户端 超时处理 系统自动处理了
	ReceiveUserPassCheck(pDesk->sGameData.m_MjWait.byCurChair,NULL,0,false,true);

}
/*--------------------------------------------------------------------------------------*/
//	自动处理胡牌	DWJ
void	ThingHandleEx::AutoHuPai()
{
	//请求胡牌
	tagHuPaiEx hu;
	hu.Clear();
	hu.byUser	= pDesk->sGameData.m_MjWait.byCurChair;
	ReceiveUserHuCard(pDesk->sGameData.m_MjWait.byCurChair, &hu, sizeof(hu), false);
}