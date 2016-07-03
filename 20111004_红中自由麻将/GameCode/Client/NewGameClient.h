#include "IGameImpl.h"
#include "UI_ForLogic/IUserInterface.h"

#include "..\ChangeFile\UserDataEx.h"
#include "..\ChangeFile\GameDataEx.h"
#include "sounddefines.h"

#include <irrKlang.h>
using namespace irrklang;
//#include "SoundsDef.h"
#include <queue>

interface IGameFrame;
interface IUserInterface;


#define     BN_VOLUM_UP            13      ///< 音量加
#define     BN_VOLUM_DOWN          14      ///< 音量减
#define     BN_VOLUM_OFF           15      ///< 音乐关
#define     BN_VOLUM_ON            16      ///< 音乐开



#define TIME_OUT_CARD			10		//出牌计时器
#define TIME_BLOCK_CARD			11		//拦牌计时器
#define TIME_ZHUAPAI			12		// 抓牌计时器
#define TIME_HIDE_SEZI_ANI		13      // 色子动画播放后，隐藏它的定时器
#define TIME_BEGIN				14      // 自动开始

#define ID_WARMING_TIME			17		// 警告定时器

#define TIME_ONE_SECOND			20      // 一秒倒计时

#define TIME_SHOW_HU			21      // 显示糊牌对话框定时器
#define ID_BEGIN_MOPAI          22      //客户端首轮展现摸牌动画
#define TIME_SHOW_NIAO          23      //展示中鸟动画



/*---------------------------------------------------------------------------*/
//const	POINT	G_PointStart[PLAY_COUNT]	= {{380,140},{620,360},{400,600},{175,345}};
//const	POINT	G_PointEnd[2][PLAY_COUNT]	= {{{360,335},{380,395},{430,390},{370,400}},
//											   {{460,350},{380,330},{365,380},{335,320}}};
const	POINT	G_PointStart[PLAY_COUNT]	= {{536,140},{876,360},{526,600},{175,345}};
const	POINT	G_PointEnd[2][PLAY_COUNT]	= {{{516,335},{536,395},{536,370},{506,400}},
												{{606,350},{536,330},{471,360},{451,320}}};
