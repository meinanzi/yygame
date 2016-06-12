#ifndef __HN_WaterMargin_GameTableUI_H__
#define __HN_WaterMargin_GameTableUI_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ui/UICheckBox.h"
#include <vector>
#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameTableUICallback.h"
#include "WaterMarginMessageHead.h"
#include "HNUIExport.h"
#include "WaterMarginGameAnimation.h"


#define Tag_UP				25
#define Tag_DOWN			26
#define Tag_AUTO			27
#define Tag_WIN				32
#define Tag_ADD				28
#define Tag_BIBEI			31
#define Tag_START			29
#define Tag_ALLSTOP			30
#define Tag_HANDLE			33
#define Tag_QUIT		   195

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

namespace WaterMargin
{
	class GameRule;
	class GameNotice;
	class GameTableLogic;
	class RollImage;
	class GameBiBei;
	class GameMaLi;
	class BDBtn;

	class GameTableUI : public HNGameUIBase, public GameTableUICallback
	{
	public:
		GameTableUI();
		virtual ~GameTableUI();

		static GameTableUI* create(BYTE bDeskIndex, bool bAutoCreate);

		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
	
		virtual void leaveDesk() override;

		virtual void setMoney(LLONG money) override;

		virtual void setDataText(int value, int num) override;

		virtual void setImageData(int* str) override;

		virtual void setBtnVisible(int Tag, bool visible) override;

		virtual void setWinScore(int value) override;

		virtual void getWinScore() override;
		

		virtual void setSaiZi(int s1, int s2) override;

		virtual void playSecondAnimate(int value, int dianShu) override;

		virtual void handleSecondDeFeng() override;

		virtual void BiBeiHistory(int* history) override;

		virtual void BiBeiAgain() override;

		virtual void MaLiResult(bool begin, int MaLicount, int Index, int* str) override;

		virtual void viewWinGold() override;

		void hideWinGold();

		virtual void MaLiEndBB() override;

		virtual void MaLIEndDF() override;

		virtual void JudgeMaLi() override;

		virtual void JudgeConnect() override;

		void MaLiGetScore(float delay);

		void secondResult();

		void secondDeFeng(float delay);

		void EnterMaLi();

		void createMaLi();

		void initData();

		void createImage();						//创建两张滚动图

		void startRoll();						//开始滚动
		void stopRoll();						//停止滚动
		
		void Rolling(float delay);				//滚动时间调度
		void queueStop(float delay);			//停止时间调度
		void JudgeRule(float delay);						//判断是否中奖
		void useRule();							//调用GameRule接口
		void lineToImageFunction();				//几线几号对应几号模块
		void playFirstAnimate(int value);
		void playSecondAnimate();
		void changeLayer();						//层切换
		void FirstResult();						//第一层的结算
		
		void changeScore(float delay);
		void setWinData();

		void BibeiChangeDefeng(float delay);	//比倍、得分切换
		void changeOver();
		void changeBegin();
		
		void SetLayer();						//roll->bibei
		
		void btnRule();							//按钮需要符合的一些规则

		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//按钮回调函数

		void headCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

		void loading();
		void addLoadding(float delay);
		void setNotic(float delay);

		void loadingCallback(Texture2D* textureData, std::string plist);
		void loadingAni();
		void gameINIT();

		void asyncLoading();
		void loadSrcUpdate(float delta);
		void StartBiBei();

		//设置自动手动按钮的状态
		void controlBtnVisible(bool visible);

		void loadMaLiBtnPanel();
		void showMaLiBDbtn();
		void maLiClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

	public:
		GameTableLogic*		_tableLogic;									

		GameRule*			_gameRule;
		vector<TextAtlas*>	_dataText;						//五个显示数据
		ImageView*			_tableBG;						//第一层背景
		Widget*				_btnWidget;
		vector<Button*>		_btn;							//9个按钮
		vector<RollImage*>	_rollImage;						//第一张滚动图
		vector<RollImage*>	_readyImage;					//第二张滚动图
		vector<RollImage*>	_blackImage;					//黑白图片
		vector<ImageView*>	_Frame;							//执行第一动画时的框架
		int					_imageType[15];					//要显示的图片结果数据
		int					_lineToImage[9][5];				//几线几号对应几号模块
		int					_line;							//第几线-1
		vector<int>			_playAnimate;					//要播放第二段动画的位置
		int					_winScore;						//结果
		bool				_stop;							//点击全停数值变true


		vector<Sprite*>		_history;						//比倍结果历史
		ImageView*			_secondBG;
		GameBiBei*			_BiBei;
		GameMaLi*			_MaLi;
		CMD_S_MaLi			_MaLiData;
		//BDBtn*				_bdBtn;
		LLONG				_userMoney;
		ImageView*			_menuBg;
		LoadingBar*			_loading;
		ImageView*			_notic;
		cocos2d::Node*		_loadingLayer;

		BYTE				_desk;
		bool				_bool;

		int					_addResources;
		Sprite*				_leftDoor;
		Sprite*				_rightDoor;
		int					_LayerIndex;
		int					_scoreChange;					//赢的金币的十分之一
		bool				_isReply;						//是否回复滚动结果
		int					_connetIndex;					//15秒断网退出

		bool                _isFirst;//判断是否是第一次转动，设置自动手动按钮时用到
		Layout*		_btnPanel;

		GameAnimation* _gameAni;
	};
}


#endif // __HN_WaterMargin_GameTableUI_H__
