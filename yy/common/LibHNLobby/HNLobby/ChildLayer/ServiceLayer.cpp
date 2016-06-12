#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNLobby/PlatformConfig.h"
#include "HNMarketExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

static const char* SERVICE_UI_JSON = "platform/serviceUi/ServiceNode.csb";

ServiceLayer::ServiceLayer()
	: _csNode(nullptr)
	, _buttonEmail(nullptr)
	, _buttonMessage(nullptr)
	, _buttonPhone(nullptr)
{

}

ServiceLayer::~ServiceLayer()
{
	HNHttpRequest::getInstance()->removeObserver(this);
}

void ServiceLayer::requestServiceInfo()
{
	std::string param = StringUtils::format("action=GetServiceInfo&key=");
	HNHttpRequest::getInstance()->request("service", network::HttpRequest::Type::POST, PlatformConfig::getInstance()->getEditUrl(), param);
	HNHttpRequest::getInstance()->addObserver(this);
}

void ServiceLayer::showService(Node* parent, Vec2 vec, int zorder, int tag)
{
	if (_csNode != nullptr)
	{
		_csNode->setPosition(Vec2(vec.x, vec.y - 80));
		parent->addChild(this, zorder, tag);
	}	
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

	// 电话
	_buttonPhone = (Button*)_csNode->getChildByName("Button_phone");
	_buttonPhone->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));
	_buttonPhone->setEnabled(false);

	// 信息
	_buttonMessage = (Button*)_csNode->getChildByName("Button_message");
	_buttonMessage->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));
	_buttonMessage->setEnabled(false);
	
	// 邮箱
	_buttonEmail = (Button*)_csNode->getChildByName("Button_email");
	_buttonEmail->addClickEventListener(CC_CALLBACK_1(ServiceLayer::onServiceClick, this));
	_buttonEmail->setEnabled(false);

	auto imageview_bg = (ImageView*)_csNode->getChildByName("Image_bg");
	auto bgRect = imageview_bg->getBoundingBox();

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
			if (bgRect.containsPoint(locationInNode)) return true;
			closeService();
			return true;
		}
		else
		{
			return false;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	requestServiceInfo();
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

	// 电话
	if (name.compare("Button_phone") == 0)
	{
		std::string param = StringUtils::format("{\"number\":\"%s\"}", _phone.c_str());
		Operator::requestChannel("sysmodule", "DealPhone", param);
	}

	// 短信
	else if (name.compare("Button_message") == 0)
	{
		std::string param = StringUtils::format("{\"number\":\"%s\"}", _message.c_str());
		Operator::requestChannel("sysmodule", "DealMsg", param);
	}

	// 邮箱
	else if (name.compare("Button_email") == 0)
	{
		std::string param = StringUtils::format("{\"to\":\"%s\", \"text\":\"\"}", _email.c_str());
		Operator::requestChannel("sysmodule", "SendEmail", param);
	}
	else
	{

	}
}

void ServiceLayer::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
{
	if (requestName.compare("service") == 0)
	{
		if (!isSucceed)
		{
			return;
		}

		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(responseData.c_str());
		if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray())
		{
			return;
		}

		for (auto i = 0; i < doc["content"].Size(); i++)
		{
			if (doc["content"][i].IsObject() && doc["content"][i].HasMember("ParaName") && doc["content"][i].HasMember("ParaValue"))
			{
				std::string name  = doc["content"][i]["ParaName"].GetString();
				std::string value = doc["content"][i]["ParaValue"].GetString();

				if (name.compare("CustomTel") == 0)
				{
					_phone = value;
				}
				else if (name.compare("CustomSms") == 0)
				{
					_message = value;
				}
				else if (name.compare("CustomEmail") == 0)
				{
					_email = value;
				}
				else
				{

				}
			}
		}

		if (!_phone.empty())
		{
			_buttonPhone->setEnabled(true);
		}

		if (!_message.empty())
		{
			_buttonMessage->setEnabled(true);
		}

		if (!_email.empty())
		{
			_buttonEmail->setEnabled(true);
		}

	}
}