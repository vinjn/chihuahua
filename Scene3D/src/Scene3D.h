#pragma once

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include "irrlicht.h"
#include "os.h"
#include "CLogger.h"
#include "FullScreenSceneNode.h"
#include "AssimpWrapper.h"

using namespace irr;
using namespace core;

static void print(const c8* text, ELOG_LEVEL ll = ELL_INFORMATION)
{
    os::Printer::log(text, ll);
}

namespace irr
{
namespace video
{
IVideoDriver* createOGLES1Driver(const SIrrlichtCreationParameters& params,
                                 io::IFileSystem* io, video::IContextManager* contextManager);

IVideoDriver* createOGLES2Driver(const SIrrlichtCreationParameters& params,
                                 io::IFileSystem* io, video::IContextManager* contextManager);

IVideoDriver* createNullDriver(io::IFileSystem* io, const dimension2d<u32>& screenSize);

IVideoDriver* createDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* filesystem)
{
    IContextManager* contextManager = NULL; // Leave context work to Java side
    IVideoDriver* videoDriver = NULL;

    switch (params.DriverType)
    {
    case video::EDT_OGLES1:
#ifdef _IRR_COMPILE_WITH_OGLES1_
        videoDriver = video::createOGLES1Driver(params, filesystem, contextManager);
#else
        print("No OpenGL ES 1.0 support compiled in.", ELL_ERROR);
#endif
        break;
    case video::EDT_OGLES2:
#ifdef _IRR_COMPILE_WITH_OGLES2_
        videoDriver = video::createOGLES2Driver(params, filesystem, contextManager);
#else
        print("No OpenGL ES 2.0 support compiled in.", ELL_ERROR);
#endif
        break;
    case video::EDT_NULL:
        videoDriver = video::createNullDriver(filesystem, params.WindowSize);
        break;
    case video::EDT_SOFTWARE:
    case video::EDT_BURNINGSVIDEO:
    case video::EDT_OPENGL:
    case video::EDT_DIRECT3D8:
    case video::EDT_DIRECT3D9:
        print("This driver is not available. Try OpenGL ES 1.0 or ES 2.0.", ELL_ERROR);
        break;
    default:
        print("Unable to create video driver of unknown type.", ELL_ERROR);
        break;
    }
    return videoDriver;
}
}

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

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ISceneNode* arRootNode; // arRootNode's parent = dummy node
scene::ICameraSceneNode* camera;
io::IFileSystem* fs;
int screenWidth, sceenHeight;

enum NodeIdCategory
{
    NODE_INVISIBLE_CATEGORY = 0,
    NODE_VISIBLE_CATEGORY = 1 << 20,
};

s32 getNewNodeId(NodeIdCategory category)
{
    // TODO:
    return category;
}

//! prints error if an error happened.
bool testGLError(const c8* comment = "")
{
    GLenum g = glGetError();
    print(comment, ELL_ERROR);
    switch (g)
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_ENUM:
        print("GL_INVALID_ENUM", ELL_ERROR);
        break;
    case GL_INVALID_VALUE:
        print("GL_INVALID_VALUE", ELL_ERROR);
        break;
    case GL_INVALID_OPERATION:
        print("GL_INVALID_OPERATION", ELL_ERROR);
        break;
    case GL_OUT_OF_MEMORY:
        print("GL_OUT_OF_MEMORY", ELL_ERROR);
        break;
    };
    return true;
}

static void createDriverAndSmgr(int width, int height, video::E_DRIVER_TYPE driverType)
{
    SIrrlichtCreationParameters params;
    params.DriverType = driverType;
    params.WindowSize.Width = width;
    params.WindowSize.Height = height;
    driver = video::createDriver(params, fs);

    os::Timer::initTimer(true);

    testGLError("video::createDriver()");

    smgr = scene::createSceneManager(driver, fs, NULL, NULL);
    scene::IDummyTransformationSceneNode* transformNode = smgr->addDummyTransformationSceneNode();
    arRootNode = smgr->addEmptySceneNode(transformNode);

    u32 loaderCount = smgr->getMeshLoaderCount();
    printf("getMeshLoaderCount: %d\n", loaderCount);
    for (u32 i = 0; i < loaderCount; ++i)
    {
        scene::IMeshLoader* loader = smgr->getMeshLoader(i);
        printf("%s\n", loader->getDebugName());
    }

    // ref: https://github.com/Yikun/Design-On-Fingertips/blob/master/magicbookshow/irrAR/irrAR.cpp
    camera = smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 100));
}

static scene::ISceneNode* addDummySceneNode()
{
    scene::IDummyTransformationSceneNode* node = smgr->addDummyTransformationSceneNode(arRootNode);
    node->setName("dummy#");
    node->setID(getNewNodeId(NODE_INVISIBLE_CATEGORY));

    return node;
}

long Scene3D_addDummyNode()
{
    return (long)addDummySceneNode();
}

