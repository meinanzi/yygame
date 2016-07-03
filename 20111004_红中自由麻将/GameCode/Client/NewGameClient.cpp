#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
#include "Define.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "..\GameMessage\UpgradeMessage.h"
#include "UI8_ControlsDefine.h"

#include "SoundDefines.h"

#include "LoveSendClassInExe.h"



#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	(( PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID)) )!=NULL)

//多字节编码转换成宽字节编码
void char2Wchar(char* pchar,wchar_t* wchar)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

//宽字节编码转换成多字节编码
void wchar2char(wchar_t* wchar,char* pchar)
{
	int len = WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),NULL,0,NULL,NULL);   
	WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),pchar,len,NULL,NULL);
}

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
    return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
   if (*ppImp!=NULL)
    {
        delete *ppImp;
        *ppImp = NULL;
    }
}

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	//游戏框架
    m_pGameFrame = pGameFrame;
	//背景音乐引擎
	m_SoundEnginBg = createIrrKlangDevice();
	//声效引擎
	m_SoundEngineGame = createIrrKlangDevice();
	//游戏背景音乐声效开关
	m_bBgMusic = true;
	//游戏音效开关
	m_bGameSound = true;
	//游戏UI
    m_pUI = pUI;
	//游戏状态
 	m_bGameStation = GS_WAIT_SETGAME;

	m_bIsSuperUser = false;

	m_nDeskStation = 0;
	m_GameData.InitData();
	m_UserData.InitData();
}

CNewGameClient::~CNewGameClient()
{
	//关闭背景音乐引擎
	if(m_SoundEnginBg != NULL)
	{
		m_SoundEnginBg->drop();
	}
	//关闭游戏音效引擎
	if(m_SoundEngineGame != NULL)
	{
		m_SoundEngineGame->drop();
	}
}

/*-----------------------------------------------------------------------------------------------------------*/
/*	一系列平台提供的需要重载来驱动游戏的函数实现，已适用大多数游戏的开发，但每个函数的实现因游戏而异。		 */
/*-----------------------------------------------------------------------------------------------------------*/
//初始化
int CNewGameClient::Initial()
{
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	LoadGameSet();
    return 0;
}
/*----------------------------------------------------------------------------------------------*/
//初始化UI
int CNewGameClient::InitUI()
{		

	ReStartGameInitUI();
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//所有玩家信息控件
		UIShowUserInfo(i,false);
	}
	
	return 0;
}
/*----------------------------------------------------------------------------------------------*/
///重新开局时的初始化
void CNewGameClient::ReStartGameInitUI()
{
	//开始按钮
	SetBtVisible(CTN_1000000_BTN_10001,false);
	//时间框
	SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,false,0);
	///左上角东风X局
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020100,false);
	///左上角荒庄X
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020101,false);
	///左上角荒庄数字
	SetInumInfo(CTN_1000000_CTN_2002000_Num_20020102,0,false);
	ShowSuperDlag(false);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		//时间上四个方向
		SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+i,false);
		//准备标识
		//SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//庄家标识
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+i,false);
		//托管标识
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+i,false);
		//东南西北标识
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+i,false);
		//花数底图
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+i,false);
		//花数
		SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+i,0,false);
		//听牌标识
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+i,false);
		//托管按钮
		SetBtVisible(CTN_1000000_BTN_2010101,true);
		//取消托管按钮
		SetBtVisible(CTN_1000000_BTN_2010102,false);

	
		//先清空所有的拦牌信息 再一个个添加
		ClearMjHandBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010 + i);
		//麻将手牌
		SetMjHandVisible(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);

		//所有牌起立
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);
		//手牌最后一张不隔开
		SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);
		//牌墙控件
		SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,0,false);
		//出牌控件
		SetMjGiveInfo(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+i,8,NULL,0,false);	
	}

	//拦牌容器
	UIShowBlockContain(false);
	//吃牌容器
	UIShowChiContain(false);
	//结算框
	UIShowResultDlg(false);
	//买码容器
	ShowMaiMaContain(false);

	//出牌动画
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
	
}
/*----------------------------------------------------------------------------------------------*/
//游戏消息	DWJ
int		CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
    switch(nSubId)
	{
	case ASS_GM_GAME_STATION:	///<设置状态(断线重连消息)	DWJ
		{
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_GM_AGREE_GAME:		//	玩家同意消息	DWJ
		{
			OnUserAgree( buffer,  nLen);           
			break;
		}
	case ASS_AUTOCARD:			//玩家托管通知/// 托管消息
		{
			OnUserTuoguan( buffer,  nLen);
			break;
		}
	case THING_GAME_BEGIN:		//	游戏开始事件	DWJ
		{
			OnGameStart( buffer,  nLen);
			break;
		}
	case THING_DING_NT:			//	定桩消息	DWJ
		{
			OnDingNt( buffer,  nLen);					
			break;
		}
	case THING_2SEZI_NT:        //第一次筛子，确认庄家方向
		{
			On2SeZiNT(buffer,nLen);
			break;
		}
	case THING_2SEZI_DIR:		//第二次筛子 确认起牌方向消息	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			On2SeZiDir(buffer,  nLen);
			break;
		}
	case THING_2SEZI_GP://	第三次筛子确认起牌墩数消息	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			On2SeZiGP(buffer,nLen);
			break;
		}
	case THING_SEND_PAI:		//	发牌消息	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			OnSendAllCard( buffer,  nLen);    
			break;
		}
	case THING_ALL_BUHUA:		//	所有玩家补花事件	DWJ
		{
			OnAllBuHua( buffer,  nLen);
			break;
		}
	case THING_BEGIN_OUT:		//	开始出牌通知	DWJ
		{
			OnBeginOutCard( buffer,  nLen);              
			break;
		}
	case THING_OUT_PAI:			//	玩家出牌消息	DWJ
		{
			OnNotifyOutCard( buffer,  nLen);          
			break;
		}
	case THING_CPG_NOTIFY:		//	吃、碰、杠、胡拦牌动作消息
		{
			//OnNotifyBlock( buffer,  nLen);         
			break;
		}
	case THING_HAVE_ACTION:
		{
			OnNotifyAction(buffer,nLen);
			break;
		}
	case THING_ZHUA_PAI:		//	抓牌消息	
		{
			OnNotifyZhuaPai( buffer,  nLen);          
			break;
		}
	case THING_ONE_BUHUA:		//	单个玩家补花
		{
			OnNotifyOneBuHua( buffer,  nLen);          
			break;
		}
	case THING_GUO_PAI:			//	放弃拦牌	
		{
			OnNotifyCanCelBlock(buffer,  nLen);
			break;
		}
	case THING_CHI_PAI:			//	吃牌结果消息
		{
			OnNotifyChiPai( buffer,  nLen);           
			break;
		}
	case THING_PENG_PAI:		//	碰牌结果消息
		{
			OnNotifyPengPai( buffer,  nLen);          
			break;
		}
	case THING_GANG_PAI:		//	杠牌结果消息
		{
			OnNotifyGangPai( buffer,  nLen);          
			break;
		}
	case THING_HU_PAI:			 //	胡牌结果消息
		{
			OnNotifyHuPai( buffer,  nLen);            
			break;
		}
	case THING_MAI_MA:	//买码
		{
			OnNotifyMaiMa(buffer,  nLen);
			break;
		}
	case THING_ZHONG_NIAO:
		{
			OnNotifyZhongNiao(buffer,nLen);
			break;
		}
	case THING_ENG_HANDLE:		/// 算分：游戏结束
		{
			OnNotifyFinish( buffer,  nLen);         
			break;
		}
	case ASS_CHANGE_PAI_RESULT:	//玩家换牌结果
		{
			OnUserChangeCard(buffer,nLen);
			break;
		}
	default:
		{
			break;
		}
	}
    return 0;
}
/*----------------------------------------------------------------------------------------------*/
//界面消息	
int		CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	if(pMessage->_uMessageType==UI_MJCTRL_MOVEONMJ) // 麻将控件消息，在禁止使用鼠标情况下，移到牌上就发消息
    {
        OnMoveOnMjCtrl(pMessage);
    }
    else if (pMessage->_uMessageType==UI_MJCTRL_LBUTTON)
    {
        OnLButtonOnMjCtrl(pMessage);
    }
	else if(pMessage->_uMessageType == UI_MOUSEENTER)	//鼠标移入移出
	{
		switch (pMessage->_uControlID)
		{
		case CTN_1_BTN_10001:
		case CTN_1000000_BTN_2010101:				///<拖管  按扭 GameGrand\Bn_tuoguan.png
		case CTN_1000000_BTN_2010102 :				///<取消拖管  按扭 GameGrand\Bn_quxiaotuoguan.png
		case CTN_1000000_CTN_2003000_BTN_2003001:   ///<过  按扭 GameGrand\guo_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003010:	///<吃  按扭 GameGrand\chi_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003011:   ///<碰  按扭 GameGrand\peng_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003012:   ///<杠  按扭 GameGrand\gang_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003013:   ///<听  按扭 GameGrand\gang_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003014:   ///<胡  按扭 GameGrand\hu_bt.png
			{
				PlayGameSound(m_nDeskStation,SOUND_MOUSE_ENTER);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				//显示吃牌底图
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070012:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070013:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,m_bMouseEnter);
				break;
			}
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080022,false);
				break;
			}
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080012:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,false);
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080022,m_bMouseEnter);
				break;
			}
		case CTN_1000000_CTN_2009000_MjHandCtrl_20090011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2009000_IMG_20090021,m_bMouseEnter);
				break;
			}
		}
	}
	else if(pMessage->_uMessageType==UI_RBUTTONDOWN)
	{
		//请求杠牌状态
		if(m_byHandState == 2)	
		{
			//设置手牌选择杠牌状态
			SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
			//手牌状态
			m_byHandState = 0xFF;
		}
		else if(m_byHandState == 1)
		{
			//隐藏吃牌容器
			UIShowChiContain(false);
			//显示拦牌容器
			UIShowBlockContain(true);
			//手牌状态
			m_byHandState = 0xFF;
		}
	}
	else if(pMessage->_uMessageType == UI_LBUTTONDOWN) // 单击
	{
		switch (pMessage->_uControlID)
		{
        case CTN_1000000_CTN_2005000_MjHandCtrl_2005012:       // 自家手牌	DWJ
            {
				if(!m_UserData.m_bTuoGuan[m_nDeskStation])//手动打牌，重现计算超时次数
				{
					m_nTimeOutCnt = 0;
				}
				//听牌了 || 托管了 不能出牌
				if(m_UserData.m_bTing[m_nDeskStation])
				{
					return 0;
				}

				//听牌了 || 托管了 不能出牌
				if(m_UserData.m_bTuoGuan[m_nDeskStation])
				{
					if(m_UserData.m_byNowOutStation == m_nDeskStation)
					{
						//m_pGameFrame->InsertNormalMessage("出牌请先取消托管!");
					}
					
					return 0;
				}

				//抓到花牌的时候 不能出牌
				if(m_bIsHua)
				{
					return 0;
				}
				
				if(m_byHandState == 0xFF)
				{
					// 请求出牌                
					OnUIRequestOutCard(pMessage->_uControlID); 
				}
				else if(m_byHandState == 2)
				{
					//请求杠牌
					OnUIRequestGangPai(pMessage->_uControlID);
				}
				break;
            }
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070011:
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080011:
		case CTN_1000000_CTN_2009000_MjHandCtrl_20090011:	//点击确定吃牌动作
			{
				if(1 == m_byHandState)
				{
					//请求吃牌
					RequestBlockChi(0);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070012:
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080012:	//点击确定吃牌动作
			{
				if(1 == m_byHandState)
				{
					//请求吃牌
					RequestBlockChi(1);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070013:	//点击确定吃牌动作
			{
				//请求吃牌状态
				if(1 == m_byHandState)
				{
					//请求吃牌
					RequestBlockChi(2);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070002:
		case CTN_1000000_CTN_2008000_BTN_20080002:
		case CTN_1000000_CTN_2009000_BTN_20090002:	//取消吃
			{
				//隐藏吃牌容器
				UIShowChiContain(false);
				//显示拦牌容器
				UIShowBlockContain(true);
				//手牌状态
				m_byHandState = 0xFF;
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003001:	//过
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003001))
				{
					m_nTimeOutCnt = 0;
					RequestCancelBlock();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003010:	//请求吃牌
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003010))
				{
					//dwjtest 右击取消吃牌
					OnUiClickChiPai();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003011:	//请求碰牌
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003011))
				{
					//	请求碰牌
					m_nTimeOutCnt = 0;
					RequestBlockPeng();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003012:	//请求杠牌
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003012))
				{
					//	请求杠牌
					m_nTimeOutCnt = 0;
					RequestBlockGang();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003013:	//请求听牌
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003013))
				{
					//	请求听牌
					RequestBlockTing();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003014:	//请求胡牌
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003014))
				{
					// 请求胡牌
					m_nTimeOutCnt = 0;
					RequestBlockHu();
				}
				break;
			}
		case 2003015://抢杠胡
			{
				if(GetBtIsEnable(2003015))
				{
					// 请求抢杠胡
					RequestBlockHu();
				}
				break;
			}
		case CTN_1000000_BTN_2010101:	//请求托管
			{
				RequestTuoGuan();
				break;
			}
		case CTN_1000000_BTN_2010102:	//请求取消托管
			{
				RequestTuoGuan(false);
				break;
			}
		case CTN_1000000_CTN_2001000_BTN_2001101:	//离开
			{
				m_pGameFrame->CloseClient();
				break;
			}
		case CTN_1000000_CTN_2001000_BTN_2001102:	//继续
			{
				InitGameDate();
				ReStartGameInitUI();
				m_pGameFrame->ClickContinue();
				break;
			}
		case CTN_1000000_BTN_1003500: //配置游戏
			{
				//显示游戏设置
				UIShowGameSetting(true);
				break;
			}
		case CTN_100000078_BTN_100000088:	//确定配置
			{
				SaveGameSet();
				//显示游戏设置
				UIShowGameSetting(false);
				break;
			}
		case CTN_100000078_BTN_100000089:	//取消配置
			{
				//显示游戏设置
				UIShowGameSetting(false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070001:
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070002:
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070003:
			{
				// 显示吃碰杠面板
				int iUpTile = 0;
				int iLogicTile = 0;

				IMjHandCtrl *pMjHand = NULL;
				pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(pMessage->_uControlID));
				if (NULL!=pMjHand)
				{
					iUpTile = pMjHand->GetUpTile();

				}  
				if (iUpTile != 0)
				{
					//设置为被选中的麻将牌
					SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,&iUpTile,1,true);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070031:	//换牌自己的手牌
			{
				int iUpTile = 0;

				IMjHandCtrl *pMjHand = NULL;
				pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(CTN_1000000_CTN_2007000_MjHandCtrl_20070031));
				if (NULL!=pMjHand)
				{
					iUpTile = pMjHand->GetUpTile();

				}  
				if (iUpTile != 0)
				{
					//设置为被选中的麻将牌
					SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,&iUpTile,1,true);
				}

				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070051:		//换牌确定
			{
				OnUiChangeCard();
				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070052:		//换牌取消
			{
				ShowSuperDlag(false);
				break;
			}
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100011 :  ///<麻将手牌控件1  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100012 :  ///<麻将手牌控件2  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100013 :  ///<麻将手牌控件3  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100014 :  ///<麻将手牌控件4  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100015 :  ///<麻将手牌控件5  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100016 :  ///<麻将手牌控件6  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100017 :  ///<麻将手牌控件7  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100018 :  ///<麻将手牌控件8  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100019 :  ///<麻将手牌控件9  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100020 :  ///<麻将手牌控件10  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100021 :  ///<麻将手牌控件11  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100022 :  ///<麻将手牌控件12  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100023 :  ///<麻将手牌控件13  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100024 :  ///<麻将手牌控件14  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100025 :  ///<麻将手牌控件15  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100026 :  ///<麻将手牌控件16  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100027 :  ///<麻将手牌控件17  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100028 :  ///<麻将手牌控件18  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100029 :  ///<麻将手牌控件19  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100030 :  ///<麻将手牌控件20  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100031 :  ///<麻将手牌控件21  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100032 :  ///<麻将手牌控件22  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100033 :  ///<麻将手牌控件23  麻将手牌控件 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100034 :  ///<麻将手牌控件24  麻将手牌控件 
			{
				//判断是否翻完了
				if (m_byMaiMaIndex >= m_GameData.m_TMaiMa.byCount)
				{
					break;;
				}
				//判断是否已经翻开
				if (0 != GetMjHandOneCard(pMessage->_uControlID))
				{
					break;
				}
				int iCard = m_GameData.m_TMaiMa.byCard[m_byMaiMaIndex];
				SetMjHandInfo(pMessage->_uControlID,&iCard,1,true);

				m_byMaiMaIndex++;

				if (m_byMaiMaIndex >= m_GameData.m_TMaiMa.byCount)
				{
					CString sText;
					sText.Format("买码结束，请等待系统确认！");
					SetTextinfo(CTN_1000000_CTN_2010000_TEXT_20100002,sText,true);
				}

				break;
			}
        default:
			{
				break;
			}
        }
    }
    else if (pMessage->_uMessageType==UI_CATOONFINISH)	//动画结束	DWJ
    {
        OnCartoonFinish(pMessage);
//        if (ZHUAPAI_ACT_MVACT==pMessage->_uControlID && m_GameData.m_byThingDoing < THING_HU_PAI)
//        {
//            // 这里加上抓牌动画
   
//            IMjWallCtrl *pWall = NULL;
//			GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL);
//            if ( pWall != NULL )
//            {             
//                pWall->SetControlVisible(false);
//            }
//        }
    }
	else if(pMessage->_uMessageType==UI_KEYDOWN)
	{
		if (pMessage->_ch == VK_F12 && m_bIsSuperUser)
		{
			ShowSuperDlag(true);
		}
	}
    return 0;
}

