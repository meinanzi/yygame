#include "LandlordGameChatDlg.h"
#include "HNLobbyExport.h"

namespace landlord
{
	static const char* CHATDLG_PATH = "landlord/game/studioUI/GameChatUi.csb";
	static const char* LIST_BG = "landlord/game/studioUI/uiRes/chatRes/list.png";               //listBG
	static const char* Chat_Image_Small = "landlord/game/studioUI/uiRes/chatSmallRes/";
	GameChatDlgLayer::GameChatDlgLayer()
	{
	}

	GameChatDlgLayer::~GameChatDlgLayer()
	{
	}

	void GameChatDlgLayer::closeChatDlg()
	{
		_chatUI.ImageView_BG->runAction(Sequence::create(ScaleTo::create(0.1f, 0.1f), CCCallFunc::create([&]()
		{
			this->removeFromParent();
		}), nullptr));
	}

	bool GameChatDlgLayer::init()
	{
		if (!HNLayer::init()) return false;

		Size winSize = Director::getInstance()->getWinSize();

		auto chat_Node = CSLoader::createNode(CHATDLG_PATH);
		this->addChild(chat_Node, 1, 10);

		_chatUI.chatUI = (Layout*)chat_Node->getChildByName("Panel_Chat");
		_chatUI.chatUI->setAnchorPoint(Vec2(0.5, 0.5));
		_chatUI.chatUI->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

		auto scalex = winSize.width / 800;
		auto scaley = winSize.height / 480;
		if (winSize.width / winSize.height <= 1.5f)
		{
			_chatUI.chatUI->setScale(scalex);
		}
		else
		{
			_chatUI.chatUI->setScale(scaley);
		}

		//聊天界面
		_chatUI.ImageView_BG = (ImageView*)_chatUI.chatUI->getChildByName("Image_chatBG");

		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标       
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = target->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
			{
				Rect uRect = (this->getChildByTag(10))->getBoundingBox();
				if (uRect.containsPoint(locationInNode)) return true;
				closeChatDlg();
				return true;
			}
			else
				return false;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		if (nullptr != _chatUI.ImageView_BG)
		{
			//常用语
			_chatUI.Button_common = (Button*)_chatUI.ImageView_BG->getChildByName("Button_common");
			_chatUI.Button_common->addTouchEventListener(CC_CALLBACK_2(GameChatDlgLayer::chatUiButtonCallBack, this));

			//表情
			_chatUI.Button_face = (Button*)_chatUI.ImageView_BG->getChildByName("Button_face");
			_chatUI.Button_face->addTouchEventListener(CC_CALLBACK_2(GameChatDlgLayer::chatUiButtonCallBack, this));

			//聊天
			_chatUI.Button_chat = (Button*)_chatUI.ImageView_BG->getChildByName("Button_chat");
			_chatUI.Button_chat->addTouchEventListener(CC_CALLBACK_2(GameChatDlgLayer::chatUiButtonCallBack, this));

			//聊天记录列表
			_chatUI.ListView_chatRecord = (ui::ListView*)_chatUI.ImageView_BG->getChildByName("ListView_chatRecord");
			_chatUI.ListView_chatRecord->setVisible(false);

			//确定
			_chatUI.Button_send = (Button*)_chatUI.ImageView_BG->getChildByName("Button_send");
			_chatUI.Button_send->addTouchEventListener(CC_CALLBACK_2(GameChatDlgLayer::chatUiButtonCallBack, this));

			//输入框
			auto TextField_text = (TextField*)_chatUI.ImageView_BG->getChildByName("TextField_msg");
            TextField_text ->setVisible(false);
            _chatUI.TextField_text  = HNEditBox::createEditBox(TextField_text, this);
			//表情列表
			_chatUI.ScrollView_face = (ui::ScrollView*)_chatUI.ImageView_BG->getChildByName("ScrollView_face");
			char str[12];
			for (int i = 0; i < 68; i++)
			{
				sprintf(str, "/:%02d", i);
				auto face = (Button*)_chatUI.ScrollView_face->getChildByName(str);
				face->addTouchEventListener(CC_CALLBACK_2(GameChatDlgLayer::faceTouchEventCallBack, this));
			}

			//常用语列表
			_chatUI.ListView_common = (ui::ListView*)_chatUI.ImageView_BG->getChildByName("ListView_common");

			for (int i = 0; i < 10; i++)
			{
				createCommonList(i);
			}

			_chatUI.ListView_common->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameChatDlgLayer::commonListEventCallBack, this)));
		}
		return true;
	}

	void GameChatDlgLayer::createCommonList(INT idx)
	{
		char *str[10] = { "快点吧，等到花儿都谢了。",
			"大家好， 很高兴见到各位。",
			"又断线了，网络怎么这么差啊。",
			"和你合作真是太愉快了。",
			"我们交个朋友吧，能告诉我你的联系方式吗？",
			"你是MM，还是GG？",
			"不要吵了，不要吵了，专心玩游戏吧。",
			"不要走，决战到天亮。",
			"各位真是不好意思，我要走了。",
			"再见了，我会想念大家的。" };

		// 常用语列表背景图
		auto listItem = ImageView::create(LIST_BG);
		float itemWidth = listItem->getContentSize().width;
		float itemHeight = listItem->getContentSize().height;
		listItem->setName("listItem");
		listItem->setTouchEnabled(true);

		// 文字
		auto commonText = Text::create("", "", 22);
		commonText->setColor(colorBlack);
		commonText->setName("commonText");
		commonText->setAnchorPoint(Vec2(0, 0.5));
		commonText->setPosition(Vec2(itemWidth * 0.1f, itemHeight * 0.55f));
		commonText->setString(GBKToUtf8(str[idx]));
		listItem->addChild(commonText);

		_chatUI.ListView_common->pushBackCustomItem(listItem);
	}

	//聊天界面按钮回调
	void GameChatDlgLayer::chatUiButtonCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED != type) return;
		auto btn = (Button*)pSender;
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

		std::string name = btn->getName();

		if (name.compare("Button_send") == 0)
		{
			if (_chatUI.TextField_text->getString().empty())
			{
				auto prompt = GamePromptLayer::create();
				prompt->showPrompt(GBKToUtf8("输入框不能为空！"));
			}
			else
			{
				if (nullptr != onSpeakeCallBack)
				{
					onSpeakeCallBack(_chatUI.TextField_text->getString());
					closeChatDlg();
				}
			}
		}
		if (name.compare("Button_common") == 0)
		{
			_chatUI.ListView_common->setVisible(true);
			_chatUI.ScrollView_face->setVisible(false);
			_chatUI.ListView_chatRecord->setVisible(false);
		}

		if (name.compare("Button_face") == 0)
		{
			_chatUI.ListView_common->setVisible(false);
			_chatUI.ScrollView_face->setVisible(true);
			_chatUI.ListView_chatRecord->setVisible(false);
		}
		if (name.compare("Button_chat") == 0)
		{
			_chatUI.ListView_common->setVisible(false);
			_chatUI.ScrollView_face->setVisible(false);
			_chatUI.ListView_chatRecord->setVisible(true);
		}
	}

	//点击表情回调
	void GameChatDlgLayer::faceTouchEventCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED != type) return;
		auto btn = (Button*)pSender;
		std::string str = _chatUI.TextField_text->getString();
		str += btn->getName();
		_chatUI.TextField_text->setString(str);
	}

	//常用语列表点击回调
	void GameChatDlgLayer::commonListEventCallBack(Ref* pSender, ui::ListView::EventType type)
	{
		if (ListView::EventType::ON_SELECTED_ITEM_END != type) return;
		ListView* listItems = dynamic_cast<ListView*>(pSender);
		UINT selected = listItems->getCurSelectedIndex();
		Widget* listItem = listItems->getItem(selected);
		auto commontext = (Text*)listItem->getChildByName("commonText");

		if (nullptr != onSpeakeCallBack)
		{
			onSpeakeCallBack(commontext->getString());
			closeChatDlg();
		}
	}

	void GameChatDlgLayer::addChatRecord(std::vector<std::string > msg)
	{
		char str[501] = { 0 };
		std::string filenameSmallImage(Chat_Image_Small);//小图路径
		auto maxFaceCount = 68;
		std::string word;
		char num[3] = { 0, 0, 0 };
		Size lineSize(540, 30);
		int fontSize = 18;
		int imageSize = 24;
		float msgAllLenght = 0;
		int ImageNum = 0;//表情个数，用来计算表情需占用的位置
		int msgNumber = 0;

		while (msgNumber < msg.size())
		{
			auto charMsg = RichText::create();
			charMsg->ignoreContentAdaptWithSize(false);
			charMsg->setContentSize(Size(lineSize.width - 20, lineSize.height));
			charMsg->setAnchorPoint(Vec2(0.5f, 1));

			std::string aloneMsg = msg.at(msg.size() - msgNumber - 1);
			for (int i = 0; aloneMsg[i] != '\0'; i++)
			{
				if (aloneMsg[i] == '/' && aloneMsg[i + 1] == ':')
				{
					//符合表情条件，再判断是否在表情范围内
					num[0] = aloneMsg[i + 2];
					num[1] = aloneMsg[i + 3];
					auto imageNumber = atoi(num);
					// 一定是表情
					if (imageNumber >= 0 && imageNumber < maxFaceCount)
					{
						if (word.length() > 0)
						{
							RichElementText* rtext = RichElementText::create(0, colorBlack, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
							charMsg->pushBackElement(rtext);
							word.clear();
						}
						ImageNum++;
						RichElementImage* img = RichElementImage::create(0, Color3B(255, 255, 255), 255, filenameSmallImage + StringUtils::format("im%d.png", imageNumber));
						charMsg->pushBackElement(img);
						i += 3;
					}
					//不是表情
					else
					{
						msgAllLenght += 0.5f;
						word.push_back(aloneMsg[i]);
					}
				}
				else
				{
					msgAllLenght += 0.5f;
					word.push_back(aloneMsg[i]);
				}
			}
			//显示文本
			if (word.length() > 0)
			{
				auto text = RichElementText::create(0, colorBlack, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
				charMsg->pushBackElement(text);
				word.clear();
			}

			float realAllLenght = msgAllLenght * fontSize + ImageNum * imageSize;//文本图片总的真实大小
			float msgRowNum = realAllLenght / 520.0f;
			float msgHeight = 0;
			if (msgRowNum < 1)
			{
				msgHeight = lineSize.height;
			}
			else
			{
				msgHeight = lineSize.height + (20 * (int)msgRowNum);
			}

			//聊天消息子项
			auto msgLayout = Layout::create();
			msgLayout->setContentSize(Size(lineSize.width, msgHeight));
			msgLayout->addChild(charMsg);
			charMsg->setPosition(Vec2(msgLayout->getContentSize().width / 2 + 10, msgLayout->getContentSize().height));
			_chatUI.ListView_chatRecord->pushBackCustomItem(msgLayout);

			msgNumber++;
			ImageNum = 0;
			msgAllLenght = 0;
		}
	}
}