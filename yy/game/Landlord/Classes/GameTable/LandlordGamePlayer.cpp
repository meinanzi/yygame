#include "LandlordGamePlayer.h"
#include "LandlordPokerCard.h"
#include "LandlordGameTableUI.h"
#include "LandlordGameUserMessageBox.h"
#include "LandlordGameTableUI.h"

#include "HNCommon/HNConverCode.h"
#include "HNLobbyExport.h"
#include "HNUIExport.h"

namespace landlord
{
	const char* Layout_Json_File = "landlord/game/studioUI/player.csb";
	const char* Player_Empty     = "landlord/game/studioUI/uiRes/room_default_none.png";
	const char* Player_Normal_M  = "landlord/game/studioUI/uiRes/men_head.png";
	const char* Player_Normal_W  = "landlord/game/studioUI/uiRes/women_head.png";
	const char* Player_Farmer_W  = "landlord/game/studioUI/uiRes/room_farmer_f.png";
	const char* Player_Farmer_M  = "landlord/game/studioUI/uiRes/room_farmer_m.png";
	const char* Player_Lord_W    = "landlord/game/studioUI/uiRes/room_lord_f.png";
	const char* Player_Lord_M    = "landlord/game/studioUI/uiRes/room_lord_m.png";
	const char* Player_Auto      = "landlord/game/studioUI/uiRes/user_ai_icon.png";
	
	//////////////////////////////////////////////////////////////////////////
	const char* File_Win_Num     = "landlord/game/fonts/win_number.fnt";
	const char* File_Lose_Num    = "landlord/game/fonts/lose_number.fnt";

	//////////////////////////////////////////////////////////////////////////
	PlayerUI* PlayerUI::create(LLONG userId)
	{
		PlayerUI* player = new PlayerUI();
		if(player->init(userId))
		{
			player->ignoreAnchorPointForPosition(false);
			player->autorelease();
			return player;
		}
		CC_SAFE_DELETE(player);
		return nullptr;
	}
	
	PlayerUI::PlayerUI()
		: _ivHead(nullptr)
		, _textName(nullptr)
		, _textMoney(nullptr)
		, _textCardCount(nullptr)
		, _userId(INVALID_USER_ID)
		, _money(0)
		, _cardCount(-1)
		, _tableUI(nullptr)
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(PlayerUI::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PlayerUI::onTouchEnded, this);
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	PlayerUI::~PlayerUI()
	{

	}

	bool PlayerUI::init(LLONG userId)
	{
		if(!HNLayer::init())
		{
			return false;
		}

		_userId = userId;

		auto player_Node = CSLoader::createNode(Layout_Json_File);
		this->addChild(player_Node);

		auto layer = (Layout*)player_Node->getChildByName("Panel_player");

		loadComponet(layer);

		Size size = layer->getContentSize();
		this->setContentSize(size);
		layer->setAnchorPoint(Vec2::ZERO);
		layer->setPosition(Vec2::ZERO);

		return true;
	}

	void PlayerUI::setUserId(LLONG userId)
	{
		_userId = userId;
	}

	void PlayerUI::setUserName(const std::string& name)
	{
		CCAssert(nullptr != _textName, "nullptr == _textName");
		if(nullptr == _textName) return;

		_name = name;
		if (!name.empty())
		{
			char tmp[128] = {0};
			sprintf(tmp, "êÇ³Æ:%s", name.c_str());
			_textName->setString(GBKToUtf8(tmp));
		}
		else
		{
			_textName->setString("");
		}
	}

	void PlayerUI::setUserMoney(const std::string& money)
	{
		CCAssert(nullptr != _textMoney, "nullptr == _textMoney");
		if(nullptr == _textMoney) return;

			_textMoney->setString(money);
	}

	void PlayerUI::setUserCardCount(int count)
	{
		CCAssert(nullptr != _textCardCount, "nullptr == _textCardCount");
		if(nullptr == _textCardCount) return;

		_cardCount = count;
		if (count >= 0)
		{
			char tmp[50] = {0};
			sprintf(tmp, "%dÕÅ", count);
			_textCardCount->setString(GBKToUtf8(tmp));
		}
		else
		{
			_textCardCount->setString("");
		}
	}

