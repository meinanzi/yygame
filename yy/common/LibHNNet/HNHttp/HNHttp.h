#ifndef __HN_Http_H__
#define __HN_Http_H__

#include <string>
#include "cocos2d.h"
#include "network/HttpClient.h"

namespace HN
{
	class HNHttpDelegate
	{
	public:
		virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) = 0;
	};

	class HNHttpRequest
	{
	public:
		static HNHttpRequest* getInstance();
		static void destroyInstance();
		void request(const std::string& name, cocos2d::network::HttpRequest::Type type, const std::string& url, const std::string& param = "");
		void addObserver(HNHttpDelegate* delegate);
		void removeObserver(HNHttpDelegate* delegate);

	public:
		HNHttpRequest();
		virtual ~HNHttpRequest();

	protected:
		void onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

	private:
		std::vector<HNHttpDelegate*> _addCache;
		std::vector<HNHttpDelegate*> _removeCache;
		std::vector<HNHttpDelegate*> _observers;

	};
}

#endif