#include "LandlordGameTableUI.h"
#include "LandlordGameTableLogic.h"
#include "LandlordCardOutList.h"
#include "LandlordCardListBoard.h"
#include "LandlordCardTypeAnimation.h"
#include "LandlordToolbar.h"
#include "LandlordGameAudio.h"
#include "LandlordGameUserMessageBox.h"
#include "LandlordExitPrompt.h"
#include "HNUIExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "LandlordGameChatDlg.h"
	
#define Max_Zorder              100
#define Table_Zorder            1
#define Player_Zorder           2
#define Action_Board_Zorder     3
#define Animation_Zorder        5
#define Toolbar_Zorder          6

#define JSON_NODE_HEAD   	"headPosition"
#define JSON_NODE_HAND_CARD "handCardPosition"
#define JSON_NODE_CLOCK     "clockPosition"
#define JSON_NODE_DEALER    "Dealer"
#define WIN_SIZE Director::getInstance()->getWinSize()

#define Tag_Out_Menu         100
#define Tag_Not_Out_Menu     101
#define Tag_Hint_Out_Menu    102

#define Word_Out             GBKToUtf8("出牌")
#define Word_Not_Out         GBKToUtf8("不出")
#define Word_Hint_Out        GBKToUtf8("提示")


#define Word_Out_N           "landlord/game/btn/out_n.png"
#define Word_Not_Out_N       "landlord/game/btn/not_out_n.png"
#define Word_Hint_Out_N      "landlord/game/btn/tips_n.png"

#define Word_Out_D           "landlord/game/btn/out_d.png"
#define Word_Not_Out_D       "landlord/game/btn/not_out_d.png"
#define Word_Hint_Out_D      "landlord/game/btn/tips_d.png"

#define Waiting_distribution_table_PNG	      "landlord/game/studioUI/uiRes/waiting.png"
#define Waiting_distribution_table_TAG	      1000

#define Tag_Call_Lord_Menu      200
#define Tag_Not_Call_Lord_Menu  201
#define Tag_Rob_Lord_Menu       202
#define Tag_Not_Rob_Lord_Menu   203
#define Tag_Double_Menu         204
#define Tag_Not_Double_Menu     205
#define Tag_Show_Menu           206
#define Tag_Show_Start_Menu     207
#define Tag_Ready               208

#define Word_Call_Lord          GBKToUtf8("叫地主")
#define Word_Not_Call_Lord      GBKToUtf8("不叫")
#define Word_Rob_Lord           GBKToUtf8("抢地主")
#define Word_Not_Rob_Lord       GBKToUtf8("不抢")
#define Word_Double             GBKToUtf8("加倍")
#define Word_Not_Double         GBKToUtf8("不加倍")
#define Word_Show               GBKToUtf8("明牌")
#define Word_ShowStart          GBKToUtf8("明牌开始")
#define Word_Ready              GBKToUtf8("准备")

#define Word_Call_Lord_N		"landlord/game/btn/call_landlord_n.png"
#define Word_Call_Lord_D		"landlord/game/btn/call_landlord_d.png"

#define Word_Not_Call_Lord_N     "landlord/game/btn/not_call_landlord_n.png"
#define Word_Not_Call_Lord_D     "landlord/game/btn/not_call_landlord_d.png"

#define Word_Rob_Lord_N          "landlord/game/btn/grab_landlord_n.png"
#define Word_Rob_Lord_D          "landlord/game/btn/grab_landlord_d.png"

#define Word_Not_Rob_Lord_N      "landlord/game/btn/not_grab_landlord_n.png"
#define Word_Not_Rob_Lord_D      "landlord/game/btn/not_grab_landlord_d.png"

#define Word_Double_N            "landlord/game/btn/double_n.png"
#define Word_Double_D            "landlord/game/btn/double_d.png"

#define Word_Not_Double_N        "landlord/game/btn/not_double_n.png"
#define Word_Not_Double_D        "landlord/game/btn/not_double_d.png"

#define Word_Show_N              "landlord/game/btn/mingpai_n.png"
#define Word_Show_D              "landlord/game/btn/mingpai_d.png"

#define Word_ShowStart_N         "landlord/game/btn/mingpai_ready_n.png"
#define Word_ShowStart_D         "landlord/game/btn/mingpai_ready_d.png"

#define Word_Ready_N             "landlord/game/btn/ready_n.png"
#define Word_Ready_D             "landlord/game/btn/ready_d.png"

#define Word_Chat_BG             "landlord/game/studioUI/uiRes/chatRes/"
#define Chat_Image_Small         "landlord/game/studioUI/uiRes/chatSmallRes/"

#define Image_Card_Notic		 "landlord/game/animation/open1.png"
static int Image_Tag =				1001;
static int Label_Tag  =				1002; 

#define Tag_Cancel_Auto_Layer   300

namespace landlord
{

#define Notice_Bg         "landlord/game/notic/notice_bg.png"
#define Notice_Bg_Stencil "landlord/game/notic/notice_bg_stencil.png"

HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
{
	GameTableUI* tableUI = new GameTableUI();
	if (tableUI->init(bDeskIndex, bAutoCreate))
	{
		tableUI->autorelease();
		return tableUI;
	}
	else
	{
		CC_SAFE_DELETE(tableUI);
		return nullptr;
	}
}

bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
{
	if (!HNLayer::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	this->ignoreAnchorPointForPosition(false);
	this->setPosition(size.width / 2, size.height / 2);

	loadLayout();

	//左上角图片
	auto image = ImageView::create(Image_Card_Notic);
	addChild(image);
	image->setAnchorPoint(Vec2(0, 1.0));
	image->setPosition(Vec2(5, size.height - 5));
	image->setVisible(false);
	image->setTag(Image_Tag);

	//左上角图片内容
	auto label = Label::create("", "", 15, Size(150, 60));
	addChild(label);
	label->setAnchorPoint(Vec2(0, 1.0));
	label->setPosition(Vec2(23, size.height - 20));
	label->setTextColor(Color4B::BLACK);
	label->setVisible(false);
	label->setTag(Label_Tag);

	//广播通知
	_NoticeBG = Sprite::create(Notice_Bg);
	_NoticeBG->setPosition(Vec2(size.width / 2, size.height * 0.80f));
	_NoticeBG->setCascadeOpacityEnabled(true);
	this->addChild(_NoticeBG, Max_Zorder);
	_NoticeBG->setVisible(false);

	_notic = GameNotice::createGameNotice(Notice_Bg_Stencil);
	_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2)); 
	_notic->setCallBack([this](){
		_NoticeBG->setVisible(false);
	});
	_NoticeBG->addChild(_notic);

	_tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
	_tableLogic->enterGame();

	schedule(schedule_selector(GameTableUI::checkTimeMatchStart), 1.0);


	//添加键盘监听事件
	auto keyBoard = EventListenerKeyboard::create();
	keyBoard->onKeyReleased= CC_CALLBACK_2(GameTableUI::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoard, this);
	return true;
}

void GameTableUI::onEnter()
{
	HNGameUIBase::onEnter();

	GameAudio::playBackground();
}

void GameTableUI::onExit()
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->removeSpriteFramesFromFile("landlord/game/plist/cards.plist");
	HNGameUIBase::onExit();
}

void GameTableUI::standUp()
{
	_tableLogic->sendUserUp();
}

void GameTableUI::forceQuit()
{
	_tableLogic->sendForceQuit();
}

bool GameTableUI::sendAuto(bool bAuto)
{
	return _tableLogic->sendAuto(bAuto);
}

void GameTableUI::dropDownEventCallBack(cocos2d::Node* pNode)
{
	if (nullptr == pNode) return;

	std::string name = pNode->getName();

	if(name.compare("toolbar") == 0)
	{
		ImageView* gameInfo = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "iCardBg"));
		if (nullptr == gameInfo) return;

		Toolbar* toolbar = dynamic_cast<Toolbar*>(pNode);
		if (nullptr == toolbar) return;

		if(toolbar->isUp())
		{
			gameInfo->runAction(MoveBy::create(0.1f, Vec2(0, -gameInfo->getContentSize().height)));
		}
		else
		{
			gameInfo->runAction(MoveBy::create(0.1f, Vec2(0, gameInfo->getContentSize().height)));
		}
	}
}