//显示买码容器
void	CNewGameClient::ShowMaiMaContain(bool bShow)
{
	SetContainVisible(CTN_1000000_CTN_2010000,false);
	if (bShow)
	{
		SetContainVisible(CTN_1000000_CTN_2010000,true);
		SetImageVisible(CTN_1000000_CTN_2010000_IMG_20100001,true);
		CString sText;
		sText.Format("请买码%d个",m_GameData.m_TMaiMa.byCount);
		SetTextinfo(CTN_1000000_CTN_2010000_TEXT_20100002,sText,true);
		int iCard = 0;
		for(int i=0; i<24; ++i)
		{
			SetMjHandInfo(CTN_1000000_CTN_2010000_MjHandCtrl_20100011+i,&iCard,1,true);
		}

		SetTimePlay(CTN_1000000_CTN_2010000_Time_20100003,true,m_GameData.m_mjRule.byOutTime,0);
	}
}

//响应换牌
void	CNewGameClient::OnUiChangeCard()
{
	if (!m_bIsSuperUser)
	{
		return;
	}
	tagSuperChangeCard TSuperChangeCard;
	TSuperChangeCard.byStation = m_nDeskStation;
	TSuperChangeCard.byChangeCard = GetMjHandOneCard(CTN_1000000_CTN_2007000_MjHandCtrl_20070021);
	TSuperChangeCard.byNewCard = GetMjHandOneCard(CTN_1000000_CTN_2007000_MjHandCtrl_20070022);

	if (TSuperChangeCard.byChangeCard == 0 || TSuperChangeCard.byChangeCard == 255  || TSuperChangeCard.byNewCard == 0 || TSuperChangeCard.byNewCard==255)
	{
		return;
	}

	SendGameDataEx(ASS_CHANGE_PAI, &TSuperChangeCard, sizeof(TSuperChangeCard));
}
//获取麻将手牌一张牌数据
int		CNewGameClient::GetMjHandOneCard(const int iMjHandID)
{
	int iLogicTile = 0;
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL!=pMjHand)
	{
		pMjHand->GetHandInfo(&iLogicTile,1);

	}  
	return iLogicTile;
}
void	CNewGameClient::ShowSuperDlag(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2007100,bFlag);
	SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070041,bFlag);
	SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070042,bFlag);


	//显示万
	int iMjCard[9];
	for(int i=0; i<9; i++)
	{
		iMjCard[i] = i+1;
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070001,iMjCard,9,bFlag);
	//条
	for(int i=0; i<9; i++)
	{
		iMjCard[i] = 10+i+1;
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070002,iMjCard,9,bFlag);
	//筒+红中
	int iMjCard1[10];
	for(int i=0; i<9; i++)
	{
		iMjCard1[i] = 20+i+1;
	}
	iMjCard1[9] = 35;
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070003,iMjCard1,10,bFlag);

	//自己的手牌麻将
	int iHandCard[MAX_HAND_PAI];
	for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
	{
		iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070031,iHandCard, m_UserData.m_byArHandPaiCount[m_nDeskStation],bFlag);

	//被换的麻将
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,NULL,0,false);
	//换需要的麻将
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,NULL,0,false);

}
/*----------------------------------------------------------------------------------------------*/
// 请求出牌		DWJ             
void	CNewGameClient::OnUIRequestOutCard(UINT32 uControlID,bool bOuttime )
{
	// 显示吃碰杠面板
	int iUpTile = 0;
	BYTE byLogicTile = 0;

	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(uControlID));
	if (NULL!=pMjHand)
	{
		iUpTile = pMjHand->GetUpTile();
	}  

	if(iUpTile == 35)
		return;

	byLogicTile = iUpTile;
	
	if(bOuttime)//超时出牌
	{
		byLogicTile = m_UserData.m_byLastZhuaPai[m_nDeskStation];
		if(byLogicTile == 255)
		{
			byLogicTile = m_UserData.GetLastHandPai(m_nDeskStation);
		}
		
		RequestOutCard(byLogicTile);
	}
	else
	{
		if (0!=byLogicTile)
		{
			RequestOutCard(byLogicTile);    
		}
	}
}
/*----------------------------------------------------------------------------------------------*/
//	点击手牌请求杠牌
void	CNewGameClient::OnUIRequestGangPai(const int iControlID)
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bGang)
	{
		return ;   
	}

	//这里要判断是否有其他玩家可以胡 如果可以胡 那么你就不能碰 必须要比碰的优先级大	
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		//给出提示DWJTEST
		return;
	}

	//获取点击的牌值
	int iUpPai = 255;
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iControlID));
	if (NULL!=pMjHand)
	{
		iUpPai = pMjHand->GetUpTile();
	}  
	if(255 == iUpPai)
	{
		return;
	}

	tagGangPaiEx gang;
	gang.Clear();
	gang.byPs		= iUpPai;
	gang.byUser		= m_nDeskStation;
	gang.byBeGang	= m_nDeskStation;
	for(int i=0; i<4; i++)
	{
		gang.byGangPai[i] = gang.byPs;
	}
	SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
}
/*----------------------------------------------------------------------------------------------*/
// 请求出牌	DWJ
void	CNewGameClient::RequestOutCard(BYTE byData)
{
	if (m_UserData.IsOutPaiPeople(m_nDeskStation))
	{
		tagOutPaiEx TOutPai;
		TOutPai.Clear();
		TOutPai.byPs = byData;
		TOutPai.byUser = m_nDeskStation;
		TOutPai.bTing = false;//听牌
		SendGameDataEx(THING_OUT_PAI, &TOutPai, sizeof(TOutPai));
	}
}
/*----------------------------------------------------------------------------------------------*/
// 放弃拦牌
void	CNewGameClient::RequestCancelBlock()
{

	SendGameDataEx(THING_GUO_PAI,NULL, 0);
	//隐藏拦牌容器
	UIShowBlockContain(false);
	//隐藏计时器(如果是拦别人的牌，放弃拦牌后就要隐藏倒计时)
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		ShowCountDownTimer(m_nDeskStation,0,false);
	}
	
	//设置手牌选择杠牌状态
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
	//手牌状态
	m_byHandState = 0xFF;
}
/*----------------------------------------------------------------------------------------------*/
//	鼠标点击吃牌按钮
void	CNewGameClient::OnUiClickChiPai()
{
	//鼠标状态
	m_byHandState = 1;
	//显示吃牌容器
	UIShowChiContain(true);
}
/*----------------------------------------------------------------------------------------------*/
//	请求吃牌	
void	CNewGameClient::RequestBlockChi(int iIndex)
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_OutPai.byPs == 255)
	{
		return ;   
	}
	//这里要判断是否有其他玩家可以碰 如果可以碰 那么你就不能吃 必须要比碰的优先级大	
	bool bCanChi = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu || m_GameData.T_CPGNotify[i].bGang || m_GameData.T_CPGNotify[i].bPeng)
		{
			bCanChi = false;
			break;
		}
	}
	if(bCanChi)
	{
		tagChiPaiEx chi;
		chi.Clear();
		chi.byBeChi = m_GameData.T_OutPai.byUser;
		chi.byPs	= m_GameData.T_OutPai.byPs;
		chi.byUser	= m_nDeskStation;
		for(int i=0; i<3; i++)
		{
			chi.byChiPs[i] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[iIndex][i];
		}
		SendGameDataEx(THING_CHI_PAI, &chi, sizeof(chi));
	}
	else
	{
		//dwjtest 给出不能吃的提示
	}
}
/*----------------------------------------------------------------------------------------------*/
//	请求碰牌
void	CNewGameClient::RequestBlockPeng()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_OutPai.byPs == 255)
	{
		return ;   
	}

	//这里要判断是否有其他玩家可以胡 如果可以胡 那么你就不能碰 必须要比碰的优先级大	
	bool bCanPeng = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanPeng = false;
			break;
		}
	}

	if(bCanPeng)
	{
		tagPengPaiEx peng;
		peng.Clear();
		peng.byPs	  = m_GameData.T_OutPai.byPs;
		peng.byUser	  = m_nDeskStation;
		peng.byBePeng = m_GameData.T_OutPai.byUser;

		SendGameDataEx(THING_PENG_PAI, &peng, sizeof(peng));
		//隐藏吃牌容器
		UIShowChiContain(false);
	}
	else
	{
		//不能碰牌 给出提示 dwjtest

	}
}
/*----------------------------------------------------------------------------------------------*/
//	请求杠牌
void	CNewGameClient::RequestBlockGang()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bGang)
	{
		return ;   
	}
	//这里要判断是否有其他玩家可以胡 如果可以胡 那么你就不能碰 必须要比碰的优先级大	
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		//给出提示DWJTEST
		return;
	}
	//如果是出牌的话 那么杠牌就是只杠玩家出的牌
	if(m_GameData.m_byThingDoing == THING_OUT_PAI)	//明杠
	{
		if(m_GameData.T_OutPai.byPs == 255)
		{
			return ;   
		}

		tagGangPaiEx gang;
		gang.Clear();
		gang.byPs		= m_GameData.T_OutPai.byPs;
		gang.byUser		= m_nDeskStation;
		gang.byBeGang	= m_GameData.T_OutPai.byUser;
		//gang.byType     = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][0];
		for(int i=0; i<4; i++)
		{
			gang.byGangPai[i] = m_GameData.T_OutPai.byPs;
		}
		SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
		
	}
	else if(m_GameData.m_byThingDoing == THING_ZHUA_PAI || m_GameData.m_byThingDoing == THING_ONE_BUHUA || m_GameData.m_byThingDoing == THING_BEGIN_OUT)	//当前是抓牌 ||补花||开始出牌
	{
		//手上是否有多个杠
		int iGangCount = 0;
		for(int i=0; i<4; i++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0] != 255 && m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1] != 255)
			{
				iGangCount++;
			}
		}
		
		if(iGangCount > 1)
		{
			//设置手牌选择杠牌状态
			SetChoiceGangPai(iGangCount,true);
		}
		else	//只有一个暗杠
		{
			tagGangPaiEx gang;
			gang.Clear();
			gang.byPs		= m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][1];
			gang.byUser		= m_nDeskStation;
			gang.byBeGang	= m_nDeskStation;
			//gang.byType     = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][0];
			for(int i=0; i<4; i++)
			{
				gang.byGangPai[i] = m_GameData.T_OutPai.byPs;
			}
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
		}
	}
	//隐藏吃牌容器
	UIShowChiContain(false);
}
/*----------------------------------------------------------------------------------------------*/
//	请求听牌
void	CNewGameClient::RequestBlockTing()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bTing)
	{
		return ;   
	}
	
	tagTingPaiEx ting;
	ting.Clear();
	ting.byUser	= m_nDeskStation;
	ting.bTing	= true;
	SendGameDataEx(THING_TING_PAI, &ting, sizeof(ting));

	//设置选择听牌状态只能选择出听的牌
	SetChoiceTingPai();

}
/*----------------------------------------------------------------------------------------------*/
// 请求胡牌
void CNewGameClient::RequestBlockHu()
{

	if(!m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		return ;   
	}

	//请求胡牌
	tagHuPaiEx hu;
	hu.Clear();
	hu.byUser	= m_nDeskStation;
	//是否别人放炮 || 抢杠胡
	if(m_GameData.m_byThingDoing == THING_OUT_PAI && m_GameData.T_OutPai.byUser != 255)
	{
		hu.byDianPao = m_GameData.T_OutPai.byUser;
		hu.byPs		 = m_GameData.T_OutPai.byPs;
	}
	else if(m_GameData.m_byThingDoing == THING_GANG_PAI && m_GameData.T_GangPai.byUser != 255)
	{
		hu.byDianPao = m_GameData.T_GangPai.byUser;
		hu.byPs		 = m_GameData.T_GangPai.byPs;
	}
	else if((m_GameData.m_byThingDoing == THING_ZHUA_PAI) || (m_GameData.m_byThingDoing == THING_ONE_BUHUA) || (m_GameData.m_byThingDoing == THING_BEGIN_OUT))	//自摸
	{
		hu.byPs = 255;
		hu.bZimo = true;
	}
	SendGameDataEx(THING_HU_PAI, &hu, sizeof(hu));

}
/*----------------------------------------------------------------------------------------------*/
// 请求托管
void CNewGameClient::RequestTuoGuan(bool bTuoguan )
{
	if(m_bGameStation != GS_PLAY_GAME)
	{
		return;
	}
	
	UserTouGuan TUserTouGuan;
	TUserTouGuan.byStation	= m_nDeskStation;
	TUserTouGuan.bIsTuoGuan = bTuoguan;
	SendGameDataEx(ASS_AUTOCARD, &TUserTouGuan, sizeof(TUserTouGuan));
}
/*----------------------------------------------------------------------------------------------*/















