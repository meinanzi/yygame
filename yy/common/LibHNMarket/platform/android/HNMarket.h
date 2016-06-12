#ifndef __HNMARKET_H__
#define __HNMARKET_H__

#include "../../HNPayCallBack.h"
#include <unordered_map>

namespace HN
{
	class Market
	{
		typedef std::unordered_map<std::string, CALLBACK_PRAGMA> PAY_QUEUE;
		typedef PAY_QUEUE::iterator ITERATOR;

	public:
		static Market* sharedMarket();
		
		void pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback);
		void payCB(const std::string& identifier, bool isSucess, const std::string& args);

    public:
        std::string requestChannel(const std::string& module, const std::string& method,
                                   const std::string& args, CALLBACK_PRAGMA* callback);
        
        void responseChannel(const std::string& callback, const std::string& args);
       
	public:
        Market();
        ~Market();
		
        CALLBACK_PRAGMA* findCB(const std::string& identifier);
		void registerCB(const std::string& identifier, CALLBACK_PRAGMA* callback);
		void removeCB(const std::string& identifier);

	private:
		PAY_QUEUE		_payQueue;
	};
}

#endif
