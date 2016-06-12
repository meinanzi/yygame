#ifndef __HN_WaterMargin_UpgradeMessage_h__
#define __HN_WaterMargin_UpgradeMessage_h__

#include "HNNetExport.h"

namespace WaterMargin
{
	//游戏ID
	enum Game_ID
	{
		NAME_ID = 33000401,
	};

	//版本定义
#define GAME_MAX_VER					1						//现在最高版本
#define GAME_LESS_VER					1						//现在最低版本
#define GAME_CHANGE_VER					0						//修改版本

	//支持类型
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
	//#define GAME_TABLE_NAME					"MatchTable"

	//游戏内部开发版本号
#define DEV_HEIGHT_VERSION				3				//内部开发高版本号(每修正一次LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				//内部开发低版本号

	//游戏指令ID
	enum CMD_ID
	{
		// 数据包处理辅助标识
		ASS_VREB_CHECK					=  179,				//用户处理
		/**************************服务端——>客户端消息**************************/
		ASS_S_GAME_BEGIN				=  	51,				//游戏开始

		ASS_S_BUY_COIN_RESULT			=  	60,				//上分下分结果消息
		ASS_S_ADD_NOTE_RESULT			=  	61,				//玩家加注结果
		ASS_S_ROLL_RESULT				=  	62,				//滚动结果
		SUB_S_MALI_START				=  	63,				//马力开始
		SUB_S_GETBIBEI_RESULT			=  	64,				//获取比倍结果
		ASS_S_GETSCORE_RESULT			=  	65,				//获取得分结果
		ASS_S_BIBEI_RESULT				=  	66,				//比倍压分结果

		/**************************客户端——>服务端消息**************************/
		ASS_C_BUY_COIN					=  	80,				//上分下分消息	
		ASS_C_ADD_NOTE					=  	81,				//玩家加注
		ASS_C_START_ROLL				=  	82,				//启动游戏滚动
		ASS_C_GET_MALI					=  	83,				//获取马力
		ASS_C_GET_BIBEI					=  	84,				//获取比倍
		ASS_C_GET_SCORE					=  	85,				//获取得分
		ASS_C_BIBEI						=  	86				//比倍压分

	};

#define SUPER_PLAYER                    10                         //超级玩家
#define MAX_NAME_INFO                   256

	//游戏状态
	enum Game_State
	{
		GS_WAIT_SETGAME					= 0,				//等待东家设置状态
		GS_WAIT_ARGEE					= 1,				//等待同意设置
		GS_PLAY_GAME					= 20,				//发牌状态
		GS_WAIT_BACK					= 21,				//等待扣压底牌
		GS_WAIT_NEXT					= 23				//等待下一盘开始 
	};

	enum Game_Const
	{
		PLAY_COUNT				    =   1,              //游戏人数
	};


	/********************************************************************************/

	//游戏数据包
	/********************************************************************************/  
	/*-------------------------------------------------------------------------------*/
	#pragma pack(1)
	//游戏状态基础数据包
	struct		GameStation_Base
	{
		BYTE				iVersion;						//游戏版本号
		BYTE				iVersion2;						//游戏版本号


		BYTE			byGameStation;					//游戏状态
		int				iLineCount;						//下注线数
		LLONG			i64UserMoney;					//用户的金币数

		GameStation_Base()
		{
			memset(this, 0 , sizeof(GameStation_Base));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//游戏开始数据包
	struct		GameBegin_Data
	{
		bool	bStart;
		GameBegin_Data()
		{
			memset(this, 0 , sizeof(GameBegin_Data));
		}
	};
	/*-------------------------------------------------------------------------------------*/
	//购买子弹
	struct		CMD_C_BuyCoin
	{
		bool			bAddOrMove;		//上或者是下

		CMD_C_BuyCoin()
		{
			memset(this, 0, sizeof(CMD_C_BuyCoin));
		}
	};
	/*-------------------------------------------------------------------------------------*/
	//购买子弹数目
	struct		CMD_S_BulletCount
	{
		BYTE			byDeskStation;		//上下分座位号
		int				iUserScore;			//玩家上下总分数
		int				iAllCellScore;		//玩家下注数
		LLONG			i64UserMoney;		//玩家身上的金币
		CMD_S_BulletCount()
		{
			memset(this, 0, sizeof(CMD_S_BulletCount));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//上注分数
	struct	CMD_S_AddScore
	{
		//当前上注
		BYTE		byDeskStation;
		bool		bUp;
		int			iAddScore;	//玩家加注数
		int			iUserScore;	//玩家的上分数量
		CMD_S_AddScore()
		{
			memset(this,0,sizeof(CMD_S_AddScore));
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//开始滚动
	struct	CMD_C_StartRoll
	{
		//当前上注
		BYTE		byDeskStation;
		CMD_C_StartRoll()
		{
			memset(this,-1,sizeof(CMD_C_StartRoll));

		}
	};
	/*-------------------------------------------------------------------------------*/
	//滚动信息
	struct CMD_S_TypeScroll
	{
		//当前上分
		int                            iUserScore;
		//输赢分数
		int                            iWinScore;
		//图形数据
		int                            iTypeImgid[15];

		CMD_S_TypeScroll()
		{
			memset(this,0,sizeof(CMD_S_TypeScroll));
		}
		//
	};
	/*-------------------------------------------------------------------------------*/
	//小马力游戏
	struct CMD_S_MaLi
	{
		//是否马上开始小玛利
		bool                           m_NowBegin;
		//马力次数
		int                            m_GameCount;
		//4个图
		int                            m_ThreeTypeId[4];
		//转转图
		int                            m_TypeIndex;
		//当前分
		int                            m_AddScore;

		CMD_S_MaLi()
		{
			memset(this,0,sizeof(CMD_S_MaLi));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//小马力游戏
	struct CMD_C_GetMaLi
	{
		BYTE		byDeskStation;

		CMD_C_GetMaLi()
		{
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//获取比倍
	struct CMD_C_GetBiBei
	{
		BYTE		byDeskStation;

		CMD_C_GetBiBei()
		{
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//比倍数据
	struct CMD_S_BiBeiData
	{
		//比倍数据
		int                            iBiBeiData[10];
		CMD_S_BiBeiData()
		{
			memset(this,0,sizeof(CMD_S_BiBeiData));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//获取得分
	struct CMD_C_GetScore
	{
		BYTE			byDeskStation;
		int             iGetType;

		CMD_C_GetScore()
		{
			memset(this,0,sizeof(CMD_C_GetScore));	
		}
	};
	/*-------------------------------------------------------------------------------*/
	//赢取分数
	struct CMD_S_WinScore
	{
		//当前赢分
		int                            iWinScore;
		//获得方式
		int                            iGameState;
		CMD_S_WinScore()
		{
			memset(this,0,sizeof(CMD_S_WinScore));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//玩家比倍
	struct CMD_C_BiBeiStart
	{
		//买大买小买和,0,大,1,小,2,和
		int		iMaxOrOther;
		//比倍方式,0,比倍,1,半比倍,2,全比倍
		int		iBiBeiType;
		CMD_C_BiBeiStart()
		{
			memset(this,0,sizeof(CMD_C_BiBeiStart));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//比倍结果
	struct CMD_S_BiBeiRelult
	{
		//骰子一
		int			iS1;
		//骰子二
		int			iS2;
		//图形数据
		int			iWinScore;
		CMD_S_BiBeiRelult()
		{
			memset(this,0,sizeof(CMD_S_BiBeiRelult));
		}
	};

#pragma pack()
}
	//-------------------------------------------------------------------------------

	/********************************************************************************/
#endif