/*-----------------------------------------------------------------------------*/
int CNewGameClient::ResetGameFrame(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//玩家进入
int CNewGameClient::GameUserCome(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//玩家离开
int		CNewGameClient::GameUserLeft(int nIndex)
{
	UIShowUserInfo(nIndex,false);
    return 0;
}
/*-----------------------------------------------------------------------------*/
//玩家断线
int CNewGameClient::GameUserOffLine(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//是否正在游戏，由游戏实现
//旁观玩家返回false，游戏玩家正在游戏中返回true，其它状态返回false
bool	CNewGameClient::IsPlayingGame()
{
 	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT)
	{
 		return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------*/
//定时器消息
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case TIME_ONE_SECOND:
		{
			m_GameData.m_iRemaindTime--;
			if(m_GameData.m_iRemaindTime <= 5 && m_GameData.m_iRemaindTime > 0)
			{
				PlayGameSound(m_nDeskStation,SOUND_COUNT_DOWN);
			}

			if(m_GameData.m_iRemaindTime <= 0)
			{
				m_pGameFrame->KillTimer(TIME_ONE_SECOND);
			}
			break;
		}

	case ID_BEGIN_MOPAI:
		{
			int iHandCount(0);
			if(m_byMoPaiNum < 12)
			{
				if(m_byGetPai < 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 4);
					m_byGetPai += 4;
				}
				else if(m_byGetPai == 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 4);
					m_byDirect = (m_byDirect+1)%PLAY_COUNT;
					m_byGetPai = 0;
				}
				else
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 2);
					m_byDirect = (m_byDirect+1)%PLAY_COUNT;
					m_byGetPai = 0;
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 2);
					m_byGetPai += 2;
				}

				iHandCount = 4 * (m_byMoPaiNum/4+1);

			}
			else if(m_byMoPaiNum == 12)
			{
				if(m_byGetPai <= 22)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+5,255,sizeof(BYTE));
				}
				else if(m_byGetPai <= 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect+1]+1,255,sizeof(BYTE));
				}
				else if(m_byGetPai <= 26)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect+1]+3,255,sizeof(BYTE));
				}
				iHandCount = 14;
			}
			else if(m_byMoPaiNum == 13)
			{
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
				m_byGetPai+=3;
			}
			else if(m_byMoPaiNum == 14)
			{
				if(m_byGetPai > 28)
				{
					m_byGetPai = m_byGetPai - 28;
				}
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
				m_byGetPai--;
			}
			else if(m_byMoPaiNum == 15)
			{
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
			}
			
			int iCards[MAX_HAND_PAI] = {255};
			SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+m_byMoPaiStation,m_iDeskHandPai[m_pGameFrame->ViewStation2DeskStation(m_byMoPaiStation)], iHandCount,true);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				UIShowWall(i,m_byDeskMenPai[i]);
			}
			m_byMoPaiStation = (m_byMoPaiStation + 3)%PLAY_COUNT;
			m_byMoPaiNum++;
			if(m_byMoPaiNum >= 16)
			{
				//UIShowAllHandPai(true);
				m_pGameFrame->KillTimer(ID_BEGIN_MOPAI);
			}
			break;
		}
    case TIME_ZHUAPAI://抓牌
        {
            m_pGameFrame->KillTimer(TIME_ZHUAPAI);
            // 设置手牌显示    
            for (int i=0; i<PLAY_COUNT; ++i)
            {
				if(m_UserData.m_bIsHu[i])//胡牌玩家不处理
					continue;
                int desk = m_pGameFrame->DeskStation2View(i);
				m_UserDataCard[desk].GetFromByte( m_UserData.m_byArHandPai[i] );   
				if(i != m_nDeskStation)
				{
					m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
				}
                // 这里加上抓牌动画，所以先设为少一张
				// UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_ucCurrentPlayer);
            }
			//抓牌动画
			POINT pt,ptDst;   
            UIShowZhuaMoveAction(pt, ptDst, 0xFF, false);
        }
        break;
	case TIME_OUT_CARD: // 出牌
		{
			OnKillAllTimer();
			// 隐藏拦牌框
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			if(m_nDeskStation != m_UserData.m_byNowOutStation)
				break;
            OnUIRequestOutCard(CTN_1_MjHandCtrl_600010,true);//出牌时间到了
			//if (!m_bIsTuoGuan[m_nDeskStation])
            {
                // 记录超时次数
                m_nTimeOutCnt ++;
                if (m_nTimeOutCnt>=3)
                {
                    RequestTuoGuan(true);
                }
                else
                {
                    //std::wostringstream oss;
                    //oss<<L"注意，您已经超时"<<m_nTimeOutCnt<<L"次，超时3次将自动托管"<<std::flush;
                    //m_pGameFrame->InsertSystemMessage((wchar_t *)oss.str().c_str());
                }
            }
			break;
		}
    case TIME_BLOCK_CARD: // 拦牌
        {
			OnKillAllTimer();
			//恢复所有的牌颜色
			if(m_UserData.m_byNowOutStation == m_nDeskStation)//自己出牌
			{			
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//处于托管状态，b并且能胡就胡牌
				{
					RequestBlockHu();//发送胡牌请求
				}
				else if(m_UserData.m_bTing[m_nDeskStation] )//听牌且能杠
				{
					
				}
			
			}
			else//拦别人的牌
			{           
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//处于托管状态，b并且能胡就胡牌
				{
					RequestBlockHu();//发送胡牌请求
				}
				else if(m_UserData.m_bTing[m_nDeskStation] )//听牌且能杠
				{
					RequestBlockGang();//发送杠请求
				}
				else
				{
					RequestCancelBlock();
				}
			}
            break;
        }
	case TIME_SHOW_HU:// 显示糊牌对话框定时器
		{
            m_pGameFrame->KillTimer(TIME_SHOW_HU);
			SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));//隐藏糊牌对话框
		}
		break;
	case TIME_SHOW_NIAO:
		{
			static bool bShow = true;
			for(int i = 0; i < 4; i++)
			{
				BYTE card = m_GameData.m_ZhongNiao.byCard[i];
				if(((card % 10)== 2 || (card % 10)== 3 || (card )== 35))
				{
					IImage* pImage = NULL;
					pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(2008106+i));
					pImage->SetControlVisible(bShow);
				}
			}
			bShow = !bShow;
		}
		break;
	}
}
/*----------------------------------------------------------------------------------------------*/
//置状态
void	CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
}
/*----------------------------------------------------------------------------------------------*/
//设置游戏状态，主要是断线重连来后处理的消息
void CNewGameClient::SetGameStation(void* pBuffer,int nLen)
{
	GameStation_Base *pGameStation = (GameStation_Base *) pBuffer;
	//设置游戏状态
	SetStationParameter(pGameStation->byGameStation);
	//保存基础数据
	m_GameData.m_mjRule.byBeginTime		= pGameStation->byBeginTime;	//开始等待时间
	m_GameData.m_mjRule.byOutTime		= pGameStation->byOutTime;		//出牌时间
	m_GameData.m_mjRule.byBlockTime		= pGameStation->byBlockTime;	//拦牌思考时间
	m_GameData.m_mjRule.bForceTuoGuan	= pGameStation->bForceTuoGuan;	//强退是否托管
	m_bIsSuperUser = pGameStation->bIsSuperUser;


	m_nDeskStation = m_pGameFrame->GetMyDeskStation();

	CString cs;
	cs.Format("llj::pGameStation->byGameStation = %d",pGameStation->byGameStation);
	OutputDebugString(cs);

	switch(pGameStation->byGameStation)
    {
	case GS_WAIT_SETGAME:	//游戏没有开始状态
	case GS_WAIT_ARGEE:		//等待玩家开始状态
	case GS_WAIT_NEXT:		//等待下一盘游戏开始
		{
			if (nLen != sizeof(GameStation_Wait_Agree))
			{
				return;
			}

			GameStation_Wait_Agree *pGameStation = (GameStation_Wait_Agree *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//显示各玩家的准备标识
			int iView = 0;
			for(int i=0; i<PLAY_COUNT; i++)
			{
				iView = m_pGameFrame->DeskStation2View(i);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,pGameStation->bUserAgree[i]);
			}
			//启动准备倒计时	
			SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,true,m_GameData.m_mjRule.byBeginTime);
			//显示准备按钮
			SetBtVisible(CTN_1000000_BTN_10001,true);

			//启动一秒倒计时 
			SetOneSecondCountDown(m_GameData.m_mjRule.byBeginTime);
			break;
		}
        case GS_SEND_CARD:		//发牌状态(还未发牌,或者已经发牌，但是没有出牌)
            {
				GameStationData* pGameStation = (GameStationData*)pBuffer;
				m_GameData.m_byNtStation = pGameStation->byNtStation;
				int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
				//显示庄家方位
				UIShowNtInfo(true);
				for(int i=0;i<PLAY_COUNT;++i)
				{
					//保存玩家的手牌数据
					m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i],MAX_HAND_PAI);
					//手牌张数
					//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
					//保存玩家的门牌数据
					SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
					m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
					// 牌墙
					UIShowWall(i, m_UserData.m_iArMenPai[i]);		//显示牌墙（门牌）//先不显示最后牌墙

					m_UserData.m_bTuoGuan[i] = pGameStation->m_bistuoguan[i];

					m_UserDataCard[i].GetFromByte( pGameStation->m_byArHandPai[i] );	//dwjdelete

					if(i != m_nDeskStation)//dwjdelete
					{
						m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
					}
				}

				for(int i = 0; i < PLAY_COUNT; i++)
				{
					for(int j = 0; j < MAX_HAND_PAI; j++)
					{
						m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
					}
				}
				UIShowAllHandPai(true);
				//东南西北显示方位
				UIShowDNXB(true);
				//保存玩家昵称信息
				GetUserName();
				//显示托管消息
				UIShowTuoGuan();
            }
            break;        
       case GS_PLAY_GAME:	//游戏中状态
            {
				GameStationDataEx* pGameStation = (GameStationDataEx*)pBuffer;
				m_GameData.m_byNtStation = pGameStation->byNtStation;
				int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
				//显示庄家方位
				UIShowNtInfo(true);
				for(int i=0;i<PLAY_COUNT;++i)
				{
					//保存玩家的手牌数据
					m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i],MAX_HAND_PAI);
					m_UserData.SetGCPData(i,pGameStation->m_UserGCPData[i]);
					//手牌张数
					//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
					//保存玩家的门牌数据
					m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
					SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
					// 牌墙
					UIShowWall(i, m_UserData.m_iArMenPai[i]);		//显示牌墙（门牌）//先不显示最后牌墙
					m_UserData.SetOutPaiData(i,pGameStation->m_byArOutPai[i],OUT_CARD_NUM);
					UIShowOutCard(i,true);
					m_UserData.m_bTuoGuan[i] = pGameStation->m_bistuoguan[i];
					m_UserDataCard[i].GetFromByte( pGameStation->m_byArHandPai[i] );	//dwjdelete
					if(i != m_nDeskStation)//dwjdelete
					{
						m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
					}
				}

				for(int i = 0; i < PLAY_COUNT; i++)
				{
					for(int j = 0; j < MAX_HAND_PAI; j++)
					{
						m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
					}
				}
				if(pGameStation->m_byNowOutStation == m_nDeskStation)
				{
					int iView = m_pGameFrame->DeskStation2View(pGameStation->m_byNowOutStation);
					SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);
				}
				
				UIShowAllHandPai(true);
				//东南西北显示方位
				UIShowDNXB(true);
				//保存玩家昵称信息
				GetUserName();
				//显示托管消息
				UIShowTuoGuan();
           }
           break;
        default:
            break;
    }

    //开始播放背景音效
	PlayBgSound(true);
}
/*----------------------------------------------------------------------------------------------*/
///玩家推出
bool	CNewGameClient::OnGameQuiting()
{
	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation <= GS_PLAY_GAME)
	{
		if (!m_pGameFrame->GetIsWatching())
		{
			//强退托管
			if(m_GameData.m_mjRule.bForceTuoGuan)
			{
				if(IDOK != AFCMessageBox("您正在游戏中, 现在离开游戏将会托管处理 ，是否要离开？", "提示",AFC_YESNO)) 
				{
					return false ; 
				}
				
			}
			else	//强退结束游戏
			{
				if(IDOK != AFCMessageBox("您正在游戏中, 现在离开将会被扣罚，是否真的要离开？", "提示",AFC_YESNO)) 
				{
					return false ; 
				} 
				//m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
			}
		}
	}
	m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------*/
// 用户托管
void	CNewGameClient::OnUserTuoguan(void *buffer, int nLen)
{
	if(nLen != sizeof(UserTouGuan))
	{
		return;
	}
    UserTouGuan *pUserTouGuan = (UserTouGuan *)buffer;
	if(NULL == pUserTouGuan)
	{
		return;
	}
 
	//记录托管状态
	for(int i=0;i<PLAY_COUNT;++i)
	{
		m_UserData.m_bTuoGuan[i] = pUserTouGuan->bTuoGuan[i];
	}
	//托管显示
	UIShowTuoGuan();
}
/*-------------------------------------------------------------------------------------------------*/
// 用户同意	DWJ
void	CNewGameClient::OnUserAgree(void *buffer, int len)
{
	///游戏中不处理同意消息
	if(m_bGameStation == GS_PLAY_GAME || m_bGameStation == GS_SEND_CARD)
	{
		return; 
	}
	if(len!=sizeof(MSG_GR_R_UserAgree))
	{
		return ;
	}
	MSG_GR_R_UserAgree * pUserAgree =(MSG_GR_R_UserAgree *)buffer;
	if(pUserAgree == NULL)
	{
		return;
	}

	if(pUserAgree->bDeskStation	== m_nDeskStation)         // 自已
	{
		ReStartGameInitUI();
		m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	}
	//显示准备标识
	int iView = m_pGameFrame->DeskStation2View(pUserAgree->bDeskStation);
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,true);
	PlayGameSound(m_nDeskStation,SOUND_READY);
}
/*-------------------------------------------------------------------------------------------------*/
// 游戏开始	DWJ
void	CNewGameClient::OnGameStart(void *buffer, int len)
{
	if(len != sizeof(tagBeginEx))
	{
		return;
	}
	tagBeginEx *pBegin = (tagBeginEx *)buffer;
	if(NULL == pBegin)
	{
		return;
	}
	SetStationParameter(GS_SEND_CARD);	
	//重置游戏界面
	ReStartGameInitUI();

	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		//隐藏所有准备标识
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//显示牌城墙
		SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
	}
	InitGameDate();
	m_GameData.m_byThingDoing = THING_GAME_BEGIN;//正在发生的事件

	//庄家位置
	m_GameData.m_byNtStation		= pBegin->byNt;
	m_GameData.m_iHuangZhuangCount	= pBegin->iHuangZhuangCount;

	if(m_GameData.m_byNtStation != 255)
	{
		//显示庄家
		int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
		//显示庄家方位
		UIShowNtInfo(true);
	}
	//显示花数量
	//UIShowHuaCount(true);
	//东南西北显示方位
	UIShowDNXB(true);
	//保存玩家昵称信息
	GetUserName();
	//开始声音
    PlayGameSound(m_nDeskStation,SOUND_BEGIN);
}
/*-------------------------------------------------------------------------------------------------*/
// 确定庄家消息	DWJ
void	CNewGameClient::OnDingNt(void *buffer, int nLen)
{	
	if(nLen != sizeof(tagDongNtEx))
	{
		return;
	}
	tagDongNtEx *pDingNt = (tagDongNtEx*)buffer;
	if(NULL == pDingNt)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_DING_NT;	//正在执行的事件
	//庄家位置
	m_GameData.m_byNtStation		= pDingNt->byNt;
	m_GameData.m_iHuangZhuangCount	= pDingNt->iHuangZhuangCount;
	
	//显示庄家
	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
	//显示庄家方位
	UIShowNtInfo(true);

}
/*-------------------------------------------------------------------------------------------------*/

