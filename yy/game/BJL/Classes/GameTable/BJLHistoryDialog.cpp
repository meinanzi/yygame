#include "BJLHistoryDialog.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
using namespace ui;

namespace BJL
{

	static const int DISTANCE_HEIGHT		= 	105;
	static const int DISTANCE_WIDTH			= 	68;
	static const int ZORDER_MAX				= 	201;

	GameHistoryDialog::~GameHistoryDialog()
	{

	}

	GameHistoryDialog::GameHistoryDialog() : _bg(nullptr), _startPoint(Vec2(113, 255)), _currentIndex(0)
	{

	}

	GameHistoryDialog* GameHistoryDialog::create(cocos2d::Node* parent, const std::vector<LuziData>& history)
	{
		GameHistoryDialog* ptr = new GameHistoryDialog();
		if (nullptr != ptr && ptr->init(parent,history))
		{
			ptr->ignoreAnchorPointForPosition(false);
			ptr->autorelease();
			return ptr;
		}
		CC_SAFE_DELETE(ptr);
		return nullptr;
	}

	bool GameHistoryDialog::init(cocos2d::Node* parent, const std::vector<LuziData>& history)
	{
		if (!HNDialogBase::init(parent))
		{
			return false;
		}
		auto distpatch = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameHistoryDialog::onTouchBegan,this);
		listener->setSwallowTouches(true);
		distpatch->addEventListenerWithSceneGraphPriority(listener,this);

		_history = history;

		if (_history.size() > BJL_MAX_NUM_VIEW)
		{
			_currentIndex = _history.size() - BJL_MAX_NUM_VIEW;
		}
		auto size = this->getContentSize();

		auto loaderbg = CSLoader::createNode("Games/BJL/game/BJLStudio/luzi.csb");
		_widget = (Layout* ) loaderbg->getChildByName("Panel_2");
		loaderbg->setScale(1.5f);
		loaderbg->setAnchorPoint(Vec2(0.5f,0.5f));
		loaderbg->setPosition(Vec2(size.width/2 , size.height/2));
		_sv = (ScrollView* ) _widget->getChildByName("scrollview");
		_sv->addEventListener(CC_CALLBACK_2(GameHistoryDialog::luziscrollview_scroll,this));
		_bg = (ImageView* ) _widget->getChildByName("bg");
		this->addChild(loaderbg,1000);
		return initUI();
	}

	bool GameHistoryDialog::initUI()
	{
		reset_pos();
		return true;
	}

	bool GameHistoryDialog::onTouchBegan(Touch *touch, Event *unused_event)
	{
		Vec2 point = this->convertToNodeSpace(touch->getLocation());
		if (!_bg->getBoundingBox().containsPoint(point))
		{
			hide();
		}
		return true;
	}

	void GameHistoryDialog::luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type)
	{
		if (type == ui::ScrollView::EventType::SCROLL_TO_RIGHT)
		{
			move_one_right();
		}
		else if (type == ui::ScrollView::EventType::SCROLL_TO_LEFT)
		{
			move_one_left();
		}
	}

	void GameHistoryDialog::move_one_left()
	{
		if (_currentIndex<1) return;
		_currentIndex --;
		reset_pos();
	}

	void GameHistoryDialog::move_one_right()
	{
		if ( _history.size() < BJL_MAX_NUM_VIEW || _currentIndex >= _history.size() - BJL_MAX_NUM_VIEW)
		{
			return;
		}
		_currentIndex ++;
		reset_pos();
	}

	//重置庄闲平的输赢
	void GameHistoryDialog::reset_pos()
	{
		BYTE k = 0;
		while (!spArr.empty())
		{
			spArr.back()->removeFromParent();
			spArr.popBack();
		}
		for (BYTE i = 0 ; i < BJL_MAX_NUM_AREA ; i++)
		{
			k = 0 ;
			for (BYTE j = _currentIndex ; j < _history.size() && j < _currentIndex + BJL_MAX_NUM_VIEW ; j++ )
			{
				//为了让平与庄位置对换
				BYTE bPosition = 0;
				if (1 == i)
				{
					bPosition = 2;
				}
				else if (2==i)
				{
					bPosition = 1;
				}
	
				if (i == _history[j].byWinResult)   // 闲 平 庄    闲 庄 平
				{
					auto win = Sprite::create("BJL/game/luzi/game_history_win.png");
					win->setAnchorPoint(Vec2(0.5f,0.5f));
					win->setPosition(Vec2(_startPoint.x + k*DISTANCE_WIDTH , _startPoint.y - bPosition*DISTANCE_HEIGHT));
					_bg->addChild(win);
					spArr.pushBack(win);
				}
				else
				{
					auto lose = Sprite::create("BJL/game/luzi/game_history_lose.png");
					lose->setAnchorPoint(Vec2(0.5f,0.5f));
					lose->setPosition(Vec2(_startPoint.x + k*DISTANCE_WIDTH , _startPoint.y - bPosition*DISTANCE_HEIGHT));
					_bg->addChild(lose);
					spArr.pushBack(lose);
				}
				k++;
			}

		}
	}


}