void GameTableUI::autoOutCallFunction(cocos2d::Node* pNode)
{
	if(pNode == nullptr) return;
	CardListBoard* outboard = (CardListBoard*)pNode;
	std::vector<BYTE> outCards;
	outboard->getTouchedCards(&outCards);
	_tableLogic->autoOutCheck(outboard->getSeatNo(), outCards);
}

int GameTableUI::getMaxZorder()
{
	return Max_Zorder;
}

//static int tmpXXXXX = 0;

bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
{
	return false;
}

void GameTableUI::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	if (Widget::TouchEventType::ENDED != touchtype)	return;

	Button* ptr = (Button*)pSender;
	switch (ptr->getTag())
	{
	case Tag_Call_Lord_Menu:
		_tableLogic->sendRobNT(1);
		break;
	case Tag_Not_Call_Lord_Menu:
		_tableLogic->sendRobNT(0);
		break;
	case Tag_Rob_Lord_Menu:
		_tableLogic->sendRobNT(1);
		break;
	case Tag_Not_Rob_Lord_Menu:
		_tableLogic->sendRobNT(0);
		break;
	case Tag_Double_Menu:
		_tableLogic->sendDouble(1);
		break;
	case Tag_Not_Double_Menu:
		_tableLogic->sendDouble(0);
		break;
	case Tag_Show_Menu:
		_tableLogic->sendShowCard(2);
		break;
	case Tag_Show_Start_Menu:
		if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
		{
			showWaitDistributionTable(true);
			// 进入排队游戏
			_tableLogic->sendQueue();
			break;
		}
		_tableLogic->sendShowStart();
		break;
	case Tag_Ready:
		if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
		{
			showWaitDistributionTable(true);
			// 进入排队游戏
			_tableLogic->sendQueue();
			break;
		}
		_tableLogic->sendAgreeGame();
		break;
	default:
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		_btnAction[i]->setVisible(false);
	}
	
}

void GameTableUI::outClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	if (Widget::TouchEventType::ENDED != touchtype)	return;

	bool bSend = false;

	Button* ptr = (Button*)pSender;
	switch (ptr->getTag())
	{
	case Tag_Hint_Out_Menu:
		{
			BYTE vSeatNo = _tableLogic->getMySeatNo();
			if(INVALID_DESKSTATION != vSeatNo)
			{
				std::vector<BYTE> cards;
				_cardListBoard[vSeatNo]->getUpCards(&cards);
				_tableLogic->outCheck(vSeatNo, cards);
			}
		}
		break;
	case Tag_Out_Menu:
		{
			BYTE mySeatNo = _tableLogic->getMySeatNo();
			if(INVALID_DESKSTATION != mySeatNo)
			{
				std::vector<BYTE> cards;
				_cardListBoard[mySeatNo]->getUpCards(&cards);
				_tableLogic->outCard(cards);

				if (!cards.empty())
				{
					if(_tableLogic->canOutCard(cards))
					{
						_tableLogic->sendOutCard(cards);
						bSend = true;
					}
				}
			}
		}
		break;
	case Tag_Not_Out_Menu:
		{
			std::vector<BYTE> cards;
			_tableLogic->sendOutCard(cards);
			bSend = true;
		}
		break;
	default:
		break;
	}

	if(bSend)
	{
		for(int i = 0; i < 3; i++)
		{
			_btnOut[i]->setVisible(false);
		}
	}
}

void GameTableUI::seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
// 	if (Widget::TouchEventType::ENDED != touchtype)
// 	{
// 		return;
// 	}
// 
// 	for (int i = 0; i < PLAY_COUNT; i++)
// 	{
// 		if (pSender == _tableUI.bSeat[i])
// 		{
// 			_tableLogic->sendUserSit(i);
// 			break;
// 		}
// 	}
}


void GameTableUI::pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	//if (Widget::TouchEventType::ENDED != touchtype)
	//{
	//	return;
	//}
	//Size winSize = Director::getInstance()->getWinSize();
	//HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
	//auto pButton = (Button*)pSender;

	//if (pButton == _tableUI.bJumpMenu[0])			//站起子按钮      
	//{
	//	_tableLogic->sendStandUp();
	//}
	//else if (pButton == _tableUI.bJumpMenu[1])			//设置子按钮
	//{
	//	pButton->setTouchEnabled(false);
	//	auto box = GameSetLayer::create();
	//	box->setPosition(Vec2(winSize.width, 0));
	//	addChild(box, 105);
	//	box->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))),
	//		CallFunc::create(CC_CALLBACK_0(GameTableUI::menuTouchReset, this, pButton)), nullptr));
	//}
	//else if (pButton == _tableUI.bJumpMenu[2])			//商城子按钮
	//{
	//	pButton->setTouchEnabled(false);
	//	auto gameStore = GameStoreLayer::createGameStore(this, 10);
	//	gameStore->setPosition(Vec2(0, winSize.height));
	//	addChild(gameStore, 105);
	//	this->runAction(MoveBy::create(0.2f, Vec2(0, -winSize.height / 2)));
	//	gameStore->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, -winSize.height / 2)),
	//		CallFunc::create(CC_CALLBACK_0(GameTableUI::menuTouchReset, this, pButton)), nullptr));
	//}
	//else if (pButton == _tableUI.bJumpMenu[3])			//商城子按钮
	//{
	//	showCardType();
	//}

	//auto des_pos = Vec2(0, winSize.height  + 10);
	//_tableUI.wPullDownMenu->stopAllActions();
	//auto mveTo = MoveTo::create(0.2f, des_pos);
	//_tableUI.wPullDownMenu->runAction(mveTo);
}

void GameTableUI::initParams()
{
	std::memset(_btnAction, 0, sizeof(_btnAction));
	std::memset(_btnOut, 0, sizeof(_btnOut));

	memset(_players, 0, sizeof(_players));
	memset(_cardListBoard, 0 , sizeof(_cardListBoard));
	memset(_lordCardBack, 0, sizeof(_lordCardBack));
	memset(_lordCardFront, 0, sizeof(_lordCardFront));
	memset(_cardOutList, 0, sizeof(_cardOutList));
	memset(_cardType, 0, sizeof(_cardType));
}

void GameTableUI::callEverySecond(float delta)
{
	do 
	{
		if(_timerText == nullptr)
		{
			stopTimer();
			break;
		}

		int count = _timerText->getTag();
		if(count <= 0)
		{
			stopTimer();
			break;
		}

		count--;
		_timerText->setTag(count);
		char str[20] = {0};
		sprintf(str, "%d", count);
		_timerText->setString(str);
	} while (0);
}

void GameTableUI::startTimer()
{
	schedule(schedule_selector(GameTableUI::callEverySecond),1.0f);
}

void GameTableUI::stopTimer()
{
	unschedule(schedule_selector(GameTableUI::callEverySecond));
}

void GameTableUI::loadUserStatus(BYTE seatNo, bool visible, const std::string &filename)
{
	char name[50] = {0};
	sprintf(name, "iReady%d", seatNo + 1);
	auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, name));
	if(ptr != nullptr)
	{
		ptr->setVisible(visible);
		if(visible)
		{
			ptr->loadTexture(filename);
		}
	}
}

void GameTableUI::loadOutCardTip(bool visible, const std::string &filename)
{
	char name[50] = {0};
	sprintf(name, "iOutCardTip");
	auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, name));
	if(ptr != nullptr)
	{
		ptr->setVisible(visible);
		if(visible)
		{
			ptr->loadTexture(filename);
			//ptr->runAction(FadeOut::create(2.0f));
		}
	}
}

void GameTableUI::addOutCardList(BYTE seatNo)
{
	char name[20] = {0};
	sprintf(name, "iOutCard%d", seatNo + 1);
	auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,name));
	if(ptr != nullptr)
	{
		_cardOutList[seatNo] = CardOutList::create();
		_cardOutList[seatNo]->setAnchorPoint(ptr->getAnchorPoint());
		ptr->getParent()->addChild(_cardOutList[seatNo], ptr->getLocalZOrder() + 1);
		Vec2 pos = ptr->getPosition();
// 		if(seatNo == 0)
// 		{
// 			_cardOutList[seatNo]->setAnchorPoint(Vec2(0.5f, 0.5f));
// 			pos.x = WIN_SIZE.width / 2;
// 			pos.y += 140;
// 		}
// 		else if(seatNo == 1)
// 		{
// 			pos.x += 40;
// 			pos.y += 60;
// 		}
// 		else if(seatNo == 2)
// 		{
// 			pos.x -= 40;
// 			pos.y += 60;
// 		}
		_cardOutList[seatNo]->setPosition(pos);
	}	
}