void CNewGameClient::On2SeZiNT(void * buffer, int nLen)
{
	if(nLen != sizeof(m_GameData.T_TwoSeziNt))
		return;

	tagTwoSeziNtEx* pTwoSeziNt = (tagTwoSeziNtEx*)buffer;

	if(!pTwoSeziNt)
		return;

	m_GameData.m_byThingDoing = THING_2SEZI_NT;
	m_GameData.T_TwoSeziNt.Clear();
	memcpy(&m_GameData.T_TwoSeziNt,pTwoSeziNt,sizeof(m_GameData.T_TwoSeziNt));
	m_GameData.m_byNtStation = pTwoSeziNt->byNt;

	int iView = m_pGameFrame->DeskStation2View(0);

	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);

	//播放色子音效
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
// 第一次掷筛子确定起牌方向	DWJ
void	CNewGameClient::On2SeZiDir(void *buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziDirEx))
	{
		return;
	}
	tagTwoSeziDirEx *pTwoSeziDir = (tagTwoSeziDirEx*)buffer;
	if(NULL == pTwoSeziDir)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_2SEZI_DIR;	//正在执行的事件

	int iView = m_pGameFrame->DeskStation2View(0);
	//隐藏之前的两个色子
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);
	//保存第一次筛子的数据
	m_GameData.T_TwoSeziDir.Clear();
	memcpy(&m_GameData.T_TwoSeziDir, pTwoSeziDir, sizeof(m_GameData.T_TwoSeziDir));
	iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	//开始播放掷筛子动画(更具庄家位置去播放)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);

	//播放色子音效
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
	
}
void CNewGameClient::On2SeZiGP(void* buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziGetPaiEx))
	{
		return;
	}

	tagTwoSeziGetPaiEx* pTwoSeziGP = (tagTwoSeziGetPaiEx*)buffer;

	if(NULL == pTwoSeziGP)
	{
		return;
	}

	m_GameData.m_byThingDoing = THING_2SEZI_GP;

	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);

	//隐藏之前的两个色子
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);

	m_GameData.T_TwoSeziGetPai.Clear();
	memcpy(&m_GameData.T_TwoSeziGetPai,pTwoSeziGP,sizeof(m_GameData.T_TwoSeziGetPai));

	//开始播放掷筛子动画(更具庄家位置去播放)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);
	//播放色子音效
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
/*-------------------------------------------------------------------------------------------------*/
//第二次筛子确认起牌墩数消息	DWJ
void	CNewGameClient::On2SeZiDirGP(void *buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziDirAndGetPaiEx))
	{
		return;
	}
	tagTwoSeziDirAndGetPaiEx *pTwoSeziDir = (tagTwoSeziDirAndGetPaiEx*)buffer;
	if(NULL == pTwoSeziDir)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_2SEZI_DIR_GP;	//正在执行的事件

	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	//隐藏之前的两个色子
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);
	//保存第2次筛子的数据
	m_GameData.T_TwoSeziDirAndGetPai.Clear();
	memcpy(&m_GameData.T_TwoSeziDirAndGetPai, pTwoSeziDir, sizeof(m_GameData.T_TwoSeziDirAndGetPai));

	//开始播放掷筛子动画(更具庄家位置去播放)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);
	//播放色子音效
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
/*-------------------------------------------------------------------------------------------------*/
// 发送手牌数据	DWJ
// 注意此时没有牌数据，跳牌后才有
void	CNewGameClient::OnSendAllCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagSendPaiEx))
	{
		return;
	}
	tagSendPaiEx *pSendPai = (tagSendPaiEx*)buffer;
	if(NULL == pSendPai)
	{
		return;
	}
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_SEND_PAI;
	SetStationParameter(GS_SEND_CARD);
	//隐藏之前的两个色子
	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);


	//拷贝发牌数据
	memcpy(&m_GameData.T_SendPai, pSendPai, sizeof(m_GameData.T_SendPai));
	for(int i=0;i<PLAY_COUNT;++i)
	{
		//保存玩家的手牌数据
		m_UserData.SetHandPaiData(i,pSendPai->m_byArHandPai[i],pSendPai->m_byArHandPaiCount[i]);
		//手牌张数
		//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
		//保存玩家的门牌数据
		m_UserData.SetMenPaiData(i,pSendPai->byMenPai[i]);

		// 牌墙
		//UIShowWall(i, m_UserData.m_iArMenPai[i]);		//显示牌墙（门牌）//先不显示最后牌墙



		m_UserDataCard[i].GetFromByte( pSendPai->m_byArHandPai[i] );	//dwjdelete
		
		if(i != m_nDeskStation)//dwjdelete
		{
			m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
		}
	}

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		for(int j = 0; j < MAX_HAND_PAI; j++)
		{
			m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
		}
	}
	memset(m_byDeskMenPai,0,sizeof(m_byDeskMenPai));
	m_byDirect = m_GameData.T_TwoSeziDir.byDirection;
	m_byGetPai = m_GameData.T_TwoSeziGetPai.byGetPai*2;
	m_byMoPaiNum = 0;
	m_byMoPaiStation = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	m_pGameFrame->SetTimer(ID_BEGIN_MOPAI,300);
	//显示所有玩家的手牌
	//UIShowAllHandPai(true);//先不显示全部手牌
}
/*-------------------------------------------------------------------------------------------------*/
//	所有玩家补花消息	DWJ
void	CNewGameClient::OnAllBuHua( void *buffer, int nLen)
{
	if(nLen != sizeof(tagAllBuHuaEx))
	{
		return;
	}
	tagAllBuHuaEx *pAllBuHua = (tagAllBuHuaEx*)buffer;
	if(NULL == pAllBuHua)
	{
		return;
	}
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_ALL_BUHUA;
	SetStationParameter(GS_SEND_CARD);

	//拷贝补花数据
	memcpy(&m_GameData.T_AllBuHua, pAllBuHua, sizeof(m_GameData.T_AllBuHua));

	//拷贝花牌数据
	for(int i=0; i<PLAY_COUNT;i++)
	{
		m_UserData.SetHuaPaiData(i,pAllBuHua->m_byArHuaPai[i]);
	}
	
	//玩家牌盖着并且打倒
	UIShowAllHandPaiBack(true,true);
	//播放音效
	PlayGameSound(m_nDeskStation,SOUND_BLOCK);	
}
/*-------------------------------------------------------------------------------------------------*/
// 游戏开始打牌了	DWJ
void	CNewGameClient::OnBeginOutCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagBeginOutPaiEx))
	{
		return;
	}
	tagBeginOutPaiEx *pBeginOutPai = (tagBeginOutPaiEx*)buffer;
	if(NULL == pBeginOutPai)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_BEGIN_OUT;//正在执行的事件
	SetStationParameter(GS_PLAY_GAME);

	//拷贝开始出牌数据
	m_GameData.T_BeginOutPai.Clear();
	memcpy(&m_GameData.T_BeginOutPai, pBeginOutPai, sizeof(m_GameData.T_BeginOutPai));


	for(int i=0;i<PLAY_COUNT;++i)
	{
		//保存玩家的手牌数据
		m_UserData.SetHandPaiData(i,pBeginOutPai->m_byArHandPai[i],pBeginOutPai->m_byArHandPaiCount[i]);
		//手牌张数
		m_UserData.m_byArHandPaiCount[i] = pBeginOutPai->m_byArHandPaiCount[i];
		//保存玩家的门牌数据
		m_UserData.SetMenPaiData(i,pBeginOutPai->byMenPai[i]);

		// 牌墙 //显示牌墙（门牌）
		UIShowWall(i, m_UserData.m_iArMenPai[i]);				
	}

	//当前出牌位置
	m_UserData.m_byNowOutStation = pBeginOutPai->byNt; 


	//排序自己的手牌数据
	m_UserData.SortHandPai(m_nDeskStation,false);
	//显示所有玩家的手牌
	UIShowAllHandPai(true);

	//设置出牌玩家的手牌隔开
	int iView = m_pGameFrame->DeskStation2View(m_UserData.m_byNowOutStation);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);

	//显示花数量
	//UIShowHuaCount(true);

	//显示倒计时
	ShowCountDownTimer(m_UserData.m_byNowOutStation,m_GameData.m_mjRule.byOutTime,true);

	if(pBeginOutPai->byUser == m_nDeskStation)
	{
		//启动一秒倒计时 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}
	//手牌状态
	m_byHandState = 0xFF;
}
/*-------------------------------------------------------------------------------------------------*/
// 出牌结果	DWJ
void	CNewGameClient::OnNotifyOutCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagOutPaiEx))
	{
		return;
	}

	tagOutPaiEx *pOutPaiResult = (tagOutPaiEx *)buffer;
	if(NULL == pOutPaiResult)
	{
		return;
	}
	//播放出牌声音
	PlayGameSound(m_nDeskStation,SOUND_OUTCARD);
	SetStationParameter(GS_PLAY_GAME);
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_OUT_PAI;
	//隐藏倒计时
	ShowCountDownTimer(m_nDeskStation,0,false);
	//隐藏拦牌容器
	UIShowBlockContain(false);

	//拷贝出牌数据
	m_GameData.T_OutPai.Clear();
	memcpy(&m_GameData.T_OutPai, pOutPaiResult, sizeof(m_GameData.T_OutPai));

	//保存出牌数据
	m_UserData.SetOutPaiData(pOutPaiResult->byUser,pOutPaiResult->m_byArOutPai[pOutPaiResult->byUser],pOutPaiResult->m_byArOutPaiCount[pOutPaiResult->byUser]);

	//显示出牌数据
	UIShowOutCard(pOutPaiResult->byUser, true);

	//显示出牌位置动画
	int iView = m_pGameFrame->DeskStation2View(pOutPaiResult->byUser);
	RECT rect = GetMjGiveRect(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+iView,pOutPaiResult->m_byArOutPaiCount[pOutPaiResult->byUser]-1);
	POINT point;
	point.x	= rect.left;
	point.y	= rect.top-20;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,true,point,true);

	//玩家之前没有听牌 现在听牌了
	if(!m_UserData.m_bTing[pOutPaiResult->byUser] && pOutPaiResult->bUserTing[pOutPaiResult->byUser])
	{
		//标识玩家听牌了
		m_UserData.m_bTing[pOutPaiResult->byUser] = pOutPaiResult->bUserTing[pOutPaiResult->byUser];
		//播放听牌音效
		PlayGameSound(pOutPaiResult->byUser,SOUND_TING);
	}
	//获取听牌标识
	for(int i=0;i<PLAY_COUNT;i++ )
	{
		m_UserData.m_bTing[i] = pOutPaiResult->bUserTing[i];
	}
	//显示听牌标识
	UIShowTingLog(true);
	//设置手牌选择杠牌状态
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);


	for(int i=0;i<PLAY_COUNT;i++ )
	{
		//手牌数据
		m_UserData.SetHandPaiData(i,pOutPaiResult->m_byArHandPai[i],pOutPaiResult->m_byArHandPaiCount[i]);
	}

	//显示手牌数据
	UIShowAllHandPai(true);

	//设置出牌玩家的手牌不隔开
	iView = m_pGameFrame->DeskStation2View(pOutPaiResult->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,false);


	//清空所有玩家的吃碰杠通知数据
	for(int i=0;i<PLAY_COUNT;++i)
	{
		m_GameData.T_CPGNotify[i].Clear();
	}
	//隐藏吃牌容器
	UIShowChiContain(false);
	//手牌状态
	m_byHandState = 0xFF;

	//出牌了 杀掉一秒倒计时计时器
	m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	
	//播放牌值声音
	PlayGameSound(pOutPaiResult->byUser,pOutPaiResult->byPs);
}
/*-------------------------------------------------------------------------------------------------*/
// 拦牌
void	CNewGameClient::OnNotifyBlock(void *buffer, int nLen)
{
	//因为服务端是以数组形式发过来的数据包 所以这里长度判断要*PLAY_COUNT
	if(nLen != sizeof(tagCPGNotifyEx)*PLAY_COUNT)
	{
		return;
	}

	tagCPGNotifyEx *pCPGNotify = (tagCPGNotifyEx *)buffer;
	if(NULL == pCPGNotify)
	{
		return;
	}
	//拷贝拦牌数据
	memcpy(&m_GameData.T_CPGNotify, pCPGNotify, sizeof(m_GameData.T_CPGNotify));
	
	if(		m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_CPGNotify[m_nDeskStation].bGang
		||	m_GameData.T_CPGNotify[m_nDeskStation].bTing|| m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		//显示拦牌容器
		UIShowBlockContain(true);
		FlashWindow();
	}

	//抓牌后有动作那么就不再显示倒计时了 因为前面会发送一个抓牌消息的时候已经显示了
	if(m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		OutputDebugString("dwjsdmj::啥也不用做");
	}
	else	//不是抓牌有动作(即 别人出牌 自己有拦牌动作 ) 那么就要显示倒计时
	{
		//设置倒计时
		ShowCountDownTimer(m_nDeskStation,m_GameData.m_mjRule.byBlockTime,true);
	}
}
/*-------------------------------------------------------------------------------------------------*/

void CNewGameClient::OnNotifyAction(void *buffer, int nLen)
{
	//因为服务端是以数组形式发过来的数据包 所以这里长度判断要*PLAY_COUNT
	if(nLen != sizeof(HaveAction))
	{
		return;
	}

	HaveAction *pCPGNotify = (HaveAction *)buffer;
	if(NULL == pCPGNotify)
	{
		return;
	}
	bool bIHaveAction = false;
	//拷贝拦牌数据
	memcpy(m_GameData.T_CPGNotify, pCPGNotify->T_CPGNotify, sizeof(m_GameData.T_CPGNotify));

	if(		m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_CPGNotify[m_nDeskStation].bGang
		||	m_GameData.T_CPGNotify[m_nDeskStation].bTing|| m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		//显示拦牌容器
		UIShowBlockContain(true);
		bIHaveAction = true;
		FlashWindow();
	}

	//抓牌后有动作那么就不再显示倒计时了 因为前面会发送一个抓牌消息的时候已经显示了
	if(m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		OutputDebugString("dwjsdmj::啥也不用做");
	}
	else	//不是抓牌有动作(即 别人出牌 自己有拦牌动作 ) 那么就要显示倒计时
	{
		//设置倒计时
		if(bIHaveAction)
			ShowCountDownTimer(m_nDeskStation,m_GameData.m_mjRule.byBlockTime,true);
		else
			ShowCountDownTimer(pCPGNotify->byDeskStation,m_GameData.m_mjRule.byBlockTime,true);
	}
}

