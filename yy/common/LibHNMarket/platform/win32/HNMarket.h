#ifndef __KGMARKET_H__
#define __KGMARKET_H__

#include "HNModule.h"
#include "HNPayCallBack.h"
#include <string>
#include <unordered_map>

namespace HN
{
    class Module;
    
	class Market
	{
		typedef std::unordered_map<std::string, CALLBACK_PRAGMA> PAY_QUEUE;
		typedef PAY_QUEUE::iterator ITERATOR;

		create_declare(Market)
        
    public:
        // 注册模块接口
        void registerModule(const std::string& key, Module* module);
        
        // 脚本请求通道
        std::string requestChannel(const std::string& module, const std::string& method,
                                   const std::string& args, CALLBACK_PRAGMA*  callback);
        
		// 脚本回应通道
        void responseChannel(CALLBACK_PRAGMA*  callback, const std::string& args);
       
		void pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA*  callback);

		void payCB(const std::string& identifier, bool isSucess, const std::string& args);

	public:
		CALLBACK_PRAGMA* findCB(const std::string& identifier);
		void registerCB(const std::string& identifier, CALLBACK_PRAGMA* callback);
		void removeCB(const std::string& identifier);

	public:
        /**
         *  构造函数
         */
        Market();
        
        /**
         *  析构函数
         */
        ~Market();
        
    private:
        /**
         *  初始化系统模块
         */
        void InitModule();


	private:
        Controller<Module*> mModuleMap;

	private:
		PAY_QUEUE		_payQueue;
	};
}

#endif