#ifndef HNMarketMarco_h__
#define HNMarketMarco_h__

#include <string>
#include <functional>

typedef std::function<void(const std::string& args)> JNI_CALLBACK;

#define HN_PAY_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)


class CALLBACK_PRAGMA
{
public:
	CALLBACK_PRAGMA() : selector(nullptr)
	{
	}

	CALLBACK_PRAGMA(JNI_CALLBACK selector) : selector(selector)
	{
	}

public:
	void doCallSelector(const std::string& args)
	{
		(selector)(args);
	}

	bool valid()
	{
		return nullptr != this->selector;
	}
public:
	JNI_CALLBACK	selector;
};


#endif // HNMarketMarco_h__

