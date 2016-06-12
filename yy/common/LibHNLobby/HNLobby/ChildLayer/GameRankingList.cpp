#include "HNLobby/ChildLayer/GameRankingList.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/GamePrompt.h"
#include "HNUIExport.h"
#include <string>

static const INT RANK					= 10;
static const INT NICKNAME				= 11;
static const INT MONEY					= 12;

static const char* RANKING_BG			= "platform/store/second_payBg.png";                           //backGround
static const char* RANKING_TITLE	    = "platform/ranking/rankingTitle.png";	
static const char* CLOSE_N				= "platform/ranking/second_back.png";   
static const char* LIST_BG				= "platform/ranking/rankingItem.png";                           //rankingListBG
static const char* SELF_FRAME	        = "platform/store/common_opaque.png";           //frame
static const char* RANK_FRAME	        = "platform/ranking/subViewFrame.png";
static const char* RANK_MYFRAME	        = "platform/ranking/myFrame.png";
static const char* LEAF_LEFT	        = "platform/subViewBambooLeft.png";               
static const char* LEAF_RIGHT	        = "platform/subViewBambooRight.png";  
static const char* RANK_ITEM	        = "platform/ranking/Item.png";
static const char* USER_MEN_HEAD		= "platform/head/men_head.png";					
static const char* USER_WOMEN_HEAD		= "platform/head/women_head.png";
static const char* MY_HEAD_FRAME		= "platform/ranking/headframe.png";



GameRankingList::GameRankingList()
{	
}

GameRankingList::~GameRankingList()
{
	PlatformLogic()->removeEventSelector(MDM_GP_PAIHANGBANG, 0);
}

void GameRankingList::openRank(Node* parent, Vec2 vec, int zorder, int tag)
{
	CCAssert(nullptr != parent, "parent is nullptr");

	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(vec);
	parent->addChild(this, zorder, tag);
	parent->runAction(MoveBy::create(0.3f, Vec2(-winSize.width, 0)));
}

void GameRankingList::closeRanking()
{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	auto winSize = Director::getInstance()->getWinSize();
	this->getParent()->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(winSize.width, 0)),
		CCCallFunc::create([&]()
	{
		this->removeFromParent();
	}), nullptr));
}

bool GameRankingList::init()
{
    if ( !HNLayer::init()) return false;

	Size winSize = Director::getInstance()->getWinSize();

	MyRankNum = -1;
	//排行榜背景
	setBackGroundImage(RANKING_BG);

	//排行榜标题
	auto titleBtn = Button::create(RANKING_TITLE,RANKING_TITLE);
	titleBtn->setAnchorPoint(Point(0.5, 1));
	titleBtn->setPosition(Vec2(winSize.width / 2, winSize.height-20));
	auto titleSize = titleBtn->getContentSize();
	titleBtn->setScale((0.8f*1280.0f/winSize.width)/(720/winSize.height),(0.8f*1280.0f/winSize.width)/(720/winSize.height));
	this->addChild(titleBtn, 2);

	//竹叶
	auto leftLeaf = Sprite::create(LEAF_LEFT);
	leftLeaf->setAnchorPoint(Point(0, 1));
	leftLeaf->setPosition(0, winSize.height);
	addChild(leftLeaf, 5);
	auto rightleaf = Sprite::create(LEAF_RIGHT);
	rightleaf->setAnchorPoint(Point(1, 1));
	rightleaf->setPosition(winSize.width, winSize.height);
	addChild(rightleaf, 5);

	auto MyListener = EventListenerTouchOneByOne::create();
	//阻止触摸向下传递
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<GameRankingList*>(event->getCurrentTarget());//获取的当前触摸的目标       
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		return rect.containsPoint(locationInNode);//判断触摸点是否在目标的范围内

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

	// 关闭按钮
	auto closeBtn = Button::create(CLOSE_N , CLOSE_N);
	closeBtn->setPosition(Vec2(winSize.width * 0.05f, winSize.height * 0.94f));
	addChild(closeBtn,5);
	closeBtn->addTouchEventListener(CC_CALLBACK_2(GameRankingList::closeEventCallBack,this));

	//排行榜背景底色
	auto rankingbg = ui::Scale9Sprite::create(SELF_FRAME);
	rankingbg->setAnchorPoint(Point(0.5, 0));
	rankingbg->setPreferredSize(Size(1300 ,( 620.0f*(1280.0f / winSize.width))/(720.0f/winSize.height)));
	rankingbg->setPosition(Vec2(winSize.width / 2,0));
	addChild(rankingbg,2);

	//排行榜的背景框
	rankingFrame = ui::Scale9Sprite::create(RANK_FRAME);
	rankingFrame->setAnchorPoint(Point(0.5, 0.5));
	rankingFrame->setPreferredSize(Size(1180 ,( 500.0f* (1280.0f / winSize.width))/(720.0f/winSize.height)));
	rankingFrame->setPosition(Size(rankingbg->getContentSize().width/2, winSize.height / 2-5));
	auto rankWidth = rankingFrame->getContentSize().width;
	auto rankHeight = rankingFrame->getContentSize().height;
	rankingbg->addChild(rankingFrame, 5);

	//排行榜目录
	auto rankImage = ui::Scale9Sprite::create(RANK_ITEM);
	rankImage->setPreferredSize(Size(1180, 70));
	rankImage->setAnchorPoint(Point(0.5, 1));
	rankImage->setPosition(Vec2(rankWidth / 2,rankHeight));
	rankingFrame->addChild(rankImage, 5);

	//排行榜列表
	_tableView = TableView::create(this, Size(1180, (rankHeight-rankImage->getContentSize().height-10)));
	_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);//设置TableView方向
	_tableView->ignoreAnchorPointForPosition(false);
	_tableView->setAnchorPoint(Vec2(0.5f, 0.5f));
	_tableView->setPosition(Vec2(rankWidth/2, rankHeight/2-rankImage->getContentSize().height/2-3));
	_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);//设置TableView的属性(从上往下)
	_tableView->setDelegate(this);//设置委托
	rankingFrame->addChild(_tableView);

	getRankListData();

	return true;
}

