#ifndef _HN_HNMatchInfoCache_h__
#define _HN_HNMatchInfoCache_h__

#include "HNNetProtocol/HNProtocolExport.h"
#include <vector>
#include <functional>

namespace HN
{

	class HNMatchInfoCache
	{
	public:
		// 获取比赛信息单例
		static HNMatchInfoCache* getInstance();

		// 重置比赛信息
		void 

		// 构造函数
		HNMatchInfoCache();

		// 析构函数
		virtual ~HNMatchInfoCache();
	};
}

#define MatchInfoCache()	HNMatchInfoCache::getInstance()

#endif // _HN_HNMatchInfoModule_h__
