#ifndef __HNSDK__Market__
#define __HNSDK__Market__

#include <string>
#include "HNModule.h"

namespace HN
{
    class Module;
    
	class Market
	{
		create_declare(Market)
        
    public:
        // 注册模块接口
        void registerModule(const std::string& key, Module* module);
        
        // 脚本请求通道
        std::string requestChannel(const std::string& module, const std::string& method,
                                   const std::string& args, CALLBACK_PRAGMA* callback);
        
		// 脚本回应通道
        void responseChannel(CALLBACK_PRAGMA* callback, const std::string& args);
       
        void pay(const std::string& args, const std::string& data, CALLBACK_PRAGMA* callback);
        
        void payCB(CALLBACK_PRAGMA* callback, const std::string& args);
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
	};
}

#endif      //__HNSDK__Market__