//关闭按钮回调
void GameRankingList::closeEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	Button* selectedBtn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		selectedBtn->setColor(Color3B(170, 170, 170));
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		closeRanking();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		selectedBtn->setColor(Color3B(255, 255, 255));
		break;
	default:
		break;
	}
}

//获取排行数据
void GameRankingList::getRankListData()
{
	MSG_GP_PaiHangBang_In rank;
	rank.count = 20;
	rank.type = 0;

	PlatformLogic()->sendData(MDM_GP_PAIHANGBANG, 0, &rank, sizeof(rank), 
		HN_SOCKET_CALLBACK(GameRankingList::openRankListEventSelector, this));
}

//申请打开排行榜回调
bool GameRankingList::openRankListEventSelector(HNSocketMessage* socketMessage)
{
	if (1 == socketMessage->messageHead.bHandleCode)
	{
		CCAssert(sizeof(MSG_GP_MoneyPaiHangBang_Result) == socketMessage->objectSize, "MSG_GP_MoneyPaiHangBang_Result is error.");
		MSG_GP_MoneyPaiHangBang_Result* iRank = (MSG_GP_MoneyPaiHangBang_Result*)socketMessage->object;

		std::string name(PlatformLogic()->loginResult.nickName);
		for (int i = 0; i < 20; i++)
		{
			if (iRank->items[i].i64Money > 0)
			{
				_iList.push_back(iRank->items[i]);
			}
			if( name.compare(iRank->items[i].nickName) == 0 )
			{
				MyRankNum = i+1;
			}
		}
		showMyRank();
		_tableView->reloadData();//更新数据
	}
	else
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("查询失败!"));
	}
	return true;
}

//添加Cell
TableViewCell* GameRankingList::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		cell->autorelease();
	}

	cell->removeAllChildrenWithCleanup(true);

	if (idx >= _iList.size()) return cell;
	//排行背景图
	auto storeItemTemplate = ImageView::create(LIST_BG);
	storeItemTemplate->setAnchorPoint(Vec2::ZERO);
	storeItemTemplate->setPosition(Vec2(0, 0));
	storeItemTemplate->setName("item");

	float itemWidth = storeItemTemplate->getContentSize().width;
	float itemHeight = storeItemTemplate->getContentSize().height;

	auto iter = _iList.begin();
	MSG_GP_MoneyPaiHangBang_Item ite = *(iter + idx);
	char str[64];
	//玩家排名
	if (idx == 0)
	{
		auto first = Sprite::create("platform/ranking/Gold.png");
		first->setScale(0.8f);
		first->setPosition(Vec2(itemWidth * 0.125f, itemHeight * 0.5f));
		storeItemTemplate->addChild(first, 1);
	}
	else if (idx == 1)
	{
		auto second = Sprite::create("platform/ranking/Sliver.png");
		second->setScale(0.8f);
		second->setPosition(Vec2(itemWidth * 0.125f, itemHeight * 0.5f));
		storeItemTemplate->addChild(second, 1);
	}
	else if (idx == 2)
	{
		auto third = Sprite::create("platform/ranking/Copper.png");
		third->setScale(0.8f);
		third->setPosition(Vec2(itemWidth * 0.125f, itemHeight * 0.5f));
		storeItemTemplate->addChild(third, 1);
	}
	else
	{
		auto other = Sprite::create("platform/ranking/hua1.png");
		other->setPosition(Vec2(itemWidth * 0.125f, itemHeight * 0.5f));
		storeItemTemplate->addChild(other, 1);
		sprintf(str, "%d", idx + 1);
		rankingText = Text::create(str, "", 28);
		rankingText->setPosition(Vec2(itemWidth * 0.125f, itemHeight * 0.5f));
		storeItemTemplate->addChild(rankingText, 1, RANK);
	}

	//玩家昵称
	sprintf(str, "%s", ite.nickName);
	auto nickNameText = Text::create(GBKToUtf8(str), "", 28);
	nickNameText->setPosition(Vec2(itemWidth * 0.47f, itemHeight * 0.5f));
	storeItemTemplate->addChild(nickNameText, 1, NICKNAME);

	//玩家金币
	sprintf(str, "%lld", ite.i64Money);
	auto moneyText = Text::create(str, "", 28);
	moneyText->setPosition(Vec2(itemWidth * 0.825f, itemHeight * 0.5f));
	storeItemTemplate->addChild(moneyText , 1 , MONEY);

	cell->addChild(storeItemTemplate);
	
	if (idx<3)
	{
		nickNameText->setColor(colorGold);
		moneyText->setColor(colorGold);
	}
	return cell;

}

