#include "GoldenToadGameUserMessageBox.h"
#include <string>
#include "HNLobbyExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;

namespace GoldenToad
{
	static const INT USERDATA_BOX			= 10;    //玩家资料框
	static const char*  CSB_FILE			= "goldenToad/userinfo/UserNode.csb";
	static const char* MEN_HEAD				= "goldenToad/userinfo/men_head.png";
	static const char* WOMEN_HEAD			= "goldenToad/userinfo/women_head.png";


	GoldenToadUserMessageBox* GoldenToadUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID, LLONG userMoney)
	{
		auto pRet = new GoldenToadUserMessageBox();
		if (pRet && pRet->init(parent, userID, userMoney))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	bool GoldenToadUserMessageBox::init(cocos2d::Node* parent, INT userID, LLONG userMoney)
	{
		if ( !HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//添加csb
		_loader = CSLoader::createNode(CSB_FILE);
		_loader->setPosition(Vec2(winSize / 2));
		addChild(_loader);
		auto layout = (Layout*) _loader->getChildByName("Panel_1");
		//创建一个点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//阻止触摸向下传递
		MyListener->onTouchBegan = [=](Touch* touch, Event* event)
		{
			auto bg = (ImageView*) layout->getChildByName("Image_bg");

			Rect rect = Rect(0, 0, bg->getContentSize().width, bg->getContentSize().height);        
			auto pos = bg->convertToNodeSpace(touch->getLocation());
			if (!rect.containsPoint(pos))
			{
				hide();
			}
			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		//玩家信息结构体
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}
		char str[32];
		auto pic = (ImageView*) layout->getChildByName("Image_userFrame");
		auto pic_size = pic->getContentSize();

		//玩家头像
		auto userIcon = Sprite::create(LogonResult->bBoy ? MEN_HEAD : WOMEN_HEAD);
		userIcon->setAnchorPoint(Vec2(0.5f , 0.5f));
		auto userWidth = userIcon->getContentSize().width;
		auto userHeight = userIcon->getContentSize().height;
		userIcon->setScale(pic->getContentSize().width / userWidth, pic->getContentSize().height / userHeight);
		userIcon->setPosition(Vec2(pic_size.width / 2, pic_size.height / 2));
		pic->addChild(userIcon, -1);

		//玩家昵称
		sprintf(str, "%s", LogonResult->nickName);
		auto name_text = (Text* ) layout->getChildByName("Text_name");
		name_text->setString(GBKToUtf8(str));

		//玩家金币
		sprintf(str, "%lld", userMoney);
		auto money_alas = (TextAtlas*) layout->getChildByName("AtlasLabel_Coin");
		money_alas->setString(str);

		//玩家经验
		sprintf(str, "%d", LogonResult->dwExperience);
		auto uEXP = (Text*) layout->getChildByName("Text_exp");
		uEXP->setString(str);

		//玩家胜局
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = (Text*) layout->getChildByName("Text_shengju");
		uWinNum->setString(str);
		
		//玩家负局
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = (Text*) layout->getChildByName("Text_fuju");
		uLostNum->setString(str);
		
		//玩家和局
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = (Text*) layout->getChildByName("Text_heju");
		uMidNum->setString(str);

		return true;
	}
}
