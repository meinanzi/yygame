#include "WaterMarginGameUserBox.h"
#include <string>
//#include "PlatformConfig.h"

namespace WaterMargin
{
	GameUserMessageBox::GameUserMessageBox()
		:_iMoney(nullptr)
	{

	}

	GameUserMessageBox* GameUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID)
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

	bool GameUserMessageBox::init(cocos2d::Node* parent, INT userID)
	{
		if ( !HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//创建一个点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//阻止触摸向下传递
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			Size s = this->getContentSize();
			HNLOG("wide = %d,hight = %d", s.width, s.height);
			Rect rect = Rect(0, 0, s.width, s.height);        
			if (rect.containsPoint(touch->getLocation()))//判断触摸点是否在目标的范围内
			{
				if (_userDataBoxRect.containsPoint(touch->getLocation())) return true;
				hide();
				return true;
			}
			else
				return false;
		};
		//把点击监听添加到监听序列中,因为只创建了一个监听,一个监听只能绑定到一个对象上,所以其他对象要使用监听则克隆
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		//玩家信息结构体
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}

		//加载json文件

		auto node = CSLoader::createNode("WaterMargin/tableui/userBox.csb");
		auto widgt = (Layout*)node->getChildByName("Panel_39");
		//auto widgt = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("WaterMargin/tableui/userBox.json");
		widgt->setAnchorPoint(Vec2(0.5, 0.5));
		this->addChild(node);
		widgt->setPosition(Vec2(winSize.width/2, winSize.height/2));
		auto bg = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widgt, "bg"));
		auto iName = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(bg, "iName"));
		iName->setString(LogonResult->nickName);

		_iMoney = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(bg, "iMoney"));
	
		return true;
	}
}