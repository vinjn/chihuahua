#pragma once

#include "irrlicht.h"
#include "FullScreenSceneNode.h"
#include "../source/irrlicht/os.h"
#include "../source/irrlicht/CLogger.h"

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_
#include <OpenGLES/ES2/gl.h>
#else
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_INFO, "Irrlicht", __VA_ARGS__);
#include <GLES2/gl2.h>
#endif

using namespace irr;
using namespace core;

void print(const c8* text, ELOG_LEVEL ll = ELL_INFORMATION);

extern video::IVideoDriver* driver;
extern scene::ISceneManager* smgr;
extern scene::ISceneNode* arRootNode; // arRootNode's parent = dummy node
extern scene::ICameraSceneNode* camera;
extern io::IFileSystem* fs;
extern int screenWidth, sceenHeight;

namespace irr
{
    namespace gui
    {
    class IGUIEnvironment;
    IGUIEnvironment* createGUIEnvironment(io::IFileSystem* fs,
                                          video::IVideoDriver* Driver, IOSOperator* op);
    }

    namespace scene
    {
    ISceneManager* createSceneManager(video::IVideoDriver* driver,
                                      io::IFileSystem* fs, gui::ICursorControl* cc, gui::IGUIEnvironment *gui);
    }

    namespace io
    {
    IFileSystem* createFileSystem();
    }
}


namespace Scene3D
{
    long addDummyNode();

    long addPointLight(float lightRadius);

    void resize(int width, int height);

    void clear();

    void render();

    long addCubeNode(float size);

    long addPlaneNode(float width, float height);

    long addSphereNode(float radius);

    long getTexture(const c8* textureName);

    void setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);

    void setNodeAnimationFps(long nodePtr, f32 fps);

    void setNodeAnimation(long nodePtr, const c8* animationName);

    void setNodeAnimationLoop(long nodePtr, bool isLoop);

    void setNodeAnimationIndex(long nodePtr, u32 index);

    void setNodeAnimationStartEnd(long nodePtr, s32 start, s32 end);

    void destroyScene();

    void removeNode(long nodePtr);

    long addMeshNode(const c8* meshName);

    long getRootNode();

    void setNodeVisible(long nodePtr, bool visible);

    void setNodePosition(long nodePtr, float x, float y, float z);

    void setNodeRotation(long nodePtr, float x, float y, float z);

    void setNodeScale(long nodePtr, float x, float y, float z);
    void setNodeParent(long nodePtr, long parentPtr);
    void setNodeModelMatrix(long nodePtr, float* matrix);

    void setViewMatrix(long nodePtr, float* matrix);

    void setProjectionMatrix(float* matrix);

    long addTexture(int width, int height);

    void updateTexture(long texturePtr, s8* srcData);

    long addFullScreenTextureNode(long texturePtr, int rotationType);

    void drawFullScreenTexture(long texturePtr, int rotationType);

    void writeTexture(long texturePtr, const c8* filename);

    long pickNodeFromScreen(int x, int y);    
}

