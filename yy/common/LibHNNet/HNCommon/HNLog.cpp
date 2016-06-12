#include "HNLog.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

namespace HN 
{

	static void _log(const char *format, va_list args, HNLOG_LEVEL errorLevel)
	{
		char buf[MAX_LOG_LENGTH];

		vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
		strcat(buf, "\n");

	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (errorLevel == LOG_DEBUG)
		{
			__android_log_print(ANDROID_LOG_DEBUG, "hn",  "%s", buf);
		}
		else if (errorLevel == LOG_INFO)
		{
			__android_log_print(ANDROID_LOG_INFO, "hn",  "%s", buf);
		}
		else if (errorLevel == LOG_WARN)
		{
			__android_log_print(ANDROID_LOG_WARN, "hn",  "%s", buf);
		}
		else if (errorLevel == LOG_ERROR)
		{
			__android_log_print(ANDROID_LOG_ERROR, "hn",  "%s", buf);
		}

	#elif CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
		WCHAR wszBuf[MAX_LOG_LENGTH] = {0};
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, wszBuf, sizeof(wszBuf));
		OutputDebugStringW(wszBuf);
		WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, buf, sizeof(buf), nullptr, FALSE);
		printf("%s", buf);
		fflush(stdout);
	#else
		// Linux, Mac, iOS, etc
		fprintf(stdout, "cocos2d: %s", buf);
		fflush(stdout);
	#endif

	#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
		Director::getInstance()->getConsole()->log(buf);
	#endif
	}

	void HNLog::logDebug(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		_log(format, args, HNLOG_LEVEL::LOG_DEBUG);
		va_end(args);
	}

	void HNLog::logError(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		_log(format, args, HNLOG_LEVEL::LOG_ERROR);
		va_end(args);
	}

	void HNLog::logWarning(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		_log(format, args, HNLOG_LEVEL::LOG_WARN);
		va_end(args);
	}

	void HNLog::logInfo(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		_log(format, args, HNLOG_LEVEL::LOG_INFO);
		va_end(args);
	}

	void HNLog::logFile(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char buf[MAX_LOG_LENGTH];

		vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
		strcat(buf, "\n");

		va_end(args);
	}

}
