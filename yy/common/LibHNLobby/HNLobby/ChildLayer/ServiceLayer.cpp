#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "HNMarketExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

static const char* SERVICE_UI_JSON = "platform/serviceUi/ServiceNode.csb";

ServiceLayer::ServiceLayer()
	: _csNode(nullptr)
	, _buttonJiFen(nullptr)
	, _buttonShangBi(nullptr)
	, _buttonClose(nullptr)
	, _byChangeType(0)
	, _iIntegralRatio(1)
{

}

ServiceLayer::~ServiceLayer()
{
	HNHttpRequest::getInstance()->removeObserver(this);
}

void ServiceLayer::requestServiceInfo(int iNum)
{
	std::string url = PlatformConfig::getInstance()->getOrderUrl();
	url.append(StringUtils::format("type=WxScoreChange"));
	url.append(StringUtils::format("&UserName=%s", PlatformLogic()->loginResult.szName));
	url.append(StringUtils::format("&Num=%d", iNum));

	HNHttpRequest::getInstance()->request("exchange", network::HttpRequest::Type::GET, url);
	HNHttpRequest::getInstance()->addObserver(this);
}

void ServiceLayer::showService(Node* parent, Vec2 vec, int zorder, int tag)
{
	if (_csNode != nullptr)
	{
		_csNode->setPosition(Vec2(vec.x, vec.y));
		parent->addChild(this, zorder, tag);
	}	
}

//显示兑换页面
//bShangbi--默认显示尚币兑换界面
void	ServiceLayer::ShowChangeView()
{
	//显示兑换尚币
	if (0 == _byChangeType)
	{
		_ChangeDataUi.Txt_Info->setText(GBKToUtf8("兑换尚币数量:"));
		//兑换比例
		_ChangeDataUi.Txt_BiLi->setText(StringUtils::format("1:%d", PlatformLogic()->loginResult.iExChageRatio));
	}
	else
	{
		_ChangeDataUi.Txt_Info->setText(GBKToUtf8("兑换积分数量:"));
		//兑换比例
		_ChangeDataUi.Txt_BiLi->setText(StringUtils::format("1:%d", _iIntegralRatio));
	}
	//元亨卡数量
	_ChangeDataUi.Txt_Lotteries->setText(StringUtils::format("%d", PlatformLogic()->loginResult.iLotteries));
	//兑换确定按钮
	_ChangeDataUi.Button_ShangBiOK->setVisible(true);
	//元亨卡不足 就不能兑换
	_ChangeDataUi.Button_ShangBiOK->setEnabled(PlatformLogic()->loginResult.iLotteries > 0);

	_buttonJiFen->setEnabled(!_byChangeType);
	_buttonShangBi->setEnabled(_byChangeType);
	
}

void ServiceLayer::closeService()
{
	if (_csNode == nullptr)
	{
		return;
	}

	_csNode->runAction(Sequence::create(
		ScaleTo::create(0.1f, 0.3f), 
		CallFunc::create([&](){
			this->removeFromParent();
		}), 
		nullptr));
}

