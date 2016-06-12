#include "HNLobby/Shop/ShopManager.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <algorithm>
#include <string>
#include <cstdlib>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "spine/Json.h"

using namespace network;

#define JSON_RESOVE(obj, key) (obj.HasMember(key) && !obj[key].IsNull())

namespace HN 
{
	static ProductManger* pruductManager = nullptr;

	ProductManger* ProductManger::getInstance()
	{        
		if (nullptr == pruductManager)
		{
			pruductManager = new (std::nothrow) ProductManger();
			return pruductManager;
		}
		return pruductManager;
	}

	void ProductManger::destroyInstance()
	{
		CC_SAFE_DELETE(pruductManager);
	}

	ProductManger::ProductManger()
	{

	}

	ProductManger::~ProductManger()
	{
		HNHttpRequest::getInstance()->removeObserver(this);
	}


	void ProductManger::onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData)
	{
		if (requestName.compare("products") == 0)
		{
			if (!isSucceed)
			{
				return;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(responseData.c_str());
			if (!doc.IsObject())
			{
				return;
			}

			int rs = 0;
			if (JSON_RESOVE(doc, "rs")) rs = doc["rs"].GetInt();

			if (rs)
			{
				rapidjson::Value& _array = doc["list"];
				if (_array.IsArray())
				{
					_products.clear();
					for (int i = 0; i < _array.Size(); ++i)
					{
						auto product = new PRODUCT_INFO();
						product->identifier = _array[i]["Remark"].GetString();
						product->price = atof(_array[i]["PayRMB"].GetString());
						product->number = atoi(_array[i]["PayChip"].GetString());

						_products.push_back(product);

					}
				}
			}
		}
	}

	void ProductManger::addProducts(const std::string& nameId)
	{
		std::string id = nameId;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		id.append("-ios");
#else
		id.append("-android");
#endif

		HNHttpRequest::getInstance()->addObserver(this);
		HNHttpRequest::getInstance()->request("products", HttpRequest::Type::GET, PlatformConfig::getInstance()->getGoodsListUrl(id).c_str());
	}

	UINT ProductManger::getSize() const
	{
		return (UINT)_products.size();
	}

	const PRODUCT_INFO* ProductManger::getProducts(INT index)
	{
		return _products.at(index);
	}

	const PRODUCT_INFO* ProductManger::getProducts(std::string& identifier)
	{
		std::vector<PRODUCT_INFO*>::iterator iter = _products.end();
		iter = std::find_if(_products.begin(), _products.end(), [&identifier](PRODUCT_INFO* product)
		{
			return identifier.compare(product->identifier);
		});
		if (iter != _products.end())
		{
			return *iter;
		}
		return nullptr;
	}

	bool ProductManger::startPay(PRODUCT_INFO *product, CALLBACK_PRAGMA* callback)
	{
		Operator::sharedOperator()->pay(product->identifier, product->serialize(), callback);
		return true;
	}

	bool ProductManger::parsePayResult(const std::string& result)
	{
		bool ret = false;
		do 
		{
			if (result.empty()) break;

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(result.c_str());
			if (doc.HasParseError()) break;

			INT status = 0;
			if (doc.IsObject() && doc.HasMember("status"))
			{
				status = doc["status"].GetInt();
			}

			ret = (status == 1);
		} while (0);

		return ret;
	}

	bool ProductManger::parsePayResult(const std::string& result, std::string& ext)
	{
		bool ret = false;
		do
		{
			if (result.empty()) break;

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(result.c_str());
			if (doc.HasParseError()) break;

			INT status = 0;
			if (doc.IsObject() && doc.HasMember("status"))
			{
				status = doc["status"].GetInt();
			}

			if(doc.IsObject() && doc.HasMember("ext"))
			{
				ext = doc["ext"].GetString();
			}

			ret = (status == 1);
		} while (0);

		return ret;
	}
}