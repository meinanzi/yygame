#include "TRACTORGameResult.h"
#include "cocostudio/CocoStudio.h"
using namespace ui;
namespace TRACTOR
{
static const  char*    LAYER_CSBFILE	=			"tractor/Settlement/Settlement.csb";

	GameResulte::GameResulte(const std::vector<TGameResult>& results , bool isWin_me):_results(results),_isWin_ME(isWin_me),_timeNumber(0)
	{
	}

	GameResulte::~GameResulte()
	{
	}

	GameResulte* GameResulte::create(const std::vector<TGameResult>& results , bool isWin_me)
	{
		GameResulte* pRet = new GameResulte(results , isWin_me);
		if (nullptr != pRet && pRet->init())
		{
			pRet->autorelease();
			pRet->ignoreAnchorPointForPosition(false);
			return pRet;
		}
		return nullptr;
	}

	bool GameResulte::init()
	{
		if (!HNLayer::init())
		{
			return false;
		}
		_timeNumber = 10;

		auto touchListen = EventListenerTouchOneByOne::create();
		touchListen->onTouchBegan = CC_CALLBACK_2(GameResulte::onTouchBegan , this);
		touchListen->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListen , this);

		auto widgetNode = CSLoader::createNode(LAYER_CSBFILE);
		_widget = (Widget*)widgetNode->getChildByName("Panel_19");
		//_widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(LAYER_JSONFILE);
		_widget->setScale(0.5f);
		auto widgetSize = _widget->getContentSize();
		this->setContentSize(widgetSize);
		_widget->setAnchorPoint(Vec2(0.5f , 0.5f));
		_widget->setPosition(Vec2(widgetSize.width / 2 , widgetSize.height / 2));
		this->addChild(widgetNode);

		//显示提示输或者赢
		if (_isWin_ME)
		{
			auto win = dynamic_cast<ImageView*>(Helper::seekWidgetByName(_widget , "win"));
			win->setVisible(true);
		}
		else
		{
			auto win = dynamic_cast<ImageView*>(Helper::seekWidgetByName(_widget , "lose"));
			win->setVisible(true);
		}
		//倒计时
		_timer = dynamic_cast<TextAtlas*>(Helper::seekWidgetByName(_widget , "time"));

		std::string type[8] = 
		{
			GBKToUtf8(""),GBKToUtf8("特殊牌型"),
			GBKToUtf8("单牌"),GBKToUtf8("对子"),
			GBKToUtf8("同花"),GBKToUtf8("顺子"),
			GBKToUtf8("同花顺"),GBKToUtf8("三条")
		};
		_viewItem = dynamic_cast<ImageView*>(Helper::seekWidgetByName(_widget , "item"));
		_listView = dynamic_cast<ListView*>(Helper::seekWidgetByName(_widget , "listView"));
		_listView->setItemModel(_viewItem);
		char str[50] = {0};
		for (BYTE i = 0; i < _results.size(); i++)
		{
			_listView->pushBackDefaultItem();
			auto item = _listView->getItem(i);
			item->setVisible(true);
			auto label_name = dynamic_cast<Text*>(Helper::seekWidgetByName( item , "name"));
			label_name->setString(GBKToUtf8(_results[i].szName.c_str()));
			auto label_cardtype = dynamic_cast<Text*>(Helper::seekWidgetByName( item , "cardtype"));
			label_cardtype->setString(type[_results[i].szType]);
			auto label_money = dynamic_cast<Text*>(Helper::seekWidgetByName( item , "money"));
			sprintf(str, "%lld" , _results[i].llMoney);
			label_money->setString(str);
		}
		// 列表监听
		_listView->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameResulte::selectedItemEventCALLBACK, this)));
		this->schedule(schedule_selector(GameResulte::updateTimer) , 1.0f);
		return true;
	}

	void GameResulte::updateTimer(float dt)
	{
		_timeNumber-- ;
		if (_timeNumber < 0)
		{
			this->unschedule(schedule_selector(GameResulte::updateTimer));
			this->setVisible(false);
			this->removeFromParent();
			return ;
		}
		char str[20];
		sprintf(str, "%d" , _timeNumber);
		_timer->setString(str);
	}

	bool GameResulte::onTouchBegan(Touch *touch, Event *unused_event)
	{
		auto touchPos = touch->getLocation();
		auto rt = _widget->getBoundingBox();
		if (rt.containsPoint(touchPos))
		{
			return true;
		}
		this->setVisible(false);
		this->removeFromParent();
		return true;
	}

	void GameResulte::setGameResulteData(const std::vector<TGameResult>& results)
	{
		_results = results;
	}

	//列表点击回调
	void GameResulte::selectedItemEventCALLBACK(cocos2d::Ref *pSender, ListView::EventType type)
	{

	}

	void GameResulte::onExit()
	{
		this->unschedule(schedule_selector(GameResulte::updateTimer));
		HNLayer::onExit();
	}
}