void GameTableUI::addHandCardList(BYTE seatNo)
{
	char name[20] = {0};
	sprintf(name, "iHandCard%d", seatNo + 1);
	auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,name));
	if(ptr == nullptr)
	{
		return;
	}
	_cardListBoard[seatNo] = CardListBoard::create(seatNo == 0, seatNo);

	_cardListBoard[seatNo]->setPosition(ptr->getPosition());
	_cardListBoard[seatNo]->setAnchorPoint(ptr->getAnchorPoint());
	_cardListBoard[seatNo]->setCallFunction(CC_CALLBACK_1(GameTableUI::autoOutCallFunction, this));
	ptr->getParent()->addChild(_cardListBoard[seatNo], ptr->getLocalZOrder());
}

GameTableUI::GameTableUI()
	: _tableWidget(nullptr)
	, _lordCardBg(nullptr)
	, _timerText(nullptr)
	, _toolbar(nullptr)
	, _bWaitOver (false)
	, _iContestNum(-1)
	, _contestIndex(1)
	, _iValue(-1)
	, _isViewBox(false)
{
	initParams();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameTableUI::~GameTableUI()
{
	HN_SAFE_DELETE(_tableLogic);
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("landlord/game/animation/huPaiAnimation.ExportJson");
}

/**************************************************************/

void GameTableUI::showLeftUpPNG(GameTaskStruct gameTask)
{
	char strName[200] ="" ; 

	const std::string CardName[MAX_CARD_TYPE] = {"3", "4", "5", "6","7" ,"8", "9", "10", \
		                   "J","Q","K" ,"A","2" , "小王" ,"大王"} ; 

	const std::string CardShpae[MAX_CARD_SHAPE] = {"单牌","对子","顺子","三带一","三带二","连对","飞机","炸弹"} ; 

	if(gameTask.byBackCardType != TYPE_NONE)
	{
		return ; 
	}
	switch(gameTask.byTaskType)
	{
	case  TYPE_HAVE_A_CARD:
		{
			sprintf(strName,"最后一手牌：带%s胜利，金币增加100%%" , CardName[gameTask.bySpecifyCard - 3].c_str())  ; 
			break;
		}
	case  TYPE_SOME_SHAPE:
		{
			sprintf(strName,"最后一手牌：以%s胜利，金币增加150%%" ,CardShpae[gameTask.bySpecifyShape].c_str()) ; 
			break;
		}
	case  TYPE_SINGLE_SOME_CARD:
		{
			sprintf(strName,"最后一手牌：以单张%s胜利，金币增加150%%" ,CardName[gameTask.bySpecifyCard - 3].c_str()) ; 
			break;
		}
	case  TYPE_DOUBLE_SOME_CARD:
		{
			sprintf(strName,"最后一手牌：以一对%s胜利，金币增加150%%" ,CardName[gameTask.bySpecifyCard - 3].c_str()) ;
			break;
		}
	default:
		{
			break;
		}
	}
	auto image = dynamic_cast<ImageView*>(this->getChildByTag(Image_Tag));
	image->setVisible(true);
	auto label = (Label*)this->getChildByTag(Label_Tag);
	label->setVisible(true);
	label->setString(GBKToUtf8(strName));
}

void GameTableUI::showBackCardTypeAndMutiple(BYTE cardType, BYTE cardMutiple)
{
	if (!cardType)		return;
	char name[64] = {0};
	sprintf(name, "landlord/game/studioUI/uiRes/addImage/paixin_%02d.png", cardType - 9);
	_cardType[0]->loadTexture(name);
	_cardType[0]->setVisible(true);

	sprintf(name, "landlord/game/studioUI/uiRes/addImage/peishu_%02d.png", cardMutiple - 1);
	_cardType[1]->loadTexture(name);
	_cardType[1]->setVisible(true);
}

void GameTableUI::showLandlordCard(bool visible, const std::vector<BYTE>& cards)
{
	if(visible)
	{
		if(cards.size() > 0)
		{
			for(int i = 0; i < 3; i++)
			{
				_lordCardFront[i]->setCardValue(cards[i]);
				_lordCardFront[i]->setVisible(true);
				_lordCardBack[i]->setVisible(false);
			}
		}
		else
		{
			for(int i = 0; i < 3; i++)
			{
				_lordCardFront[i]->setVisible(false);
				_lordCardBack[i]->setVisible(true);
			}
		}
		_lordCardBg->setVisible(true);
	}
	else
	{
		_lordCardBg->setVisible(false);
	}
}

/************************************************************************/
void GameTableUI::addUser(BYTE seatNo, bool bMe)
{
	if (!isValidSeat(seatNo))	return;

	BYTE deskStation = _tableLogic->viewToLogicSeatNo(seatNo);
	_players[seatNo]->setUserId(_tableLogic->getUserId(deskStation));
	_players[seatNo]->setHead(_tableLogic->getUserIsBoy(deskStation));
}

void GameTableUI::removeUser(BYTE seatNo, bool bMe)
{
	if (!isValidSeat(seatNo))	return;

	_players[seatNo]->setUserId(INVALID_USER_ID);
	_players[seatNo]->setEmptyHead();
}

void GameTableUI::downUserCards(BYTE seatNo, bool bMe)
{
	if (bMe)
	{
		_cardListBoard[seatNo]->downCards();
	}
}

void GameTableUI::setUserName(BYTE seatNo, const std::string& name)
{
	if (!isValidSeat(seatNo))	return;
	_players[seatNo]->setUserName(name);
}

void GameTableUI::showUserMoney(BYTE seatNo, bool visible)
{
	if (!isValidSeat(seatNo))	return;
	_players[seatNo]->showMoney(visible);
}

void GameTableUI::setUserMoney(BYTE seatNo, const std::string& money)
{
	if (!isValidSeat(seatNo))	return;
	_players[seatNo]->setUserMoney(money);
}

void GameTableUI::setUserAutoHead(BYTE seatNo)
{
	if(!isValidSeat(seatNo))	return;
	_players[seatNo]->setAutoHead();
}

void GameTableUI::setUserHead(BYTE seatNo, bool isEmpty)
{
	if(!isValidSeat(seatNo))	return;
	if(isEmpty)
	{
		_players[seatNo]->setEmptyHead();
	}
	else
	{
		_players[seatNo]->setHead(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::setUserGameHead(BYTE seatNo, bool isLord)
{
	if(!isValidSeat(seatNo))	return;
	_players[seatNo]->setGameHead(_tableLogic->getUserIsBoy(seatNo), isLord);
}

void GameTableUI::showUserHandCardCount(BYTE seatNo, bool visible)
{
	if (!isValidSeat(seatNo))	return;
	_players[seatNo]->showCard(visible);
}

void GameTableUI::setUserHandCardCount(BYTE seatNo, INT count)
{
	if (!isValidSeat(seatNo))	return;
	_players[seatNo]->setUserCardCount(count);
}

void GameTableUI::showUserHandCardOut(BYTE seatNo, const std::vector<BYTE>& cards)
{
	if(isValidSeat(seatNo))
	{
		if(_cardListBoard[seatNo] != nullptr)
		{
			_cardListBoard[seatNo]->removeCardAllOnce(cards);
		}

		if(_cardOutList[seatNo] == nullptr)
		{
			addOutCardList(seatNo);	
		}
		_cardOutList[seatNo]->outCard(cards);
	}
}

void GameTableUI::showUserOutCardType(BYTE seatNo, BYTE type)
{
	if(!isValidSeat(seatNo)) return;

	switch (type)
	{
	case UG_KING_BOMB:
		CardTypeAnimation::playRocket(this, Animation_Zorder);
		break;
	case UG_BOMB:
		CardTypeAnimation::playBomb(this, Animation_Zorder);
		break;
	case UG_STRAIGHT:
		CardTypeAnimation::playStraight(this, Animation_Zorder);
		break;
	case UG_DOUBLE_SEQUENCE:
		CardTypeAnimation::playDoubleStraight(this, Animation_Zorder);
		break;
	case UG_THREE_SEQUENCE:
	case UG_THREE_ONE_SEQUENCE:
	case UG_THREE_DOUBLE_SEQUENCE:
		CardTypeAnimation::playThree(this, Animation_Zorder);
		break;

	default:
		break;
	}
}

void GameTableUI::setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards)
{
	if(!isValidSeat(seatNo) || _cardListBoard[seatNo] == nullptr) return;
	_cardListBoard[seatNo]->upCards(upCards);
}

void GameTableUI::getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards)
{
	if(!isValidSeat(seatNo) || _cardListBoard[seatNo] == nullptr) return;
	_cardListBoard[seatNo]->getTouchedCards(upCards);
}

void GameTableUI::showLandlord(BYTE seatNo, bool visible)
{
	if(isValidSeat(seatNo))
	{
		char name[50] = {0};
		sprintf(name, "iLandlord%d", seatNo + 1);
		auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, name));
		if(ptr != nullptr)
		{
			ptr->setVisible(visible);
		}
	}
}