// 抓牌
void	CNewGameClient::OnNotifyZhuaPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagZhuaPaiEx))
	{
		return;
	}
	tagZhuaPaiEx *pZhuaPai = (tagZhuaPaiEx *)buffer;
	if(NULL == pZhuaPai)
	{
		return;
	}
	//先清空抓牌数据
	m_GameData.T_ZhuaPai.Clear();
	//拷贝抓牌数据
	memcpy(&m_GameData.T_ZhuaPai, pZhuaPai, sizeof(m_GameData.T_ZhuaPai));

	SetStationParameter(GS_PLAY_GAME);
	m_GameData.m_byThingDoing = THING_ZHUA_PAI;//正在执行的事件

	m_UserData.m_byNowOutStation = pZhuaPai->byUser; //当前出牌位置

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//保存玩家的门牌数据
		m_UserData.SetMenPaiData(i,pZhuaPai->byMenPai[i]);
		//显示牌墙（门牌）	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);			
	}

	//手牌张数
	//m_UserData.m_byArHandPaiCount[pZhuaPai->byUser] = pZhuaPai->m_byArHandPaiCount[pZhuaPai->byUser];
	//手牌数据
	m_UserData.SetHandPaiData(pZhuaPai->byUser,pZhuaPai->m_byArHandPai[pZhuaPai->byUser],pZhuaPai->m_byArHandPaiCount[pZhuaPai->byUser]);
	//显示手牌
	UIShowAllHandPai(true);
	
	//是花牌就不显示倒计时  因为立马就要补花了
	if(!m_UserData.CheckIsHuaPai(pZhuaPai->byPs))
	{
		//显示出牌倒计时
		ShowCountDownTimer(pZhuaPai->byUser,m_GameData.m_mjRule.byOutTime,true);
		//标识是花牌  此时不能出牌
		m_bIsHua	  = false;
	}
	else
	{
		OutputDebugString("dwjsdmj::C_是花牌_等到后面补花之后再显示倒计时");
		//标识是花牌  此时能出牌
		m_bIsHua	  = true;
	}
	

	//记录抓到的牌
	m_UserData.m_byLastZhuaPai[pZhuaPai->byUser] = pZhuaPai->byPs;

	//设置出牌玩家的手牌不隔开
	int iView = m_pGameFrame->DeskStation2View(pZhuaPai->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);

	//隐藏拦牌容器
	UIShowBlockContain(false);
	//隐藏吃牌容器
	UIShowChiContain(false);
	//手牌状态
	m_byHandState = 0xFF;
	//播放抓牌音效
	PlayGameSound(m_nDeskStation,SOUND_ZHUAPAI);
	if(pZhuaPai->byUser == this->m_nDeskStation)
	{
		//闪烁窗口
		FlashWindow();
		//启动一秒倒计时 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}
}
/*-------------------------------------------------------------------------------------------------*/
//	单个玩家补花	DWJ
void	CNewGameClient::OnNotifyOneBuHua( void *buffer, int nLen)
{
	if(nLen != sizeof(tagOneBuHuaEx))
	{
		return;
	}
	tagOneBuHuaEx *pOneBuHua = (tagOneBuHuaEx *)buffer;
	if(NULL == pOneBuHua)
	{
		return;
	}
	//先清补花数据
	m_GameData.T_OneBuHua.Clear();
	//拷贝抓牌数据
	memcpy(&m_GameData.T_OneBuHua, pOneBuHua, sizeof(m_GameData.T_OneBuHua));

	SetStationParameter(GS_PLAY_GAME);
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_ONE_BUHUA;

	//当前出牌位置
	m_UserData.m_byNowOutStation = pOneBuHua->byUser; 

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//保存玩家的门牌数据
		m_UserData.SetMenPaiData(i,pOneBuHua->byMenPai[i]);
		//显示牌墙（门牌）	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);		
	}

	//拷贝花牌数据
	m_UserData.SetHuaPaiData(pOneBuHua->byUser,pOneBuHua->m_byArHuaPai[pOneBuHua->byUser]);
	//显示花牌个数
	UIShowHuaCount(true);

	//手牌张数
	//m_UserData.m_byArHandPaiCount[pOneBuHua->byUser] = pOneBuHua->m_byArHandPaiCount[pOneBuHua->byUser];
	//手牌数据
	m_UserData.SetHandPaiData(pOneBuHua->byUser,pOneBuHua->m_byArHandPai[pOneBuHua->byUser],pOneBuHua->m_byArHandPaiCount[pOneBuHua->byUser]);
	//显示手牌
	UIShowAllHandPai(true);

	//是花牌就不显示倒计时  因为立马就要补花了
	if(!m_UserData.CheckIsHuaPai(pOneBuHua->byPs))
	{
		//显示出牌倒计时
		ShowCountDownTimer(pOneBuHua->byUser,m_GameData.m_mjRule.byOutTime,true);
		//标识是花牌  此时不能出牌
		m_bIsHua	  = false;
	}
	else
	{
		//标识是花牌  此时不能出牌
		m_bIsHua	  = true;
	}


	//记录抓到的牌
	m_UserData.m_byLastZhuaPai[pOneBuHua->byUser] = pOneBuHua->byPs;

	//设置出牌玩家的手牌不隔开
	int iView = m_pGameFrame->DeskStation2View(pOneBuHua->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);


	//播放抓牌音效
	PlayGameSound(m_nDeskStation,SOUND_ZHUAPAI);
	if(pOneBuHua->byUser == this->m_nDeskStation)
	{
		//闪烁窗口
		FlashWindow();
		//启动一秒倒计时 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}

}
/*-------------------------------------------------------------------------------------------------*/
//	玩家放弃拦牌
void	CNewGameClient::OnNotifyCanCelBlock(void *buffer, int nLen)
{
	//当前不是自己出牌 就要隐藏计时器
	if(m_UserData.m_byNowOutStation != m_nDeskStation)
	{
		//隐藏拦牌容器
		UIShowBlockContain(false);
		//隐藏倒计时容器
		ShowCountDownTimer(m_nDeskStation,0,false);
	}
}
/*-------------------------------------------------------------------------------------------------*/
//	吃牌
void	CNewGameClient::OnNotifyChiPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagChiPaiEx))
	{
		return;
	}
	tagChiPaiEx *pChiPai = (tagChiPaiEx *)buffer;
	if(NULL == pChiPai)
	{
		return;
	}

	//清理吃牌数据
	m_GameData.T_ChiPai.Clear();
	//拷贝吃牌数据
	memcpy(&m_GameData.T_ChiPai, pChiPai, sizeof(m_GameData.T_ChiPai));
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_CHI_PAI;

	//当前出牌位置
	m_UserData.m_byNowOutStation = pChiPai->byUser; 

	//隐藏吃牌容器
	UIShowChiContain(false);
	//隐藏拦牌容器
	UIShowBlockContain(false);

	//拦牌数据
	m_UserData.SetGCPData(pChiPai->byUser,pChiPai->m_UserGCPData[pChiPai->byUser]);

	//手牌张数
	//m_UserData.m_byArHandPaiCount[pChiPai->byUser] = pChiPai->m_byArHandPaiCount[pChiPai->byUser];
	//手牌数据
	m_UserData.SetHandPaiData(pChiPai->byUser,pChiPai->m_byArHandPai[pChiPai->byUser],pChiPai->m_byArHandPaiCount[pChiPai->byUser]);
	//显示手牌
	UIShowAllHandPai(true);

	//出牌数据
	m_UserData.SetOutPaiData(pChiPai->byBeChi, pChiPai->m_byArOutPai[pChiPai->byBeChi],pChiPai->m_byArOutPaiCount[pChiPai->byBeChi]);
	//显示出牌数据
	UIShowOutCard(pChiPai->byBeChi, true);

	//启动出牌计时器
	ShowCountDownTimer(pChiPai->byUser,m_GameData.m_mjRule.byOutTime,true);

	//手牌状态
	m_byHandState = 0xFF;

	//播放吃牌音效
	PlayGameSound(pChiPai->byUser,SOUND_CHI);

	//出牌被吃了 就隐藏出牌动画
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
	

}
/*-------------------------------------------------------------------------------------------------*/
// 碰牌
void	CNewGameClient::OnNotifyPengPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagPengPaiEx))
	{
		return;
	}
	tagPengPaiEx *pPengPai = (tagPengPaiEx *)buffer;
	if(NULL == pPengPai)
	{
		return;
	}

	//清理碰牌数据
	m_GameData.T_PengPai.Clear();
	//拷贝碰牌数据
	memcpy(&m_GameData.T_PengPai, pPengPai, sizeof(m_GameData.T_PengPai));
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_PENG_PAI;

	//当前出牌位置
	m_UserData.m_byNowOutStation = pPengPai->byUser; 

	//隐藏吃牌容器
	UIShowChiContain(false);
	//隐藏拦牌容器
	UIShowBlockContain(false);

	//拦牌数据
	m_UserData.SetGCPData(pPengPai->byUser,pPengPai->m_UserGCPData[pPengPai->byUser]);

	//手牌数据
	m_UserData.SetHandPaiData(pPengPai->byUser,pPengPai->m_byArHandPai[pPengPai->byUser],pPengPai->m_byArHandPaiCount[pPengPai->byUser]);
	//显示手牌
	UIShowAllHandPai(true);

	//出牌数据
	m_UserData.SetOutPaiData(pPengPai->byBePeng, pPengPai->m_byArOutPai[pPengPai->byBePeng],pPengPai->m_byArOutPaiCount[pPengPai->byBePeng]);
	//显示出牌数据
	UIShowOutCard(pPengPai->byBePeng, true);

	//启动出牌计时器
	ShowCountDownTimer(pPengPai->byUser,m_GameData.m_mjRule.byOutTime,true);

	//手牌状态
	m_byHandState = 0xFF;

	//// 播放碰牌动画

	//播放碰声音
	PlayGameSound(pPengPai->byUser,SOUND_PENG);

	//出牌被拦了 就隐藏出牌动画
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
}
/*-------------------------------------------------------------------------------------------------*/
// 杠牌
void	CNewGameClient::OnNotifyGangPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagGangPaiEx))
	{
		return;
	}
	tagGangPaiEx *pGangPai = (tagGangPaiEx *)buffer;
	if(NULL == pGangPai)
	{
		return;
	}

	//清理碰牌数据
	m_GameData.T_GangPai.Clear();
	//拷贝杠牌数据
	memcpy(&m_GameData.T_GangPai, pGangPai, sizeof(m_GameData.T_GangPai));
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_GANG_PAI;

	//当前出牌位置
	m_UserData.m_byNowOutStation = pGangPai->byUser; 

	//隐藏吃牌容器
	UIShowChiContain(false);
	//隐藏拦牌容器
	UIShowBlockContain(false);

	//拦牌数据
	m_UserData.SetGCPData(pGangPai->byUser,pGangPai->m_UserGCPData[pGangPai->byUser]);

	//手牌数据
	m_UserData.SetHandPaiData(pGangPai->byUser,pGangPai->m_byArHandPai[pGangPai->byUser],pGangPai->m_byArHandPaiCount[pGangPai->byUser]);
	//显示手牌
	UIShowAllHandPai(true);

	//设置出牌玩家的手牌隔开
	int iView = m_pGameFrame->DeskStation2View(pGangPai->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,false);
	
	//出牌数据
	m_UserData.SetOutPaiData(pGangPai->byBeGang, pGangPai->m_byArOutPai[pGangPai->byBeGang],pGangPai->m_byArOutPaiCount[pGangPai->byBeGang]);
	//显示出牌数据
	UIShowOutCard(pGangPai->byBeGang, true);

	//// 播放杠牌动画
	//播放杠声音
	PlayGameSound(pGangPai->byUser,SOUND_GANG);
	PlayGameSound(m_nDeskStation,SOUND_BLOCK);
	

	//出牌被吃了 就隐藏出牌动画
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);

	//设置手牌选择杠牌状态
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
	//手牌状态
	m_byHandState = 0xFF;
}
/*-------------------------------------------------------------------------------------------------*/
// 胡牌
void CNewGameClient::OnNotifyHuPai(void *buffer, int nLen)  
{
	if(nLen != sizeof(tagHuPaiEx))
	{
		return;
	}
	tagHuPaiEx *pHuPai = (tagHuPaiEx *)buffer;
	if(NULL == pHuPai)
	{
		return;
	}

	//拷贝胡牌数据
	memcpy(&m_GameData.T_HuPai, pHuPai, sizeof(m_GameData.T_HuPai));

	//隐藏吃牌容器
	UIShowChiContain(false);
	//隐藏拦牌容器
	UIShowBlockContain(false);

	// 播放胡牌动画
	POINT point;
	point.x=point.y=0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000001,true,point,point);
	//SetAnimatePlay(2000003,false);
	//如果是自摸 播放自摸音效
	if (pHuPai->bZimo)
	{
		PlayGameSound(pHuPai->byUser,SOUND_ZIMO);//自摸声音
	}
	else 
	{		 
		PlayGameSound(pHuPai->byUser,SOUND_HU);
	}

	//记录胡牌玩家
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(pHuPai->bHaveHu[i])
		{
			m_UserData.m_bIsHu[i] = true;
		}
	}

	//胡牌玩家的手牌数据
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//m_UserData.SetHandPaiData(pHuPai->byUser,pHuPai->m_byArHandPai[pHuPai->byUser],pHuPai->m_byArHandPaiCount[pHuPai->byUser]);
		m_UserData.SetHandPaiData(i,pHuPai->m_byArHandPai[i],pHuPai->m_byArHandPaiCount[i]);
	}
	
	//显示手牌
	UIShowAllHandPai(true,true);

	//隐藏自己的倒计时
	ShowCountDownTimer(pHuPai->byUser,0,false);

}
/*-------------------------------------------------------------------------------------------------*/
// 结算
void CNewGameClient::OnNotifyFinish(void *buffer, int nLen)
{
	if(nLen != sizeof(tagCountFenEx))
	{
		return;
	}
	tagCountFenEx *pCountFen = (tagCountFenEx *)buffer;
	if(NULL == pCountFen)
	{
		return;
	}
	m_pGameFrame->KillTimer(TIME_SHOW_NIAO);
	SetStationParameter(GS_WAIT_NEXT);
	//正在执行的事件
	m_GameData.m_byThingDoing = THING_ENG_HANDLE;
	m_GameData.T_CountFen.Clear();
	memcpy(&m_GameData.T_CountFen, pCountFen, sizeof(m_GameData.T_CountFen));
	IExAnimate * pExAnimate = NULL;
	pExAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(2000003));
	if(pExAnimate)
		pExAnimate->SetControlVisible(false);
	//SetAnimatePlay(2000003,false);
	SetContainVisible(2008100,false);
	//显示结算狂
	UIShowResultDlg(true);
	//启动准备倒计时	
	//SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,false,m_GameData.m_mjRule.byBeginTime);
	//启动一秒倒计时 
	SetOneSecondCountDown(m_GameData.m_mjRule.byBeginTime);
}

//玩家换牌结果
void	CNewGameClient::OnUserChangeCard(void *buffer, int nLen)
{
	OutputDebugString("xxmj::OnUserChangeCard-0");
	if(nLen != sizeof(tagSuperChangeCard))
	{
		return;
	}
	tagSuperChangeCard *pChangeCard = (tagSuperChangeCard *)buffer;
	if(NULL == pChangeCard)
	{
		return;
	}
	OutputDebugString("xxmj::OnUserChangeCard-1");

	//换牌成功
	if (pChangeCard->bSuccese)
	{
		OutputDebugString("xxmj::OnUserChangeCard-2");
		//保存玩家的手牌数据
		m_UserData.SetHandPaiData(m_nDeskStation,pChangeCard->byArHandPai,pChangeCard->byArHandPaiCount);

		//自己的手牌麻将
		int iHandCard[MAX_HAND_PAI];
		for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
		{
			iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070031,iHandCard, m_UserData.m_byArHandPaiCount[m_nDeskStation],true);

		//被换的麻将
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,NULL,0,true);
		//换需要的麻将
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,NULL,0,true);

		//显示所有玩家的手牌
		UIShowAllHandPai(true);
		OutputDebugString("xxmj::OnUserChangeCard-3");
	}
}
































 /// 强退结束游戏
void CNewGameClient::OnForceFinish(void *buffer, int nLen)     
{
	tagCountFenEx *pCmd = (tagCountFenEx *)buffer;   
	if(pCmd->byCloseFlag == GFF_FORCE_FINISH)
	{//显示强退结束结算框
		OnSetTimer(2,TIME_BEGIN);
		// 托管状态
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));
		// 显示开始按钮
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));
		// 隐藏拦牌面板
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
		UIShowTimer(0xFF, 0, false);
		SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));///隐藏胡牌对话框
		//先隐藏
		for(int i=IMG_Liu_Ju;i<TEXT_JS_ZhuanYi_3+1;++i)
		{
			SAFE_CTRL_OPERATE(IText,  i, SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,  i, SetControlVisible(false));
		}
		//非流局结算：有玩家胡牌
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_20500, SetControlVisible(true));//显示结算框GetGameXY 
		wchar_t wszFen[500]=L"";
		for (int i=0; i<PLAY_COUNT; ++i)
		{					
			//总结算
			//昵称
			SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetControlVisible(true));
			
			//分
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->iZongFen[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetText(wszFen));
			//金币
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->i64Money[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetText(wszFen));
			// 番型
			IText *pFan = NULL;
			GETCTRL(IText, pFan, m_pUI, TEXT_JS_Hu_Type_1);
			if (pFan)
			{
				wchar_t wszFan[500]=L"强退结束游戏";
				pFan->SetText(wszFan);
				pFan->SetControlVisible(true);
			}
		}
	}
}
















/*---------------------------------------------------------------------------------------------*/
//所有玩家信息控件
void	CNewGameClient::UIShowUserInfo(BYTE byStation, bool bFlag)
{
	int iView = m_pGameFrame->DeskStation2View(byStation);
	//昵称文字
	SetTextinfo(CTN_1_TEXT_70+iView,"",bFlag);
	//昵称底图
	SetImageVisible(CTN_1_IMG_80+iView,bFlag);
	//玩家头像
	SetImageVisible(CTN_1_IMG_90+iView,bFlag);
	//金币
	SetTextinfo(CTN_1_TEXT_100+iView,"",bFlag);

	//准备标识
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,bFlag);
	//庄家标识
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,bFlag);
	//托管标识
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+iView,bFlag);
	//东南西北标识
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+iView,bFlag);
	//花数底图
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+iView,bFlag);
	//花数
	SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+iView,0,bFlag);
	//听牌标识
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,bFlag);
}
/*---------------------------------------------------------------------------------------------*/
// 数据格式：255表没有牌， 0表牌背，其它与手牌一样	DWJ
void	CNewGameClient::UIShowWall(BYTE byStation, BYTE byArrMenPai[])
{
	int iViewStation = m_pGameFrame->DeskStation2View(byStation);
	BYTE byTmpArrMenPai[MEN_CARD_NUM];
	memcpy(byTmpArrMenPai, byArrMenPai, sizeof(byTmpArrMenPai));

	for (int i=0; i<MEN_CARD_NUM; i++)
	{
		int nCurIndex = i;
		if (nCurIndex%2==1)
		{            
			if (255 == byTmpArrMenPai[nCurIndex] /*这张在下面，无牌*/ &&  255 != byTmpArrMenPai[nCurIndex-1] /*这张在上面，有牌*/)
			{
				// 按物理特性很明显，下面无牌，上面有牌，上下应该交换                
				byTmpArrMenPai[nCurIndex] = byTmpArrMenPai[nCurIndex-1];
				byTmpArrMenPai[nCurIndex-1] = 255;
			}        
		}
	}

	int count = 0;
	for (int i=0; i<MEN_CARD_NUM; ++i)       // 自右向左
	{   
		if (255 == byTmpArrMenPai[i])    // 无牌
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, -1 ,G_iWallChangePos[i],true);
		}
		else if (0 == byTmpArrMenPai[i])	//牌背
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, 0 ,G_iWallChangePos[i],true);
		}
		else if (byTmpArrMenPai[i]>0 && byTmpArrMenPai[i]<38 && byTmpArrMenPai[i]%10!=0)	//某牌的数据
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, 0 ,G_iWallChangePos[i],true);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//	显示所有手牌	DWJ
void	CNewGameClient::UIShowAllHandPai(bool bFlag,bool bFinish)
{
	int iHandCard[MAX_HAND_PAI];

	for (int i = 0; i <PLAY_COUNT; i++)
	{
		for (int m = 0; m < HAND_CARD_NUM; m ++)
		{
			iHandCard[m] = 255;
		}

		int iViewDesk = m_pGameFrame->DeskStation2View(i);
		//先清空所有的拦牌信息 再一个个添加
		ClearMjHandBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010 + iViewDesk);
		//自己的手牌
		if (i == m_nDeskStation)
		{
			for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
			{
				iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
			}
		}
		else if(m_UserData.m_bTing[i] || m_UserData.m_bIsHu[i] || bFinish)
		{
			for (int j = 0; j < m_UserData.m_byArHandPaiCount[i]; j ++)
			{
				iHandCard[j] = m_UserData.m_byArHandPai[i][j];
			}
		}
		
		SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,iHandCard, m_UserData.m_byArHandPaiCount[i],bFlag);
		//是否明牌了 明牌了 就要打倒
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,(m_UserData.m_bTing[i] || m_UserData.m_bIsHu[i] || bFinish));

		
		int iBeView = 0;
		//拦牌数据 
		for(int j=0; j<5;j++)
		{
			if(m_UserData.m_UserGCPData[i][j].byType != 255)
			{
				iBeView = m_pGameFrame->DeskStation2View(m_UserData.m_UserGCPData[i][j].iBeStation);
				AddMjHandOneBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk, (MJ_ACTION_TYPE)m_UserData.m_UserGCPData[i][j].byType, m_UserData.m_UserGCPData[i][j].byData, iBeView/*m_UserData.m_UserGCPData[i][j].iBeStation*/);
			}
		}

	}	
}
/*---------------------------------------------------------------------------------------------*/
//显示花数量	DWJ
void	CNewGameClient::UIShowHuaCount(bool bFlag)
{
	int iView = 0;
	int iHuaNum = 0;
	for(int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		//花数底图
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+iView,bFlag);

		//花数数量
		iHuaNum = 0;
		for(int j=0; j<HUA_CARD_NUM;j++)
		{
			if(255 != m_UserData.m_byArHuaPai[i][j])
			{
				iHuaNum++;
			}
		}
		SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+iView,(__int64)iHuaNum,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//东南西北显示方位
void	CNewGameClient::UIShowDNXB(bool bFlag)
{
	int iView = 0;
	for(int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		//东南西北方位图
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+iView,bFlag,i*21);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	显示庄家局数信息
void	CNewGameClient::UIShowNtInfo(bool bFlag)
{

	if(m_GameData.m_byNtStation != 255)
	{
		int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
	}
	return;
	///左上角东风X局
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020100,bFlag,113*m_GameData.m_byNtStation);
	///左上角荒庄X
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020101,bFlag);
	
	if(m_GameData.m_iHuangZhuangCount >= 5)
	{
		m_GameData.m_iHuangZhuangCount = 5;
	}
	int iNum = ::pow(2.0,m_GameData.m_iHuangZhuangCount);

	///左上角荒庄数字
	SetInumInfo(CTN_1000000_CTN_2002000_Num_20020102,(__int64)iNum,bFlag);
}
/*---------------------------------------------------------------------------------------------*/
//	显示听牌标识	DWJ
void	CNewGameClient::UIShowTingLog(bool bFlag)
{
	int iView = 0;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		if(m_UserData.m_bTing[i])
		{
			SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,bFlag);
		}
		else
		{
			SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,false);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//	设置所有的牌显示牌背	DWJ