long Scene3D_addPointLight(float lightRadius)
{
    driver->setAmbientLight(video::SColorf(0.2f, 0.2f, 0.2f));

    f32 dist = lightRadius / 2;
    scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(dist, dist, dist));
    light->setLightType(video::ELT_POINT);
    light->setRadius(lightRadius);
    light->getLightData().DiffuseColor.set(1, 1, 1);

    return (long)light;
}

void Scene3D_resize(int width, int height)
{
    print("resize()");

    // TODO: memory leak
    // if (driver == NULL)
    {
        // createDriverAndSmgr(width, height, isGlesV2 ? video::EDT_OGLES2 : video::EDT_OGLES1);
        createDriverAndSmgr(width, height, video::EDT_OGLES2);
    }

    dimension2d<u32> dim(width, height);
    driver->OnResize(dim);
    testGLError("driver->OnResize");
}

void Scene3D_clear()
{
    // print("clear()");

    driver->beginScene(true, true, video::SColor(255, 100, 100, 100));
    // driver->drawPixel(0, 0, video::SColor(255, 255, 0, 0));
    // driver->draw2DRectangleOutline(recti(10, 10, 100, 100));
}

void Scene3D_render()
{
    // print("render()");
    os::Timer::tick();
    // printf("fps: %d\n", driver->getFPS());

    smgr->drawAll();

    driver->endScene();
}

static void postProcessNode(scene::ISceneNode* node, const c8* name)
{
    if (node)
    {
        node->setID(getNewNodeId(NODE_VISIBLE_CATEGORY));
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
        node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
        node->setName(name);
    }
}

long Scene3D_addCubeNode(float size)
{
    scene::ISceneNode* node = smgr->addCubeSceneNode(size, addDummySceneNode());
    postProcessNode(node, "cube");

    return (long)node;
}

long Scene3D_addPlaneNode(float width, float height)
{
    scene::IMesh* planeMesh = smgr->getGeometryCreator()->
                                createPlaneMesh(core::dimension2d<f32>(width, height));
    scene::ISceneNode* node = smgr->addMeshSceneNode(planeMesh, addDummySceneNode());
    node->setRotation(core::vector3df(-90, 0, 0));
    planeMesh->drop();

    postProcessNode(node, "plane");

    return (long)node;
}

long Scene3D_addSphereNode(float radius)
{
    scene::ISceneNode* node = smgr->addSphereSceneNode(radius, 16, addDummySceneNode());
    postProcessNode(node, "sphere");

    return (long)node;
}

long Scene3D_getTexture(const c8* textureName)
{
    video::ITexture* texture = NULL;
    if (textureName)
    {
        texture = driver->getTexture(textureName);
    }

    return (long)texture;
}

void Scene3D_setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    video::ITexture* texture = (video::ITexture*)texturePtr;
    printf("getMaterialCount: %d\n", node->getMaterialCount());

    node->setMaterialTexture(textureLayer, texture);
}

void Scene3D_setNodeAnimationFps(long nodePtr, f32 fps)
{
    scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
    {
        node->setAnimationSpeed(fps);
    }
}

void Scene3D_setNodeAnimation(long nodePtr, const c8* animationName)
{
    scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
    {
        node->setAnimation(animationName);
    }
}

void Scene3D_setNodeAnimationLoop(long nodePtr, bool isLoop)
{
    scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
    {
        node->setLoopMode(isLoop);
    }
}

void Scene3D_setNodeAnimationIndex(long nodePtr, u32 index)
{
    scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
    {
        node->setAnimation(index);
    }
}

void Scene3D_setNodeAnimationStartEnd(long nodePtr, s32 start, s32 end)
{
    scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
    {
        node->setFrameLoop(start, end);
    }
}

void Scene3D_destroyScene()
{
    arRootNode->removeAll();
}

void Scene3D_removeNode(long nodePtr)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());
    delete transformNode;
}

long Scene3D_addMeshNode(const c8* meshName)
{
    scene::IAnimatedMeshSceneNode* node = NULL;
    {
        scene::IAnimatedMesh* mesh = getMeshFromAssimp(smgr, meshName);
        node = smgr->addAnimatedMeshSceneNode(mesh, addDummySceneNode());
        if (node)
        {
            postProcessNode(node, meshName);
            node->setAnimation(0U);
        }
    }
    return (long)node;
}

long Scene3D_getRootNode()
{
    return (long)arRootNode;
}

void Scene3D_setNodeVisible(long nodePtr, bool visible)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    node->setVisible(visible);
}

void Scene3D_setNodePosition(long nodePtr, float x, float y, float z)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    node->setPosition(vector3df(x, y, z));
}

void Scene3D_setNodeRotation(long nodePtr, float x, float y, float z)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    node->setRotation(vector3df(x, y, z));
}

void Scene3D_setNodeScale(long nodePtr, float x, float y, float z)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    node->setScale(vector3df(x, y, z));
}

void Scene3D_setNodeParent(long nodePtr, long parentPtr)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::ISceneNode* parent = (scene::ISceneNode*)parentPtr;
    node->setParent(parent);
}

