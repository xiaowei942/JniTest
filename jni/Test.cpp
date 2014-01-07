#include<iostream>
#include"Test.h"
#include<jni.h>
#include<android/log.h>
#include<assert.h>

namespace my_test
{
    my_test::CppTest *ct = NULL;

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

    int init(decoder_sys_t *p_sys)
    {
        int error = 0;
        my_log("*************** %s 1 ****************", __FUNCTION__);

        if ((p_sys = (decoder_sys_t *)calloc(1, sizeof(*p_sys))) == NULL)
            return -1;

        my_log("*************** %s 2 ****************", __FUNCTION__);

        JNIEnv* env = NULL;
        myVm->AttachCurrentThread(&env, NULL);
    #if 0
        for (int i = 0; classes[i].name; i++) {
            my_log( "%d" , i);
            *(jclass*)((uint8_t*)p_sys + classes[i].offset) = (jclass) env->NewGlobalRef (
                (env)->FindClass(classes[i].name) );

            if ((env)->ExceptionOccurred()) {
                my_log("Unable to find class %s", classes[i].name);
                (env)->ExceptionClear();
                goto error;
            }
        }

        my_log("*************** %s 3 ****************", __FUNCTION__);
        jclass last_class;
        for (int i = 0; members[i].name; i++) {
            if (i == 0 || strcmp(members[i].pclass, members[i - 1].pclass))
                last_class = env->FindClass(members[i].pclass);

            if (env->ExceptionOccurred()) {
                my_log("Unable to find class %s", members[i].pclass);
                env->ExceptionClear();
                goto error;
            }

            switch (members[i].type) {
            case METHOD:
                *(jmethodID*)((uint8_t*)p_sys + members[i].offset) =
                    env->GetMethodID(last_class, members[i].name, members[i].sig);
                break;
            case STATIC_METHOD:
                *(jmethodID*)((uint8_t*)p_sys + members[i].offset) =
                    env->GetStaticMethodID(last_class, members[i].name, members[i].sig);
                break;
            case FIELD:
                *(jfieldID*)((uint8_t*)p_sys + members[i].offset) =
                    env->GetFieldID(last_class, members[i].name, members[i].sig);
                break;
            }
            if (env->ExceptionOccurred()) {
                my_log("Unable to find the member[%d] %s in %s", i,
                         members[i].name, members[i].pclass);
                env->ExceptionClear();
                goto error;
            }
        }
    #endif
        my_log("Init success");
        //myVm->DetachCurrentThread();
        return 1;
error:
        //myVm->DetachCurrentThread();
        return -1;
        }
}

CppTest::CppTest()
{
    p_sys = new decoder_sys_t();
    my_log("Into CppTest Constructure");
}

CppTest::~CppTest()
{

    delete(p_sys);
    p_sys=NULL;
    my_log("Into CppTest Destructure");
    myVm->DetachCurrentThread();
}

int CppTest::Init()
{
    return init(p_sys);
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

    my_log("Now call the JniTest class method");
    env->CallVoidMethod(p_sys->jnitest_class, p_sys->printmsg, obj);
}

    extern "C" {
        JNIEXPORT int JNICALL Java_org_suirui_test_MyActivity_JniMethodInit(JNIEnv *, jobject)
        {
            ct = new my_test::CppTest();
            return ct->Init();
        }

        JNIEXPORT void JNICALL Java_org_suirui_test_MyActivity_DoTest(JNIEnv *, jobject)
        {
            ct->DoTest("Test");
        }
    }
}