void	CNewGameClient::UIShowAllHandPaiBack(bool bFlag, bool bLayDown)
{
	int iHandCard[MAX_HAND_PAI];

	for (int i = 0; i <PLAY_COUNT; i++)
	{
		for (int m = 0; m < MAX_HAND_PAI; m ++)
		{
			iHandCard[m] = 0;
		}

		int iViewDesk = m_pGameFrame->DeskStation2View(i);
		SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,iHandCard, m_UserData.m_byArHandPaiCount[i],bFlag);
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,bLayDown);
	}	
}
/*---------------------------------------------------------------------------------------------*/
//显示出牌倒计时	DWJ
void	CNewGameClient::ShowCountDownTimer(BYTE byDeskStation, BYTE byTime, bool bFlag)
{
	if(byDeskStation < 0 && byDeskStation >= PLAY_COUNT)
	{
		return;
	}
	for(int i=0; i<PLAY_COUNT;i++)
	{
		SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+i,false);
	}
	
	SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,bFlag,byTime);
	int iView = m_pGameFrame->DeskStation2View(byDeskStation);
	SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+iView,bFlag);

}
/*---------------------------------------------------------------------------------------------*/
// 设置出牌
void	CNewGameClient::UIShowOutCard(BYTE byStation, bool bShow)
{
	SetContainVisible(CTN_1000000_CTN_2006000,bShow);
	int iView = 0;
	int iArOutPai[OUT_CARD_NUM]={0};
	for(int i=0; i<PLAY_COUNT;  i++)
	{
		memset(iArOutPai, 0, sizeof(iArOutPai));

		for(int j = 0; j < OUT_CARD_NUM; j++)
		{
			iArOutPai[j] = m_UserData.m_byArOutPai[i][j];
		}

		iView = m_pGameFrame->DeskStation2View(i);
		SetMjGiveInfo(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+iView,8,iArOutPai,m_UserData.m_byArOutPaiCount[i],bShow);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	显示拦牌容器	DWJ
void	CNewGameClient::UIShowBlockContain(bool bFlag)
{
	//容器
	SetContainVisible(CTN_1000000_CTN_2003000,bFlag);

	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003001,bFlag);	///<过
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003010,m_GameData.T_CPGNotify[m_nDeskStation].bChi&&bFlag);	///<吃
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003011,m_GameData.T_CPGNotify[m_nDeskStation].bPeng&&bFlag);	///<碰 
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003012,m_GameData.T_CPGNotify[m_nDeskStation].bGang&&bFlag);	///<杠
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003013,m_GameData.T_CPGNotify[m_nDeskStation].bTing&&bFlag);	///<听
	if(m_GameData.T_CPGNotify[m_nDeskStation].bQiangGang)
	{
		SetBtEnable(2003014,m_GameData.T_CPGNotify[m_nDeskStation].bQiangGang && bFlag);
		return;
	}
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003014,m_GameData.T_CPGNotify[m_nDeskStation].bHu&&bFlag);	///<胡

}
/*---------------------------------------------------------------------------------------------*/
//	显示吃牌容器	DWJ
void	CNewGameClient::UIShowChiContain(bool bFlag)
{
	ShowChiContain1(false);
	ShowChiContain2(false);
	ShowChiContain3(false);

	//计算有几个拦牌的数据
	int iChiCount = 0;
	for(int i=0; i<3; i++)
	{
		if(m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][0] != 255)
		{
			iChiCount++;
		}
	}

	if(1 == iChiCount)
	{
		ShowChiContain1(bFlag);
	}
	else if(2 == iChiCount)
	{
		ShowChiContain2(bFlag);
	}
	else if(3 == iChiCount)
	{
		ShowChiContain3(bFlag);
	}

	//鼠标还没有移入
	m_bMouseEnter = false;

}
/*---------------------------------------------------------------------------------------------*/
//	显示吃牌容器1	DWJ
void	CNewGameClient::ShowChiContain1(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2009000,bFlag);
	int iHandCard[3] = {0};

	for(int i=0; i<2; i++)
	{
		//先清空2个手牌数据
		SetMjHandInfo(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,iHandCard,0,false);
		//不使用鼠标穿透
		//SetMjHandUseMouse(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,false);
		//隐藏2个底图
		SetImageVisible(CTN_1000000_CTN_2009000_IMG_20090021,false);
	}
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,iHandCard,3,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	显示吃牌容器1	DWJ
void	CNewGameClient::ShowChiContain2(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2008000,bFlag);
	int iHandCard[3] = {0};

	for(int i=0; i<2; i++)
	{
		//先清空2个手牌数据
		SetMjHandInfo(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,iHandCard,0,false);
		//不使用鼠标穿透
		//SetMjHandUseMouse(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,false);
		//隐藏2个底图
		SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,false);
	}
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,iHandCard,3,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	显示吃牌容器1	DWJ
void	CNewGameClient::ShowChiContain3(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2007000,bFlag);

	int iHandCard[3] = {0};

	for(int i=0; i<3; i++)
	{
		//先清空三个手牌数据
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,iHandCard,0,false);
		//不使用鼠标穿透
		//SetMjHandUseMouse(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,false);
		//隐藏三个底图
		SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
	}
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,iHandCard,3,bFlag);
	}
	
}
/*---------------------------------------------------------------------------------------------*/
//	设置选择杠牌	DWJ
void	CNewGameClient::SetChoiceGangPai(int iGangCount,bool bFlag)
{
	int iHand[HAND_CARD_NUM];
	memset(iHand ,255, sizeof(iHand));
	if(iGangCount > 0 && bFlag)
	{
		for(int i=0; i<HAND_CARD_NUM; i++)
		{
			iHand[i] = m_UserData.m_byArHandPai[m_nDeskStation][i];
			for(int j=0; j<iGangCount; j++)
			{
				if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[j][1] == m_UserData.m_byArHandPai[m_nDeskStation][i])
				{
					iHand[i] = 255;
					break;
				}
			}
		}
		int iTmpHand[HAND_CARD_NUM];
		int iCount = 0;
		for(int i=0; i<HAND_CARD_NUM; i++)
		{
			iTmpHand[i] = 255;
			if(iHand[i] != 255)
			{
				iTmpHand[iCount] = iHand[i];
				iCount++;
			}
		}
		//开始设置手牌不能选
		SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,iTmpHand,iCount);
		m_byHandState = 2;
	}
	else
	{
		//开始设置手牌不能选
		SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
		m_byHandState = 0xFF;
	}
	
}
/*---------------------------------------------------------------------------------------------*/
//设置选择听牌状态只能选择出听的牌
void	CNewGameClient::SetChoiceTingPai()
{
	//计算听牌的张数
	int iTingCount = 0;
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		if(m_GameData.T_CPGNotify[m_nDeskStation].m_byTingCanOut[i] != 255)
		{
			iTingCount++;
		}
	}

	int iHand[HAND_CARD_NUM];
	memset(iHand ,255, sizeof(iHand));
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		iHand[i] = m_UserData.m_byArHandPai[m_nDeskStation][i];
		for(int j=0; j<iTingCount; j++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_byTingCanOut[j] == iHand[i])
			{
				iHand[i] = 255;
				break;
			}
		}
	}

	int iTmpHand[HAND_CARD_NUM];
	int iCount = 0;
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		iTmpHand[i] = 255;
		if(iHand[i] != 255)
		{
			iTmpHand[iCount] = iHand[i];
			iCount++;
		}
	}
	//开始设置手牌不能选
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,iTmpHand,iCount);
	//m_byHandState = 3;
}
/*---------------------------------------------------------------------------------------------*/
//托管显示
void	CNewGameClient::UIShowTuoGuan()
{
	//托管logo
	int iView = 0;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+iView,m_UserData.m_bTuoGuan[i]);
	}

	//托管按钮
	SetBtVisible(CTN_1000000_BTN_2010101,!m_UserData.m_bTuoGuan[m_nDeskStation]);
	//取消托管按钮
	SetBtVisible(CTN_1000000_BTN_2010102,m_UserData.m_bTuoGuan[m_nDeskStation]);
}
/*---------------------------------------------------------------------------------------------*/
// 显示结算框
void	CNewGameClient::UIShowResultDlg(bool bFlag)
{
	//先隐藏容器中所有的控件
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001103,false);	///<荒庄
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001,false);	///<结算背景1
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001002,false);	///<结算背景2
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001003,false);	///<结算背景3
	for(int i=0; i<4; i++)
	{
		///<昵称
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001011+i,"",false);
		///杠番
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001021+i,"",false);
		///胡牌
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001031+i,"",false);
		//枪番
		SetTextinfo(2001071+i,"",false);
		//输赢
		SetTextinfo(2001061+i,"",false);
		//庄家标识
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001051+i,false);
	}
	for(int i=0; i<3; i++)
	{

		//先清空所有的拦牌信息 
		ClearMjHandBlock(CTN_1000000_CTN_2001000_MjHandCtrl_2001041 + i);
		///<麻将手牌控件
		SetMjHandVisible(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+i,false);
		///<胡牌玩家
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001044+i,"",false);
		///<牌型
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001047+i,"",false);
	}


	//显示容器
	SetContainVisible(CTN_1000000_CTN_2001000,bFlag);
	///<退出
	SetBtVisible(CTN_1000000_CTN_2001000_BTN_2001101,bFlag);
	///<继续
	SetBtVisible(CTN_1000000_CTN_2001000_BTN_2001102,bFlag);
	

	//庄家标识
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001051+m_GameData.m_byNtStation,bFlag);

	//是否荒庄
	if(m_GameData.T_CountFen.bIsLiuJu)
	{
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001103,bFlag);
		//根据胡牌的人数 显示不同的背景
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001,bFlag);
	}
	else
	{
		//根据胡牌的人数 显示不同的背景
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001+m_GameData.T_CountFen.byHuUserNum-1,bFlag);
	}
	

	CString sTextInfo;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		//显示结算昵称
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001011+i,m_sUserNick[i],bFlag);
		///<输赢
		if(m_GameData.T_CountFen.i64Money[i] > 0)
		{
			sTextInfo.Format("+%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		else if(m_GameData.T_CountFen.i64Money[i] < 0)
		{
			sTextInfo.Format("%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		else
		{
			sTextInfo.Format("%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		//sTextInfo.Format("%d",);
		SetTextinfo(2001061+i,sTextInfo,bFlag);

		//显示杠番
		sTextInfo.Format("%d",m_GameData.T_CountFen.iGangFan[i]);
		SetTextinfo(2001021+i,sTextInfo,bFlag);
		//显示胡番
		sTextInfo.Format("%d",m_GameData.T_CountFen.iHuFan[i]);
		SetTextinfo(2001031+i,sTextInfo,bFlag);
		//显示枪番
		sTextInfo.Format("%d",m_GameData.T_CountFen.iZhongFan[i]);
		SetTextinfo(2001071+i,sTextInfo,bFlag);
	}

	//是否自摸 自摸只有一个人胡牌
	//SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001050,bFlag&&m_GameData.T_CountFen.bZiMo);

	int iShowCount = 0;
	int iHandPai[HAND_CARD_NUM]={0};
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_GameData.T_CountFen.m_bIsHu[i])
		{
			///<胡牌玩家
			//SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001044+iShowCount,m_sUserNick[i],bFlag);

			//设置结算框上边麻将手牌的信息
			for(int j=0;j<HAND_CARD_NUM;j++)
			{
				iHandPai[j] = 0;
				iHandPai[j] = m_GameData.T_CountFen.m_byArHandPai[i][j];
			}
			//手牌数据
			SetMjHandInfo(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount,iHandPai,m_GameData.T_CountFen.m_byArHandPaiCount[i],bFlag);
			//麻将分开
			SetMjHandGiveMode(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount,bFlag);
			//拦牌数据
			int iBeView = 0;
			//拦牌数据 
			for(int j=0; j<5;j++)
			{
				if(m_UserData.m_UserGCPData[i][j].byType != 255)
				{
					iBeView = m_pGameFrame->DeskStation2View(m_UserData.m_UserGCPData[i][j].iBeStation);
					AddMjHandOneBlock(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount, (MJ_ACTION_TYPE)m_UserData.m_UserGCPData[i][j].byType, m_UserData.m_UserGCPData[i][j].byData, iBeView);
				}
			}
			//胡牌类型
			sTextInfo.Format("牌型:");
			//是否是清一色
			bool bIsQingYiSe = false;
			for(int j=0;j<MAX_HUPAI_TYPE;j++)
			{
				if(m_GameData.T_CountFen.byHuType[i][j] == 255)
				{
					continue;
				}
				if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_QING_YI_SE)
				{
					bIsQingYiSe = true;
					break;
				}
			}
			if(bIsQingYiSe)
			{
				sTextInfo.Format("%s 清一色[5番]",sTextInfo);
			}
			else
			{
				sTextInfo.Format("%s 推倒胡[1番]",sTextInfo);
			}

			for(int j=0;j<MAX_HUPAI_TYPE;j++)
			{
				if(m_GameData.T_CountFen.byHuType[i][j] == 255)
				{
					continue;
				}
				if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_TIAN_HU)
				{
					sTextInfo.Format("%s 天胡[6番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_DI_HU)
				{
					sTextInfo.Format("%s 地胡[5番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_JIANG_YI_SE)
				{
					sTextInfo.Format("%s 将一色[5番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_HUN_YI_SE)
				{
					sTextInfo.Format("%s 将一色[2番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_QING_LONG)
				{
					sTextInfo.Format("%s 一条龙[2番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_GANG_KAI)
				{
					sTextInfo.Format("%s 杠上花[2番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_PENG_PENG_HU)
				{
					sTextInfo.Format("%s 碰碰胡[2番]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_AN_QI_DUI)
				{
					sTextInfo.Format("%s 七对[2番]",sTextInfo);
				}
			}

			///<牌型
			//SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001047+iShowCount,sTextInfo,bFlag);

			iShowCount++;
		}
	}
	
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(2001004));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(m_GameData.m_mjRule.byBeginTime);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
	}
	//播放音效
	if(bFlag)
	{
		if(m_GameData.T_CountFen.i64Money[m_nDeskStation] > 0)
		{
			//赢声音
			PlayGameSound(m_nDeskStation,SOUND_WIN);
		}
		else if(m_GameData.T_CountFen.i64Money[m_nDeskStation] < 0)
		{
			//输声音
			PlayGameSound(m_nDeskStation,SOUND_LOST);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//显示游戏设置
void	CNewGameClient::UIShowGameSetting(bool bFlag)
{
	SetContainVisible(CTN_100000078,bFlag);
	//背景音效
	SetRButtonSelected(CTN_100000078_RButton_100000079,m_bBgMusic,bFlag);
	//游戏音效
	SetRButtonSelected(CTN_100000078_RButton_100000081,m_bGameSound,bFlag);
}
/*---------------------------------------------------------------------------------------------*/















//显示移动动画控件
void CNewGameClient::UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow)
{
    // 移动控件
    IContain *pCtn = NULL;
    if ( GETCTRL(IContain, pCtn, m_pUI, CONTAIN_ZHUAPAI_ACT) )
    {
        pCtn->SetControlVisible(bShow);
    }
    if (!bShow)
    {
        m_pGameFrame->KillTimer(TIME_ZHUAPAI);
        return;
    }
    IMjWallCtrl *pWall = NULL;
    if ( GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL) )
    {
        if (ucViewStatin<PLAY_COUNT)
        {
            pWall->SetViewDirection(ucViewStatin);
        }
        pWall->FillWallBack(1);
        pWall->SetControlVisible(true);
    }
    IMoveAction *pMvact = NULL;
    GETCTRL(IMoveAction, pMvact, m_pUI, ZHUAPAI_ACT_MVACT);
    if (pMvact)
    {                                    
        pMvact->SetStartP(ptStart);
        pMvact->SetEndP(ptEnd);
        pMvact->SetPlayState(true);
    }
}




// 当麻将控件不可用时，鼠标移动到控件上
int CNewGameClient::OnMoveOnMjCtrl(TUIMessage* pMessage)
{
	CString cs;
	cs.Format("hzmj::pMessage->_ch=%d",pMessage->_ch);
	OutputDebugString(cs);

   if (pMessage->_uControlID == CTN_1_MjHandCtrl_600010)
    {
		if (2==m_byHandState)  // 请求杠牌状态
        {
			IMjHandCtrl *pHand = NULL;
            GETCTRL(IMjHandCtrl, pHand, m_pUI, CTN_1_MjHandCtrl_600010);
            if (NULL!=pHand)
            {
                MJ_TILE_UIINFO uiInfo[MAX_TILES_INCTRL];
                pHand->GetTilesInfo(uiInfo, MAX_TILES_INCTRL);
                // 将选中的弹起 
                for (int i=0; i<pHand->GetTileCount(); ++i)
                {   
                    if (LT_EMPTY==uiInfo[i].ucActFlag && uiInfo[i].ucTiles[0]==pMessage->_ch)
                    {
                        uiInfo[i].bSelect = true;
                    }
                    else
                    {
                        uiInfo[i].bSelect = false;
                    }        
                }
                pHand->SetTilesInfo(uiInfo, pHand->GetTileCount());
            }
        }
    }
	return 0;
}
// 当麻将控件不可用时，鼠标单击控件
int CNewGameClient::OnLButtonOnMjCtrl(TUIMessage* pMessage)
{
	if(pMessage->_uControlID == CTN_1_MjGangCtrl_600011)//杠牌控件1
	{
		
        tagGangPaiEx gang;
        gang.Clear();
		gang.bIsBigGang = m_bIsBigGang[0];
    
       
		
        gang.byUser = m_nDeskStation;
        gang.byBeGang = m_ucCurrentPlayer;
        SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
        // 隐藏拦牌面板
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
        // 不变灰
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
        m_byHandState = 0xFF;//手牌状态
		
		return 0;
	}
	
    if (pMessage->_uControlID!=CTN_1_MjHandCtrl_600010)
    {
		if (2==m_byHandState)  // 请求杠牌状态
		{
			BYTE type = 255;
			BYTE byPs = pMessage->_ch;
			for(int i=0;i<4;++i)
			{
				if(byPs == m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1])
				{
					type = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0];
				}
			}
			tagGangPaiEx gang;
			gang.Clear();
			gang.byPs = byPs;
			gang.byType = type;
			memset(gang.byGangPai,byPs,sizeof(gang.byGangPai));
			gang.byUser = m_nDeskStation;
			gang.byBeGang = m_ucCurrentPlayer;
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
			// 隐藏拦牌面板
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			// 不变灰
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
			m_byHandState = 0xFF;//手牌状态
		}
        return 0;
    }
	return 0;
}
/*--------------------------------------------------------------------------------------------*/
// 动画控件消息	DWJ
int CNewGameClient::OnCartoonFinish(TUIMessage *pMessage)
{
    
	switch(pMessage->_uControlID)
	{
	case CTN_1000000_CTN_1000100_ExAnimate_2000010:
	case CTN_1000000_CTN_1000100_ExAnimate_2000011:
	case CTN_1000000_CTN_1000100_ExAnimate_2000012:
	case CTN_1000000_CTN_1000100_ExAnimate_2000013:
		{
			int iFramNum = 0;
			int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
			if(m_GameData.m_byThingDoing == THING_2SEZI_NT)
			{
				iView = m_pGameFrame->DeskStation2View(0);
				iFramNum = 14 + m_GameData.T_TwoSeziNt.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);

				if(m_GameData.m_byNtStation != 255)
				{
					//显示庄家
					int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
					SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
					//显示庄家方位
					UIShowNtInfo(true);
				}
			}
			else if(m_GameData.m_byThingDoing == THING_2SEZI_DIR)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziDir.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);
			}
			else if (m_GameData.m_byThingDoing == THING_2SEZI_GP)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziGetPai.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);
			}
			break;
		}
	case CTN_1000000_CTN_1000100_ExAnimate_2000020:
	case CTN_1000000_CTN_1000100_ExAnimate_2000021:
	case CTN_1000000_CTN_1000100_ExAnimate_2000022:
	case CTN_1000000_CTN_1000100_ExAnimate_2000023:
		{
			int iFramNum = 0;
			int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
			if(m_GameData.m_byThingDoing == THING_2SEZI_NT)
			{
				iView = m_pGameFrame->DeskStation2View(0);
				iFramNum = 14 + m_GameData.T_TwoSeziNt.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			else if(m_GameData.m_byThingDoing == THING_2SEZI_DIR)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziDir.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			else if (m_GameData.m_byThingDoing == THING_2SEZI_GP)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziGetPai.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			break;
		}
	case CTN_1000000_CTN_2006000_Time_2006001:	//时间框时间到
	case 2001004:
		{
			if(m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_NEXT || m_bGameStation == GS_WAIT_SETGAME)
			{
				m_pGameFrame->CloseClient();
			}
			else
			{
				ShowCountDownTimer(m_nDeskStation,0,false);
			}
			break;
		}
	case CTN_1000000_CTN_2010000_Time_20100003:	//买码时间到
		{
			ShowMaiMaContain(false);
			break;
		}
	}
   
	return 0;
}
/*--------------------------------------------------------------------------------------------*/
//保存玩家的的昵称
void	CNewGameClient::GetUserName()
{
	UserInfoForGame UserInfo;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			m_sUserNick[i].Format("%s",UserInfo.szNickName);
		}
	}
}
/*--------------------------------------------------------------------------------------------*/
//启动一秒倒计时 m_iRemaindTime
void	CNewGameClient::SetOneSecondCountDown(int iTime)
{
	OutputDebugString("dwjsdmj::SetOneSecondCountDown");
	m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	m_GameData.m_iRemaindTime = iTime;
	m_pGameFrame->SetTimer(TIME_ONE_SECOND, 1000);
}
/*--------------------------------------------------------------------------------------------*/
//设置计时器
void CNewGameClient::OnSetTimer(BYTE station,int type)
{
	OnKillAllTimer();
	BYTE viewstation = m_pGameFrame->DeskStation2View(station);//玩家视图位置
	int itime = 1;
	switch(type)
	{
	case TIME_OUT_CARD://出牌定时器
		{
			//itime = m_MJSetting.ucOutcardTime;
			//if(station == m_nDeskStation && (m_UserData.m_bTuoGuan[station] || m_UserData.m_bTing[station]))//自己，并且是托管状态
			//{
			//	itime = m_MJSetting.nAutoTime;
			//}
		}
		break;
	case TIME_BLOCK_CARD://拦牌定时器
		{
// 			itime = m_MJSetting.ucBlockTime;
// 			if(station == m_nDeskStation && m_UserData.m_bTuoGuan[station] )//自己，并且是托管状态
// 			{
// 				itime = m_MJSetting.nAutoTime;
// 			}
		}
		break;
	case TIME_BEGIN:// 自动开始
		{
/*			itime = m_MJSetting.ucBeginTime;*/
		}
		break;
	
	}
	m_pGameFrame->SetTimer(type, itime*1000);
    UIShowTimer(viewstation, itime, true);//显示计时器动画
}
//删除所有时间
void CNewGameClient::OnKillAllTimer()
{
    UIShowTimer(0, 0, false);//隐藏计时器动画
	m_pGameFrame->KillTimer(TIME_OUT_CARD);
	m_pGameFrame->KillTimer(TIME_BLOCK_CARD);
	m_pGameFrame->KillTimer(TIME_BEGIN);
}
/// 消除定时器
BOOL CNewGameClient::KillGameTimer(int TimerID)
{
	m_pGameFrame->KillTimer(TimerID);
	m_pGameFrame->KillTimer(ID_WARMING_TIME);//删除警告计时器
	UIShowTimer(0,0,false);//隐藏计时器
	return FALSE;
}

// 设置定时器
void CNewGameClient::UIShowTimer(BYTE bViewStation, int nTime, bool bShow )
{
    // 方向控件ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_CTN_600031_IMG_600020  , CTN_1_CTN_600031_IMG_600020+1  , CTN_1_CTN_600031_IMG_600020+2  , CTN_1_CTN_600031_IMG_600020+3  };
    if (!bShow)
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(false) );
        }
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
        return;
    }
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetControlVisible(true));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMaxNum(nTime));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMinNum(1));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetPLay(true));
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(i==bViewStation));
    }
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
}