/*---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
//牌墙隐藏数据用到
//static int G_iWallChangePos[MEN_CARD_NUM] ={35,34,33,32,31,30,29,28,27,26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
//static int G_iWallChangePos[MEN_CARD_NUM] ={0,1,2,3,4,5,6,7,8,9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
static int G_iWallChangePos[] ={0,1,2,3,4,5,6,7,8,9,10,11, 12, 13, 14, 15, 16, 17, 18, 19,20,21, 22, 23, 24, 25, 26, 27, 28, 29,30, 31, 32, 33, 34,35};
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct MjUiInfo
{
    int         m_nUserHandInfo[20];
    int         m_nHandCardCount;

    int         m_nUserOutInfo[OUT_CARD_NUM];
    int         m_nOutCardCount;

	bool        m_bIsBigGang[5];//是否大杠
    BYTE        m_ucCPGData[5][4];
    BYTE        m_ucSpeCPGData[5][4];//特殊杠数据
    BYTE        m_ucSpeCPGNum[5][4];//特殊杠牌的数量
    BYTE        m_ucChair[5];
    BYTE        m_ucCPGTile[5];
    MJ_LOGIC_TYPE m_CPGtype[5];
    int         m_nCPGCount;
    
    MjUiInfo()
    {
        memset(this, 0, sizeof(MjUiInfo));
    }
    void ClearHand()
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = 0;
    }
    void GetFromByte(BYTE ucByteInfo[])
    {
        ClearHand();
        for (int i=0; i<17; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nHandCardCount++;
            m_nUserHandInfo[i] = ucByteInfo[i];
        }
    }
    void FillWithBack(int nNum)
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = nNum;
    }
    void ClearOut()
    {
        memset(m_nUserOutInfo, 0, sizeof(m_nUserOutInfo));
        m_nOutCardCount = 0;
    }
    void GetOutFromByte(BYTE ucByteInfo[])
    {
        ClearOut();
        for (int i=0; i<OUT_CARD_NUM; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nUserOutInfo[i] = ucByteInfo[i];
			m_nOutCardCount++;
        }
    }
    void ClearCPG()
    {
        memset(m_bIsBigGang, 0, sizeof(m_bIsBigGang));//是否大杠
        memset(m_ucSpeCPGData, 0, sizeof(m_ucSpeCPGData));//特殊杠数据
        memset(m_ucSpeCPGNum, 0, sizeof(m_ucSpeCPGNum));//特殊杠牌的数量
        memset(m_ucCPGData, 0, sizeof(m_ucCPGData));
        memset(m_ucChair, 0, sizeof(m_ucChair));
        memset(m_ucCPGTile, 0, sizeof(m_ucCPGTile));
        memset(m_CPGtype, LT_EMPTY, sizeof(m_CPGtype));
        m_nCPGCount = 0;
    }
    void GetFromCPGData(GCPStructEx cpgstr[],bool show=true)
    {
        ClearCPG();
		int index = 0;
        for (int i=0; i<5; ++i)
        {
			if(cpgstr[i].byType == 255)
				continue;
			if(cpgstr[i].bIsBigGang)
			{
				if (-1==cpgstr[i].byBigGang[0][0] || 255==cpgstr[i].byBigGang[0][0] || 0==cpgstr[i].byBigGang[0][0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucSpeCPGData[i][j] = cpgstr[i].byBigGang[j][0];	
					m_ucSpeCPGNum[i][j]	 = cpgstr[i].byBigGang[j][1];	//每张牌的数量
				} 
				m_bIsBigGang[i] = true;//标志大杠
			}
			else
			{
				if (-1==cpgstr[i].byData[0] || 255==cpgstr[i].byData[0] || 0==cpgstr[i].byData[0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucCPGData[i][j] = cpgstr[i].byData[j];                
				} 
			}
            m_ucChair[i] = cpgstr[i].iBeStation;
            m_ucCPGTile[i] = cpgstr[i].iOutpai;
            m_nCPGCount ++;

            if (ACTION_CHI==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_COLLECT;
            }
            else if (ACTION_PENG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_TRIPLET;
            }
            else if (ACTION_AN_GANG==cpgstr[i].byType)
            {
                for (int j=0; j<3; ++j)
                {
                    m_ucCPGData[i][j] = 0;
                }
				if(!show)
					m_ucCPGData[i][3] = 0;
                m_CPGtype[i] = LT_QUADRUPLET_CONCEALED;
            }
            else if (ACTION_BU_GANG==cpgstr[i].byType || ACTION_MING_GANG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_QUADRUPLET_REVEALED;
            }
        }
    }
};


class CNewGameClient : public IGameImpl
{

public:

    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);

    virtual ~CNewGameClient();

private:

    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

public:

#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
	CTRL_TYPE* PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL) PRE->OPERATE; \
	}\

/*-----------------------------------------------------------------------------------------------------------
	一系列平台提供的需要重载来驱动游戏的函数，已适用大多数游戏的开发，但每个函数的实现因游戏而异。
  ----------------------------------------------------------------------------------------------------------*/
public:
    //初始化
    virtual int		Initial();
    //初始化UI
    virtual int		InitUI();
    //游戏消息
    virtual int		HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    //界面消息
    virtual int		OnUIMessage(TUIMessage* pMessage);
    //
    virtual int		ResetGameFrame(void);
    //玩家进入
    virtual int		GameUserCome(void);
    //玩家离开
    virtual int		GameUserLeft(int nIndex);
    //玩家断线
    virtual int		GameUserOffLine(void);
    //是否正在游戏，由游戏实现
    //旁观玩家返回false，游戏玩家正在游戏中返回true，其它状态返回false
    virtual bool	IsPlayingGame();
    //定时器消息
    virtual void OnTimer(int nTimerID);
    //置状态
    virtual void	SetStationParameter(BYTE bGameStation);
	//设置游戏状态，主要是断线重连来后处理的消息
    virtual void	SetGameStation(void * pBuffer,int nLen);
	//退出游戏
	virtual bool	OnGameQuiting();




/*-----------------------------------------------------------------------------------------------------------
	所有逻辑处理模块
  ----------------------------------------------------------------------------------------------------------*/
