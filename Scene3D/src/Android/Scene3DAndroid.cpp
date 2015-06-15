// Wrapper of Scene3D.h for Android JNI
#include <jni.h>
#include <android/log.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "CAndroidAssetFileArchive.h"

#define printf(...) __android_log_print(ANDROID_LOG_INFO, "Irrlicht", __VA_ARGS__);
#include "FullScreenSceneNode.h"

#include "../Scene3D.h"

extern "C"
{
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
    {
        os::Printer::Logger = new CLogger(NULL);

        return JNI_VERSION_1_2;
    }

    JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
    {
        // delete os::Printer::Logger;
        // delete driver;
        // delete smgr;
        // delete fs;
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_initializeFileSystem(JNIEnv * env, jclass cls, jstring jInternalDataPath, jobject assetManager)
    {
        print("initializeFileSystem()");

        AAssetManager* amgr = AAssetManager_fromJava(env, assetManager);

        // TODO: recreation

        fs = io::createFileSystem();
        io::CAndroidAssetFileArchive* assets = new io::CAndroidAssetFileArchive(amgr, false, false);
        assets->addDirectoryToFileList("");
        fs->addFileArchive(assets);
        assets->drop();

        /* Your media must be somewhere inside the assets folder. The assets folder is the root for the file system.
           This example copies the media in the Android.mk makefile. */
        io::path mediaPath = "media/";

        // The Android assets file-system does not know which sub-directories it has (blame google).
        // So we have to add all sub-directories in assets manually. Otherwise we could still open the files,
        // but existFile checks will fail (which are for example needed by getFont).
        for ( u32 i = 0; i < fs->getFileArchiveCount(); ++i )
        {
            io::IFileArchive* archive = fs->getFileArchive(i);
            if ( archive->getType() == io::EFAT_ANDROID_ASSET )
            {
                archive->addDirectoryToFileList(mediaPath);
                break;
            }
        }

        // assimp filesystem
        const char *internalDataPath = env->GetStringUTFChars(jInternalDataPath, JNI_FALSE);
        setAssimpJNIAssetPath(internalDataPath, amgr);
        env->ReleaseStringUTFChars(jInternalDataPath, internalDataPath);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addPointLight(JNIEnv * env, jclass cls, jfloat radius)
    {
        return Scene3D_addPointLight(radius);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_resize(JNIEnv * env, jclass cls, jint width, jint height)
    {
        Scene3D_resize(width, height);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_clear(JNIEnv * env, jclass cls)
    {
        Scene3D_clear();
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_render(JNIEnv * env, jclass cls)
    {
        Scene3D_render();
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addCubeNode(JNIEnv * env, jclass cls, jfloat size)
    {
        return Scene3D_addCubeNode(size);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addSphereNode(JNIEnv * env, jclass cls, jfloat radius)
    {
        return Scene3D_addSphereNode(radius);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_getTexture(JNIEnv * env, jclass cls, jstring jTextureName)
    {
        const char *textureName = env->GetStringUTFChars(jTextureName, JNI_FALSE);
        jlong texture = Scene3D_getTexture(textureName);
        env->ReleaseStringUTFChars(jTextureName, textureName);

        return texture;
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeTextureAtLayer(JNIEnv * env, jclass cls, jlong nodePtr, jint textureLayer, jlong texturePtr)
    {
        Scene3D_setNodeTextureAtLayer(nodePtr, textureLayer, texturePtr);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationFps(JNIEnv * env, jclass cls, jlong nodePtr, jfloat fps)
    {
        Scene3D_setNodeAnimationFps(nodePtr, fps);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationStartEnd(JNIEnv * env, jclass cls, jlong nodePtr, jint start, jint end)
    {
        Scene3D_setNodeAnimationStartEnd(nodePtr, start, end);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimation(JNIEnv * env, jclass cls, jlong nodePtr, jstring jAnimationName)
    {
        const char *animationName = env->GetStringUTFChars(jAnimationName, JNI_FALSE);
        Scene3D_setNodeAnimation(nodePtr, animationName);
        env->ReleaseStringUTFChars(jAnimationName, animationName);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationIndex(JNIEnv * env, jclass cls, jlong nodePtr, jlong index)
    {
        Scene3D_setNodeAnimationIndex(nodePtr, index);
    }    

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addDummyNode(JNIEnv * env, jclass cls)
    {
        return Scene3D_addDummyNode();
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addMeshNode(JNIEnv * env, jclass cls, jstring jMeshName)
    {
        const char *meshName = env->GetStringUTFChars(jMeshName, JNI_FALSE);
        jlong node = Scene3D_addMeshNode(meshName);
        env->ReleaseStringUTFChars(jMeshName, meshName);

        return node;
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_getRootNode(JNIEnv *env, jclass cls)
    {
        return Scene3D_getRootNode();
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeVisible(JNIEnv *env, jclass cls, jlong nodePtr, jboolean visible)
    {
        Scene3D_setNodeVisible(nodePtr, visible);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodePosition(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D_setNodePosition(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeRotation(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D_setNodeRotation(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeScale(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D_setNodeScale(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeParent(JNIEnv * env, jclass cls, jlong nodePtr, jlong parentPtr)
    {
        Scene3D_setNodeParent(nodePtr, parentPtr);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeModelMatrix(JNIEnv * env, jclass cls, jlong nodePtr, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D_setNodeModelMatrix(nodePtr, matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setViewMatrix(JNIEnv * env, jclass cls, jlong nodePtr, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D_setViewMatrix(nodePtr, matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setProjectionMatrix(JNIEnv * env, jclass cls, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D_setProjectionMatrix(matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addTexture(JNIEnv * env, jclass cls, jint width, jint height)
    {
        return Scene3D_addTexture(width, height);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_updateTexture(JNIEnv * env, jclass cls, jlong texturePtr, jbyteArray jImageData)
    {
        jbyte *srcData = env->GetByteArrayElements(jImageData, JNI_FALSE);
        Scene3D_updateTexture(texturePtr, srcData);
        env->ReleaseByteArrayElements(jImageData, srcData, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addFullScreenTextureNode(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        return Scene3D_addFullScreenTextureNode(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_drawFullScreenTexture(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        Scene3D_drawFullScreenTexture(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_writeTexture(JNIEnv *env, jclass cls, jlong texturePtr, jstring jFilename)
    {
        const char* filename = env->GetStringUTFChars(jFilename, JNI_FALSE);
        Scene3D_writeTexture(texturePtr, filename);
        env->ReleaseStringUTFChars(jFilename, filename);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_pickNodeFromScreen(JNIEnv *env, jclass cls, jint x, jint y)
    {
        return Scene3D_pickNodeFromScreen(x, y);
    }

}