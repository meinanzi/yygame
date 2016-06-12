#ifndef __NH_HNLOG_H__
#define __NH_HNLOG_H__

#include "cocos2d.h"
USING_NS_CC;

// cocos2d debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define HNLOG(...) do {} while (0)
#define HNLOG_DEBUG(...)       do {} while (0)
#define HNLOG_ERROR(...)   do {} while (0)
#define HNLOG_INFO(...)  do {} while (0)
#define HNLOG_WARNING(...)   do {} while (0)

#elif (COCOS2D_DEBUG == 1)
#define HNLOG(format,...)			HNLog::logDebug("%s (%s:%d) " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_DEBUG(format,...)		HNLog::logDebug("Text >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_ERROR(format,...)		HNLog::logError("Error >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_INFO(format,...)		HNLog::logInfo("Information >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_WARNING(format,...)	HNLog::logWarning("Warning >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#elif COCOS2D_DEBUG > 1
#define HNLOG(format,...)			HNLog::logDebug("%s (%s:%d) " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_DEBUG(format,...)		HNLog::logDebug("Text >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_ERROR(format,...)		HNLog::logError("Error >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_INFO(format,...)		HNLog::logInfo("Information >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define HNLOG_WARNING(format,...)	HNLog::logWarning("Warning >> (%s:%d) " format, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define HNLOG(...) 
#define HNLOG_DEBUG(...)	
#define HNLOG_ERROR(...)		
#define HNLOG_INFO(...)		
#define HNLOG_WARNING(...)	
#endif // COCOS2D_DEBUG

namespace HN 
{
	enum HNLOG_LEVEL
	{
		LOG_DEBUG = 0,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR
	};

	class HNLog
	{
	public:
		static void logDebug(const char* format, ...);
		static void logError(const char* format, ...);
		static void logInfo(const char* format, ...);
		static void logWarning(const char* format, ...);
		static void logFile(const char* format, ...);
	};
};

#endif		//__NH_HNLOG_H__
