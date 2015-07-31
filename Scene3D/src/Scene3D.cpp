#include "Scene3D.h"
#include "AssimpWrapper.h"
#include "FullScreenSceneNode.h"

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
}

namespace irr
{
    class CIrrDeviceIPhone;
namespace video
{

IVideoDriver* createOGLES2Driver(const SIrrlichtCreationParameters& params,
        io::IFileSystem* io
#if defined(_IRR_COMPILE_WITH_X11_DEVICE_) || defined(_IRR_WINDOWS_API_) || defined(_IRR_COMPILE_WITH_ANDROID_DEVICE_) || defined(_IRR_COMPILE_WITH_FB_DEVICE_)
        , IContextManager* contextManager
#elif defined(_IRR_COMPILE_WITH_IPHONE_DEVICE_)
        , CIrrDeviceIPhone* device
#endif
    );

IVideoDriver* createNullDriver(io::IFileSystem* io, const core::dimension2d<u32>& screenSize);


IVideoDriver* createDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* filesystem)
{
    IVideoDriver* videoDriver = NULL;

    switch (params.DriverType)
    {
    case video::EDT_OGLES2:
        videoDriver = video::createOGLES2Driver(params, filesystem, NULL);
        break;
    case video::EDT_NULL:
        videoDriver = video::createNullDriver(filesystem, params.WindowSize);
        break;        
    default:
        printf("This driver is not available. Try OpenGL ES 2.0.");
        break;
    }
    return videoDriver;
}
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
bool testGLError(const char* comment = "")
{
    GLenum g = glGetError();
    printf("%s\n", comment);
    switch (g)
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_ENUM:
        printf("GL_INVALID_ENUM");
        break;
    case GL_INVALID_VALUE:
        printf("GL_INVALID_VALUE");
        break;
    case GL_INVALID_OPERATION:
        printf("GL_INVALID_OPERATION");
        break;
    case GL_OUT_OF_MEMORY:
        printf("GL_OUT_OF_MEMORY");
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

namespace Scene3D
{
    long addDummyNode()
    {
        return (long)addDummySceneNode();
    }

    long addPointLight(float lightRadius)
    {
        driver->setAmbientLight(video::SColorf(0.2f, 0.2f, 0.2f));

        f32 dist = lightRadius / 2;
        scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(dist, dist, dist));
        light->setLightType(video::ELT_POINT);
        light->setRadius(lightRadius);
        light->getLightData().DiffuseColor.set(1, 1, 1);

        return (long)light;
    }

    void resize(int width, int height)
    {
        printf("resize()");

        // TODO: memory leak
        // if (driver == NULL)
        {
            createDriverAndSmgr(width, height, video::EDT_OGLES2);
        }

        dimension2d<u32> dim(width, height);
        driver->OnResize(dim);
        testGLError("driver->OnResize");
    }

    void clear()
    {
        // printf("clear()");

        driver->beginScene(true, true, video::SColor(255, 100, 100, 100));
        // driver->drawPixel(0, 0, video::SColor(255, 255, 0, 0));
        // driver->draw2DRectangleOutline(recti(10, 10, 100, 100));
    }

    void render()
    {
        // printf("render()");
        os::Timer::tick();
        // printf("fps: %d\n", driver->getFPS());

        smgr->drawAll();

        driver->endScene();
    }

    static void postProcessNode(scene::ISceneNode* node, const char* name)
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

    long addCubeNode(float size)
    {
        scene::ISceneNode* node = smgr->addCubeSceneNode(size, addDummySceneNode());
        postProcessNode(node, "cube");

        return (long)node;
    }

    long addPlaneNode(float width, float height)
    {
        scene::IMesh* planeMesh = smgr->getGeometryCreator()->
                                    createPlaneMesh(core::dimension2d<f32>(width, height));
        scene::ISceneNode* node = smgr->addMeshSceneNode(planeMesh, addDummySceneNode());
        node->setRotation(core::vector3df(-90, 0, 0));
        planeMesh->drop();

        postProcessNode(node, "plane");

        return (long)node;
    }

    long addSphereNode(float radius)
    {
        scene::ISceneNode* node = smgr->addSphereSceneNode(radius, 16, addDummySceneNode());
        postProcessNode(node, "sphere");

        return (long)node;
    }

    long getTexture(const char* textureName)
    {
        video::ITexture* texture = NULL;
        if (textureName)
        {
            texture = driver->getTexture(textureName);
        }

        return (long)texture;
    }

    void setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        video::ITexture* texture = (video::ITexture*)texturePtr;
        printf("getMaterialCount: %d\n", node->getMaterialCount());

        node->setMaterialTexture(textureLayer, texture);
    }

