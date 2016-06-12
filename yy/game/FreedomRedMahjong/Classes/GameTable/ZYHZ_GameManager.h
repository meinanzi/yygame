#ifndef _ZYHZ_GAMEMANAGER_H_
#define _ZYHZ_GAMEMANAGER_H_

#include "ZYHZ_CardPoolUser.h"
#include "ZYHZ_ResourceLoader.h"
#include "ZYHZ_GameTableLogic.h"
#include "ZYHZ_MessageHead.h"
#include "ZYHZ_Factory.h"
#include "ZYHZ_MahjongCardPoolUser.h"

namespace ZYHZ
{
	class GameManager :
		public cocos2d::Node
	{
	public:
		GameManager(void);
		~GameManager(void);

		// 初始化类
	public:
		virtual bool init() override;		
		CREATE_FUNC(GameManager);
		virtual void loadResource();						// 加载资源

	private:
		virtual void initData();                               // 初始化数据
		void restartSetData();
		virtual void addLoader();							// 添加资源管理者
		virtual void setText();									// 设置文本
		virtual void setButton();								// 设置按钮
		virtual void addUI();									// 配置UI层
		void reParent(std::string name);				// 重置cocos_node节点父节点
		void resetDiKuang();									// 重置底框按钮和状态

		// cocos 按钮回调函数
		void startButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void trusteeshepButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void quxiaotuoguangButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void returnButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queWanButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queTongButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queTiaoButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void chiButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void pengButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void gangButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void tingButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void huButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void qiButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void confirmButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void cancelButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void continueButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void leaveButtonClickCallBack(Ref* ref,Widget::TouchEventType type);

		// 按纽或计时器调用实现
	private:
		void endQueMen(INT queColor = 0, bool isRand = false);												// 结束定缺,按钮或者计时器调用
		void endAllTimer();																										// 终止所有计时器
		int getRd_A_N(int a, int n);

		// 计时器
	public:
		void startTimer(const sitDir& timerPointDir, timerType timerType);
	private:
		void waitTimer(float dt);																							    // 等候开始
		void queMenTimer(float dt);																							// 等候定缺
		void waitOutTimer(float dt);																							// 等候玩家出牌
		void waitThinkTimer(float dt);																						// 等候其他玩家动作决定
		void waitActionTimer(float dt);																						// 等候自己是否做动作
		void playBgTimer(float dt);																							// 播放背景音乐
		void setRemainTime(const float& time) { _remainTime = time; }									// 设置剩余时间
		
		// 计时器相关数据
	private:
		LabelAtlas* _pAtlasTimerNumber;																					// 计时器数字表示
		float _remainTime;																										// 当前倒计时剩余时间
		bool _autoBegin;																										   // 是否自动开始
		unsigned int _clockID;																								   // 倒计时音效ID

		// 自动返回大厅当卡号时
	public:
		void setConnectSuccess() { _isConnectSuccess = true; }
	private:
		void autoReturnDeskWhenLag();
		void checkConnectSuccess(float dt);
		void willReturnTimer(float dt);
		void returnDesk(bool safeLeave = true);
		void trusteeshepSendMsg(bool trusteeshep = true);	// 托管
		bool _isConnectSuccess;

		// 单例指针
	public:
		static GameManager* getInstance();
	private:
		static GameManager* _instance;
		
		// 资源指针
	public:
		cocosResourceLoader* getCR() { return _cLoader; }
		spriteResourceLoader* getSR() { return _sLoader; }
		plistResourceLoader* getPR() { return _pLoader; }

	private:
		cocosResourceLoader* _cLoader;																					// cocostudio资源
		spriteResourceLoader* _sLoader;																					// sprite资源 
		plistResourceLoader* _pLoader;																					// plist资源

	private:

		// 流程控制
	public:
		virtual void addUser(const sitDir& dir, const UserInfoStruct& user);																						//  增添玩家
		virtual void userLeave(const sitDir& dir);																																//  玩家离开
		virtual void userTrusteeshep(const sitDir& dir, bool isTrust = true);																														//  玩家托管
		virtual void userAgree(const sitDir& dir);																																// 玩家准备
		virtual void startGame(const bool& autoBegin = true);																											// 4人准备完毕时
		virtual void playGame();																																						// 开始打牌
		const vector<CardPoolUser *>& getVecUser();
		virtual void catchCard(const sitDir& dir, const INT& number, const bool& head = true);														// 抓牌
		virtual void outCard(const sitDir& dir, const INT& number);																									// 出牌
		void passAction();																																									// 过牌
		void showMoney();
		void showMoneyCallBack();
		//  '重连时用
		void reconnected(																																									// 断线重连
			std::vector<std::vector<INT>>& vvHandCard,								
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir);																																									// end 断线重连
		void hideAllBottomCard(bool hide = true);																															// 隐藏所有底牌
		void showPatternBottomCard(std::vector<std::vector<INT>>& vvec);																					// 显示部分底牌
		void showHandCard(std::vector<std::vector<INT>>& vvec);																									// 显示手牌
		void showActionCard(std::vector<std::vector<std::vector<INT>>>& vvvec);																			// 显示动作牌
		void showOutCard(std::vector<std::vector<INT>>& vvec);																									// 显示已出牌
		void touziAction(const sitDir& startDir, const int& num1, const int& num2);																			// 骰子动画
		Animation* getAni(std::string name);																																		// 取帧动画
		void setNt(const sitDir& dir);																																					// 设置庄家
		void setCatchDir(const sitDir& dir);																																        // 设置抓牌方向
		void setCatchPos(const INT& sendPos);																																// 设置发牌点
		void finishGame(const std::vector<std::vector<int>>& vvNum);																							// 结束游戏
		void restartGame();																																								// 重启游戏
		void initAllHandCard(std::vector<std::vector<INT>> vvSouthCard);																						// 启动发牌入口
		Card* commonCatchCard(const sitDir& dir, const INT& mahjongNumber);																			// 正前抓单张牌
		Card* tailCatchCard(const sitDir& dir, const INT& mahjongNumber);																					//  开杠尾部抓牌
		void addCardPool(CardPool * pool);																																		// 增加方位卡片池
		std::vector<CardPool *>& getUserCardPool();																														// 获取卡片池
		
		// 胡牌时用
		void hasHu(const sitDir& dir, bool isZiMo = false, bool isQiangGang = false);																		// 胡牌时用
		void setLastActionCard(const sitDir& dir, const INT& number);																								// 设置最后动作牌
		void setHasHu(const sitDir& dir, const bool& bHasHu);																										// 设置玩家胡牌
		void showHuTmpCard(const sitDir& huDir, std::vector<int> vecHuHandCard);
		void showShowCardToFinishUI(const int& huCount, std::vector<std::vector<int>> vvecCardNum);

		// 取牌
	public:
		Card* getCard(const sitDir& dir, const INT& mahjongNumber);																							// 取正立牌
		Card* getZhengDaoCard(const INT& mahjongNumber);																										// 正倒
		Card* getXiaoDaoCard(const sitDir& dir, const INT& mahjongNumber);																				// 小倒

		// 立牌
	private:
		Card* getZhengLiCard(const INT& mahjongNumber);																											// 正立牌
		Card* getBeiLiCard();																																								// 背立牌
		Card* getZuoLiCard();																																							// 左立牌
		Card* getYouLiCard();																																							// 右立牌
		
		// 发牌控制
	public:
		void autoSendCard(const sitDir& dir);																										// 自动出牌
		cocos2d::Vec2 getOutCardDeskPos(const sitDir& dir);																				// 出牌桌面摆放位置
		void afterOutCard(const sitDir& dir);																											// 出牌后
		void startOutCard(const sitDir& dir);																											// 庄家第一次开始出牌
		void setMinusOutCount(const sitDir& dir);																								// 减一次打出的牌数目
		Card*& getLastestOutCard();																														// 得到最新出的牌
		bool isNoCardCanCatch();																															// 是否没牌抓了
		void checkHandCardData(const sitDir& outDir);																														// 校验手牌收据
		void setHandCardFormServer(const std::vector<std::vector<int>>& vvNum);											// 同步服务端手牌数据
		void showNiao(const std::vector<INT>& vCardNumber);																			// 显示中鸟
		void hideNiao();

	private:
		void sendCard(const sitDir& dir/*起始方位*/, const INT& heapCount /*起始堆数*/);									// 开局发牌
		void sendCardToPool(const INT& cardCount);																							// 发送底牌给方向池, 计时器调用
		void sendCardTimer(float dt);																													// 发牌计时器
		
		// 基础数据
	public:
		void setQueMen(INT queMen);                                                // 设置缺门
		INT& getQueMen();																   // 获取缺门
		void setPlayGame(bool bs = true);										   // 设置是否正在打牌
		const bool& isGamePlaying();												   // 获取是否正在打牌
		void setKongType(INT type);													   // 设置杠牌类型
		void setConcealedNumber(INT number);								   // 设置暗杠牌值
		tagHuPaiEx& getHuMsg();													   // 获取胡牌消息
		sitDir& getStartDir();																   // 取庄家值
		void setSendStatus(bool hasSendCard, bool startSendCard);   // 设置发牌状态
		void resetCatchPos(bool addApos = false);
		
	private:
		std::vector<CardPoolUser *> _vecUser;
		bool _isPlayGame;																		// 游戏状态是否正在4人打牌
		bool _isTrusteeShep;																	// 游戏状态是否托管
		bool _isSouthPlayOper;																// 是否轮到玩家操作
		INT _queMen;																			// 当前玩家缺门
		sitDir _startDir;																			// 庄家位置
		INT _sendPos;																			// 发牌点
		INT _kongType;																			// 杠牌类型 明、暗、补
		INT _concealedNumber;															// 暗杠牌
		tagHuPaiEx _huMsg;																	// 胡牌消息
		std::vector<Card *> _vecTmpHuCard;										// 胡临时牌
		std::vector<Card *> _vecHuCardShow;										// 胡临时牌
		std::vector<Card *> _vecHuFinishUiCard;

		std::vector<std::vector<Card *>> _vvLastShow;							// 最后显示所有的牌
		std::vector<Card *> _vecNiao;											        // 中鸟牌
		bool _hasSendAllCard;																// 是否发完了牌
		bool _bStartSendCard;																// 开始发牌阶段

	private:
		Card * _lastestOutCard;																								// 最后打出的牌
		std::vector<std::vector<int>> _vvNum;																		// 服务端传来的数据
		std::vector<Card *> _vecBottomCard;																		// 底牌容器
		std::vector<std::vector<std::vector<Card *>>> _vvvZhengLiCard;								// 正立牌容器
		std::vector<std::vector<std::vector<Card *>>> _vvvZhengDaoCard;							// 正倒牌容器
		std::vector<Card *> _vecBeiLiCard;																			//  背立牌容器
		std::vector<Card *> _vecZuoLiCard;																			// 左立牌容器
		std::vector<Card *> _vecYouLiCard;																			// 右立牌容器
		std::vector<std::vector<std::vector<std::vector<Card *>>>> _vvvvXiaoDaoCard;		// 倒牌容器
		std::vector<std::vector<Card *>> _vvecHuCard;														// 胡牌容器
		INT northSouthCount;																								// 南北牌堆数
		INT westEastCount;																									// 东西牌堆数
		INT _currCardIndex;																									// 当前抓牌位置
		INT _lastCardIndex;																									// 尾牌位置
		INT _sendCardCount;																								// 发牌的次数
		std::vector<cocos2d::Vec2> _vecCatchEndPos;															// 抓牌的安置位置容器
		std::vector<CardPool *> _vecCardPool;																		// 4方卡片池
		std::vector<cocos2d::Vec2> _vecCatchPos;																// 4方抓牌安置点
		sitDir _startSendDir;																									// 牌桌发牌开始位置
		sitDir _startCatchDir;																									// 开始抓牌方向
		std::vector<std::vector<INT>> _vvStartHandCard;														// 起始手牌
		std::vector<INT> _vecCurrSendIndex;																		// 正在发的牌最新的位置 4个方向
		std::vector<cocos2d::Rect> _vecOutCardRect;															// 出牌摆放rect
		std::vector<INT> _vecHasOutCardCount;																	// 已经出牌的数目
		INT _zorderWest;																										// 西牌zoder
		INT _zorderEast;																										// 东牌zoder
		INT _zorderSouth;																										// 南牌zoder
		INT _zorderNorth;																										// 北牌zoder
		INT _lineCount;																											// 每一行摆麻将
		INT _bigShowZorder;
		INT _niaoGlobalZorder;

		cocos2d::Size _mahjongSzEW;
		cocos2d::Size _mahjongSzNS;

		// 花牌
		std::vector<std::vector<Card *>> _vvColorZhengDa;													// 正立花牌
		std::vector<std::vector<Card *>> _vvColorZhengDao;												// 正倒花牌
		std::vector<std::vector<std::vector<Card *>>> _vvvColorXiaoDao;							// 小倒花牌

		// 多分辨率适应
	public:
		void setGlobalScale();																// 设置全局缩放
		const float& getGlobalScale();													// 获取全局缩放
		const Vec2& getNewOrigin();													// 获取全局原点

	private:
		float _globalScale;																		// 全局缩放
		Vec2 _newOrigin;																		// 全局原点坐标

		// 动作牌
	public:
		PoolAction* _touchCard;															// 碰牌
		PoolAction* _meldedKong;														// 明杠
		PoolAction* _concealedKong;													// 暗杠
		PoolAction* _touchKong;															// 补杠
		PoolAction* _huCard;																// 胡牌

		// 音效相关
		//void playEffect(int a, int n, std::string head, int index);				// 播放音效
		void playSexNumberSound(INT speakStartion, INT cardNumber);
		void playSexActionSound(INT speakStartion, std::string actionName);
		void playCommonSound(std::string soundName);

		// 玩家信息
		bool isMan(int index);																// 是男人还是女人

		// 临时资源管理, 每一局游戏结束清理
	public:
		Card* &getBigShowSp();															// 获取提示打出的牌
		void addBigShowSp(const sitDir& dir, const INT& number);		// 添加提示打出的牌

	private:
		Card* _bigShowCard;																// 提示打出的牌

	public:  // 红中出牌抓牌数据
		void block(bool canPeng, bool canGang, bool canHu);
		void setCurrOperNumber(const INT& number);
		const INT& getCurrOperNumber();
		void setCurrOperDir(const sitDir& dir);
		const sitDir& getCurrOperDir();
		void setCurrInfo(const sitDir& dir, const INT& number);

	private:
		INT _currOperNumber;
		sitDir _currOperDir;
	};

}

#endif // _GAMEMANAGER_H_