//发送游戏数据:发送通知消息，不包含数据
void CNewGameClient::SendGameDataEx(BYTE id)
{
	m_pGameFrame->SendGameData(id, NULL, 0);
}

//发送游戏数据
void CNewGameClient::SendGameDataEx(BYTE id,void *pData,UINT uSize)
{
	m_pGameFrame->SendGameData( id,pData, uSize);
}

//闪烁窗口
void CNewGameClient::FlashWindow()
{
	HWND hwnd = m_pUI->GetWindowsHwnd();
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 10;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}








///发牌控制
//将字符串转换成牌数据
void CNewGameClient::GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type)
{
	memset(data,255,sizeof(data));
	count=0;
	switch(type)
	{
	case 0://换全部手牌
		{
			for(int i=2;i<MAX_HAND_PAI*2 + 2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2-1] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 1://部分牌换成某些牌
		{
			for(int i=0;i<MAX_HAND_PAI*2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 2://设置下一张抓牌
		{
			data[0] = ChangeChar(pszPai[2],pszPai[3]);
			count = 1;
		}
		break;
	}
}
//将两个字符转换成牌
BYTE CNewGameClient::ChangeChar(wchar_t data1,wchar_t data2)
{
	BYTE pai = 255;
	BYTE num1 = 0,num2 = 0;

	if(data2 == 'b')//同
	{
		num2 = 20;
	}
	else if(data2 == 't')//条
	{
		num2 = 10;
	}
	if(data1 >= '1' && data1<='9' && (data2 == 'w' || data2 == 'b' || data2 == 't'))
	{
		num1 = data1 - '1' +1;
		pai = num1+num2;
	}
	else if(data1 == 'd' && data2=='f')//东风
	{
		pai = 31;
	}
	else if(data1 == 'n' && data2=='f')//南风
	{
		pai = 32;
	}
	else if(data1 == 'x' && data2=='f')//西风
	{
		pai = 33;
	}
	else if(data1 == 'b' && data2=='f')//北风
	{
		pai = 34;
	}
	else if(data1 == 'h' && data2=='z')//红中
	{
		pai = 35;
	}
	else if(data1 == 'f' && data2=='c')//发财
	{
		pai = 36;
	}
	else if(data1 == 'b' && data2=='b')//白板
	{
		pai = 37;
	}
	return pai;
}





/*----------------------------------------------------------------------------------------------*/
//加载游戏配置
void CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//背景音效
	m_bBgMusic		= ("1" == f.GetKeyVal(key,"BgMusic","1"));
	//游戏音效
	m_bGameSound	= ("1" == f.GetKeyVal(key,"GameSound","1"));
	return;
}
/*----------------------------------------------------------------------------------------------*/
//保存游戏配置
void CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;
	//背景音效
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_100000078_RButton_100000079));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//本来没有播放 那么这里就要开始播放背景音乐了
			if (!m_bBgMusic)
			{
				m_bBgMusic = true;
				PlayBgSound(true);
			}
			f.SetKeyValString(key,"BgMusic","1");
		}
		else
		{
			//如果正在播放 那么就要停止了
			if (m_bBgMusic)
			{
				m_bBgMusic = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgMusic","0");
		}
	}
	//游戏音效
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_100000078_RButton_100000081));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bGameSound = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//如果正在播放音效 此时就要停止
			if (m_bGameSound)
			{
				m_bGameSound = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
	return;
}
/*----------------------------------------------------------------------------------------------*/
//播放背景音效	DWJ
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	if(!m_bBgMusic)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"sound\\bg.ogg");
	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}
