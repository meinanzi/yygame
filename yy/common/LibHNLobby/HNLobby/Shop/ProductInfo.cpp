#include "HNLobby/Shop/ProductInfo.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace HN 
{
	std::string PRODUCT_INFO::serialize()
	{
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("identifier", rapidjson::StringRef(identifier.c_str()), allocator);
		object.AddMember("number", number, allocator);
		object.AddMember("price", price, allocator);
		object.AddMember("payType", payType, allocator);
		object.AddMember("xmlFile", rapidjson::StringRef(xmlFile.c_str()), allocator);
		object.AddMember("orderID", rapidjson::StringRef(orderID.c_str()), allocator);
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
		object.Accept(write);
		return buffer.GetString();
	}

	void PRODUCT_INFO::deserialize(std::string data)
	{
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if (!doc.HasParseError())
		{
			if (doc.IsObject() && doc.HasMember("status"))
			{
				identifier = doc["identifier"].GetString();
				number = doc["number"].GetInt64();
				price = doc["price"].GetDouble();
				payType = doc["payType"].GetInt();
				xmlFile = doc["xmlFile"].GetString();
				orderID = doc["orderID"].GetString();
			}
		}
	}
}                                                  