void GameTableUI::showActionButton(BYTE flag)
{
	if((flag & SHOW_CALL_LORD) != 0 && (flag & SHOW_NO_CALL_LORD) != 0)
	{
		_btnAction[0]->setVisible(true);
		_btnAction[2]->setVisible(true);
		_btnAction[1]->setVisible(false);

		_btnAction[0]->loadTextures(Word_Call_Lord_N, Word_Call_Lord_D);
		_btnAction[2]->loadTextures(Word_Not_Call_Lord_N, Word_Not_Call_Lord_D);
		//_btnAction[0]->setTitleText(Word_Call_Lord);
		//_btnAction[2]->setTitleText(Word_Not_Call_Lord);

		_btnAction[0]->setTag(Tag_Call_Lord_Menu);
		_btnAction[2]->setTag(Tag_Not_Call_Lord_Menu);
	}
	else if((flag & SHOW_ROB_LORD) != 0 && (flag & SHOW_NO_ROB_LORD) != 0)
	{
		_btnAction[0]->setVisible(true);
		_btnAction[2]->setVisible(true);
		_btnAction[1]->setVisible(false);

		_btnAction[0]->loadTextures(Word_Rob_Lord_N, Word_Rob_Lord_D);
		_btnAction[2]->loadTextures(Word_Not_Rob_Lord_N, Word_Not_Rob_Lord_D);

		//_btnAction[0]->setTitleText(Word_Rob_Lord);
		//_btnAction[2]->setTitleText(Word_Not_Rob_Lord);

		_btnAction[0]->setTag(Tag_Rob_Lord_Menu);
		_btnAction[2]->setTag(Tag_Not_Rob_Lord_Menu);
	}
	else if((flag & SHOW_DOUBLE) != 0 && (flag & SHOW_NO_DOUBLE) != 0)
	{
		_btnAction[0]->setVisible(true);
		_btnAction[2]->setVisible(true);
		_btnAction[1]->setVisible(false);

		_btnAction[0]->loadTextures(Word_Double_N, Word_Double_D);
		_btnAction[2]->loadTextures(Word_Not_Double_N, Word_Not_Double_D);

		//_btnAction[0]->setTitleText(Word_Double);
		//_btnAction[2]->setTitleText(Word_Not_Double);

		_btnAction[0]->setTag(Tag_Double_Menu);
		_btnAction[2]->setTag(Tag_Not_Double_Menu);
	}
	else if((flag & SHOW_SHOW_CARD) != 0 && (flag & SHOW_START) != 0 && !(_tableLogic->getbContestRoom()))
	{
		_btnAction[0]->setVisible(true);
		_btnAction[2]->setVisible(true);
		_btnAction[1]->setVisible(false);

		_btnAction[0]->loadTextures(Word_ShowStart_N, Word_ShowStart_D);
		_btnAction[2]->loadTextures(Word_Ready_N, Word_Ready_D);

		//_btnAction[0]->setTitleText(Word_ShowStart);
		//_btnAction[2]->setTitleText(Word_Ready);

		_btnAction[0]->setTag(Tag_Show_Start_Menu);
		_btnAction[2]->setTag(Tag_Ready);
	}
	else if((flag & SHOW_SHOW_CARD) != 0)
	{
		_btnAction[0]->setVisible(false);
		_btnAction[2]->setVisible(false);
		_btnAction[1]->setVisible(true);

		//auto wait = dynamic_cast<Armature*>(this->getChildByName("waitAction"));
		if ((!_bWaitOver))
			_btnAction[1]->loadTextures(Word_Show_N, Word_Show_D);

		_btnAction[1]->setTag(Tag_Show_Menu);
	}
	else if((flag & SHOW_START) != 0 && !(_tableLogic->getbContestRoom()))
	{
		_btnAction[0]->setVisible(false);
		_btnAction[2]->setVisible(false);
		_btnAction[1]->setVisible(true);

		_btnAction[1]->loadTextures(Word_Ready_N, Word_Ready_D);

		_btnAction[1]->setTag(Tag_Ready);
	}
	else 
	{
		_btnAction[0]->setVisible(false);
		_btnAction[2]->setVisible(false);
		_btnAction[1]->setVisible(false);
	}
}

void GameTableUI::showWin(BYTE winnerSeatNo)
{
	//std::vector<std::vector<LLONG>> winPool;
	//_dealer->dealWinPool(winPool);

	// recycle all chips to the winner
	//_dealer->recycleChips(winnerSeatNo);
}

void GameTableUI::showUserHandCard(BYTE seatNo, const std::vector<BYTE>& values, bool isUp, bool isOnce)
{
	if (isValidSeat(seatNo) && _players[seatNo] != nullptr)
	{
		if(_cardListBoard[seatNo] == nullptr)
		{
			addHandCardList(seatNo);
		}
		if(!isUp)
		{
			_cardListBoard[seatNo]->clear();
		}
		
		isOnce ? _cardListBoard[seatNo]->addCardAllOnce(values, isUp) : _cardListBoard[seatNo]->addCardOneByOne(values);
	}
}

void GameTableUI::clearDesk(bool isContestEnd)
{
	for(int i = 0; i < 3; i++)
	{
		if(_lordCardBack[i] != nullptr)
		{
			_lordCardBack[i]->setVisible(false);
		}

		if(_lordCardFront[i] != nullptr)
		{
			_lordCardFront[i]->setVisible(false);
		}
		
		if(_cardListBoard[i] != nullptr)
		{
			_cardListBoard[i]->clear();
		}
		
		if(_cardOutList[i] != nullptr)
		{
			_cardOutList[i]->removeAllChildren();
		}
		
		if (isContestEnd)
		{
			if (_btnAction[i]->isVisible())
			{
				_btnAction[i]->setVisible(false);
			}
		}
	}

	//左上角图片重置
	auto image = dynamic_cast<ImageView*>(this->getChildByTag(Image_Tag));
	image->setVisible(false);
	auto label = (Label*)this->getChildByTag(Label_Tag);
	label->setVisible(false);
	

	//隐藏底牌类型和倍数
	_cardType[0]->setVisible(false);
	_cardType[1]->setVisible(false);

	setMultiple(0);
	showWaitDistributionTable(false);
}