//设置Cell大小
Size GameRankingList::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size( 1090, 85);
} 

//设置Cell个数
ssize_t GameRankingList::numberOfCellsInTableView(TableView *table)
{
	return _iList.size();
}

//Cell触摸事件
void GameRankingList::tableCellTouched(TableView *table, TableViewCell *cell)
{
	log("cell touched at index %d", cell->getIdx());
}

void GameRankingList::showMyRank()
{
	Size winSize = Director::getInstance()->getWinSize();

	//我的排名的背景框
	auto myItemTemplate = ui::Scale9Sprite::create(RANK_MYFRAME);
	myItemTemplate->setPreferredSize(Size(1180, 80));
	myItemTemplate->setAnchorPoint(Vec2(0.5, 1));
	myItemTemplate->setPosition(Vec2(rankingFrame->getContentSize().width/2, -10));
	float myrankWidth = myItemTemplate->getContentSize().width;
	float myrankHeight = myItemTemplate->getContentSize().height;
	rankingFrame->addChild(myItemTemplate, 10);

	//我的头像
	auto myHead = Sprite::create(PlatformLogic()->loginResult.bBoy?USER_MEN_HEAD:USER_WOMEN_HEAD);
	myHead->setScale(0.7f);
	myHead->setPosition(0.06f*myrankWidth,myrankHeight/2);
	myItemTemplate->addChild(myHead, 1);
	/*auto myHeadFrame = Sprite::create(MY_HEAD_FRAME);
	myHeadFrame->setScale(0.76);
	myHeadFrame->setPosition(0.06f*myrankWidth,myrankHeight/2);
	myItemTemplate->addChild(myHeadFrame, 1);*/
	//我的排名
	auto other = Sprite::create("platform/ranking/hua1.png");
	other->setPosition(Vec2(myrankWidth * 0.128f, myrankHeight * 0.5f));
	myItemTemplate->addChild(other, 1);
	char str[64];
	if (MyRankNum > 0 && MyRankNum <= 20)
	{
		sprintf(str, "%d", MyRankNum);
		auto myrankingText = Text::create(str, "", 28);
		myrankingText->setPosition(Vec2(myrankWidth * 0.13f, myrankHeight * 0.5f));
		myItemTemplate->addChild(myrankingText, 1, RANK);
	}
	else
	{
		auto myrankingText = Text::create("20+", "", 28);
		myrankingText->setPosition(Vec2(myrankWidth * 0.13f, myrankHeight * 0.5f));
		myItemTemplate->addChild(myrankingText, 1, RANK);
	}

	std::string nickName(PlatformLogic()->loginResult.nickName);
	if (!nickName.empty())
	{
		sprintf(str, "%s",GBKToUtf8(PlatformLogic()->loginResult.nickName));
	}
	else
	{
		sprintf(str, "%s",GBKToUtf8("未知"));
	}
	auto myNameText = Text::create(str, "", 28);
	myNameText->setPosition(Vec2(myrankWidth * 0.47f, myrankHeight * 0.5f));
	myItemTemplate->addChild(myNameText, 1, NICKNAME);

	sprintf(str, "%lld", PlatformLogic()->loginResult.i64Money);
	auto mymoneyText = Text::create(str, "", 28);
	mymoneyText->setPosition(Vec2(myrankWidth * 0.825f, myrankHeight * 0.5f));
	myItemTemplate->addChild(mymoneyText , 1 , MONEY);
}
