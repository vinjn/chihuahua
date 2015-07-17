#pragma once

#include "irrlicht.h"
#include "FullScreenSceneNode.h"
#include "../source/irrlicht/os.h"
#include "../source/irrlicht/CLogger.h"

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_
#include <OpenGLES/ES2/gl.h>
#else
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

long Scene3D_addDummyNode();

long Scene3D_addPointLight(float lightRadius);

void Scene3D_resize(int width, int height);

void Scene3D_clear();

void Scene3D_render();

long Scene3D_addCubeNode(float size);

long Scene3D_addPlaneNode(float width, float height);

long Scene3D_addSphereNode(float radius);

long Scene3D_getTexture(const c8* textureName);

void Scene3D_setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);

void Scene3D_setNodeAnimationFps(long nodePtr, f32 fps);

void Scene3D_setNodeAnimation(long nodePtr, const c8* animationName);

void Scene3D_setNodeAnimationLoop(long nodePtr, bool isLoop);

void Scene3D_setNodeAnimationIndex(long nodePtr, u32 index);

void Scene3D_setNodeAnimationStartEnd(long nodePtr, s32 start, s32 end);

void Scene3D_destroyScene();

void Scene3D_removeNode(long nodePtr);

long Scene3D_addMeshNode(const c8* meshName);

long Scene3D_getRootNode();

void Scene3D_setNodeVisible(long nodePtr, bool visible);

void Scene3D_setNodePosition(long nodePtr, float x, float y, float z);

void Scene3D_setNodeRotation(long nodePtr, float x, float y, float z);

void Scene3D_setNodeScale(long nodePtr, float x, float y, float z);
void Scene3D_setNodeParent(long nodePtr, long parentPtr);
void Scene3D_setNodeModelMatrix(long nodePtr, float* matrix);

void Scene3D_setViewMatrix(long nodePtr, float* matrix);

void Scene3D_setProjectionMatrix(float* matrix);

long Scene3D_addTexture(int width, int height);

void Scene3D_updateTexture(long texturePtr, s8* srcData);

long Scene3D_addFullScreenTextureNode(long texturePtr, int rotationType);

void Scene3D_drawFullScreenTexture(long texturePtr, int rotationType);

void Scene3D_writeTexture(long texturePtr, const c8* filename);

long Scene3D_pickNodeFromScreen(int x, int y);