bool ServiceLayer::init()
{
	if (!HNLayer::init())
	{
		return false;
	}

	_winSize = Director::getInstance()->getWinSize();

	// 屏蔽后面的层
	quicklyShade(100);

	_csNode = CSLoader::createNode(SERVICE_UI_JSON);	
	_csNode->setScale(0);
	addChild(_csNode, 2, 3);
	_csNode->runAction(ScaleTo::create(0.2f, 1.0f));

	//兑换尚币界面
	_ChangeDataUi.img_BG = (ImageView*)_csNode->getChildByName("Image_ShangBi");
	auto bgRect = _ChangeDataUi.img_BG->getBoundingBox();
	//元亨卡数量
	_ChangeDataUi.Txt_Lotteries = (Text*)_ChangeDataUi.img_BG->getChildByName("Text_ShuLiang");
	_ChangeDataUi.Txt_Lotteries->setText(StringUtils::format("%d", PlatformLogic()->loginResult.iLotteries));

	//兑换信息
	_ChangeDataUi.Txt_Info = (Text*)_ChangeDataUi.img_BG->getChildByName("Text_Info");
		
	//兑换比例
	_ChangeDataUi.Txt_BiLi = (Text*)_ChangeDataUi.img_BG->getChildByName("Text_BiLi");
	_ChangeDataUi.Txt_BiLi->setText(StringUtils::format("%d", PlatformLogic()->loginResult.iLotteries));
	//兑换数量
	auto TextField_ChangeSBNum = (TextField*)_ChangeDataUi.img_BG->getChildByName("TextField_Num");
	TextField_ChangeSBNum->setVisible(false);
	_ChangeDataUi.edit_Num = HNEditBox::createEditBox(TextField_ChangeSBNum, this);
	_ChangeDataUi.edit_Num->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
	_ChangeDataUi.edit_Num->setMaxLength(6);
	//兑换确定按钮
	_ChangeDataUi.Button_ShangBiOK = (Button*)_ChangeDataUi.img_BG->getChildByName("Button_ShangBiOK");
	_ChangeDataUi.Button_ShangBiOK->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onChangeClick, this));
	
	// 关闭
	_buttonClose = (Button*)_csNode->getChildByName("Button_Close");
	_buttonClose->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));

	// 兑换尚币
	_buttonShangBi = (Button*)_csNode->getChildByName("Button_ShangBi");
	_buttonShangBi->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));
	_buttonShangBi->setEnabled(false);
	// 兑换积分
	_buttonJiFen = (Button*)_csNode->getChildByName("Button_JiFen");
	_buttonJiFen->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));
	_buttonJiFen->setEnabled(true);

	//默认显示兑换页面
	ShowChangeView();

	// 创建一个点击事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		// 获取的当前触摸的目标
		auto target = dynamic_cast<ServiceLayer*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size size = target->getContentSize();
		Rect rect = Rect(0, 0, size.width, size.height);
		if (rect.containsPoint(locationInNode))
		{
			if (bgRect.containsPoint(locationInNode))
			{
				return true;
			}
			return true;
		}
		else
		{
			return false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ServiceLayer::setBGPositon(Vec2 vec)
{
	_csNode->setPosition(Vec2(vec.x, vec.y - 100));
}

void ServiceLayer::onServiceClick(Ref* pRef)
{
	Node* pNdoe = (Node*)pRef;
	std::string name = pNdoe->getName();

	// 关闭
	if (name.compare("Button_Close") == 0)
	{
		closeService();
	}

	// 兑换尚币
	else if (name.compare("Button_ShangBi") == 0)
	{
		_byChangeType = 0;
		ShowChangeView();
	}

	// 兑换积分
	else if (name.compare("Button_JiFen") == 0)
	{
		_byChangeType = 1;
		ShowChangeView();
	}
}

//兑换回调
void ServiceLayer::onChangeClick(Ref* pRef)
{
	//判断元亨卡是否足够
	std::string sNum;
	sNum = _ChangeDataUi.edit_Num->getText();
	INT iNum = atoi(sNum.c_str());
	if (iNum <= 0)
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入兑换数量!"));
		return;
	}
	
	if (iNum > PlatformLogic()->loginResult.iLotteries)
	{
		GamePromptLayer::create()->showPrompt(GBKToUtf8("超出元亨卡兑换数量!"));
		return;
	}

	MSG_GP_S_EXCHANGE	TExChange;
	TExChange.iExChangeNum = iNum;

	if (0 == _byChangeType)
	{
		//兑换尚币
		PlatformLogic()->sendData(MDM_GP_EXCHANGE, ASS_GP_EXCHANGE_COIN, &TExChange, sizeof(TExChange), HN_SOCKET_CALLBACK(ServiceLayer::ExchangeEventSelector, this));
	}
	else if (1 == _byChangeType)
	{
		//兑换积分走网页
		requestServiceInfo(iNum);
	}
}

// 兑换操作处理函数
bool ServiceLayer::ExchangeEventSelector(HNSocketMessage* socketMessage)
{
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	// 兑换尚币
	if (ASS_GP_EXCHANGE_COIN == bAssistantID)
	{
		switch (bHandleCode)
		{
			case 1://				3		//兑换成功了
			{
				// 广播过来
				CCAssert(sizeof(MSG_GP_S_EXCHANGE_RESULT) == socketMessage->objectSize, "MSG_GP_S_EXCHANGE_RESULT size is error.");
				if (sizeof(MSG_GP_S_EXCHANGE_RESULT) != socketMessage->objectSize)
				{
					return true;
				}
				MSG_GP_S_EXCHANGE_RESULT * pExChangeResult = reinterpret_cast<MSG_GP_S_EXCHANGE_RESULT*>(socketMessage->object);
				if (nullptr == pExChangeResult)
				{
					return true;
				}
				char buffer[100];
				sprintf(buffer, "恭喜您兑换获得%d尚币！", pExChangeResult->iGetMoenyNum);
				GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
				//更新自己的元亨卡数量
				PlatformLogic()->loginResult.iLotteries = pExChangeResult->iRemaindLotteries;
				ShowChangeView();
				break;
			} 
			case 0://					1		//参数错误
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("兑换参数错误!"));
				break;
			} 
			case 2://					2		//元亨卡数量不足
			{
				GamePromptLayer::create()->showPrompt(GBKToUtf8("元亨卡兑换数量不足!"));
				break;
			} 
			default:
			{
				break;
			}
		}
	}
	else if (ASS_GP_EXCHANGE_INTEGRAL == bAssistantID)
	{

	}

	return true;
}


void ServiceLayer::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
{
	const char* szRs = "rs";
	const char* szMsg = "msg";
	if (requestName.compare("exchange") == 0)
	{
		if (!isSucceed)
		{
			return;
		}
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(responseData.c_str());
		if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember(szRs) || doc[szRs].IsArray())
		{
			return;
		}
		int rs = doc[szRs].GetInt();
		std::string msg = doc[szMsg].GetString();
		//1-表示兑换成功了 
		if (1 == rs)
		{
			const char* szLotteries = "Lotteries";
			//就要更新元亨卡数量
			if (doc.HasMember(szLotteries) && !doc[szLotteries].IsArray())
			{
				int iLotteries = doc[szLotteries].GetInt();
				//更新自己的元亨卡数量
				PlatformLogic()->loginResult.iLotteries = iLotteries;
				ShowChangeView();

			}
		}
		GamePromptLayer::create()->showPrompt(GBKToUtf8(msg.c_str()));
	}
}