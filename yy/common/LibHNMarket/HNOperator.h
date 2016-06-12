#ifndef __HNOperator_H__
#define __HNOperator_H__

#include "HNPayCallBack.h"
#include <string>

namespace HN
{
	class Operator
	{
	public:
		Operator();
		static Operator* sharedOperator();

		static std::string requestChannel(const std::string& module, const std::string& method,
                                          const std::string& args = "", CALLBACK_PRAGMA* callback = nullptr);
        static void responseChannel(CALLBACK_PRAGMA* callback, const std::string& args);

		static void pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback);
		static void payCB(CALLBACK_PRAGMA* callback, const std::string& args);
	};
}

#endif	//__HNOperator_H__
