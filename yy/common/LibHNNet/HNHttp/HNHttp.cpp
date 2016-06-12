#include "HNHttp.h"

USING_NS_CC;
using namespace network;

namespace HN
{
	static HNHttpRequest* s_hn_http_request = nullptr;

	HNHttpRequest* HNHttpRequest::getInstance()
	{
		if (s_hn_http_request == nullptr)
		{
			s_hn_http_request = new (std::nothrow) HNHttpRequest();
		}
		return s_hn_http_request;
	}

	void HNHttpRequest::destroyInstance()
	{
		delete s_hn_http_request;
		s_hn_http_request = nullptr;
	}

	void HNHttpRequest::request(const std::string& name, cocos2d::network::HttpRequest::Type type, const std::string& url, const std::string& param)
	{
		HttpRequest* request = new (std::nothrow) HttpRequest();
		request->setUrl(url.c_str());
		request->setRequestType(type);
		request->setResponseCallback(CC_CALLBACK_2(HNHttpRequest::onHttpResponse, this));
		request->setTag(name.c_str());

		if (type == HttpRequest::Type::POST && !param.empty())
		{
			request->setRequestData(param.c_str(), param.size());
		}

		HttpClient::getInstance()->sendImmediate(request);
		request->release();
	}

	void HNHttpRequest::addObserver(HNHttpDelegate* delegate)
	{
		auto observer = std::find(_addCache.begin(), _addCache.end(), delegate);
		if (observer == _addCache.end())
		{
			_addCache.push_back(delegate);
		}

		observer = std::find(_removeCache.begin(), _removeCache.end(), delegate);
		if (observer != _removeCache.end())
		{
			_removeCache.erase(observer);
		}		
	}

	void HNHttpRequest::removeObserver(HNHttpDelegate* delegate)
	{
		auto observer = std::find(_removeCache.begin(), _removeCache.end(), delegate);
		if (observer == _removeCache.end())
		{
			_removeCache.push_back(delegate);
		}

		observer = std::find(_addCache.begin(), _addCache.end(), delegate);
		if (observer != _addCache.end())
		{
			_addCache.erase(observer);
		}
	}

	HNHttpRequest::HNHttpRequest()
	{

	}

	HNHttpRequest::~HNHttpRequest()
	{

	}

	void HNHttpRequest::onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
	{
		// add cache
		for (auto observer : _addCache)
		{
			if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end())
			{
				_observers.push_back(observer);
			}
		}
		_addCache.clear();

		// remove cache
		for (auto observer : _removeCache)
		{
			auto iter = std::find(_observers.begin(), _observers.end(), observer);
			if (iter != _observers.end())
			{
				_observers.erase(iter);
			}
		}
		_removeCache.clear();


		bool bRet = response->isSucceed();
		std::string data;

		if (bRet)
		{
			std::vector<char> recv = *response->getResponseData();
			data.assign(recv.begin(), recv.end());
		}
		else
		{
			data = response->getErrorBuffer();
		}

		// dispatch
		for (auto observer : _observers)
		{
			observer->onHttpResponse(response->getHttpRequest()->getTag(), bRet, data);
		}
	}
}

