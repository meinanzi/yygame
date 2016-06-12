#include "HNJNIHelper.h"
#include "platform/android/jni/JniHelper.h"
USING_NS_CC;

namespace HN
{
    JNICM HNJNIHelper::getStaticMethod(const char *classname, const char *methodname, const char *sig)
    {
        JNICM tjnicm;
        tjnicm.env = nullptr;
        tjnicm.classID = nullptr;
        tjnicm.methodID = nullptr;
        JavaVM* jVM = JniHelper::getJavaVM();
        jVM->AttachCurrentThread(&tjnicm.env, nullptr);
        if (tjnicm.env)
        {
            tjnicm.classID = tjnicm.env->FindClass(classname);
            jthrowable expClass = tjnicm.env->ExceptionOccurred();
            if(expClass)
            {
                tjnicm.env->ExceptionClear();
            }
            
            if (tjnicm.classID)
            {
                tjnicm.methodID= tjnicm.env->GetStaticMethodID(tjnicm.classID, methodname, sig);
                jthrowable expMethod = tjnicm.env->ExceptionOccurred();
                if(expMethod)
                {
                    tjnicm.env->ExceptionClear();
                }
            }
        }
        return tjnicm;
    }

    JNICM HNJNIHelper::getObjectMethod(jobject jobj, const char *methodname, const char *sig)
    {
        JNICM tjnicm;
        tjnicm.env = nullptr;
        tjnicm.classID = nullptr;
        tjnicm.methodID = nullptr;	
        JavaVM* jVM = JniHelper::getJavaVM();
        jVM->AttachCurrentThread(&tjnicm.env, nullptr);
        if (tjnicm.env)
        {
            tjnicm.classID = tjnicm.env->GetObjectClass(jobj);
            jthrowable expClass = tjnicm.env->ExceptionOccurred();
            if(expClass)
            {
                tjnicm.env->ExceptionClear();
            }

            if (tjnicm.classID)
            {
                tjnicm.methodID = tjnicm.env->GetStaticMethodID(tjnicm.classID, methodname, sig);
                jthrowable expMethod = tjnicm.env->ExceptionOccurred();
                if(expMethod)
                {
                    tjnicm.env->ExceptionClear();
                }
            }
        }
        return tjnicm;
    }

}
