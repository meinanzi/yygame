#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "GameDataEx.h"

//全局对象
GameDataEx g_GameData;

GameDataEx::GameDataEx(void)
{
	GameData::GameData();
	
	LoadIni();

}

GameDataEx::~GameDataEx(void)
{
	GameData::~GameData();
}

	  
///设置所有事件的后接事件
void GameDataEx::SetThingNext()
{
	GameData::SetThingNext();

}

///设置某个事件的后接事件
void GameDataEx::SetThingNext(BYTE id)
{
	GameData::SetThingNext(id);
}

//virtual 初始化数据
void GameDataEx::InitData()
{
    memset(m_byThingRecord,255,sizeof(m_byThingRecord));
    ///待执行事件
    m_byNextAction = ACTION_NO;
	
    ///正在发生的事件号
    m_byThingDoing = 255;  

    ///吃牌牌事件,临时数据
    temp_ChiPai.Clear();
    ///碰牌牌事件,临时数据
    temp_PengPai.Clear();
    ///杠牌牌事件,临时数据
    temp_GangPai.Clear();

    ///游戏开始事件
    T_Begin.Clear();

    ///以东为庄事件
    T_DongNt.Clear();

    ///掷2颗色子的点数和为庄事件
    T_TwoSeziNt.Clear();

    ///掷2颗色子确定庄家和起牌点位置事件
    T_TwoSeziNtAndGetPai.Clear();

    ///轮流掷2颗色子确定庄家
    T_TurnSeziNt.Clear();

    ///掷2颗色子确定起牌位置事件
    T_TwoSeziDir.Clear();

    ///掷2颗色子确定起牌位置（点数和）和起牌点（最小点）事件
    T_TwoSeziDirAndGetPai.Clear();

    ///掷2颗色子确定起牌敦数事件
    T_TwoSeziGetPai.Clear();

    ///发牌事件
    T_SendPai.Clear();

    ///跳牌事件
    T_TiaoPai.Clear();

    ///掷色子2颗色子定精牌事件
    T_TwoSeziJing.Clear();

    ///掷色子1颗色子定金牌事件
    T_OneSeziJin.Clear();

    ///所有玩家补花事件
    T_AllBuHua.Clear();

    ///单个玩家补花事件
    T_OneBuHua.Clear();

    ///开始出牌通知事件
    T_BeginOutPai.Clear();

    ///出牌事件
    T_OutPai.Clear();

    ///抓牌牌事件
    T_ZhuaPai.Clear();

    /////吃碰杠糊牌通知事件牌事件
    //T_CPGNotify[4];

    ///吃牌牌事件
    T_ChiPai.Clear();

    ///碰牌牌事件
    T_PengPai.Clear();



  

    ///杠牌牌事件
    T_GangPai.Clear();

    ///听牌牌事件
    T_TingPai.Clear();

    ///糊牌牌事件
    T_HuPai.Clear();

    ///算分事件
    T_CountFen.Clear();

    ///游戏结束处理事件
    T_EndHandle.Clear();

    ///非正常结束处理事件
    T_UnnormalEndHandle.Clear();

	m_ZhongNiao.clear();
	m_TMaiMa.clear();
}

///初始化数据
void GameDataEx::LoadIni()
{
	GameData::LoadIni();
}


