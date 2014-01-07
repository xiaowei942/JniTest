#include<iostream>
#include"Test.h"
#include<jni.h>
#include<android/log.h>
#include<assert.h>

namespace my_test
{
    my_test::CppTest *ct = NULL;
    jclass clazz;
    jobject object;

extern "C" {
    static void my_log(const char *format, ...)
    {
        va_list list;
        va_start(list, format);
        __android_log_vprint(ANDROID_LOG_DEBUG, "MYTEST", format, list);
        va_end(list);
    }

    static JavaVM *myVm;
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *ajvm, void *reserved) {
    	myVm = ajvm;

    	JNIEnv *myEnv = NULL;
    	if(myVm->GetEnv((void **)&myEnv, JNI_VERSION_1_4) != JNI_OK)
    	{
    		return -1;
    	}
    	assert(myEnv != NULL);
        my_log("Jni version is 1.4");
    	return JNI_VERSION_1_4;
    }
}

CppTest::CppTest()
{
    my_log("Into CppTest Constructure");
}

CppTest::~CppTest()
{
    delete(p_sys);
    p_sys=NULL;
    my_log("Into CppTest Destructure");
    myVm->DetachCurrentThread();
}

void CppTest::DoTest(char *msg)
{
    my_log("---------- Into: %s -----------", __FUNCTION__);
    JNIEnv *env;
    myVm->AttachCurrentThread(&env, NULL);
    jclass cls = env->FindClass("java/lang/String");
    if(env->ExceptionOccurred())
    {
        my_log("Cannot find java class: String");
        env->ExceptionClear();
        return;
    }
    my_log("Success find java class: String");

    jmethodID mtd = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
    if(env->ExceptionOccurred())
    {
        my_log("Cannot find java function: String()");
        env->ExceptionClear();
        return;
    }
    my_log("Success find java function: String()");

    jstring message = env->NewStringUTF(msg);
    jobject obj = env->NewObject(cls,mtd, message);
    if(env->ExceptionOccurred())
    {
        my_log("Cannot new a object");
        env->ExceptionClear();
        return;
    }
    my_log("Success new a object");

    jclass cls_jnitest = env->GetObjectClass(object);
    if(env->ExceptionOccurred())
    {
        my_log("Cannot find JniTest class");
        env->ExceptionClear();
        return;
    }
    my_log("Success find jnitest class");

    jmethodID mid = env->GetMethodID(cls_jnitest, "printMessage", "(I)V");
    if(env->ExceptionOccurred())
    {
        my_log("Cannot find class method printMessage");
        return;
    }
    my_log("Success get method id");
    my_log("Now call the JniTest class method");
    char *tmp = "TestJni";
    jstring jstr = env->NewStringUTF(tmp);
    //env->CallVoidMethod(p_sys->jnitest_class, p_sys->printmsg, obj);
    env->CallVoidMethod(cls_jnitest, mid, 2);
}

    extern "C" {
        JNIEXPORT void JNICALL Java_org_suirui_test_MyActivity_SetEnv(JNIEnv *env, jobject obj)
        {
            my_log("---------- Into: %s -----------", __FUNCTION__);
            ct = new my_test::CppTest();
            if(!ct) {
                my_log("Error when new a CppTest object", __FUNCTION__);
                return;
            }
            ct->set_env();

            my_log("Now save MyActivity object");
            object = env->NewGlobalRef(obj); //这个object和MyActivity相关，可以后面用GetObjectClass来获取MyActivity类

            my_log("---------- Out: %s -----------", __FUNCTION__);
        }

        JNIEXPORT void JNICALL Java_org_suirui_test_MyActivity_RunCppThread(JNIEnv *, jobject)
        {
            my_log("---------- Into: %s -----------", __FUNCTION__);
            ct = new my_test::CppTest();
            if(!ct) {
                my_log("Error when new a CppTest object", __FUNCTION__);
                return;
            }
            ct->set_env();
            my_log("---------- Out: %s -----------", __FUNCTION__);
        }
        JNIEXPORT void JNICALL Java_org_suirui_test_MyActivity_DoTest(JNIEnv *, jobject)
        {
            my_log("---------- Into: %s -----------", __FUNCTION__);
            if(!ct) {
                my_log("Error CppTest object is null", __FUNCTION__);
                return;
            }
            ct->DoTest();
            my_log("---------- Out: %s -----------", __FUNCTION__);
        }
    }

void CppTest::set_env()
{
    my_log("---------- Into: %s -----------", __FUNCTION__);
    JNIEnv *env = NULL;
    myVm->AttachCurrentThread(&env, NULL);

    my_log("Now find JniTest class");
    jclass cls = env->FindClass("org/suirui/test/JniTest");
    if(env->ExceptionOccurred())
    {
        my_log("Cannot find JniTest class");
        env->ExceptionClear();
        return;
    }
    my_log("Now save clazz");
    clazz = (jclass)env->NewGlobalRef(cls);
}

void CppTest::DoTest()
{
    my_log("---------- Into: %s -----------", __FUNCTION__);
    JNIEnv *env;
    myVm->AttachCurrentThread(&env, NULL);

    jclass clazz1 = clazz;
    jclass clazz2;

    if(object) {
        clazz2 = env->GetObjectClass(object);
    }

    if(clazz1)
    {
        my_log("Class JniTest is not null");

        jmethodID mid1_1 = env->GetMethodID(clazz1, "<init>", "()V");
        jobject clsobj1 = env->NewObject(clazz1, mid1_1, "");

        my_log("Success 1");
        jfieldID fid = env->GetFieldID(clazz1, "test", "I");

        my_log("Success 2");
        int test = env->GetIntField(clsobj1, fid);
        my_log("test in JniTest is: %d", test);

        jmethodID mid1_2 = env->GetMethodID(clazz1, "printMessage", "()V");
        if(env->ExceptionOccurred())
        {
            my_log("Cannot find class method printMessage");
            env->ExceptionClear();
            return;
        }
        my_log("Success get method id");
        env->CallVoidMethod(clsobj1, mid1_2, "");
    }

    if(clazz2)
    {
        my_log("Class MyActivity is not null");

        jmethodID mid2_1 = env->GetMethodID(clazz2, "<init>", "()V");
        jobject clsobj2 = env->NewObject(clazz2, mid2_1, "");

        jfieldID fid = env->GetFieldID(clazz2, "test", "I");

        int test = env->GetIntField(clsobj2, fid);
        my_log("test in MyActivity is: %d", test);

        jmethodID mid2_2 = env->GetMethodID(clazz2, "printMessage", "()V");
        if(env->ExceptionOccurred())
        {
            my_log("Cannot find class method printMessage");
            env->ExceptionClear();
            return;
        }
        my_log("Success get method id");
        env->CallVoidMethod(clsobj2, mid2_2, "");
    }
    //myVm->DetachCurrentThread();
}
}