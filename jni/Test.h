#include <jni.h>
namespace my_test
{
    enum Types
    {
        METHOD, STATIC_METHOD, FIELD
    };

    struct decoder_sys_t
    {
        jclass jnitest_class;
        jmethodID printmsg;
        jobject myclass;
    };

    #define OFF(x) offsetof(struct decoder_sys_t, x)
    #define GET_INTEGER(obj, name) env->CallIntMethod( obj, p_sys->get_integer, env->NewStringUTF( name))

    struct classname
    {
        const char *name;
        int offset;
    };

    static const struct classname classes[] = {
        { "org/suirui/test/JniTest", OFF(jnitest_class) },
        { NULL, 0 },
    };

    struct member
    {
        const char *name;
        const char *sig;
        const char *pclass;
        int offset;
        int type;
    };

    static const struct member members[] = {
        { "printMessage", "(I)V", "org/suirui/test/JniTest", OFF(printmsg), METHOD },
        { NULL, NULL, NULL, 0, 0 },
    };

    class CppTest
    {
    public:
        CppTest();
        ~CppTest();
        int Init();
        void DoTest(char *msg);
    private:
        struct decoder_sys_t *p_sys;
    };
}