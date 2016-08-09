// Wrapper of chihuahua.h for Android JNI
#include "irrlicht.h"
#include "../source/os.h"
#include "../source/CLogger.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "CAndroidAssetFileArchive.h"
#include "../chihuahua.h"

using namespace ue;
using namespace core;

// extern video::IVideoDriver* driver;
// extern scene::ISceneManager* smgr;
// extern scene::ISceneNode* arRootNode; // arRootNode's parent = dummy node
// extern scene::ICameraSceneNode* camera;
extern io::IFileSystem* fs;
// extern int screenWidth, sceenHeight;

namespace ue
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
        printf("Binary library built on [%s] [%s]\n", __DATE__, __TIME__);

        return JNI_VERSION_1_2;
    }

    JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
    {
        // delete os::Printer::Logger;
        // delete driver;
        // delete smgr;
        // delete fs;
    }
}

extern "C"
{
    // http://stackoverflow.com/questions/16069209/invoking-jni-functions-in-android-package-name-containing-underscore
    // http://homepage.cs.uiowa.edu/~slonnegr/wpj/JNI.pdf
    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1initializeFileSystem)(JNIEnv * env, jclass cls, jstring jInternalDataPath, jobject assetManager)
    {
        printf("initializeFileSystem()");

        AAssetManager* amgr = AAssetManager_fromJava(env, assetManager);

        // TODO: recreation
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

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addLightNode)(JNIEnv * env, jclass cls)
    {
        return Scene_addLightNode();
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1initializeRenderer)(JNIEnv * env, jclass cls, 
        jint width, jint height, jlong windowHandle, ApiType apiType)
    {
        Scene_initializeRenderer(width, height, windowHandle, apiType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1setVisible)(JNIEnv * env, jclass cls, jboolean visible)
    {
        Scene_setVisible(visible);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1clear)(JNIEnv * env, jclass cls, jint r, jint g, jint b, jint a)
    {
        Scene_clear(r, g, b, a);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1render)(JNIEnv * env, jclass cls)
    {
        Scene_render();
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1getNodeFromName)(JNIEnv * env, jclass cls, jstring jNodeName)
    {
        const char *nodeName = env->GetStringUTFChars(jNodeName, JNI_FALSE);
        jlong node = Scene_getNodeFromName(nodeName);
        env->ReleaseStringUTFChars(jNodeName, nodeName);

        return node;
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1loadScene)(JNIEnv * env, jclass cls, jstring jSceneFileName)
    {
        const char *sceneFileName = env->GetStringUTFChars(jSceneFileName, JNI_FALSE);
        jlong node = Scene_loadScene(sceneFileName);
        env->ReleaseStringUTFChars(jSceneFileName, sceneFileName);

        return node;
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1runScript)(JNIEnv * env, jclass cls, jstring jScriptFileName)
    {
        const char *scriptFileName = env->GetStringUTFChars(jScriptFileName, JNI_FALSE);
        Scene_runScript(scriptFileName);
        env->ReleaseStringUTFChars(jScriptFileName, scriptFileName);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addCubeNode)(JNIEnv * env, jclass cls, jfloat size)
    {
        return Scene_addCubeNode(size);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addPlaneNode)(JNIEnv * env, jclass cls, jfloat width, jfloat height)
    {
        return Scene_addPlaneNode(width, height);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addSphereNode)(JNIEnv * env, jclass cls, jfloat radius)
    {
        return Scene_addSphereNode(radius);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addTexture)(JNIEnv * env, jclass cls, jstring jTextureName)
    {
        const char *textureName = env->GetStringUTFChars(jTextureName, JNI_FALSE);
        jlong texture = Scene_addTexture(textureName);
        env->ReleaseStringUTFChars(jTextureName, textureName);

        return texture;
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setTextureAt)(JNIEnv * env, jclass cls, jlong nodePtr, jint mtrl, jlong texturePtr)
    {
        Node_setTextureAt(nodePtr, mtrl, texturePtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setTexture)(JNIEnv * env, jclass cls, jlong nodePtr, jlong texturePtr)
    {
        Node_setTexture(nodePtr, texturePtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setSecondTextureAt)(JNIEnv * env, jclass cls, jlong nodePtr, jint mtrl, jlong texturePtr)
    {
        Node_setSecondTextureAt(nodePtr, mtrl, texturePtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(MeshNode_1setAnimationFps)(JNIEnv * env, jclass cls, jlong nodePtr, jfloat fps)
    {
        MeshNode_setAnimationFps(nodePtr, fps);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(MeshNode_1setAnimationLoop)(JNIEnv * env, jclass cls, jlong nodePtr, jboolean isLoop)
    {
        MeshNode_setAnimationLoop(nodePtr, isLoop);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(MeshNode_1setAnimationByRange)(JNIEnv * env, jclass cls, jlong nodePtr, jint start, jint end)
    {
        MeshNode_setAnimationByRange(nodePtr, start, end);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(MeshNode_1setAnimationByName)(JNIEnv * env, jclass cls, jlong nodePtr, jstring jAnimationName)
    {
        const char *animationName = env->GetStringUTFChars(jAnimationName, JNI_FALSE);
        MeshNode_setAnimationByName(nodePtr, animationName);
        env->ReleaseStringUTFChars(jAnimationName, animationName);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(MeshNode_1setAnimationByIndex)(JNIEnv * env, jclass cls, jlong nodePtr, jlong index)
    {
        MeshNode_setAnimationByIndex(nodePtr, index);
    }

    // http://www.math.uni-hamburg.de/doc/java/tutorial/native1.1/implementing/method.html
    /*
    JNIEXPORT void JNICALL
    Java_Callbacks_nativeMethod(JNIEnv *env, jobject obj, jint depth)
    {
        jclass cls = (*env)->GetObjectClass(env, obj);
        jmethodID mid = (*env)->GetMethodID(env, cls, "callback", "(I)V");
        if (mid == 0) {
            return;
        }
        printf("In C, depth = %d, about to enter Java\n", depth);
        (*env)->CallVoidMethod(env, obj, mid, depth);
        printf("In C, depth = %d, back from Java\n", depth);
    }
    */
    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1setAnimationCallback)(JNIEnv * env, jclass cls, jobject jCallbackInterface)
    {
        jclass objclass = env->GetObjectClass(jCallbackInterface);
        jCallbackInterface = env->NewGlobalRef(jCallbackInterface);

        // TODO: when shall we call DeleteGlobalRef()?
        objclass = static_cast<jclass>(env->NewGlobalRef(objclass));

        jmethodID method = env->GetMethodID(objclass, "onAnimationEnded", "(I)V");
        if (method == 0)
        {
            printf("Scene_setAniamtionCallback: could not get method id!\n");
            return;
        }

        struct MyAnimationEndCallBack : public scene::IAnimationEndCallBack
        {
            MyAnimationEndCallBack(JNIEnv* env, jobject jCallbackInterface, jmethodID method)
            {
                mEnv = env;
                mInterface = jCallbackInterface;
                mMethod = method;
            }

            virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node)
            {
                // printf("JNI calling %d\n", (jlong)node);
                mEnv->CallVoidMethod(mInterface, mMethod, (int)node);
            }

            JNIEnv* mEnv;
            jobject mInterface;
            jmethodID mMethod;
        };

        setGlobalAnimationEndCallback(new MyAnimationEndCallBack(env, jCallbackInterface, method));
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addMeshNode)(JNIEnv * env, jclass cls, jstring jMeshName)
    {
        const char *meshName = env->GetStringUTFChars(jMeshName, JNI_FALSE);
        jlong node = Scene_addMeshNode(meshName);
        env->ReleaseStringUTFChars(jMeshName, meshName);

        return node;
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addImageFromFile)(JNIEnv * env, jclass cls, jstring jImageName)
    {
        const char *imageFileName = env->GetStringUTFChars(jImageName, JNI_FALSE);
        jlong img = Scene_addImageFromFile(imageFileName);
        env->ReleaseStringUTFChars(jImageName, imageFileName);

        return img;
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addTextureFromImage)(JNIEnv * env, jclass cls, jlong imagePtr)
    {
        return Scene_addTextureFromImage(imagePtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1removeNode)(JNIEnv * env, jclass cls, jlong nodePtr)
    {
        Scene_removeNode(nodePtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Scene_1destroy)(JNIEnv * env, jclass cls)
    {
        Scene_destroy();
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1getRootNode)(JNIEnv *env, jclass cls)
    {
        return Scene_getRootNode();
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setLighting)(JNIEnv *env, jclass cls, jlong nodePtr, jboolean enabled)
    {
        Node_setLighting(nodePtr, enabled);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setVisible)(JNIEnv *env, jclass cls, jlong nodePtr, jboolean visible)
    {
        Node_setVisible(nodePtr, visible);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setBillboard)(JNIEnv *env, jclass cls, jlong nodePtr, jboolean isBillboard)
    {
        Node_setBillboard(nodePtr, isBillboard);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setPosition)(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Node_setPosition(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setRotation)(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Node_setRotation(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setScale)(JNIEnv * env, jclass cls, jlong nodePtr, jfloat x, jfloat y, jfloat z)
    {
        Node_setScale(nodePtr, x, y, z);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setParent)(JNIEnv * env, jclass cls, jlong nodePtr, jlong parentPtr)
    {
        Node_setParent(nodePtr, parentPtr);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setMaterialTypeAt)(JNIEnv * env, jclass cls, jlong nodePtr, jint mtrl, MaterialType materialType)
    {
        Node_setMaterialTypeAt(nodePtr, mtrl, materialType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setMaterialType)(JNIEnv * env, jclass cls, jlong nodePtr, MaterialType materialType)
    {
        Node_setMaterialType(nodePtr, materialType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Node_1setModelMatrix)(JNIEnv * env, jclass cls, jlong nodePtr, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Node_setModelMatrix(nodePtr, matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Camera_1setViewMatrix)(JNIEnv * env, jclass cls, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Camera_setViewMatrix(matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Camera_1setProjectionMatrix)(JNIEnv * env, jclass cls, jfloatArray jMatrix)
    {
        jfloat *matrix = env->GetFloatArrayElements(jMatrix, JNI_FALSE);
        Camera_setProjectionMatrix(matrix);
        env->ReleaseFloatArrayElements(jMatrix, matrix, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addEmptyTexture)(JNIEnv * env, jclass cls, jint width, jint height)
    {
        return Scene_addEmptyTexture(width, height);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Texture_1update)(JNIEnv * env, jclass cls, jlong texturePtr, jbyteArray jImageData)
    {
        jbyte *srcData = env->GetByteArrayElements(jImageData, JNI_FALSE);
        Texture_update(texturePtr, (char*)srcData);
        env->ReleaseByteArrayElements(jImageData, srcData, JNI_FALSE);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1addFullScreenTextureNode)(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        return Scene_addFullScreenTextureNode(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Texture_1renderFullScreen)(JNIEnv *env, jclass cls, jlong texturePtr, jint rotationType)
    {
        Texture_renderFullScreen(texturePtr, rotationType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(Texture_1write)(JNIEnv *env, jclass cls, jlong texturePtr, jstring jFilename)
    {
        const char* filename = env->GetStringUTFChars(jFilename, JNI_FALSE);
        Texture_write(texturePtr, filename);
        env->ReleaseStringUTFChars(jFilename, filename);
    }

    JNIEXPORT jlong JNICALL WRAP_FUNCTION_NAME(Scene_1pickNodeFromScreen)(JNIEnv *env, jclass cls, jint x, jint y)
    {
        return Scene_pickNodeFromScreen(x, y);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(LightNode_1setType)(JNIEnv *env, jclass cls, jlong nodePtr, LightType lightType)
    {
        LightNode_setType(nodePtr, lightType);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(LightNode_1setRadius)(JNIEnv *env, jclass cls, jlong nodePtr, float radius)
    {
        LightNode_setRadius(nodePtr, radius);
    }

    JNIEXPORT void JNICALL WRAP_FUNCTION_NAME(LightNode_1setDiffuseColor)(JNIEnv *env, jclass cls, jlong nodePtr, float r, float g, float b, float a)
    {
        LightNode_setDiffuseColor(nodePtr, r, g, b, a);
    }

} // extern "C"