/*----------------------------------------------------------------------------------------------*/
//播放游戏音效
void	CNewGameClient::PlayGameSound(BYTE byDeskStation, int iSoundID, bool bISLoop )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_bGameSound)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)); 
	if(iSoundID >= 100)
	{
		switch(iSoundID)
		{
		case SOUND_BEGIN:	//开始
			{
				wsprintf(szFileName,"sound\\Begin.ogg");
				break;
			}
		case SOUND_READY:	//准备
			{
				wsprintf(szFileName,"sound\\Ready.ogg");
				break;
			}
		case SOUND_ZHUAPAI:	//抓牌
			{
				wsprintf(szFileName,"sound\\Zhuapai.ogg");
				break;
			}
		case SOUND_BLOCK:	//拦牌
			{
				wsprintf(szFileName,"sound\\Block.ogg");
				break;
			}
		case SOUND_OUTCARD:	//出牌
			{
				wsprintf(szFileName,"sound\\OutcardWeight.ogg");
				break;
			}
		case SOUND_WIN:		//赢了
			{
				wsprintf(szFileName,"sound\\win.ogg");
				break;
			}
		case SOUND_LOST:	//输了
			{
				wsprintf(szFileName,"sound\\lost.ogg");
				break;
			}
		case SOUND_SEZI:	//色子
			{
				wsprintf(szFileName,"sound\\Sezi.ogg");
				break;
			}
		case SOUND_MOUSE_ENTER:	//鼠标进入
			{
				wsprintf(szFileName,"sound\\MouseEnter.ogg");
				break;
			}
		case SOUND_COUNT_DOWN:
			{
				wsprintf(szFileName,"sound\\CountDown.ogg");
				break;
			}
		}
	}
	else
	{
		UserInfoForGame user;
		if (m_pGameFrame->GetUserInfo(byDeskStation,user))
		{
			if (user.bBoy)
			{
				//万
				if(iSoundID >=1 && iSoundID <=9)
				{
					wsprintf(szFileName,"sound\\putong\\man\\%dwan.wav",iSoundID);
				}
				else if(iSoundID >=11 && iSoundID <=19)	//条
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\man\\%dtiao.wav",iSoundID);
				}
				else if(iSoundID >=21 && iSoundID <=29)	//筒
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\man\\%dtong.wav",iSoundID);
				}
				else if(iSoundID == 31)	//东风
				{
					wsprintf(szFileName,"sound\\putong\\man\\dongfeng.wav",iSoundID);
				}
				else if(iSoundID == 32)	//南风
				{
					wsprintf(szFileName,"sound\\putong\\man\\nanfeng.wav",iSoundID);
				}
				else if(iSoundID == 33)	//西风
				{
					wsprintf(szFileName,"sound\\putong\\man\\xifeng.wav",iSoundID);
				}
				else if(iSoundID == 34)	//北风
				{
					wsprintf(szFileName,"sound\\putong\\man\\beifeng.wav",iSoundID);
				}
				else if(iSoundID == 35)	//红中
				{
					wsprintf(szFileName,"sound\\putong\\man\\zhong.wav",iSoundID);
				}
				else if(iSoundID == 36)	//发财
				{
					wsprintf(szFileName,"sound\\putong\\man\\fa.wav",iSoundID);
				}
				else if(iSoundID == 37)	//白板
				{
					wsprintf(szFileName,"sound\\putong\\man\\bai.wav",iSoundID);
				}
				else if(iSoundID == SOUND_CHI)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\man\\chi.wav",iSoundID);
				}
				else if(iSoundID == SOUND_PENG)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\man\\peng.wav",iSoundID);
				}
				else if(iSoundID == SOUND_GANG)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\man\\gang.wav",iSoundID);
				}
				else if(iSoundID == SOUND_TING)	//听
				{
					wsprintf(szFileName,"sound\\putong\\man\\ting.wav",iSoundID);
				}
				else if(iSoundID == SOUND_HU)	//胡
				{
					wsprintf(szFileName,"sound\\putong\\man\\hu.wav",iSoundID);
				}
				else if(iSoundID == SOUND_ZIMO)	//胡
				{
					wsprintf(szFileName,"sound\\putong\\man\\zimo.ogg",iSoundID);
				}
			}
			else
			{
				//万
				if(iSoundID >=1 && iSoundID <=9)
				{
					wsprintf(szFileName,"sound\\putong\\wom\\%dwan.wav",iSoundID);
				}
				else if(iSoundID >=11 && iSoundID <=19)	//条
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\wom\\%dtiao.wav",iSoundID);
				}
				else if(iSoundID >=21 && iSoundID <=29)	//筒
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\wom\\%dtong.wav",iSoundID);
				}
				else if(iSoundID == 31)	//东风
				{
					wsprintf(szFileName,"sound\\putong\\wom\\dongfeng.wav",iSoundID);
				}
				else if(iSoundID == 32)	//南风
				{
					wsprintf(szFileName,"sound\\putong\\wom\\nanfeng.wav",iSoundID);
				}
				else if(iSoundID == 33)	//西风
				{
					wsprintf(szFileName,"sound\\putong\\wom\\xifeng.wav",iSoundID);
				}
				else if(iSoundID == 34)	//北风
				{
					wsprintf(szFileName,"sound\\putong\\wom\\beifeng.wav",iSoundID);
				}
				else if(iSoundID == 35)	//红中
				{
					wsprintf(szFileName,"sound\\putong\\wom\\zhong.wav",iSoundID);
				}
				else if(iSoundID == 36)	//发财
				{
					wsprintf(szFileName,"sound\\putong\\wom\\fa.wav",iSoundID);
				}
				else if(iSoundID == 37)	//白板
				{
					wsprintf(szFileName,"sound\\putong\\wom\\bai.wav",iSoundID);
				}
				else if(iSoundID == SOUND_CHI)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\wom\\chi.wav",iSoundID);
				}
				else if(iSoundID == SOUND_PENG)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\wom\\peng.wav",iSoundID);
				}
				else if(iSoundID == SOUND_GANG)	//碰
				{
					wsprintf(szFileName,"sound\\putong\\wom\\gang.wav",iSoundID);
				}
				else if(iSoundID == SOUND_TING)	//听
				{
					wsprintf(szFileName,"sound\\putong\\wom\\ting.wav",iSoundID);
				}
				else if(iSoundID == SOUND_HU)	//胡
				{
					wsprintf(szFileName,"sound\\putong\\wom\\hu.wav",iSoundID);
				}
				else if(iSoundID == SOUND_ZIMO)	//胡
				{
					wsprintf(szFileName,"sound\\putong\\wom\\zimo.ogg",iSoundID);
				}
			}
		}
		else
		{
			//万
			if(iSoundID >=1 && iSoundID <=9)
			{
				wsprintf(szFileName,"sound\\putong\\man\\%dwan.wav",iSoundID);
			}
			else if(iSoundID >=11 && iSoundID <=19)	//条
			{
				iSoundID = iSoundID%10;
				wsprintf(szFileName,"sound\\putong\\man\\%dtiao.wav",iSoundID);
			}
			else if(iSoundID >=21 && iSoundID <=29)	//筒
			{
				iSoundID = iSoundID%10;
				wsprintf(szFileName,"sound\\putong\\man\\%dtong.wav",iSoundID);
			}
			else if(iSoundID == 31)	//东风
			{
				wsprintf(szFileName,"sound\\putong\\man\\dongfeng.wav",iSoundID);
			}
			else if(iSoundID == 32)	//南风
			{
				wsprintf(szFileName,"sound\\putong\\man\\nanfeng.wav",iSoundID);
			}
			else if(iSoundID == 33)	//西风
			{
				wsprintf(szFileName,"sound\\putong\\man\\xifeng.wav",iSoundID);
			}
			else if(iSoundID == 34)	//北风
			{
				wsprintf(szFileName,"sound\\putong\\man\\beifeng.wav",iSoundID);
			}
			else if(iSoundID == 35)	//红中
			{
				wsprintf(szFileName,"sound\\putong\\man\\zhong.wav",iSoundID);
			}
			else if(iSoundID == 36)	//发财
			{
				wsprintf(szFileName,"sound\\putong\\man\\fa.wav",iSoundID);
			}
			else if(iSoundID == 37)	//白板
			{
				wsprintf(szFileName,"sound\\putong\\man\\bai.wav",iSoundID);
			}else if(iSoundID == SOUND_CHI)	//碰
			{
				wsprintf(szFileName,"sound\\putong\\man\\chi.wav",iSoundID);
			}
			else if(iSoundID == SOUND_PENG)	//碰
			{
				wsprintf(szFileName,"sound\\putong\\man\\peng.wav",iSoundID);
			}
			else if(iSoundID == SOUND_GANG)	//碰
			{
				wsprintf(szFileName,"sound\\putong\\man\\gang.wav",iSoundID);
			}
			else if(iSoundID == SOUND_TING)	//听
			{
				wsprintf(szFileName,"sound\\putong\\man\\ting.wav",iSoundID);
			}
			else if(iSoundID == SOUND_HU)	//胡
			{
				wsprintf(szFileName,"sound\\putong\\man\\hu.wav",iSoundID);
			}
			else if(iSoundID == SOUND_ZIMO)	//胡
			{
				wsprintf(szFileName,"sound\\putong\\man\\zimo.ogg",iSoundID);
			}
		}
	}


	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/*----------------------------------------------------------------------------------------------*/
//初始化数据
void CNewGameClient::InitGameDate()
{   
	m_bMouseEnter = false;
	m_bIsHua	  = false;


    m_ucCurrentPlayer = 0xFF;                   //当值玩家
 
    m_byHandState = 0xFF;                       //手牌状态 0表无状态 1表吃牌选择状态
  

   
	///初始化一些变量	
	for (int i=0; i<PLAY_COUNT; ++i)
    {
        m_UserDataCard[i].ClearOut();
        m_UserDataCard[i].ClearCPG();
        m_UserDataCard[i].ClearHand();        
    }
    m_UserData.InitData();
	m_GameData.InitData();
    m_nTimeOutCnt = 0;
	m_GameData.m_iRemaindTime = 0;
}

























//--------------------------------ADD-BY-DWJ--------------------------------------------------------------
/*以下函数 所有游戏可通用*/
//--------------------------------------------------------------------------------------------------------
//设置动画控件播放动画
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,POINT pointStart,POINT pointEnd,CURVE_TYPE CT_TYPE,BYTE byPlayNum, bool bIsLoop)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetPlayNum(byPlayNum);
		if (pointEnd.x != 0 && 0 != pointEnd.y)
		{
			pAnimate->SetEnd(pointEnd);
		}

		if (pointStart.x != 0 && 0 != pointStart.y)
		{
			pAnimate->SetStartP(pointStart);
		}

		pAnimate->SetCurveInstance(CT_TYPE);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}

}
//--------------------------------------------------------------------------------------------------------
//设置动画控件播放动画
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,POINT position,bool bIsLoop ,CURVE_TYPE CT_TYPE )
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetGameXY(position.x,position.y);
		pAnimate->SetCurveInstance(CT_TYPE);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置动画控件播放指定动画
void	CNewGameClient::SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum , bool bIsLoop )
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetExtFilePath(wsFilePath) ;
		pAnimate->LoadFromFile();
		pAnimate->SetPlayNum(byPlayNum);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置动画显示某一帧
void	CNewGameClient::SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame,POINT point)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		if (-1 == iFrame)
		{
			pAnimate->SetShowFrame(pAnimate->GetFrameNum()-1);
		}
		else
		{
			pAnimate->SetShowFrame(iFrame);
		}
		if(point.x != 0 && point.y != 0)
		{
			pAnimate->SetGameXY(point.x, point.y);
		}
		pAnimate->SetControlVisible(bFlag);

	}
}
//--------------------------------------------------------------------------------------------------------
//设置按钮是否可用
void	CNewGameClient::SetBtEnable(const int iButtonID, bool bFlag)
{

	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetEnable(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置按钮是否可见
void	CNewGameClient::SetBtVisible(const int iButtonID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//获取按钮是否可用
bool	CNewGameClient::GetBtIsEnable(const int iButtonID)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		return	pButton->GetEnable();
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
//设置时间框显示倒计时
void	CNewGameClient::SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime)
{
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(iTimeID));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(byMaxTime);
		pTime->SetShowMinNum(byMinTime);
		pTime->SetPLay(bFlag);
		pTime->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置图片控件的位置
void	CNewGameClient::SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetGameXY(iPositonX,iPositonY);
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置图片控件是否可见
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag,int iSrcX, int iSrcY)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetControlVisible(bFlag);
		pImage->SetSrcX(iSrcX);
		pImage->SetSrcY(iSrcY);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置图片控件加载显示指定的文件
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
	//virtual int LoadPic(wchar_t *szFileName)=0;
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置容器控件可见
void	CNewGameClient::SetContainVisible(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置文本控件显示内容
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%I64d",iNum);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置文本控件显示内容
void	CNewGameClient::SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",sChar);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置文本控件显示内容
void	CNewGameClient::SetTextinfo(const int iTextID,CString sText, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置数字空间显示内容
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置筹码控件显示值
void	CNewGameClient::SetShCtrlInfo(const int iShCtrlID, int iNum, bool bFlag)
{
	IShCtrl *pShCtrl = NULL;
	pShCtrl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCtrlID));
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(iNum);
		pShCtrl->SetControlVisible(bFlag);
	}

}
//--------------------------------------------------------------------------------------------------------
//设置单选框控件是否被选中
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将牌墙控件是否可见
void	CNewGameClient::SetMjWallInfo(const int iMjWallID,int iFillNum,bool bFlag)
{
	IMjWallCtrl *pMjWall = NULL;
	pMjWall = dynamic_cast<IMjWallCtrl*>(m_pUI->GetIControlbyID(iMjWallID));
	if (NULL != pMjWall)
	{
		pMjWall->FillWallBack(iFillNum);
		pMjWall->SetMaxLength(iFillNum);
		pMjWall->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将城墙指定的麻将信息
void	CNewGameClient::SetMjWallTittle(const int iMjWallID,int iValue,int iPosition,bool bFlag)
{
	IMjWallCtrl *pMjWall = NULL;
	pMjWall = dynamic_cast<IMjWallCtrl*>(m_pUI->GetIControlbyID(iMjWallID));
	if (NULL != pMjWall)
	{
		pMjWall->SetWallTile(iValue,iPosition);
		pMjWall->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将出牌控件信息
void	CNewGameClient::SetMjGiveInfo(const int iMjGiveID, int iLineNum, int iHandTiles[], int iCount, bool bFlag)
{
	IMjGiveCtrl *pMjGive = NULL;
	pMjGive = dynamic_cast<IMjGiveCtrl*>(m_pUI->GetIControlbyID(iMjGiveID));
	if (NULL != pMjGive)
	{
		pMjGive->SetGiveInfo(iHandTiles,iCount);
		pMjGive->SetGiveLineCnt(iLineNum);
		pMjGive->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//获取麻将绘制的举行
RECT	CNewGameClient::GetMjGiveRect(const int iMjGiveID,int iIndex)
{
	RECT r;
	memset(&r,0,sizeof(r));
	IMjGiveCtrl *pMjGive = NULL;
	pMjGive = dynamic_cast<IMjGiveCtrl*>(m_pUI->GetIControlbyID(iMjGiveID));
	if (NULL != pMjGive)
	{
		r = pMjGive->GetTileRectByIndex(iIndex);
	}
	return r;
}
//--------------------------------------------------------------------------------------------------------
//设置麻将手牌控件是否可见
void	CNewGameClient::SetMjHandVisible(const int iMjHandID, bool bFlag)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将手牌控件信息
void	CNewGameClient::SetMjHandInfo(const int iMjHandID, int byCards[], int iCardCount,bool bFlag,bool bLaydown)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetHandInfo(byCards,iCardCount);
		pMjHand->SetControlVisible(bFlag);
		pMjHand->SetIsLaydown(bLaydown);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将手牌控件打倒
void	CNewGameClient::SetMjHandLayDown(const int iMjHandID, bool bLayDown)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetIsLaydown(bLayDown);
	}
}
//--------------------------------------------------------------------------------------------------------
//设置麻将手牌控件最后一张是否隔开
void	CNewGameClient::SetMjHandGiveMode(const int iMjHandID, bool bMode)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetGiveTileMode(bMode);
	}
}
//--------------------------------------------------------------------------------------------------------
//清除麻将控件拦牌信息
void	CNewGameClient::ClearMjHandBlock(const int iMjHandID)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->ClearAllBlock();
	}
}
//--------------------------------------------------------------------------------------------------------
//设置拦牌信息
void	CNewGameClient::AddMjHandOneBlock(const int iMjHandID, MJ_ACTION_TYPE BlockType, BYTE byCardData[], BYTE byBeChair)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));

	if (NULL != pMjHand)
	{
		BYTE byMyView = m_pGameFrame->DeskStation2View(m_nDeskStation);

		if(BlockType == AT_QUADRUPLET_CONCEALED && byBeChair != byMyView)
		{
			memset(byCardData,0,sizeof(byCardData));
		}
		
		pMjHand->AddOneBlock((MJ_LOGIC_TYPE)BlockType, byCardData, byBeChair);
	}

}
//--------------------------------------------------------------------------------------------------------
void	CNewGameClient::SetMjHandUseMouse(const int iMjHandID, bool bFlag)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));

	if (NULL != pMjHand)
	{
		pMjHand->SetIsUseMouse(bFlag);
	}
}
/*-----------------------------------------------------------------------*/
//设置手牌麻将不可选择
void	CNewGameClient::SetMjHandInvalidTile(const int iMjHandID, int iHandCard[], int iCount)
{

	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetInvalidTile(iHandCard,iCount);
	}
}

/*-----------------------------------------------------------------------*/
//设置聊天框显示内容	
void	CNewGameClient::InsertTalkInfo(const int iTalkID,CString sText)
{
	ITalk *pTalk = NULL;
	pTalk = dynamic_cast<ITalk*>(m_pUI->GetIControlbyID(iTalkID));
	if(NULL != pTalk)
	{
		TCHAR* pResult = new TCHAR[MAX_PATH];
		memset(pResult, 0, MAX_PATH * sizeof(TCHAR));
		wsprintf(pResult, TEXT("<font><color=0xffffff00,size=12,style=楷书> %s</font>"),sText);
		pTalk->SetText(CT2W(pResult));
		delete pResult;
	}
}

/*-----------------------------------------------------------------------*/
//清除聊天框信息
void	CNewGameClient::ClearTalkInfo(const int iTalkID)
{
	ITalk *pTalk = NULL;
	pTalk = dynamic_cast<ITalk*>(m_pUI->GetIControlbyID(iTalkID));
	if(NULL != pTalk)
	{
		pTalk->ClearText();
	}
}

void CNewGameClient::OnNotifyMaiMa(BYTE* buffer, int nLen)
{

	if(nLen != sizeof(m_GameData.m_TMaiMa))
	{
		return;
	}

	tagMaiMa* pMaiMa = (tagMaiMa*)buffer;
	if (NULL == pMaiMa)
	{
		return;
	}
	memcpy(&m_GameData.m_TMaiMa,pMaiMa,nLen);

	//是谁买码
	if (m_GameData.m_TMaiMa.byUser == m_nDeskStation)
	{
		m_byMaiMaIndex = 0;
		//显示买码容器
		ShowMaiMaContain();
	}

}


void CNewGameClient::OnNotifyZhongNiao(BYTE* buffer, int nLen)
{
	if(nLen != sizeof(m_GameData.m_ZhongNiao))
	{
		return;
	}

	SetContainVisible(2008100,true);

	ZhongNiao* pZhongNiao = (ZhongNiao*)buffer;
	memcpy(&m_GameData.m_ZhongNiao,pZhongNiao,nLen);


	for(int i = 0; i < 4; i++)
	{
		IMjHandCtrl* pMjHandCtrl = NULL;
		pMjHandCtrl = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(2008102 + i));
		if(!pMjHandCtrl) continue;
		pMjHandCtrl->SetHandInfo(NULL,0);
		int card = {m_GameData.m_ZhongNiao.byCard[i]};
		if (255 != card)
		{
			pMjHandCtrl->SetHandInfo(&card,1);
			pMjHandCtrl->SetControlVisible(true);
		}
	}

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//保存玩家的门牌数据
		m_UserData.SetMenPaiData(i,pZhongNiao->byMenPai[i]);
		//显示牌墙（门牌）	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);			
	}

	for(int i = 0; i < 4; i++)
	{
		IImage* pImage = NULL;
		pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(2008106+i));
		pImage->SetControlVisible(false);
	}

	m_pGameFrame->SetTimer(TIME_SHOW_NIAO,500);
	
}