void GameTableUI::contestleaveTableDlg()
{
	if (!_isViewBox)
	{
		_isViewBox = true;
	}
	else
	{
		_isViewBox = false;
		auto bg = dynamic_cast<Sprite*>(this->getChildByName("leaveDlg"));
		if (nullptr != bg)
		{
			bg->runAction(Sequence::create(ScaleTo::create(0.1f, 0.0f), RemoveSelf::create(true), nullptr));
		}
		return;
	}

	Size winSize = Director::getInstance()->getWinSize();

	auto dlg = Sprite::create("landlord/game/contest/out.png");
	dlg->setPosition(Vec2(winSize.width/2, winSize.height/2));
	dlg->setScale(0.0f);
	dlg->setName("leaveDlg");
	this->addChild(dlg, 5000);
	dlg->runAction(ScaleTo::create(0.1f, 1.0f));

	auto yesBtn = Button::create("landlord/game/contest/match_yes0.png", "landlord/game/contest/match_yes1.png");
	yesBtn->setPosition(Vec2(dlg->getContentSize().width * 0.35, dlg->getContentSize().height * 0.3));
	yesBtn->setName("outTrue");
	dlg->addChild(yesBtn);

	auto noBtn = Button::create("landlord/game/contest/match_no0.png", "landlord/game/contest/match_no1.png");
	noBtn->setPosition(Vec2(dlg->getContentSize().width * 0.65, dlg->getContentSize().height * 0.3));
	noBtn->setName("wait");
	dlg->addChild(noBtn);

	yesBtn->addTouchEventListener(Widget::ccWidgetTouchCallback(CC_CALLBACK_2(GameTableUI::configBoxEventCallBack, this)));
	noBtn->addTouchEventListener(Widget::ccWidgetTouchCallback(CC_CALLBACK_2(GameTableUI::configBoxEventCallBack, this)));

	auto MyListener = EventListenerTouchOneByOne::create();
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto leaveDlg = (Sprite*)this->getChildByName("leaveDlg");
		auto dlgSize = leaveDlg->getBoundingBox();
		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);        
		if (rect.containsPoint(touch->getLocation()))
		{
			if (dlgSize.containsPoint(touch->getLocation())) return true;
			leaveDlg->runAction(Sequence::create(ScaleTo::create(0.1f, 0.0f), RemoveSelf::create(true), nullptr));
			return true;
		}
		else
			return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, dlg);
}

void GameTableUI::configBoxEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
	auto btn = (Button*)pSender;
	if (type != Widget::TouchEventType::ENDED) return;
	if (btn->getName() == "outTrue")
	{
		leaveDesk(true);
	}
	if (btn->getName() == "wait")
	{
		(btn->getParent())->runAction(Sequence::create(ScaleTo::create(0.1f, 0.0f), RemoveSelf::create(true), nullptr));
	}
}

void GameTableUI::showGameContestKick()
{
	for (int i = 0; i < 3; i++)
	{
		if (_btnAction[i] != nullptr)
		{
			_btnAction[i]->setVisible(false);
		}
	}
	//比赛移除等待动画
	removeContestAnimate();

	_bWaitOver = true;
	auto kickImg = Sprite::create("landlord/game/contest/ContestKick.png");
	kickImg->setPosition(Vec2(400, 240));
	kickImg->setName("kickPtr");
	kickImg->setScale(0.0f);
	_tableWidget->addChild(kickImg, 25);

	kickImg->runAction(Sequence::create(DelayTime::create(3.7f), 
		Spawn::create(ScaleTo::create(1.0f, 1.0f),
		RotateBy::create(1.0f, 360.0f), nullptr), DelayTime::create(3.0f),
		CallFunc::create([=](){leaveDesk(true);}), nullptr));
}

void GameTableUI::showGameContestWaitOver()
{
	//比赛移除等待动画
	removeContestAnimate();

	_bWaitOver = true;
	auto waitPtr = Sprite::create("landlord/game/contest/ContestWait.png");
	waitPtr->setPosition(Vec2(400, 240));
	waitPtr->setName("waitPtr");
	_tableWidget->addChild(waitPtr, 25);

	for (int i = 0; i < 3; i++)
	{
		_btnAction[i]->setVisible(false);
		_btnAction[i]->setEnabled(false);
	}
}

void GameTableUI::updateMyRankNum(int value)
{
	_iContestNum = value;
}

void GameTableUI::addContestUI()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("landlord/game/animation/huPaiAnimation.ExportJson");

	auto size = Director::getInstance()->getWinSize();
	auto contestIndex = Text::create();
	contestIndex->setAnchorPoint(Vec2(1.0, 0.5));
	contestIndex->setPosition(Vec2(size.width-10, size.height-20));
	addChild(contestIndex);
	contestIndex->setFontSize(23);
	contestIndex->setName("TextJu");
	contestIndex->setString(GBKToUtf8("第 1 局"));

	auto PaiMing = Text::create();
	PaiMing->setAnchorPoint(Vec2(1.0, 0.5));
	PaiMing->setPosition(Vec2(size.width-10, size.height-50));
	addChild(PaiMing);
	PaiMing->setFontSize(23);
	PaiMing->setName("PaiMing");
}

void GameTableUI::setJuShu(const std::string& str)
{
	auto JuShu = dynamic_cast<Text*>(this->getChildByName("TextJu"));
	if (JuShu == nullptr)	return;
	JuShu->setString(str);
}

void GameTableUI::setPaiMing(const std::string& str)
{
	auto PaiMing = dynamic_cast<Text*>(this->getChildByName("PaiMing"));
	if (PaiMing == nullptr)	return;
	PaiMing->setString(str);
}

void GameTableUI::playContestAnimate()
{
	if (_bWaitOver) return;
	auto wait = dynamic_cast<Armature*>(this->getChildByName("waitAction"));
	if (wait != nullptr)		return;
	char name[64] = {0};	
	sprintf(name,"wait");
	
	auto size = Director::getInstance()->getWinSize();
	auto waitArmature = Armature::create("huPaiAnimation");
	waitArmature->setPosition(Vec2(size.width/2, size.height/2));
	waitArmature->setScale(0.5);
	waitArmature->getAnimation()->play(name);
	waitArmature->setName("waitAction");
	addChild(waitArmature, 10);

	for (int i = 0; i < 3; i++)
	{
		_btnAction[i]->setVisible(false);
		//_btnAction[i]->setEnabled(false);
	}

}

void GameTableUI::removeContestAnimate()
{
	auto wait = dynamic_cast<Armature*>(this->getChildByName("waitAction"));
	if (wait != nullptr)
	{
		wait->removeFromParent();
	}
}

//检测报名的定时赛是否开始
void GameTableUI::checkTimeMatchStart(float dt)
{
	char str[64] = {0};
	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "MARK_%d", i);
		if (UserDefault::getInstance()->getIntegerForKey(str) == 2)
		{
			sprintf(str, "beginTime_%d", i);
			std::string beginTime = UserDefault::getInstance()->getStringForKey(str);
			time_t bTime = 0;
			for (auto iter = beginTime.begin(); iter != beginTime.end(); iter++)
			{
				bTime = bTime*10 + *iter - '0';
			}
			time_t nTime = time(0);
			if (nTime + 60 + 8*3600 >= bTime && nTime + 8*3600 <= bTime)
			{
				sprintf(str, "UserID_%d", i);
				_iValue = i;
				if (UserDefault::getInstance()->getIntegerForKey(str) != PlatformLogic()->loginResult.dwUserID)		continue;
				unschedule(schedule_selector(GameTableUI::checkTimeMatchStart));

				auto prompt = GamePromptLayer::create();
				prompt->showPrompt(GBKToUtf8("斗地主定时赛马上开始，是否进入？"));
				prompt->setPromptCanSelect();
				prompt->setCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 3);
					UserDefault::getInstance()->flush();
					RoomLogic()->close();
					GamePlatform::createPlatform();
				});
				prompt->setCancelCallBack([&](){
					char str[64] = {0};
					sprintf(str, "MARK_%d", _iValue);
					UserDefault::getInstance()->setIntegerForKey(str, 4);
					UserDefault::getInstance()->flush();
					schedule(schedule_selector(GameTableUI::checkTimeMatchStart), 1.0);
				});
			}
		}
	}
}

