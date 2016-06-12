#include "GoldenFlowerGameUserMessageBox.h"
#include <string>
#include "HNLobbyExport.h"

namespace goldenflower
{
	static const INT USERDATA_BOX = 10;    //玩家资料框

	static const char* MESSAGE_BOX_BG		= "goldenflower/userinfo_messagebox/table_userinfo_bg.png";
	static const char* HEAD_BOX				= "goldenflower/userinfo_messagebox/head_box.png";

	GameUserMessageBox* GameUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID, std::string headImage)
	{
		auto pRet = new GameUserMessageBox();
		if (pRet && pRet->init(parent, userID, headImage))
		{
			pRet->autorelease();
			return pRet;
		}
		delete pRet;
		pRet = nullptr;
		return pRet;
	}

	bool GameUserMessageBox::init(cocos2d::Node* parent, INT userID, std::string headImage)
	{
		if ( !HNDialogBase::init(parent)) return false;

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

		//玩家资料框
		auto userDataBox = Sprite::create(MESSAGE_BOX_BG);
		userDataBox->setPosition(Vec2(winSize.width/2, winSize.height/2));
		addChild(userDataBox, 10, USERDATA_BOX);
		_userDataBoxRect = userDataBox->getBoundingBox();

		//玩家头像
		auto userIcon = Sprite::create(headImage);
		userIcon->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(userIcon, 2);

		/*
		//玩家头像背景框
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(headBox, 3);*/

		char str[128];

		//玩家昵称
		sprintf(str, "%s", LogonResult->nickName);
		//auto uNickName = createLabel(gFontConfig_18, GBKToUtf8(str), colorGold);
		auto uNickName = Label::createWithSystemFont(GBKToUtf8(str), "", 18);

		uNickName->setAnchorPoint(Vec2(0, 0.5));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uNickName, 2);

		//玩家性别(const std::string& text, float fontSize, Color3B color = colorGold);
		Label* uSex = nullptr;
		if (LogonResult->bBoy) uSex = createLabel(GBKToUtf8("男"), 18, colorGold);
		else uSex = createLabel(GBKToUtf8("女"), 18, colorGold);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uSex, 2);

		//玩家经验
		sprintf(str, "%d", LogonResult->dwPoint);
		auto uEXP = createLabel(str, 18, colorGold);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uEXP, 2);

		//玩家胜局
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = createLabel(str, 18, colorGold);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uWinNum, 2);

		//玩家负局
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = createLabel(str, 18, colorGold);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uLostNum, 2);

		//玩家和局
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = createLabel(str, 18, colorGold);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uMidNum, 2);

		//个性签名
		sprintf(str, "%s", LogonResult->szSignDescr);
		auto uSignDescr = createLabel(GBKToUtf8(str), 18, colorGold);
		uSignDescr->setAnchorPoint(Vec2(0, 0.5));
		uSignDescr->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.335));
		userDataBox->addChild(uSignDescr, 2);

		return true;
	}

}


