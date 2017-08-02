//
// Created by we.kim on 2017-07-25.
//

#ifndef _COMMON_H
#define _COMMON_H

#include <android/log.h>
#include <GLES3/gl3.h>
#define DEBUG 1

#define real GLfloat
#define EPSILON		0.0001f

#define LOG_TAG "RenderEngine"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#endif