#include "DriftingCarGameTableUI.h"
#include "DriftingCarGameTableLogic.h"
#include "DriftingCarGameCell.h"
#include "DriftingCarGameBoard.h"
#include "DriftingCarGameResultHistory.h"
#include "DriftingCarGameAudio.h"
#include "HNLobbyExport.h"
#include "HNUIExport.h"
#include "json/rapidjson.h"
#include "json/document.h"


namespace DriftingCar
{
	std::string ValueToString( LLONG value )
	{

		char tem[30] = {0};
		sprintf(tem,"%lld", value);

		std::string str(tem);

		//为负值时，符号不在计算长度中，要删除
		if(value < 0 )
		{
			str.erase(str.begin());
		}

		int ilen = str.length() / 3;
		if(str.length() % 3 ==0)
		{
			ilen -= 1;
		}

		string::reverse_iterator iter = str.rbegin();

		string strResult;

		for (int i=0; iter != str.rend(); ++iter)
		{
			strResult.push_back(*iter);
			i++;
			if(i >= 3 && ilen > 0)
			{
				strResult.push_back(',');
				i = 0;
				--ilen;
			}

		}

		std::reverse(strResult.begin(), strResult.end());

		if(value < 0)
		{
			strResult.insert(strResult.begin(),'-');//添加负号
		}

		return strResult;
	}
	GameTableUI* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
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

		_tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
		_tableLogic->sendGameInfo();

