//
// Created by we.kim on 2017-06-26.
//

//
// Created by we.kim on 2017-06-20.
//
// Glue codes for android native interface

#include<jni.h>
#include<android/input.h>
#include "RenderEngine.h"

static APP* g_renderer = NULL;

static APP* CreateRenderApp()
{
    g_renderer = APP::GetInstance();
    if(!g_renderer->init())
    {
        g_renderer->destroy();
        g_renderer = NULL;
    }
    return g_renderer;
}

static void PrintGLString(const char* name, GLenum s)
{
    const char* result = (const char*) glGetString(s);
    ALOGV("GL %s : %s\n", name, result);
}
extern "C"
{
JNIEXPORT void JNICALL Java_com_samsung_esrenderengine_RenderApp_init(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_samsung_esrenderengine_RenderApp_resize(JNIEnv* env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_samsung_esrenderengine_RenderApp_draw(JNIEnv* env, jobject obj);
JNIEXPORT void JNICALL Java_com_samsung_esrenderengine_RenderApp_onTouchEvent(JNIEnv* env, jobject obj, jint action,
                                                                              jintArray id, jintArray x, jintArray y, jint count);
}

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

JNIEXPORT void JNICALL Java_com_samsung_esrenderengine_RenderApp_init(JNIEnv* env, jobject obj)
{
    ALOGD("RenderEngineJNIGlue, init called.");
    PrintGLString("Version", GL_VERSION);
    PrintGLString("Vender", GL_VENDOR);
    PrintGLString("Renderer", GL_RENDERER);
    PrintGLString("Extensions", GL_EXTENSIONS);

    g_renderer = CreateRenderApp();
}

JNIEXPORT void JNICALL
Java_com_samsung_esrenderengine_RenderApp_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    ALOGD("RenderEngineJNIGlue, resize called.");
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
    else ALOGE("RenderEngineJNIGlue, g_renderer has died on resize!");
}

JNIEXPORT void JNICALL
Java_com_samsung_esrenderengine_RenderApp_draw(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        g_renderer->render();
    }
    else ALOGE("RenderEngineJNIGlue, g_renderer has died on draw!");
}

JNIEXPORT void JNICALL
Java_com_samsung_esrenderengine_RenderApp_onTouchEvent(JNIEnv* env, jobject obj, jint action, jintArray id, jintArray x, jintArray y, jint count) {
    if (g_renderer) {
        jint* tid = env->GetIntArrayElements(id, NULL);
        jint* tx = env->GetIntArrayElements(x, NULL);
        jint* ty = env->GetIntArrayElements(y, NULL);
        g_renderer->onTouchEvent(action, tid, tx, ty, count);
    }
    else ALOGE("RenderEngineJNIGlue, g_renderer has died on touch event!");
}
