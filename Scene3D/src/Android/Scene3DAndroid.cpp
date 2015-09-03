// Wrapper of Scene3D.h for Android JNI
#include "irrlicht.h"
#include "../source/irrlicht/os.h"
#include "../source/irrlicht/CLogger.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "CAndroidAssetFileArchive.h"

#include "../Scene3D.h"

using namespace irr;
using namespace core;

extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern scene::ISceneNode* arRootNode; // arRootNode's parent = dummy node
extern scene::ICameraSceneNode* camera;
extern io::IFileSystem* fs;
extern int screenWidth, sceenHeight;

namespace irr
{
    namespace io
    {
        IFileSystem* createFileSystem();
    }
}

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
        printf("initializeFileSystem()");

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
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addPointLight(JNIEnv * env, jclass cls, jfloat radius)
    {
        return Scene3D::addPointLight(radius);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_resize(JNIEnv * env, jclass cls, jint width, jint height)
    {
        Scene3D::resize(width, height);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_clear(JNIEnv * env, jclass cls)
    {
        Scene3D::clear();
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_render(JNIEnv * env, jclass cls)
    {
        Scene3D::render();
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_getNodeFromName(JNIEnv * env, jclass cls, jstring jNodeName)
    {
        const char *nodeName = env->GetStringUTFChars(jNodeName, JNI_FALSE);
        jlong node = Scene3D::getNodeFromName(nodeName);
        env->ReleaseStringUTFChars(jNodeName, nodeName);

        return node;
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_loadScene(JNIEnv * env, jclass cls, jstring jSceneFileName)
    {
        const char *sceneFileName = env->GetStringUTFChars(jSceneFileName, JNI_FALSE);
        jlong node = Scene3D::loadScene(sceneFileName);
        env->ReleaseStringUTFChars(jSceneFileName, sceneFileName);

        return node;
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addCubeNode(JNIEnv * env, jclass cls, jfloat size)
    {
        return Scene3D::addCubeNode(size);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addPlaneNode(JNIEnv * env, jclass cls, jfloat width, jfloat height)
    {
        return Scene3D::addPlaneNode(width, height);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addSphereNode(JNIEnv * env, jclass cls, jfloat radius)
    {
        return Scene3D::addSphereNode(radius);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_getTexture(JNIEnv * env, jclass cls, jstring jTextureName)
    {
        const char *textureName = env->GetStringUTFChars(jTextureName, JNI_FALSE);
        jlong texture = Scene3D::getTexture(textureName);
        env->ReleaseStringUTFChars(jTextureName, textureName);

        return texture;
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeTextureAtLayer(JNIEnv * env, jclass cls, jlong nodePtr, jint textureLayer, jlong texturePtr)
    {
        Scene3D::setNodeTextureAtLayer(nodePtr, textureLayer, texturePtr);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationFps(JNIEnv * env, jclass cls, jlong nodePtr, jfloat fps)
    {
        Scene3D::setNodeAnimationFps(nodePtr, fps);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationStartEnd(JNIEnv * env, jclass cls, jlong nodePtr, jint start, jint end)
    {
        Scene3D::setNodeAnimationStartEnd(nodePtr, start, end);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationLoop(JNIEnv * env, jclass cls, jlong nodePtr, jboolean isLoop)
    {
        Scene3D::setNodeAnimationLoop(nodePtr, isLoop);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimation(JNIEnv * env, jclass cls, jlong nodePtr, jstring jAnimationName)
    {
        const char *animationName = env->GetStringUTFChars(jAnimationName, JNI_FALSE);
        Scene3D::setNodeAnimation(nodePtr, animationName);
        env->ReleaseStringUTFChars(jAnimationName, animationName);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeAnimationIndex(JNIEnv * env, jclass cls, jlong nodePtr, jlong index)
    {
        Scene3D::setNodeAnimationIndex(nodePtr, index);
    }    

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addMeshNode(JNIEnv * env, jclass cls, jstring jMeshName)
    {
        const char *meshName = env->GetStringUTFChars(jMeshName, JNI_FALSE);
        jlong node = Scene3D::addMeshNode(meshName);
        env->ReleaseStringUTFChars(jMeshName, meshName);

        return node;
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_removeNode(JNIEnv * env, jclass cls, jlong nodePtr)
    {
        Scene3D::removeNode(nodePtr);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_destroyScene(JNIEnv * env, jclass cls)
    {
        Scene3D::destroyScene();
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_getRootNode(JNIEnv *env, jclass cls)
    {
        return Scene3D::getRootNode();
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeLight(JNIEnv *env, jclass cls, jlong nodePtr, jboolean enabled)
    {
        Scene3D::setNodeLighting(nodePtr, enabled);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeVisible(JNIEnv *env, jclass cls, jlong nodePtr, jboolean visible)
    {
        Scene3D::setNodeVisible(nodePtr, visible);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeBillboard(JNIEnv *env, jclass cls, jlong nodePtr, jboolean isBillboard)
    {
        Scene3D::setNodeBillboard(nodePtr, isBillboard);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodePosition(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D::setNodePosition(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeRotation(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D::setNodeRotation(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeScale(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Scene3D::setNodeScale(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeParent(JNIEnv * env, jclass cls, jlong nodePtr, jlong parentPtr)
    {
        Scene3D::setNodeParent(nodePtr, parentPtr);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeMaterialType(JNIEnv * env, jclass cls, jlong nodePtr, jlong materialType)
    {
        Scene3D::setNodeMaterialType(nodePtr, (Scene3D::MaterialType)materialType);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setNodeModelMatrix(JNIEnv * env, jclass cls, jlong nodePtr, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D::setNodeModelMatrix(nodePtr, matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setViewMatrix(JNIEnv * env, jclass cls, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D::setViewMatrix(matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_setProjectionMatrix(JNIEnv * env, jclass cls, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Scene3D::setProjectionMatrix(matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addTexture(JNIEnv * env, jclass cls, jint width, jint height)
    {
        return Scene3D::addTexture(width, height);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_updateTexture(JNIEnv * env, jclass cls, jlong texturePtr, jbyteArray jImageData)
    {
        jbyte *srcData = env->GetByteArrayElements(jImageData, JNI_FALSE);
        Scene3D::updateTexture(texturePtr, (char*)srcData);
        env->ReleaseByteArrayElements(jImageData, srcData, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_addFullScreenTextureNode(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        return Scene3D::addFullScreenTextureNode(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_drawFullScreenTexture(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        Scene3D::drawFullScreenTexture(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL Java_com_hiscene_Scene3D_writeTexture(JNIEnv *env, jclass cls, jlong texturePtr, jstring jFilename)
    {
        const char* filename = env->GetStringUTFChars(jFilename, JNI_FALSE);
        Scene3D::writeTexture(texturePtr, filename);
        env->ReleaseStringUTFChars(jFilename, filename);
    }

    JNIEXPORT jlong JNICALL Java_com_hiscene_Scene3D_pickNodeFromScreen(JNIEnv *env, jclass cls, jint x, jint y)
    {
        return Scene3D::pickNodeFromScreen(x, y);
    }

}