    void setNodeAnimationFps(long nodePtr, float fps)
    {
        scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
        {
            node->setAnimationSpeed(fps);
        }
    }

    void setNodeAnimation(long nodePtr, const char* animationName)
    {
        scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
        {
            node->setAnimation(animationName);
        }
    }

    void setNodeAnimationLoop(long nodePtr, bool isLoop)
    {
        scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
        {
            node->setLoopMode(isLoop);
        }
    }

    void setNodeAnimationIndex(long nodePtr, int index)
    {
        scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
        {
            node->setAnimation(index);
        }
    }

    void setNodeAnimationStartEnd(long nodePtr, int start, int end)
    {
        scene::IAnimatedMeshSceneNode* node = (scene::IAnimatedMeshSceneNode*)nodePtr;
        {
            node->setFrameLoop(start, end);
        }
    }

    void destroyScene()
    {
        arRootNode->removeAll();
    }

    void removeNode(long nodePtr)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());
        delete transformNode;
    }

    long addMeshNode(const char* meshName)
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

    long getRootNode()
    {
        return (long)arRootNode;
    }

    void setNodeVisible(long nodePtr, bool visible)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        node->setVisible(visible);
    }

    void setNodePosition(long nodePtr, float x, float y, float z)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        node->setPosition(vector3df(x, y, z));
    }

    void setNodeRotation(long nodePtr, float x, float y, float z)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        node->setRotation(vector3df(x, y, z));
    }

    void setNodeScale(long nodePtr, float x, float y, float z)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        node->setScale(vector3df(x, y, z));
    }

    void setNodeParent(long nodePtr, long parentPtr)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        scene::ISceneNode* parent = (scene::ISceneNode*)parentPtr;
        node->setParent(parent);
    }

    void setNodeModelMatrix(long nodePtr, const float* matrix)
    {
        scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
        scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());

    #if 0
        printf("setNodeModelMatrix");
        for (int i = 0; i < 4; ++i)
        {
            printf("%.1f %.1f %.1f %.1f", matrix[i * 4], matrix[i * 4 + 1], matrix[i * 4 + 2], matrix[i * 4 + 3]);
        }
    #endif
        // IDummyTransformationSceneNode
        transformNode->getRelativeTransformationMatrix().setM(matrix);
    }

    void setViewMatrix(long nodePtr, const float* matrix)
    {
        printf("setViewMatrix unimplemented.");

        // matrix4 mat;
        // mat.setM(matrix);
        // camera->setProjectionMatrix(matrix);
    }

    void setProjectionMatrix(const float* matrix)
    {
        matrix4 mat;
        mat.setM(matrix);
        camera->setProjectionMatrix(mat);
    }

    long addTexture(int width, int height)
    {
        dimension2du dim(width, height);
        video::ITexture* texture =  driver->addTexture(dim, "texture#", video::ECF_A8R8G8B8);
        return (long)texture;
    }

    void updateTexture(long texturePtr, const char* srcData)
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

    long addFullScreenTextureNode(long texturePtr, int rotationType)
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

    void drawFullScreenTexture(long texturePtr, int rotationType)
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

    void writeTexture(long texturePtr, const char* filename)
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

    long pickNodeFromScreen(int x, int y)
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
}