	void PlayerUI::setAutoHead()
	{
		CCAssert(nullptr != _ivHead, "nullptr == _ivHead");
		if(nullptr == _ivHead) return;

		_ivHead->loadTexture(Player_Auto);
	}

	void PlayerUI::setHead(bool isBoy)
	{
		CCAssert(nullptr != _ivHead, "nullptr == _ivHead");
		if(nullptr == _ivHead) return;

		std::string name = isBoy ? Player_Normal_M : Player_Normal_W;
		_ivHead->loadTexture(name);
	}

	void PlayerUI::setGameHead(bool isBoy, bool isLord)
	{
		CCAssert(nullptr != _ivHead, "nullptr == _ivHead");
		if(nullptr == _ivHead) return;

		std::string filename = isLord ? (isBoy ? Player_Lord_M : Player_Lord_W) : (isBoy ? Player_Farmer_M : Player_Farmer_W);
		_ivHead->loadTexture(filename);
	}

	void PlayerUI::setEmptyHead()
	{
		CCAssert(nullptr != _ivHead, "nullptr == _ivHead");
		if(nullptr == _ivHead) return;

		_ivHead->loadTexture(Player_Empty);
	}

	void PlayerUI::showMoney(bool visible)
	{
		CCAssert(nullptr != _textMoney, "nullptr == _textMoney");
		if(nullptr == _textMoney) return;

		_textMoney->setVisible(visible);
	}

	void PlayerUI::showCard(bool visible)
	{
		CCAssert(nullptr != _textCardCount, "nullptr == _textCardCount");
		if(nullptr == _textCardCount) return;

		_textCardCount->setVisible(visible);
	}

	void PlayerUI::setTableUI(cocos2d::Node* tableUI)
	{
		_tableUI = tableUI;
	}

	void PlayerUI::showProfit(LLONG money)
	{
		Size size = this->getContentSize();

		char str[60] = {0};
		if (money > 0)
		{
			sprintf(str, "+%lld", money);
		}
		else
		{
			sprintf(str, "%lld", money);
		}

		Label* text = nullptr;  
		if(money > 0)
		{
			text = Label::createWithBMFont(File_Win_Num, str);
		}
		else
		{
			text = Label::createWithBMFont(File_Lose_Num, str);
		}

		text->setAnchorPoint(Vec2(0.0f, 0.5f));
		text->setPosition(Vec2(0.0f, 0.0f));
		this->addChild(text, 10000);

		text->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, size.height)), DelayTime::create(3.0f), RemoveSelf::create(), nullptr));
	}

	bool PlayerUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		if(_userId == INVALID_USER_ID || _tableUI == nullptr)
		{
			return false;
		}

		Rect rect(0, 0, this->getContentSize().width, this->getContentSize().height);
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		if(rect.containsPoint(pos))
		{
			return true;
		}
		return	false;
	}

	void PlayerUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		if(_tableUI == nullptr)	return;

		Size winSize = Director::getInstance()->getWinSize();

		//Vec2 pos = _tableUI->convertToNodeSpace(this->getPosition());
		Vec2 pos = touch->getLocation();
		GameUserMessageBox* message = GameUserMessageBox::createMessageBox(_tableUI, _userId);
		message->setPosition(winSize.width / 2, winSize.height / 2);
		message->setHidePoint(pos);
		_tableUI->reorderChild(message, ((GameTableUI*)_tableUI)->getMaxZorder());
	}

	void PlayerUI::loadComponet(ui::Widget* widget)
	{
		_ivHead     = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widget, "iHead"));

		_textName      = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(widget, "lName"));
		_textMoney     = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(widget, "lMoney"));
		_textMoney->setPosition(Vec2(_textMoney->getPosition().x-25, _textMoney->getPosition().y+140));
		_textCardCount = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(widget, "lCard"));

		_textName->setString("");
		_textMoney->setString("");
		_textCardCount->setString("");
	}
}