private:

	//游戏框架
    IGameFrame* m_pGameFrame;
	//游戏UI
    IUserInterface* m_pUI;
	//背景音乐引擎
	ISoundEngine* m_SoundEnginBg;
	//游戏音效引擎
	ISoundEngine* m_SoundEngineGame;
	//游戏背景音乐声效开关
	bool			m_bBgMusic;
	//游戏音效开关
	bool			m_bGameSound;
	///<自己在服务器上的坐位号
	int             m_nDeskStation;    
	//游戏状态
	BYTE			m_bGameStation;
	//手牌状态 0表无状态 1表吃牌选择状态 2表示选择杠牌状态 3表示选择听牌状态
	int            m_byHandState;      
	//标示鼠标是否移入(选择吃牌的时候有用)
	bool			m_bMouseEnter;		
	//当前抓到的牌 是不是花牌 
	bool			m_bIsHua;			
	//游戏中玩家昵称
	CString			m_sUserNick[PLAY_COUNT];
	//是否超端用户
	bool			m_bIsSuperUser;
	//游戏数据
	GameDataEx      m_GameData;		
	///玩家的数据：手牌，出牌，吃碰杠牌等
	UserDataEx      m_UserData;	

	






	//该客户端是否允许被旁观
	bool m_bAllowWatch;
    BYTE            m_ucCurrentPlayer;  //当值玩家
    MjUiInfo        m_UserDataCard[PLAY_COUNT]; 
    int               m_nTimeOutCnt;
	bool			m_bIsBigGang[2];//是否大杠
	BYTE			m_byCheckPai;//选择的杠牌

	//用来展现首轮摸牌动画
	BYTE            m_byMoPaiNum; //摸牌次数，首轮共要摸16次
	int				m_iDeskHandPai[PLAY_COUNT][MAX_HAND_PAI];
	BYTE            m_byDeskMenPai[PLAY_COUNT][MEN_CARD_NUM];
	BYTE            m_byMoPaiStation;//当前摸牌玩家位置
	BYTE			m_byDirect;   //起始位置
	BYTE            m_byGetPai;      //数量

	BYTE			m_byMaiMaIndex;	//买码的数量
private:
	//	用户同意	DWJ
	void	OnUserAgree(void *buffer, int len);
	// 用户托管
	void	OnUserTuoguan(void *buffer, int nLen);
	//	游戏开始	DWJ
	void	OnGameStart(void *buffer, int len);
	//	确定庄家消息	DWJ
	void	OnDingNt(void *buffer, int nLen);
	//	第一次掷筛子确定起牌方向	DWJ
	void	On2SeZiDir(void *buffer, int nLen);
	//	第二次筛子确认起牌墩数消息	DWJ
	void	On2SeZiDirGP(void *buffer, int nLen);
	//	发送手牌数据	DWJ
	void	OnSendAllCard(void *buffer, int nLen);
	//	所有玩家补花消息
	void	OnAllBuHua( void *buffer, int nLen);
	//	游戏开始打牌了
	void	OnBeginOutCard(void *buffer, int nLen);
	//	出牌结果
	void	OnNotifyOutCard(void *buffer, int nLen);
	//	拦牌
	void	OnNotifyBlock(void *buffer, int nLen);
	//拦牌 llj
	void	OnNotifyAction(void *buffer, int nLen);
	//	抓牌	DWJ
	void	OnNotifyZhuaPai(void *buffer, int nLen);
	//	单个玩家补花	DWJ
	void	OnNotifyOneBuHua( void *buffer, int nLen);   
	//	玩家放弃拦牌
	void	OnNotifyCanCelBlock(void *buffer, int nLen);
	//	吃牌	
	void	OnNotifyChiPai(void *buffer, int nLen);
	//	碰牌
	void	OnNotifyPengPai(void *buffer, int nLen);
	// 杠牌
	void	OnNotifyGangPai(void *buffer, int nLen);
	// 胡牌
	void	OnNotifyHuPai(void *buffer, int nLen);  
	// 结算
	void	OnNotifyFinish(void *buffer, int nLen);