void GameTableUI::showGameContestOver(MSG_GR_ContestAward* contestAward)
{
	auto waitPtr = dynamic_cast<Sprite*>(_tableWidget->getChildByName("waitPtr"));
	if (waitPtr != nullptr)
	{
		waitPtr->removeFromParent();
	}
	//比赛移除等待动画
	removeContestAnimate();

	for (int i = 0; i < 3; i++)
	{
		_btnAction[i]->setVisible(false);
		_btnAction[i]->setEnabled(false);
	}

	for (int i = 0; i < PLAY_COUNT; i++)
	{	
		auto seatNo = _tableLogic->logicToViewSeatNo(i);
		if (seatNo) 
		{
			setUserName(seatNo, "");
			showUserHandCardCount(seatNo, false);
			setUserMoney(seatNo, "");
			setUserHandCardCount(seatNo, -1);
			hideUserStatus(seatNo);
			removeUser(seatNo, false);
		}
	}

	_bWaitOver = true;

	if (!contestAward->iAward)
	{
		showGameContestKick();
		return;
	}

	Sprite* awardDlg = nullptr;
	if (contestAward->iAwardType)
	{
		awardDlg = Sprite::create("landlord/game/contest/ContestAwardsX.png");
	}
	else
	{
		awardDlg = Sprite::create("landlord/game/contest/ContestAwards.png");
	}
	awardDlg->setPosition(Vec2(400, 480 * 0.6));
	_tableWidget->addChild(awardDlg, 28);
	
	awardDlg->setName("awardDlg");
	ParticleSystem *meteor = ParticleSystemQuad::create("landlord/game/contest/huoyan00.plist");
	meteor->setPosition(Vec2(awardDlg->getContentSize().width / 2, 10));
	meteor->setAutoRemoveOnFinish(true);
	awardDlg->addChild(meteor);

	awardDlg->setScale(0.0f);
	awardDlg->runAction(Sequence::create(DelayTime::create(3.7f), CallFunc::create([&](){removeContestAnimate();}), ScaleTo::create(0.2f, 1.0f), nullptr));

	char str[64] = {0};
	sprintf(str, "%d", _iContestNum);
	auto rankText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
	rankText->setPosition(Vec2(awardDlg->getContentSize().width * 0.73, awardDlg->getContentSize().height * 0.555));
	awardDlg->addChild(rankText);

	sprintf(str, "%d", contestAward->iAward);
	auto awardText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
	awardText->setPosition(Vec2(awardDlg->getContentSize().width * 0.45, awardDlg->getContentSize().height * 0.345));
	awardText->setAnchorPoint(Vec2(0, 0.5));
	awardDlg->addChild(awardText);

	auto MyListener = EventListenerTouchOneByOne::create();
	MyListener->setSwallowTouches(true);
	MyListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		auto awardDlg = (Sprite*)_tableWidget->getChildByName("awardDlg");
		auto dlgSize = awardDlg->getBoundingBox();
		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);        
		if (rect.containsPoint(touch->getLocation()))
		{
			if (dlgSize.containsPoint(touch->getLocation())) return true;
			awardDlg->runAction(Sequence::create(ScaleTo::create(0.2f, 0.0f), CallFunc::create([&](){leaveDesk(true);}), RemoveSelf::create(true), nullptr));
			return true;
		}
		else
			return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, awardDlg);

}


void GameTableUI::leaveDesk(bool isForceQuit)
{
	if ((RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) || isForceQuit)
	{
		RoomLogic()->close();
		GamePlatform::returnPlatform(LayerType::ROOMLIST);
	}
	else
	{
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}
}

//显示牌桌准备
void GameTableUI::showLoading(bool bVisible)
{
// 	if (bVisible)
// 	{
// 		_loadSprite = Sprite::create("table/loading_view.jpg");
// 		_loadSprite->setVisible(true);
// 		Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
// 		_loadSprite->setAnchorPoint(Vec2(0, 0));
// 		_loadSprite->setPosition(Vec2(0, 0));
// 		this->addChild(_loadSprite, MEMBAN_ZORDER);
// 	}
// 	else
// 	{
// 		auto fadeOut = FadeOut::create(2.0f);
// 		_loadSprite->runAction(fadeOut);
// 	}
}


void GameTableUI::showNotice(const std::string &message)
{
	_NoticeBG->setVisible(true);
	_notic->postMessage(GBKToUtf8(message.c_str()));
}

void GameTableUI::showUserProfit(BYTE seatNo, LLONG money)
{
	if(isValidSeat(seatNo) && _players[seatNo] != nullptr)
	{
		_players[seatNo]->showProfit(money);
		HNLOG_WARNING("seatno = %d >>> money = %ll", seatNo, money);
	}
}


void GameTableUI::setWaitTime(BYTE seatNo, BYTE time, bool visible)
{
	if(isValidSeat(seatNo))
	{
		char name[50] = {0};
		sprintf(name, "iClock%d", seatNo + 1);
		auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, name));
		if(ptr != nullptr)
		{
			ptr->setVisible(visible);
			if(visible)
			{
				memset(name, 0, sizeof(name));
				sprintf(name, "lClockNum%d", seatNo + 1);
				auto label = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(_tableWidget, name));
				if(label != nullptr)
				{
					char cTime[20] = {0};
					sprintf(cTime, "%d", time);
					label->setTag(time);
					label->setString(cTime);
					_timerText = label;
					startTimer();
				}
			}
			else
			{
				stopTimer();
				_timerText = nullptr;
			}
		}
	}
}

void GameTableUI::showDashboard(bool visible)
{
	//visible ? _dashboard->show() : _dashboard->hide();
}


void GameTableUI::showUserCallScore(BYTE seatNo)
{

}

void GameTableUI::showUserCallScoreResult(BYTE seatNo, int score)
{

}

void GameTableUI::showUserAddDoubleResult(BYTE seatNo, int value)
{

}

void GameTableUI::setBasePoint(int point)
{
	auto text = ui::Helper::seekWidgetByName(_tableWidget, "lBasePointNum");
	if(text != nullptr)
	{
		char str[50] = {0};
		sprintf(str, "%d", point);
		((ui::Text*)text)->setString(str);
	}
}

void GameTableUI::setMultiple(int multiple)
{
	auto text = ui::Helper::seekWidgetByName(_tableWidget, "lMultipleNum");
	if(text != nullptr)
	{
		char str[50] = {0};
		sprintf(str, "%d", multiple);
		((ui::Text*)text)->setString(str);
	}
}

void GameTableUI::showCancelAutoMask(bool visible)
{
	CancelAuto* layer = nullptr;

	if(visible)
	{
		layer = (CancelAuto*)this->getChildByTag(Tag_Cancel_Auto_Layer);
		if(layer == nullptr)
		{
			layer = CancelAuto::create(this);
			this->addChild(layer, Max_Zorder, Tag_Cancel_Auto_Layer);
		}
	}
	else
	{
		layer = (CancelAuto*)this->getChildByTag(Tag_Cancel_Auto_Layer);
		if(layer != nullptr)
		{
			layer->removeFromParent();
		}
	}
}

/*
 * 第一手出牌，必须要出牌，显示出牌、提示
 * 已经有人出过牌，显示出牌、提示不出
 */
void GameTableUI::setOutVisible(bool visible, bool firstOut)
{
	for(int i = 0; i < 3; i++)
	{
		_btnOut[i]->setVisible(visible);
	}

	if(visible)
	{
		_btnOut[2]->setTag(Tag_Out_Menu);
		_btnOut[2]->loadTextures(Word_Out_N, Word_Out_D);

		if(firstOut)
		{
			_btnOut[1]->setVisible(false);	

			_btnOut[0]->setTag(Tag_Hint_Out_Menu);
			_btnOut[0]->loadTextures(Word_Hint_Out_N, Word_Hint_Out_D);
		}
		else
		{
			_btnOut[1]->setTag(Tag_Hint_Out_Menu);
			_btnOut[1]->loadTextures(Word_Hint_Out_N, Word_Hint_Out_D);

			_btnOut[0]->setTag(Tag_Not_Out_Menu);
			_btnOut[0]->loadTextures(Word_Not_Out_N, Word_Not_Out_D);
		}
	}
}

