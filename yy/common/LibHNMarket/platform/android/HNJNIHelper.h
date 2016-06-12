#ifndef __HNJniHelper_H__
#define __HNJniHelper_H__

#include <jni.h>


namespace HN
{

	struct JNICM {
		JNIEnv* env;
		jclass 	classID;
		jmethodID methodID;

		bool vaild()
		{
			return env != nullptr;
		}

	};

    class HNJNIHelper
    {
    public:
        static JNICM getStaticMethod(const char *classname, const char *methodname, const char *sig);

        static JNICM getObjectMethod(jobject jobj, const char *methodname, const char *sig);
    };
}

#endif  //__HNJniHelper_H__
