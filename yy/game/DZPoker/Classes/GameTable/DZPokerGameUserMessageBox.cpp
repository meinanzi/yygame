#include "DZPokerGameUserMessageBox.h"
#include "HNNetExport.h"
#include "HNLobbyExport.h"
#include <string>

namespace DZPoker
{

	static const INT USERDATA_BOX		= 10;    //玩家资料框

	static const char* MESSAGE_BOX_BG	= "dzpoker/userinfo_messagebox/table_userinfo_bg.png";
	static const char* HEAD_BOX			= "dzpoker/userinfo_messagebox/head_box.png";
	static const char* MEN_HEAD			= "dzpoker/userinfo_messagebox/men_head.png";
	static const char* WOMEN_HEAD		= "dzpoker/userinfo_messagebox/women_head.png";

	GameUserMessageBox* GameUserMessageBox::createMessageBox(cocos2d::Node* parent, int userID)
	{
		auto pRet = new GameUserMessageBox();
		if (pRet && pRet->init(parent, userID))
		{
			pRet->autorelease();
			return pRet;
		}
		delete pRet;
		pRet = nullptr;
		return pRet;
	}

	bool GameUserMessageBox::init(cocos2d::Node* parent, int userID)
	{
		if (!HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//创建一个点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//阻止触摸向下传递
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto sp = this->getChildByTag(USERDATA_BOX);
			Rect rect = Rect(0, 0, sp->getContentSize().width, sp->getContentSize().height);        
			auto pos = sp->convertToNodeSpace(touch->getLocation());
			if (!rect.containsPoint(pos))
			{
				hide();
			}
			return true;
		};
		//把点击监听添加到监听序列中,因为只创建了一个监听,一个监听只能绑定到一个对象上,所以其他对象要使用监听则克隆
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		//玩家信息结构体
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}

		// 玩家资料框
		auto userDataBox = Sprite::create(MESSAGE_BOX_BG);
		userDataBox->setPosition(Vec2(winSize.width/2, winSize.height/2));
		addChild(userDataBox, 10, USERDATA_BOX);
		_userDataBoxRect = userDataBox->getBoundingBox();

		// 玩家头像
		auto userIcon = Sprite::create(LogonResult->bBoy ? MEN_HEAD : WOMEN_HEAD);
		userIcon->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userIcon->setScale(1.144f);
		userDataBox->addChild(userIcon, 2);

		// 玩家头像背景框
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(headBox, 3);

		char str[128];

		// 玩家昵称
		std::string text = StringUtils::format("%s", LogonResult->nickName);
		auto uNickName = createLabel(GBKToUtf8(str), 20.0f, colorGold);
		uNickName->setAnchorPoint(Vec2(0, 0.5));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.89f));
		userDataBox->addChild(uNickName, 2);

		// 玩家性别
		Label* uSex = nullptr;
		if (LogonResult->bBoy) uSex = createLabel(GBKToUtf8("男"), 20.0f, colorGold);
		else uSex = createLabel(GBKToUtf8("女"), 20.0f, colorGold);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.89f));
		userDataBox->addChild(uSex, 2);
		// 玩家经验
		sprintf(str, "%d", LogonResult->dwPoint);
		auto uEXP = createLabel(str, 20, colorGold);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.71f));
		userDataBox->addChild(uEXP, 2);

		// 玩家胜局
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = createLabel(str, 20, colorGold);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.71f));
		userDataBox->addChild(uWinNum, 2);

		// 玩家负局
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = createLabel(str, 20, colorGold);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.529f));
		userDataBox->addChild(uLostNum, 2);

		// 玩家和局
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = createLabel(str, 20, colorGold);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.529f));
		userDataBox->addChild(uMidNum, 2);

		// 个性签名
		sprintf(str, "%s", LogonResult->szSignDescr);
		auto uSignDescr = createLabel(GBKToUtf8(str), 20);
		uSignDescr->setColor(colorGold);
		uSignDescr->setAnchorPoint(Vec2(0, 0.5));
		uSignDescr->setPosition(Vec2(userDataBox->getContentSize().width*0.12f, userDataBox->getContentSize().height*0.335f));
		userDataBox->addChild(uSignDescr, 2);

		return true;
	}


}