void GameTableUI::showUserCallNT(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_call_landlord.png");
	if(visible)
	{
		GameAudio::playCallNT(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::showUserNotCallNT(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_pass_score.png");
	if(visible)
	{
		GameAudio::playNotCallNT(_tableLogic->getUserIsBoy(seatNo));
		_btnAction[0]->setVisible(false);
		_btnAction[2]->setVisible(false);
	}
}

void GameTableUI::showUserRobNT(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_grab_landlord.png");
	if(visible)
	{
		GameAudio::playRobNT(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::showUserNotRobNT(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_pass_call.png");
	if(visible)
	{
		GameAudio::playNotRobNT(_tableLogic->getUserIsBoy(seatNo));
		_btnAction[0]->setVisible(false);
		_btnAction[2]->setVisible(false);
	}
}

void GameTableUI::showUserReady(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_ready.png");
}

void GameTableUI::showUserDouble(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_double.png");
	if(visible)
	{
		GameAudio::playDouble(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::showUserNotDouble(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_not_double.png");
	if(visible)
	{
		GameAudio::playNotDouble(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::showUserPublishCard(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_show_card.png");
	if(visible)
	{
		GameAudio::playPublish(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::showUserNotOut(BYTE seatNo, bool visible)
{
	this->loadUserStatus(seatNo, visible, "landlord/game/studioUI/uiRes/player_not_out.png");
	if(visible)
	{
		GameAudio::playNotOut(_tableLogic->getUserIsBoy(seatNo));
	}
}

void GameTableUI::hideUserStatus(BYTE seatNo)
{
	loadUserStatus(seatNo, false, "landlord/game/studioUI/uiRes/player_not_out.png");
}

void GameTableUI::showUserInvalidOut(bool visible)
{
	loadOutCardTip(visible, "landlord/game/studioUI/uiRes/tips_outerror.png");
}

void GameTableUI::showUserCannotOut(bool visible)
{
	loadOutCardTip(visible, "landlord/game/studioUI/uiRes/tips_onlypass.png");
}

void GameTableUI::hideOutCardTip()
{
	loadOutCardTip(false, "landlord/game/studioUI/uiRes/tips_outerror.png");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameTableUI::playSingle(BYTE seatNo, int value)
{
    char str[10] = {0};
    sprintf(str, "%d", value);
    GameAudio::playSingle(str, _tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playDouble(BYTE seatNo, int value)
{
    char str[10] = {0};
    sprintf(str, "%d", value);
    GameAudio::playDouble(str, _tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playThree(BYTE seatNo, int value)
{
    GameAudio::playThree(value, _tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playStraight(BYTE seatNo)
{
	GameAudio::playStraight(_tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playDoubleSequence(BYTE seatNo)
{
	GameAudio::playDoubleSequence(_tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playPlan(BYTE seatNo)
{
	GameAudio::playPlan(_tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playFourTake(BYTE seatNo, bool isDouble)
{
	GameAudio::playFourTake(isDouble, _tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playRocket(BYTE seatNo)
{
	GameAudio::playRocket(_tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playBomb(BYTE seatNo)
{
	GameAudio::playBomb(_tableLogic->getUserIsBoy(seatNo));
}

void GameTableUI::playCardLast(BYTE seatNo, int value)
{
	GameAudio::playCardLast(_tableLogic->getUserIsBoy(seatNo), value);
}

void GameTableUI::playWin()
{
    GameAudio::playWin();
}

void GameTableUI::playLose()
{
    GameAudio::playLose();
}

/************************line*********************************/
void GameTableUI::loadLayout()
{
	//add image cache.
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("landlord/game/plist/cards.plist");

	this->setBackGroundImage("landlord/game/studioUI/uiRes/room_bg.png");

	Size winSize = Director::getInstance()->getWinSize();

	auto table_Node = CSLoader::createNode("landlord/game/studioUI/table.csb");
	addChild(table_Node, Table_Zorder);

	auto scalex = winSize.width / 800;
	auto scaley = winSize.height / 480;

	_tableWidget = (Layout*)table_Node->getChildByName("Panel_Table");
	_tableWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
	_tableWidget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	if (winSize.width / winSize.height <= 1.5f)
	{
		_tableWidget->setScale(scalex);
	}
	else
	{
		_tableWidget->setScale(scaley);
	}

	_lordCardBg = (ImageView*)Helper::seekWidgetByName(_tableWidget, "iCardBg");
	Vec2 lordBgPos = _lordCardBg->getParent()->convertToNodeSpace(Vec2(winSize.width / 2, winSize.height));
	_lordCardBg->setPosition(lordBgPos);
	
	char str[64] = {0};

	for (int i = 0; i < 2; i++)
	{
		sprintf(str, "Image_%d", i+1);
		_cardType[i] = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_lordCardBg, str));
		_cardType[i]->setLocalZOrder(10);
		_cardType[i]->setVisible(false);
	}
	for(int i = 0; i < 3; i++)
	{
		sprintf(str, "iCard%d", i + 1);
		_lordCardBack[i] = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, str));
		if(_lordCardBack[i] != nullptr)
		{
			_lordCardFront[i] = PokerCard::create(0);
			_lordCardFront[i]->setScale(_lordCardBack[i]->getScale());
			_lordCardFront[i]->setPosition(_lordCardBack[i]->getPosition());
			_lordCardBack[i]->getParent()->addChild(_lordCardFront[i]);
			_lordCardBack[i]->setVisible(false);
			_lordCardFront[i]->setVisible(false);
		}

	}

	char name[64] = {0};
	for(int i = 0; i < 3; i++)
	{
		sprintf(name, "btnAction%d", i + 1);
		_btnAction[i] = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget, name));
		_btnAction[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));
		_btnAction[i]->setVisible(false);

		sprintf(name, "btnOut%d", i + 1);
		_btnOut[i] = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget, name));
		_btnOut[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::outClickCallback, this));
		_btnOut[i]->setVisible(false);
	}

	char playerName[64] = {0};
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		sprintf(playerName, "iPlayer%d", i + 1);
		auto ptr = dynamic_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(_tableWidget,playerName));
		if(ptr != nullptr)
		{
			PlayerUI* player = PlayerUI::create(INVALID_USER_ID);
			player->setAnchorPoint(ptr->getAnchorPoint());
			player->setPosition(ptr->getPosition());
			player->setEmptyHead();
			player->setTableUI(this);
			_players[i] = player;
			ptr->getParent()->addChild(player, ptr->getLocalZOrder());
		}
	}	

	//load toolbar
	Toolbar* toolbar = Toolbar::create(/*this, */CC_CALLBACK_1(GameTableUI::dropDownEventCallBack, this));
	toolbar->setAnchorPoint(Vec2(0.5f, 0));
	toolbar->setPosition(winSize.width / 2, winSize.height);

	if (winSize.width / winSize.height <= 1.5f)
	{
		toolbar->setScale(scalex);
	}
	else
	{
		toolbar->setScale(scaley);
	}

	toolbar->setExitCallBack([this]()
	{
		if (_tableLogic->playing())
		{
			if (_tableLogic->getbContestRoom())
			{
				contestleaveTableDlg();
			}
			else
			{
				ExitPromptLayer* exitLayer = ExitPromptLayer::create();
				this->addChild(exitLayer, 1000);

				Size winSize = Director::getInstance()->getWinSize();

				auto scalex = winSize.width / 800;
				auto scaley = winSize.height / 480;

				if (winSize.width / winSize.height <= 1.5f)
				{
					exitLayer->setScale(scalex);
				}
				else
				{
					exitLayer->setScale(scaley);
				}
				exitLayer->setExitCallBack([this]()
				{
					_tableLogic->sendUserUp();
					_tableLogic->sendForceQuit();
				});
			}
		}
		else
		{
			_tableLogic->sendUserUp();
			_tableLogic->sendForceQuit();
		}
	});

	toolbar->setSettingCallBack([this]()
	{
		auto setLayer = GameSetLayer::create();
		setLayer->showSet(this, Max_Zorder);
	});

	toolbar->setAutoCallBack([this]()
	{
		sendAuto(true);
	});

	toolbar->onSpeakeCallBack = [this]()
	{
		if (RoomLogic()->getRoomRule() & GRR_FORBID_GAME_TALK)
		{
			auto prompt = GamePromptLayer::create();
			prompt->showPrompt(GBKToUtf8("禁止游戏聊天"));
			return;
		}
		auto chatLayer = GameChatDlgLayer::create();
		chatLayer->ignoreAnchorPointForPosition(false);
		chatLayer->setAnchorPoint(Vec2(0.5, 0.5));
		chatLayer->setPosition(_tableWidget->getPosition());

		chatLayer->onSpeakeCallBack = [this](const std::string& text)
		{
			_tableLogic->sendChatMsg(text);
		};
		//加载聊天记录
		chatLayer->addChatRecord(recordMsg);

		this->addChild(chatLayer, Max_Zorder);
	};


	this->addChild(toolbar, Toolbar_Zorder);
}

bool GameTableUI::isValidSeat(BYTE seatNo)
{
	return (seatNo < PLAY_COUNT && seatNo >= 0);
}

void GameTableUI::showWaitDistributionTable(bool show)
{
	Size winSize = Director::getInstance()->getWinSize();

	if (show)
	{
		auto waiting = dynamic_cast<Sprite*>(this->getChildByTag(Waiting_distribution_table_TAG));
		if (nullptr == waiting)
		{
			waiting = Sprite::create(Waiting_distribution_table_PNG);
			waiting->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
			addChild(waiting, 10, Waiting_distribution_table_TAG);
		}
	}
	else
	{
		auto waiting = dynamic_cast<Sprite*>(this->getChildByTag(Waiting_distribution_table_TAG));
		if (nullptr != waiting)
		{
			waiting->removeFromParent();
		}
	}
}

void GameTableUI::showUserChatMsg(BYTE seatNo, CHAR msg[])
{
	char str[501] = { 0 };
	std::string record;
	int _RichTag = 0;
	std::string filename(Word_Chat_BG);//气泡路径
	std::string filenameSmallImage(Chat_Image_Small);//小图路径
	auto maxFaceCount = 68;
	std::string word;
	char num[3] = { 0, 0, 0 };
	Size lineSize(360, 58);
	int fontSize = 18;
	int imageSize = 24;
	float msgAllLenght = 0;
	int ImageNum = 0;//表情个数，用来计算表情需占用的位置
	Color3B colorArray[] = { colorMagenta, colorBlue, colorRed, colorBlack };

	auto randNumber = INT(std::rand() / float(RAND_MAX + 1) * (sizeof(colorArray) / sizeof(Color3B)));
	Color3B fontColor = colorArray[randNumber];

	auto node = Node::create();
	auto pos = node->getAnchorPoint();
	pos = node->getPosition();
	_tableWidget->addChild(node, Max_Zorder);
	Point ptr = _players[seatNo]->getPosition();
	if (seatNo == 0)
	{
		ptr.x += 65;
		ptr.y += 65;
	}
	else if (seatNo == 1)
	{
		ptr.x += 65;
		ptr.y -= 40;
	}
	else if (seatNo == 2)
	{
		ptr.x -= 65;
		ptr.y -= 40;
	}

	sprintf(str, "node%d", seatNo);
	auto chat_Node = (Node*)_tableWidget->getChildByName(str);
	if (chat_Node)
	{
		chat_Node->removeFromParent();
	}

	sprintf(str, "chat_bg_text%d.png", seatNo);
	filename.append(str);
	auto tmp = Sprite::create(filename);
	auto tmpSize = tmp->getContentSize();
	Rect fullRect(0, 0, tmpSize.width, tmpSize.height);
	Rect insetRect;
	if (2 == seatNo)
	{
		insetRect = Rect(30, 25, 10, 10);
	}
	else
	{
		insetRect = Rect(40, 25, 10, 10);//自动缩放的区域
	}

	auto chatBG = ui::Scale9Sprite::create(filename, fullRect, insetRect);
	sprintf(str, "node%d", seatNo);
	node->setName(str);
	node->addChild(chatBG);
	node->setPosition(ptr);

	auto charMsg = RichText::create();
	charMsg->ignoreContentAdaptWithSize(false);
	charMsg->setContentSize(Size(320, lineSize.height));
	node->addChild(charMsg, Max_Zorder);

	// 在聊天记录和弹出语中分别显示说话玩家名字
	BYTE seat = _tableLogic->viewToLogicSeatNo(seatNo);
	UserInfoStruct* LogonResult = _tableLogic->getUserBySeatNo(seat);
	std::string nameSay = StringUtils::format("[%s说]:", LogonResult->nickName);
	record.append(nameSay);
	record.append(msg);
	recordMsg.push_back(record);

	if (recordMsg.size() == 101)
	{
		for (int i = 0; i < 100; i++)
		{
			recordMsg[i] = recordMsg[i + 1];
		}
		//移除最后加进去的一行
		recordMsg.pop_back();
	}

	RichElementText* text = nullptr;

	text = RichElementText::create(_RichTag++, colorBlue, 255, GBKToUtf8(" "), "Arial", fontSize);

	charMsg->pushBackElement(text);

	for (int i = 0; msg[i] != '\0'; i++)
	{
		if (msg[i] == '/' && msg[i + 1] == ':')
		{
			//符合表情条件，再判断是否在表情范围内
			num[0] = msg[i + 2];
			num[1] = msg[i + 3];
			auto imageNumber = atoi(num);
			// 一定是表情
			if (imageNumber >= 0 && imageNumber < maxFaceCount)
			{
				if (word.length() > 0)
				{
					RichElementText* rtext = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
					charMsg->pushBackElement(rtext);
					word.clear();
				}
				ImageNum++;
				RichElementImage* img = RichElementImage::create(_RichTag++, Color3B(255, 255, 255), 255, filenameSmallImage + StringUtils::format("im%d.png", imageNumber));
				charMsg->pushBackElement(img);
				i += 3;
			}
			//不是表情
			else
			{
				msgAllLenght += 0.5f;
				word.push_back(msg[i]);
			}
		}
		else
		{
			msgAllLenght += 0.5f;
			word.push_back(msg[i]);
		}
	}

	//显示文本
	if (word.length() > 0)
	{
		auto text = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
		charMsg->pushBackElement(text);
		word.clear();
	}

	float realAllLenght = msgAllLenght * fontSize + ImageNum * imageSize;//文本图片总的真实大小
	float msgRowNum = realAllLenght / 320.0f;
	float bgWidht, bgHeight;
	bgWidht = bgHeight = 0;
	if (msgRowNum < 1)
	{
		//msgRowNum小于1，说明消息只有一行，需缩放气泡的宽
		if (msgRowNum <= 0.12f)
		{
			bgWidht = 100;
			bgHeight = lineSize.height;
		}
		else
		{
			bgWidht = realAllLenght + 60;
			bgHeight = lineSize.height;
		}
	}
	else
	{
		//msgRowNum大于1，需缩放气泡的高
		bgWidht = lineSize.width + 60;
		bgHeight = (20.0f * (int)msgRowNum) + lineSize.height;	
	}
	//设置气泡大小
	chatBG->setContentSize(Size(bgWidht, bgHeight));
	charMsg->setContentSize(Size(lineSize.width, bgHeight));

	if (2 == seatNo)
	{
		chatBG->setAnchorPoint(Vec2(1, 0.5f));
		charMsg->setAnchorPoint(Vec2(0, 0.5f));
		charMsg->setPosition(Vec2(chatBG->getPositionX() - chatBG->getContentSize().width + 20, chatBG->getPositionY() - 15));
	}
	else
	{
		chatBG->setAnchorPoint(Vec2(0, 0.5f));
		charMsg->setAnchorPoint(Vec2(0, 0.5f));
		charMsg->setPosition(Vec2(chatBG->getPositionX() + 35, chatBG->getPositionY() - 15));
	}
	node->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(true), nullptr));
}

//手机返回键监听回调函数
void GameTableUI::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EventKeyboard::KeyCode::KEY_BACK != keyCode) return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (EventKeyboard::KeyCode::KEY_F1 != keyCode) return;
#endif
	if (_tableLogic->playing())
	{
		if (_tableLogic->getbContestRoom())
		{
			contestleaveTableDlg();
		}
		else
		{
			ExitPromptLayer* exitLayer = ExitPromptLayer::create();
			this->addChild(exitLayer, 1000);

			Size winSize = Director::getInstance()->getWinSize();

			auto scalex = winSize.width / 800;
			auto scaley = winSize.height / 480;

			if (winSize.width / winSize.height <= 1.5f)
			{
				exitLayer->setScale(scalex);
			}
			else
			{
				exitLayer->setScale(scaley);
			}
			exitLayer->setExitCallBack([this]()
			{
				_tableLogic->sendUserUp();
				_tableLogic->sendForceQuit();
			});
		}
	}
	else
	{
		_tableLogic->sendUserUp();
		_tableLogic->sendForceQuit();
	}
}

}
