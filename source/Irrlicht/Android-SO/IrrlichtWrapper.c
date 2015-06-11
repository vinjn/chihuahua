#include <jni.h>
#include <android/log.h>

#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* These functions are called from Java. */

JNIEXPORT void JNICALL Java_com_hiscene_Irrlicht_clear(JNIEnv * env, jclass cls) {
	ALOGE("clear\n");
}