void Scene3D_setNodeModelMatrix(long nodePtr, float* matrix)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());

#if 0
    printf("Scene3D_setNodeModelMatrix");
    for (int i = 0; i < 4; ++i)
    {
        printf("%.1f %.1f %.1f %.1f", matrix[i * 4], matrix[i * 4 + 1], matrix[i * 4 + 2], matrix[i * 4 + 3]);
    }
#endif
    // IDummyTransformationSceneNode
    transformNode->getRelativeTransformationMatrix().setM(matrix);
}

void Scene3D_setViewMatrix(long nodePtr, float* matrix)
{
    printf("Scene3D_setViewMatrix unimplemented.");

    // matrix4 mat;
    // mat.setM(matrix);
    // camera->setProjectionMatrix(matrix);
}

void Scene3D_setProjectionMatrix(float* matrix)
{
    matrix4 mat;
    mat.setM(matrix);
    camera->setProjectionMatrix(mat);
}

long Scene3D_addTexture(int width, int height)
{
    dimension2du dim(width, height);
    video::ITexture* texture =  driver->addTexture(dim, "texture#", video::ECF_A8R8G8B8);
    return (long)texture;
}

void Scene3D_updateTexture(long texturePtr, s8* srcData)
{
    video::ITexture* texture = (video::ITexture*)texturePtr;

    u8* dstData;
    dstData = (u8*)(texture->lock());
    if (dstData && srcData)
    {
        int pixelCount = texture->getSize().Width * texture->getSize().Height;

        // TODO: optimize
        for (int i = 0; i < pixelCount; i++)
        {
            dstData[0] = srcData[2];
            dstData[1] = srcData[1];
            dstData[2] = srcData[0];
            dstData[3] = srcData[3];
            dstData += 4;
            srcData += 4;
        }
        texture->unlock();
    }
    else
    {
        printf("Camera's ITexture could not be written?\n");
    }
}

long Scene3D_addFullScreenTextureNode(long texturePtr, int rotationType)
{
    // full screen node is not dependent on arRootNode
#if 0
    scene::ISceneNode* node = new FullScreenSceneNode(arRootNode, smgr, 0, rotationType);
#else
    scene::ISceneNode* node = new FullScreenSceneNode(smgr->getRootSceneNode(), smgr, 0, rotationType);
#endif
    video::ITexture* texture = (video::ITexture*)texturePtr;
    node->setMaterialTexture(0, texture);

    postProcessNode(node, "fullscreen");

    return (long)node;
}

void Scene3D_drawFullScreenTexture(long texturePtr, int rotationType)
{
    dimension2du screenSize = driver->getScreenSize();
    video::ITexture* texture = (video::ITexture*)texturePtr;
    dimension2du imgSize = texture->getSize();

    f32 dstW = screenSize.Width;
    f32 dstH = screenSize.Height;
    f32 dstRatio = dstW / dstH;

    f32 srcX = 0;
    f32 srcY = 0;
    f32 srcW = imgSize.Width;
    f32 srcH = imgSize.Height;
    if (rotationType == 1 || rotationType == 3)
    {
        swap(srcW, srcH);
    }
    f32 srcRatio = srcW / srcH;
    if (srcRatio > dstRatio)
    {
        // srcW is too big, let's make it smaller
        f32 newSrcW = srcH * dstRatio;
        srcX = (srcW - newSrcW) / 2;
        srcW = newSrcW;
    }
    else
    {
        // let's make srcH smaller
        f32 newSrcH = srcW / dstRatio;
        srcY = (srcH - newSrcH) / 2;
        srcH = newSrcH;
    }
    if (rotationType == 1 || rotationType == 3)
    {
        swap(srcW, srcH);
        swap(srcX, srcY);
    }

    driver->draw2DImageRotated(texture, rect<s32>(0, 0, dstW, dstH), rect<s32>(srcX, srcY, srcX + srcW, srcY + srcH), rotationType);
    // driver->draw2DRectangle(video::SColor(255, 255, 0, 0), recti(0, 0, 10, 10));
}

void Scene3D_writeTexture(long texturePtr, const c8* filename)
{
    video::ITexture* texture = (video::ITexture*)texturePtr;
    video::IImage* image = driver->createImage(texture, position2d<s32>(0, 0), texture->getSize());
    printf("Writing %s\n", filename);

    if (!driver->writeImageToFile(image, filename))
    {
        printf("Fail\n");
    }
    image->drop();
}

long Scene3D_pickNodeFromScreen(int x, int y)
{
    s32 idBitMask = NODE_VISIBLE_CATEGORY;
    bool bNoDebugObjects = false;
    scene::ISceneNode* hitNode = smgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(
                                     position2di(x, y), idBitMask, bNoDebugObjects, arRootNode);

    if (hitNode)
    {
        printf("hit %s\n", hitNode->getName());
    }

    return (long)hitNode;
}