		return true;
	}

	void GameTableUI::onEnter()
	{
		HNGameUIBase::onEnter();
	}

	void GameTableUI::onExit()
	{
		HNGameUIBase::onExit();
	}

	void GameTableUI::areaClickCallback(cocos2d::Node* pNode)
	{				 
		//没有选择下多少
		if(_betIndex == -1)	return;

		int index = pNode->getTag() - 1000;

		//越界检查
		if(index < 0 || index >= AREA_COUNT)	return;
		_tableLogic->requestUserBet(index, _betIndex);
	}

	/************************************************************************/
	void GameTableUI::setBetEnabled(bool enabled)
	{
		for(int i = 0; i < BET_COUNT; i++)
		{
			if(_betButton[i] != nullptr)
			{
				_betButton[i]->setTouchEnabled(enabled);
				_betButton[i]->setBright(enabled);
			}
		}		
	}

	void GameTableUI::setFreeTime(BYTE time)
	{
		SetShowTime(time);
	}

	void GameTableUI::setBetTime(BYTE time)
	{
		_ImagenoNT->runAction(FadeOut::create(1.0f));

		SetShowTime(time);
	}

	void GameTableUI::setSendTime(BYTE time)
	{
		SetShowTime(time);
	}

	void GameTableUI::setMsg(const std::string &msg)
	{
		GamePromptLayer::create()->showPrompt(msg.c_str());
	}

	void GameTableUI::startSend( int end, BYTE time)
	{
		_iWinAward = end;

		if(_gameBoard != nullptr)
		{
			_gameBoard->startPlay( end, time);
		}
	}

	void GameTableUI::addRecord(const std::vector<BYTE>& records, BYTE byResult)
	{
		if(_resultHistory != nullptr)
		{
			_resultHistory->addRecord(records, byResult);
			_resultHistory->ShowProbability();//显示统计概率
		}

	}


	void GameTableUI::addNT(BYTE seatNo, const std::string& name, LLONG money)
	{

		NTInfo info;
		info.byseatno = seatNo;
		info.UserName = name;
		info.lmoney = money;

		vtNtInfp.push_back(info);

		ShowWaitNTList();


	}

	void GameTableUI::removeAllNT()
	{
		vtNtInfp.clear();

		ShowWaitNTList();
	}


	void GameTableUI::setBetAreaTotal(bool isMy, BYTE AreaNo, LLONG value)
	{
		if(AreaNo >=0 && AreaNo < AREA_COUNT)
		{
			_gameBetAreaText[AreaNo]._userTotalText->setString(ValueToString(value));

// 			ScaleBy* scale = nullptr;
// 
// 			if(_gameBetAreaText[AreaNo]._userTotalText->getScaleX() > 5.0f)
// 			{
// 				scale = ScaleBy::create(0.5f,1.0f);
// 			}
// 			else
// 			{
// 				scale = ScaleBy::create(0.5f,1.5f);
// 			}
// 			if(isMy)
// 			{
// 				_gameBetAreaText[AreaNo]._userTotalText->runAction(Sequence::create(scale,ScaleTo::create(1.0f,1.0),nullptr));
// 			}
		}

	}	

	void GameTableUI::setBetAreaUser(bool isMy, BYTE AreaNo, LLONG value)
	{
		if(AreaNo >= 0 && AreaNo < AREA_COUNT)
		{
			_gameBetAreaText[AreaNo]._userText->setString(ValueToString(value));

// 			ScaleBy* scale = nullptr;
// 
// 			if(_gameBetAreaText[AreaNo]._userText->getScaleX() > 5.0f)
// 			{
// 				scale = ScaleBy::create(0.5f,1.0f);
// 			}
// 			else
// 			{
// 				scale = ScaleBy::create(0.5f,1.5f);
// 			}
// 
// 			if(isMy)
// 			{
// 				_gameBetAreaText[AreaNo]._userText->runAction(Sequence::create(scale,ScaleTo::create(1.0f,1.0),nullptr));
// 			}
		}
	}

	void GameTableUI::setBetTotal(LLONG value)
	{

	}

	void GameTableUI::setNTName(const std::string& name)
	{
		_NTNameText->setString(name);
	}

	void GameTableUI::setNTCount(int count)
	{

		_NTCountText->setString(ValueToString(count));
	}

	void GameTableUI::setNTMoney(LLONG value)
	{
		_NTMoneyText->setString(ValueToString(value));
	}

	void GameTableUI::setNTGrade(LLONG value)
	{
		_NTGradeText->setString(ValueToString(value));
	}

	void GameTableUI::setUserName(const std::string& name)
	{
		_userNameText->setString(name);
	}

	void GameTableUI::setUserMoney(LLONG value)
	{
		_userMoneyText->setString(ValueToString(value));
	}

	void GameTableUI::setUserGrade(LLONG value)
	{
		_userGradeText->setString(ValueToString(value));
	}

	void GameTableUI::setUserBetMoney( LLONG value )
	{
		_userBetText->setString(ValueToString(value));
	}

	void GameTableUI::leaveDesk()
	{
		RoomLogic()->close();
		GamePlatform::returnPlatform(LayerType::ROOMLIST);
	}

	void GameTableUI::setOpenArea(int index)
	{
		//底图显示
		_image_bet_bg->setVisible(true);

		switch(index)
		{
		case 0:case 2: case 4: case 6:
			{
				Playanimation(index);	//如果是大奖，显示动画
				ShowImageCarBlink();	//车灯闪烁
				break;
			}
		case 1: case 3: case 5: case 7:
			{
				ShowSmallAwardCar(index);//如果是小奖显示图
				break;
			}
		}

	}

	void GameTableUI::hideOpenArea()
	{
		_image_bet_bg->setVisible(false);
		_image_bet_car->setVisible(false);
	}
	void GameTableUI::setApplyAndLeaveButtonEnabled(bool enabled)
	{
		if (_applyButton && _leaveButton)
		{
			_applyButton->setTouchEnabled(enabled);
			_leaveButton->setTouchEnabled(enabled);

			_applyButton->setBright(enabled);
			_leaveButton->setBright(enabled);
		}	
	}

	/************************************************************************/

	void GameTableUI::startTimer()
	{
		schedule(schedule_selector(GameTableUI::reduceTime), 1.0f, kRepeatForever, 1.0f);
	}

	void GameTableUI::stopTimer()
	{
		unschedule(schedule_selector(GameTableUI::reduceTime));
	}

	void GameTableUI::reduceTime(float delta)
	{
		int tag = _timeAtlat->getTag();
		tag--;

		ShowGameStatusImage(tag);//显示状态图
		PlaytimeWaring(tag);
		if(tag >= 0)
		{
			_timeAtlat->setTag(tag);
			char tmp[20] = {0};
			sprintf(tmp, "%d", tag);
			_timeAtlat->setString(tmp);
		}
		else
		{
			stopTimer();
		}
	}

	GameTableUI::GameTableUI()
		: _tableLogic(nullptr)
		, _betIndex(-1)
		
	{
		initParams();
	}

	GameTableUI::~GameTableUI()
	{
		HN_SAFE_DELETE(_tableLogic);
	}

	/************************line*********************************/
	void GameTableUI::initParams()
	{
		_timeAtlat		   = nullptr;
		_stateTimeText     = nullptr;
		_gameBoard         = nullptr;
		_resultHistory     = nullptr;
		_totalBetText      = nullptr;
		_NTNameText        = nullptr;
		_NTCountText       = nullptr;
		_NTMoneyText       = nullptr;
		_NTGradeText       = nullptr;
		_userNameText      = nullptr;
		_userMoneyText     = nullptr;
		_userGradeText     = nullptr;
		_applyButton       = nullptr;
		_exitButton		   = nullptr;
		_tableLogic		   = nullptr;
		_waitNTNum		   = nullptr;
		_spriteAnimation   = nullptr;
		_ImageStatus	   = nullptr;
		_layoutBetArea	   = nullptr;

		_iWaitNtIndex	   = 0;
		_iWinAward		   = -1;
		_iGameStatus	   = -1;

		_betButton.fill(nullptr);
		_gameBetArea.fill(nullptr);


	}

	void GameTableUI::loadLayout()
	{
//		setBackGroundImage("DriftingCar/CardUi/res/view_back.png");

		Size winSize = Director::getInstance()->getWinSize();


		//加载布局文件
		auto widget = CSLoader::createNode("DriftingCar/CardUi/Layer.csb");
		widget->setAnchorPoint(Vec2(0,0));
		widget->setPosition(Vec2(0, 0));
		addChild(widget);
	
		//层
		auto layout = widget->getChildByName("Panel_All");

		auto back = (ImageView*)layout->getChildByName("Image_bg");

		back->setScaleY( winSize.height / back->getContentSize().height);

		ImageView* pbottom =(ImageView*) layout->getChildByName("Image_bottom");

		//设置位置
		int iTopWidth = layout->getContentSize().height - pbottom->getContentSize().height;

		int iLayoutY = (winSize.height - pbottom->getContentSize().height - iTopWidth) / 2;

		layout->setPosition(Vec2(0, iLayoutY));

		pbottom->setPosition(Vec2(winSize.width / 2, -iLayoutY));

		//////////////////////////////////////////////////////////////////////////

		//游戏状态图
		_ImageStatus = (ImageView*)layout->getChildByName("Image_status");

		//上庄、下庄按钮事件
		_applyButton = (Button* ) layout->getChildByName("Button_apply");;
		_applyButton->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));

		_leaveButton = (Button*)layout->getChildByName("Button_leave");
		_leaveButton->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));

		_ImagenoNT = (ui::ImageView*)layout->getChildByName("Image_noNT");
		_ImagenoNT->setOpacity(0);

		_timeAtlat = (ui::TextAtlas*)layout->getChildByName("AtlasLabel_time");								//时钟

		_userNameText = (ui::Text*)pbottom->getChildByName("Text_userName");								//玩家昵称
		_userNameText->setString("");

		_userMoneyText = (ui::TextBMFont*)pbottom->getChildByName("BitmapFontLabel_UserScore");			//玩家金币
		_userMoneyText->setString("");

		_userGradeText = (ui::TextBMFont*)pbottom->getChildByName("BitmapFontLabel_UserWinLose");			//玩家成绩
		_userGradeText->setString("");

		_userBetText = (ui::TextBMFont*)pbottom->getChildByName("BitmapFontLabel_UserBetNum");			//玩家下注
		_userBetText->setString("");

		//庄家信息
		_NTNameText = (Text* ) layout->getChildByName("Text_NtUser");
		_NTNameText->setString("");

		_NTMoneyText = (TextBMFont* ) layout->getChildByName("BitmapFontLabel_score");
		_NTMoneyText->setString("");

		_NTGradeText = (TextBMFont* ) layout->getChildByName("BitmapFontLabel_winlose");
		_NTGradeText->setString("");

		_NTCountText = (TextBMFont* ) layout->getChildByName("BitmapFontLabel_ntcount");
		_NTCountText->setString("");

		_exitButton = (Button* ) pbottom->getChildByName("Button_exit");
		_exitButton->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuClickCallback, this));

		//庄家列表翻页
		_waitUpButton = (Button*)layout->getChildByName("Button_wait_left");
		_waitUpButton->addTouchEventListener(CC_CALLBACK_2(GameTableUI::WaitNTListClickCallback, this));

		_waitDwonButton = (Button*)layout->getChildByName("Button_wait_right");
		_waitDwonButton->addTouchEventListener(CC_CALLBACK_2(GameTableUI::WaitNTListClickCallback, this));

		//车灯
		for (int i = 0; i < CAR_LIGHT; i++)
		{
			char tm[30] = {0};
			sprintf(tm,"Image_car%d",i + 1);
			_arrayLight[i] = (ImageView*)layout->getChildByName(tm);
			_arrayLight[i]->setOpacity(0);
		}

		//下注数量按钮事件
		LoadBetButton(pbottom);

		//安装下注模块
		LoadBetArea(layout);

		//安装跑马灯模块
		LoadPaoMaDeng(layout);

		//在加结算面板
		LoadGameEndResult(layout);

		//安装上庄列表
		LoadNTList(layout);

		//安装历史记录模块
		LoadHistory(pbottom);

	}

	void GameTableUI::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{

		if(touchtype != Widget::TouchEventType::ENDED)
			return;

		ui::Button* pButton = (ui::Button*)pSender;
		std::string strName = pButton->getName();

		if(strName.compare("Button_apply") == 0)
		{
			_tableLogic->requestApplyNT();			
		}
		else if(strName.compare("Button_leave") == 0)
		{
			_tableLogic->requestApplyLeaveNT();

		}
		else if(strName.compare("Button_exit") == 0)
		{
			_tableLogic->requestLeaveTable();
		}
	}

	void GameTableUI::betClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if(touchtype != Widget::TouchEventType::ENDED)
			return;
		ui::Button* pButton = (ui::Button*)pSender;
		int index = pButton->getTag() - Bet_Button_Tag;

		//越界检查
		if(index < 0 || index >= BET_COUNT)	return;

		for(int i = 0; i < BET_COUNT; i++)
		{
			if(pSender == _betButton[i])
			{
				_betIndex = i;
				break;
			}
		}
		ShowblinkBetButton(_betIndex);//闪烁

	}

	void GameTableUI::SetShowTime( BYTE time )
	{
		char tmp[30] = {0};
		sprintf(tmp, "%d", time);
		_timeAtlat->setString(tmp);
		_timeAtlat->setTag(time);
		startTimer();
	}

	void GameTableUI::LoadBetButton(Node* pNode)
	{
		if(pNode == NULL)
			return;
		for(int i = 0; i < BET_COUNT; i++)
		{
			auto pBetLayout = pNode->getChildByName("Panel_BetArea");
			if(pBetLayout)
			{
				char strName[30] = {0};
				sprintf(strName, "Button_bet%d", i + 1);
				_betButton[i] = (Button* ) pBetLayout->getChildByName(strName);
				_betButton[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::betClickCallback, this));
				_betButton[i]->setTag(Bet_Button_Tag + i);
				_betButton[i]->setGlobalZOrder(0.1);
			}

		}
	}

	void GameTableUI::LoadBetArea(Node* pNode)
	{
		if(nullptr == pNode)
			return;

		_spriteAnimation = Sprite::create();

		auto pNodelayout = (Layer*)pNode->getChildByName("Layout_bet");

		_image_bet_bg =(ImageView*) pNodelayout->getChildByName("Image_bet_bg");
		_image_bet_car = (ImageView*)pNodelayout->getChildByName("Image_car");

		_image_bet_car->setVisible(false);
		_image_bet_bg->setVisible(false);


		_spriteAnimation->setPosition(_image_bet_bg->getPosition());
		_spriteAnimation->setLocalZOrder(_image_bet_bg->getLocalZOrder()+3);
		_image_bet_bg->getParent()->addChild(_spriteAnimation);

		for(int i = 0; i < AREA_COUNT; i++)
		{

			char str[30] = {0};

			sprintf(str, "Button_%d", i + 1);
			_gameBetArea[i] =(Button*) pNodelayout->getChildByName(str);

			sprintf(str, "BFlabel_user%d",i+1);
			_gameBetAreaText[i]._userText =(TextBMFont*) pNodelayout->getChildByName(str);
			_gameBetAreaText[i]._userText->setString("");

			sprintf(str, "BFlabel_tatol%d",i+1);
			_gameBetAreaText[i]._userTotalText =(TextBMFont*) pNodelayout->getChildByName(str);
			_gameBetAreaText[i]._userTotalText->setString("");

			_gameBetArea[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::betAreaClickCallback,this));
			_gameBetArea[i]->setTag(i+ betArea_button_tag);

		}

		_layoutBetArea = pNodelayout;

		_layoutBetArea->setOpacity(0);

	}

	void GameTableUI::LoadHistory(Node* pNode)
	{
		_resultHistory = GameResultHistory::create();
		pNode->addChild(_resultHistory);

		_resultHistory->LoadLayout(pNode);

	}
	void GameTableUI::WaitNTListClickCallback( cocos2d::Ref* pSender, Widget::TouchEventType touchType )
	{
		if(touchType != Widget::TouchEventType::ENDED)
			return;

		Button* pButton = (Button*)pSender;
		std::string strName = pButton->getName();

		if(strName.compare("Button_wait_left") == 0)
		{
			//点击左边
			--_iWaitNtIndex;
		}
		else if(strName.compare("Button_wait_right") == 0)
		{
			//点击右边
			++_iWaitNtIndex;
		}

		ShowWaitNTList();
	}
	void GameTableUI::betAreaClickCallback( cocos2d::Ref* pSender, Widget::TouchEventType touchtype )
	{
		if(touchtype != Widget::TouchEventType::ENDED) return;

		ui::Button* pButton = (ui::Button*)pSender;
		int index = pButton->getTag() - betArea_button_tag;

		//越界检查
		if(index < 0 || index >= AREA_COUNT || _betIndex == -1)	
			return;

		_tableLogic->requestUserBet(index, _betIndex);

	}

	void GameTableUI::LoadPaoMaDeng( Node* layout)
	{
		if(nullptr == layout)
			return;

		auto boardContainer =  layout->getChildByName("Panel_PaoMaDeng");

		GameBoard* board = GameBoard::create(boardContainer);

		boardContainer->getParent()->addChild(board);
		board->setAnchorPoint(boardContainer->getAnchorPoint());
		board->setPosition(boardContainer->getPosition());
		board->setLocalZOrder(boardContainer->getLocalZOrder());

		board->CallBackPlayAnimation = [this](Node* pNode)//跑灯完成回调
		{
			//跑灯跑完
			setOpenArea(_iWinAward % BET_ARES);

			ShowCarLogoblink(_iWinAward);

			_tableLogic->ShowGameEndResult();

			CCLOG("PlayAnimation over");
		};

		board->CallBackCarIndex = [this](int index)
		{
			//表示对应位置，开始车灯闪烁
			int iOut = -1;

			switch(index)
			{
			case 8:
				{
					iOut = 1;
					break;
				}
			case 13:
				{
					iOut = 2;
					break;
				}
			case 24:
				{
					iOut = 3;
					break;
				}
			case 29:
				{
					iOut = 0;
					break;
				}
			default:
				{
					break;
				}
			}
			if(iOut == -1)
				return;


			_arrayLight[iOut]->setOpacity(255);
			_arrayLight[iOut]->runAction(FadeOut::create(0.2f));

		};

		_gameBoard = board;
	}

	void GameTableUI::LoadGameEndResult( Node* layout)
	{
		if(nullptr == layout)
			return;

		auto pImage = layout->getChildByName("Image_end");

		_UserCapital	= (Text*) pImage->getChildByName("Text_User2");
		_UserWinScore	= (Text*) pImage->getChildByName("Text_User1");

		_UserNTScore	= (Text*) pImage->getChildByName("Text_NT1");
		_UserNTCapital	= (Text*) pImage->getChildByName("Text_NT2");

		pImage->setVisible(false);
		_imageEnd =(ImageView*) pImage;

	}

	void GameTableUI::SetGameEndResult( LLONG userScore, LLONG userCapital, LLONG userNTScore, LLONG userNTCapital )
	{
		_imageEnd->setVisible(true);

		_UserCapital->setString(ValueToString(userCapital));
		_UserWinScore->setString(ValueToString(userScore));

		_UserNTScore->setString(ValueToString(userNTScore));
		_UserNTCapital->setString(ValueToString(userNTCapital));

	}

	void GameTableUI::hideGameEndImage()
	{
		_imageEnd->setVisible(false);
	}

	void GameTableUI::LoadNTList( Node* pLayout )
	{
		if(nullptr == pLayout)
			return;

		char tm[30] = {0};
		for (int index = 0; index < Wait_NTList; ++index)
		{
			sprintf(tm, "Text_NT%ld", index + 1);
			_arrayWaitNt[index] = (Text*)pLayout->getChildByName(tm);
			_arrayWaitNt[index]->setString("");
		}

		_waitNTNum = (Text*)pLayout->getChildByName("Text_NT_NUM");
		_waitNTNum->setString("");

	}

	void GameTableUI::ShowWaitNTList()
	{
		CheckWaitNTList();

		int Index = _iWaitNtIndex * Wait_NTList;

		for (int i = 0; i < Wait_NTList; i++)
		{
			if(Index >= vtNtInfp.size())
			{
				_arrayWaitNt[i]->setString("");
				continue;
			}
			char tm[256]={0};
			sprintf(tm,"%s  %lld", vtNtInfp[Index].UserName.c_str(), vtNtInfp[Index].lmoney);

			_arrayWaitNt[i]->setString(tm);
			++Index;

		}
	}


	void GameTableUI::ShowWaitNTCount( BYTE mySeatNo )
	{
		int index = 0;
		bool bexist = false;

		for (; index < vtNtInfp.size(); ++ index)
		{
			if(vtNtInfp[index].byseatno == mySeatNo )
			{
				bexist = true;
				break;
			}
		}

		if(!bexist)
		{
			_waitNTNum->setString("");
			return;
		}

		char tm[30]={0};
		sprintf(tm,"%ld",index + 1);

		_waitNTNum->setString(tm);

	}

	void GameTableUI::ShowApplyButton()
	{
		_applyButton->setTouchEnabled(true);
		_applyButton->setVisible(true);

		_leaveButton->setTouchEnabled(false);
		_leaveButton->setVisible(false);
	}

	void GameTableUI::ShowDownNTButton()
	{
		_applyButton->setTouchEnabled(false);
		_applyButton->setVisible(false);

		_leaveButton->setTouchEnabled(true);
		_leaveButton->setVisible(true);
	}

	void GameTableUI::Playanimation( int index )
	{
		Sprite* sprite = Sprite::create();

		sprite->setPosition(_spriteAnimation->getPosition());
		sprite->setLocalZOrder(_spriteAnimation->getLocalZOrder());
		_spriteAnimation->getParent()->addChild(sprite);

		Animation* animation= Animation::create();
		for (int i = 1; i <= 13; i++)
		{
			char szname[100] = {0};
			sprintf(szname,"DriftingCar/CardUi/Res/ShowCar/%d/%d.png",index+1, i);
			animation->addSpriteFrameWithFile(szname);
		}
		animation->setDelayPerUnit(2.0 / 13.0f);


		Animation* animationCar = Animation::create();
		for (int i = 1; i <= 9; i++)
		{
			char szName[100] = {0};
			sprintf(szName, "DriftingCar/CardUi/Res/ShowCar/%dcar/%d.png",index+1, i);
			animationCar->addSpriteFrameWithFile(szName);
		}

		animationCar->setDelayPerUnit(2.0f / 10.0f);

		auto action = Animate::create(animation);


		sprite->runAction(cocos2d::Sequence::create(action, Animate::create(animationCar),cocos2d::DelayTime::create(2.0f),RemoveSelf::create(), nullptr));


	}
	void GameTableUI::CheckWaitNTList()
	{
		int iMaxIndex = vtNtInfp.size() / 5;
		if(vtNtInfp.size() % 5 == 0)
		{
			--iMaxIndex;
		}

		_iWaitNtIndex = std::max(0, _iWaitNtIndex);

		_iWaitNtIndex = std::min(_iWaitNtIndex, iMaxIndex);
	}

	void GameTableUI::ShowSmallAwardCar(int index)
	{


		std::string strInfo = StringUtils::format("DriftingCar/CardUi/Res/ShowCar/car%ld.png", index + 1);

		_image_bet_car->loadTexture(strInfo);

		_image_bet_car->setVisible(true);

	}

	void GameTableUI::ShowNoNT()
	{
		_ImagenoNT->runAction(FadeIn::create(1.5f));
	}
	void GameTableUI::ShowBetAreaAction()								//显示下注区域
	{
		_layoutBetArea->runAction(FadeIn::create(1.0f));
	}

	void GameTableUI::HideBetAreaAction()									//隐藏下注区域
	{
		_layoutBetArea->runAction(FadeOut::create(1.0f));
	}
	void GameTableUI::ShowCarLogoblink( int index )
	{
		int iLogos = 0;

		iLogos = (index - 1) % CAR_LOGO_COUNT;//与服务器坐标差1

		if(index <= 0)
		{
			iLogos = 31;
		}

		_gameBoard->ShowCarBlinkLogos(iLogos);

	}

	void GameTableUI::HideAllCarLogos()
	{

	}

	void GameTableUI::ShowblinkBetButton( int index )
	{
		if(index < 0 || index > _betButton.size())
			return;

		StopAllBetButton();//先停止全部

		_betButton[index]->runAction(Blink::create(50,100));

	}

	void GameTableUI::StopAllBetButton()
	{
		for (int i = 0; i < _betButton.size(); i++)
		{
			_betButton[i]->setVisible(true);
			_betButton[i]->stopAllActions();
		}
	}

	void GameTableUI::startGameFree()
	{
		_iGameStatus = GS_FREE_STATUS;

		HideImageCarBlink();

		_gameBoard->HideCarAndAllCell();
		GameAudio::resumeBackground();
	}

	void GameTableUI::startGameBeginBet()
	{
		_iGameStatus = GS_BET_STATUS;
		ShowBetAreaAction();
		_gameBoard->showCarStart();
		GameAudio::BetTishi();

	}

	void GameTableUI::startGamePaoMaDeng()
	{
		_iGameStatus = GS_SEND_STATUS;

		_betIndex = -1;	//表示没有点击下注筹码

		StopAllBetButton();

		GameAudio::pauseBackground();
		GameAudio::StartPaoMaDeng();
	}

	void GameTableUI::startGameEndResult()
	{

	}

	void GameTableUI::ShowGameStatusImage( int iTime )
	{
		//最后3秒开始调用
		if(iTime != 3)
			return;

		switch (_iGameStatus)
		{
		case GS_FREE_STATUS:
			{
				LoadImageGame(1);
				break;
			}
		case GS_BET_STATUS:
			{
				LoadImageGame(2);
				
				break;
			}
		case GS_SEND_STATUS:
			{
				break;
			}
		default:
			break;
		}

	}

	void GameTableUI::LoadImageGame( int itype )
	{
		char tm[40] = {0};

		switch(itype)
		{
		case 1://准备下注
			{
				strcpy(tm,"DriftingCar/CardUi/Res/status_01.png");
				break;
			}
		case 2://准备开奖
			{
				strcpy(tm,"DriftingCar/CardUi/Res/status_02.png");
				break;
			}
		case 3://你已上庄
			{
				strcpy(tm,"DriftingCar/CardUi/Res/status_03.png");
				break;
			}
		case 4://你已下庄
			{
				strcpy(tm,"DriftingCar/CardUi/Res/status_04.png");
				break;
			}
		}

		if(strlen(tm) > 0)
		{
			_ImageStatus->loadTexture(tm);
			_ImageStatus->setVisible(true);
			_ImageStatus->setOpacity(255);
			_ImageStatus->runAction(FadeOut::create(3.0f));
		}
	}

	void GameTableUI::ShowImageMyNT()
	{
		LoadImageGame(3);
	}

	void GameTableUI::ShowImageMyNoNT()
	{
		LoadImageGame(4);
	}

	void GameTableUI::Test()
	{
		_gameBoard->startPlay(0, 15);
	}

	void GameTableUI::ShowImageCarBlink()
	{
		std::for_each(_arrayLight.begin(), _arrayLight.end(),[=](ImageView * pImageView){
			pImageView->setOpacity(255);
			pImageView->runAction(Sequence::create(Blink::create(3,9), Blink::create(3,16), nullptr));
			
		});
	}

	void GameTableUI::HideImageCarBlink()
	{
		std::for_each(_arrayLight.begin(), _arrayLight.end(),[=](ImageView * pImageView){
			pImageView->setOpacity(0);
			pImageView->stopAllActions();
			
		});
	}

	void GameTableUI::PlaytimeWaring( int itime )
	{
		if(itime <= 3 && _iGameStatus == GS_BET_STATUS)
		{
			GameAudio::TimeWarnining();
		}
	}

	void GameTableUI::GameBetReuslt()
	{
		GameAudio::PlayBetChouma();
	}

	void GameTableUI::OnGameStation()
	{
		GameAudio::PlayBackground();

	//	GameAudio::pauseBackground();
	}





}