private:
	//玩家换牌结果
	void	OnUserChangeCard(void *buffer, int nLen);
	///重新开局时的初始化UI界面	DWJ
	void	ReStartGameInitUI();
	//所有玩家信息控件
	void	UIShowUserInfo(BYTE byStation, bool bFlag);
	//	显示城墙数据	DWJ
	//	数据格式：255表没有牌， 0表牌背，其它与手牌一样
	void	UIShowWall(BYTE byStation, BYTE byArrMenPai[]);
	//	显示所有手牌	DWJ
	void	UIShowAllHandPai(bool bFlag,bool bFinish = false);
	//	设置所有的牌显示牌背	DWJ
	void	UIShowAllHandPaiBack(bool bFlag, bool bLayDown);
	//	显示花数量		DWJ
	void	UIShowHuaCount(bool bFlag);
	//	东南西北显示方位
	void	UIShowDNXB(bool bFlag);
	//	显示庄家局数信息
	void	UIShowNtInfo(bool bFlag);
	//	显示听牌标识	DWJ
	void	UIShowTingLog(bool bFlag);
	//	显示倒计时	DWJ
	void	ShowCountDownTimer(BYTE byDeskStation, BYTE byTime,bool bFlag);
	//	显示所有玩家出牌数据
	void	UIShowOutCard(BYTE byStation, bool bShow);
	//	显示拦牌容器	DWJ
	void	UIShowBlockContain(bool bFlag);
	//	显示吃牌容器	DWJ
	void	UIShowChiContain(bool bFlag);
	//	显示吃牌容器1	DWJ
	void	ShowChiContain1(bool bFlag);
	//	显示吃牌容器2	DWJ
	void	ShowChiContain2(bool bFlag);
	//	显示吃牌容器3	DWJ
	void	ShowChiContain3(bool bFlag);
	//	设置选择杠牌	DWJ
	void	SetChoiceGangPai(int iGangCount,bool bFlag);
	//	设置选择听牌状态只能选择出听的牌
	void	SetChoiceTingPai();
	// 显示结算框
	void	UIShowResultDlg(bool bFlag);
	//托管显示
	void	UIShowTuoGuan();
	//显示游戏设置
	void	UIShowGameSetting(bool bFlag);
	//显示超端容器
	void	ShowSuperDlag(bool bFlag);
	//响应换牌
	void	OnUiChangeCard();
private:	/*-----------UI响应事件--------------*/
	//	请求出牌                
	void	OnUIRequestOutCard(UINT32 uControlID,bool outtime = false);
	//	请求出牌
	void	RequestOutCard(BYTE byData);
	//	放弃拦牌
	void	RequestCancelBlock();
	//	鼠标点击吃牌按钮
	void	OnUiClickChiPai();
	//	请求吃牌
	void	RequestBlockChi(int iIndex);
	//	请求碰牌
	void	RequestBlockPeng();
	//	请求杠牌
	void	RequestBlockGang();
	//	点击手牌请求杠牌
	void	OnUIRequestGangPai(const int iControlID);
	//	请求听牌
	void	RequestBlockTing();
	//	请求胡牌
	void	RequestBlockHu();
	// 请求托管
	void	RequestTuoGuan(bool bTuoguan = true);
	// 动画控件消息
	int		OnCartoonFinish(TUIMessage *pMessage);


	
	
	



	 /// 强退结束游戏
	void OnForceFinish( void *buffer, int nLen);

    // @brief 显示移动动画控件
    void UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow);

    // 当麻将控件不可用时，鼠标移动到控件上
    int OnMoveOnMjCtrl(TUIMessage* pMessage);
    // 当麻将控件不可用时，鼠标单击控件
    int OnLButtonOnMjCtrl(TUIMessage* pMessage);
    



public:
	

	//设置计时器
	void OnSetTimer(BYTE station,int type);
	//删除所有时间
	void OnKillAllTimer();
	/// 消除定时器
	BOOL KillGameTimer(int TimerID);
    // 设置定时器
    void UIShowTimer(BYTE bViewStation, int nTime, bool bShow = true);


private:
	//发送游戏数据:发送通知消息，不包含数据
	void	SendGameDataEx(BYTE id);
	//发送游戏数据
	void	SendGameDataEx(BYTE id,void *pData,UINT uSize);
	//闪烁窗口
	void	FlashWindow();



	//将字符串转换成牌数据
	void GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type);
	//将两个字符转换成牌
	BYTE ChangeChar(wchar_t data1,wchar_t data2);






	
private:
	//初始化数据
	void InitGameDate();
	//加载游戏配置
	void	LoadGameSet();
	//保存游戏配置
	void	SaveGameSet();
	//播放背景音效	DWJ
	void	PlayBgSound(bool bISLoop);

	//播放游戏音效	DWJ
	void	PlayGameSound(BYTE byDeskStation,int SoundID, bool bISLoop = false);
	//保存玩家的的昵称
	void	GetUserName();
	//启动一秒倒计时 m_iRemaindTime
	void	SetOneSecondCountDown(int iTime);
	



