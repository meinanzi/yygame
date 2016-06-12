#include "HNMarket.h"
#include "HNJNIHelper.h"
#include "../../HNOperator.h"
#include <algorithm>
#include <map>
#include <assert.h>
#include <stdlib.h>	

#define	CLASS_NAME		"com/hn/market/export/GameChannel"

extern "C"
{
JNIEXPORT void JNICALL Java_com_hn_market_export_GameChannel_payCB(JNIEnv * env, jobject,
		jstring ID, jboolean isSucess, jstring args)
{
	const char* c_id = env->GetStringUTFChars(ID, 0);
	const char* c_args = env->GetStringUTFChars(args, 0);

	std::string str_id(c_id);
	std::string str_args(c_args);
	HN::Market::sharedMarket()->payCB(str_id, isSucess, str_args);

	env->ReleaseStringUTFChars(ID, c_id);
	env->ReleaseStringUTFChars(args, c_args);
}

JNIEXPORT void JNICALL Java_com_hn_market_export_GameChannel_responseChannel(
		JNIEnv * env, jobject, jstring callback, jstring args)
{
	//jsval* address_callback = (jsval*)((int)callback);

	const char* c_callback = env->GetStringUTFChars(callback, 0);
	const char* c_args = env->GetStringUTFChars(args, 0);

	std::string str_callback(c_callback);
	std::string str_args(c_args);
	HN::Market::sharedMarket()->responseChannel(str_callback, str_args);

	env->ReleaseStringUTFChars(callback, c_callback);
	env->ReleaseStringUTFChars(args, c_args);
}
}
;

namespace HN
{
static Market* sMarket = nullptr;

Market* Market::sharedMarket()
{
	if (nullptr == sMarket)
	{
		sMarket = new Market();
	}
	return sMarket;
}

Market::Market()
{

}

Market::~Market()
{
}

std::string Market::requestChannel(const std::string& module, const std::string& method,
		const std::string& args, CALLBACK_PRAGMA* callback)
{
	JNICM jnicm = HNJNIHelper::getStaticMethod(CLASS_NAME, "requestChannel",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
	if (!jnicm.vaild()) return "";
    
	char szCallBackBuff[128] = { 0 };

	if (nullptr != callback && callback->valid())
	{
		CALLBACK_PRAGMA * newCallBack = new CALLBACK_PRAGMA(*callback);
		sprintf(szCallBackBuff, "%d", (int)newCallBack);
	}
    
    jstring strmodule = jnicm.env->NewStringUTF(module.c_str());
    jstring strmethod = jnicm.env->NewStringUTF(method.c_str());
    jstring strargs = jnicm.env->NewStringUTF(args.c_str());
    jstring strcallback = jnicm.env->NewStringUTF(szCallBackBuff);
    jstring strValue = (jstring) jnicm.env->CallStaticObjectMethod(
            jnicm.classID, 
            jnicm.methodID, 
            strmodule, 
            strmethod, 
            strargs,
            strcallback);
            
    const char* jniRet = jnicm.env->GetStringUTFChars(strValue, nullptr);
    
    std::string ret = jniRet;
    
    jnicm.env->DeleteLocalRef(strmodule);
    jnicm.env->DeleteLocalRef(strmethod);
    jnicm.env->DeleteLocalRef(strargs);
    jnicm.env->DeleteLocalRef(strcallback);
    jnicm.env->ReleaseStringUTFChars(strValue, jniRet);
    jnicm.env->DeleteLocalRef(strValue);
    jnicm.env->DeleteLocalRef(jnicm.classID);
	return ret;
}

void Market::responseChannel(const std::string& callback, const std::string& args)
{
	int address_callback = atoi(callback.c_str());
	CALLBACK_PRAGMA* newCallback = (CALLBACK_PRAGMA*)address_callback;

	Operator::responseChannel(newCallback, args);

	delete newCallback;
}

void Market::pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback)
{
    assert(!identifier.empty());
    if (identifier.empty()) return;
    
    assert(!data.empty());
    if (data.empty()) return;
    
	JNICM jnicm = HNJNIHelper::getStaticMethod(CLASS_NAME, "pay", "(Ljava/lang/String;)V");
	if (!jnicm.vaild()) return;    
    
	registerCB(identifier, callback);
    
    jstring strData = jnicm.env->NewStringUTF(data.c_str());
    jnicm.env->CallStaticVoidMethod(jnicm.classID, jnicm.methodID, strData);
    jnicm.env->DeleteLocalRef(strData);
    jnicm.env->DeleteLocalRef(jnicm.classID);
}

void Market::payCB(const std::string& identifier, bool isSucess, const std::string& args)
{
	CALLBACK_PRAGMA* callback = findCB(identifier);
	if (nullptr != callback)
    {
        if (callback->valid())
        {
            try
            {
                Operator::payCB(callback, args);
            }
            catch (...)
            {

            }
            removeCB(identifier);    
        }
    }
}

void Market::registerCB(const std::string& identifier, CALLBACK_PRAGMA* callback)
{
    CALLBACK_PRAGMA newCallBack(*callback);
	auto value = std::make_pair(identifier, newCallBack);
	_payQueue.insert(value);
}

CALLBACK_PRAGMA* Market::findCB(const std::string& identifier)
{
	auto iter = _payQueue.find(identifier);
	if (iter != _payQueue.end())
	{
        CALLBACK_PRAGMA* callback = &iter->second;
		return callback;
	}
    else
    {
        return nullptr;
    }
}

void Market::removeCB(const std::string& identifier)
{
	auto iter = _payQueue.find(identifier);
	if (iter != _payQueue.end())
	{
		_payQueue.erase(iter);
	}
}
}