/*----------------------------------------------一下函数所有UI编辑器版本的游戏都可以使用---------------------------------------------------------------------*/
public:
	//设置动画控件播放动画
	void	SetAnimatePlay(const int AnimateID,bool bFlag,POINT pointStart,POINT pointEnd,CURVE_TYPE CT_TYPE = CT_NONE,BYTE byPlayNum = 1, bool bIsLoop = false);
	//设置动画控件播放动画
	void	SetAnimatePlay(const int AnimateID,bool bFlag,POINT position,bool bIsLoop = false,CURVE_TYPE CT_TYPE = CT_NONE);
	//设置动画控件播放指定动画
	void	SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//设置动画显示某一帧
	void	SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame,POINT point);

	//设置按钮是否可用
	void	SetBtEnable(const int iButtonID, bool bFlag);
	//设置按钮是否可见
	void	SetBtVisible(const int iButtonID, bool bFlag);
	//获取按钮是否可用
	bool	GetBtIsEnable(const int iButtonID);

	//设置时间框显示倒计时
	void	SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);

	//设置图片控件的位置
	void	SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);
	//设置图片控件是否可见
	void	SetImageVisible(const int iImageID, bool bFlag,int iSrcX = 0, int iSrcY = 0);
	//设置图片控件加载显示指定的文件
	void	SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);

	//设置容器控件可见
	void	SetContainVisible(const int iContainID,bool bFlag);

	//设置文本控件显示内容
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//设置文本控件显示内容
	void	SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag);
	//设置文本控件显示内容
	void	SetTextinfo(const int iTextID,CString sText, bool bFlag);

	//设置数字空间显示内容
	void	SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag);

	//设置筹码控件显示值
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);

	//设置单选框控件是否被选中
	void	SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag);

	//设置麻将牌墙控件填充大小&&是否可见
	void	SetMjWallInfo(const int iMjWallID,int iFillNum,bool bFlag);
	//设置麻将城墙指定的麻将信息
	void	SetMjWallTittle(const int iMjWallID,int iValue,int iPosition,bool bFlag);

	//设置麻将出牌控件信息
	void	SetMjGiveInfo(const int iMjGiveID, int iLineNum, int iHandTiles[], int iCount, bool bFlag);
	//获取麻将绘制的举行
	RECT	GetMjGiveRect(const int iMjGiveID,int iIndex);

	//设置麻将手牌控件是否可见
	void	SetMjHandVisible(const int iMjHandID, bool bFlag);
	//设置麻将手牌控件信息
	void	SetMjHandInfo(const int iMjHandID, int byCards[], int iCardCount,bool bFlag,bool bLaydown = false);
	//设置麻将手牌控件打倒
	void	SetMjHandLayDown(const int iMjHandID, bool bLayDown);
	//设置麻将手牌控件最后一张是否隔开
	void	SetMjHandGiveMode(const int iMjHandID, bool bMode);
	//清除麻将控件拦牌信息
	void	ClearMjHandBlock(const int iMjHandID);
	//设置拦牌信息
	void	AddMjHandOneBlock(const int iMjHandID, MJ_ACTION_TYPE BlockType, BYTE byCardData[], BYTE byBeChair);
	//设置麻将手牌控件是否鼠标穿透pMjHand->SetIsUseMouse(false);
	void	SetMjHandUseMouse(const int iMjHandID, bool bFlag);
	//设置手牌麻将不可选择
	void	SetMjHandInvalidTile(const int iMjHandID, int iHandCard[], int iCount);

	//设置聊天框显示内容
	void	InsertTalkInfo(const int iTalkID, CString sText);
	//清除聊天框信息
	void	ClearTalkInfo(const int iTalkID);


	void On2SeZiNT(void * buffer, int nLen);
	void On2SeZiGP(void* buffer, int nLen);
	void OnNotifyMaiMa(BYTE* buffer, int nLen);
	void OnNotifyZhongNiao(BYTE* buffer, int nLen);

	//获取麻将手牌一张牌数据
	int		GetMjHandOneCard(const int iMjHandID);

	//显示买码容器
	void	ShowMaiMaContain(bool bShow = true);
};