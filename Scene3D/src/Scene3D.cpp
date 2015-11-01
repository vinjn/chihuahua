#include "Scene3D.h"
#include "FullScreenSceneNode.h"

#include "irrlicht.h"
#include "FullScreenSceneNode.h"
#include "../source/irrlicht/os.h"
#include "../source/irrlicht/CLogger.h"
#include "../source/irrlicht/COGLES2Texture.h"
#include "../source/irrlicht/CSceneManager.h"
#include "../source/irrlicht/CCameraSceneNode.h"
#include "../source/XEffects/XEffects.h"
#include "../source/v7/v7.h"

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif

using namespace irr;
using namespace core;

struct v7 *v7;

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

io::IFileSystem* fs;

video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ISceneCollisionManager* coll;

scene::ISceneNode* arRootNode; // arRootNode's parent = dummy node
scene::CCameraSceneNode* camera;

EffectHandler* effect;

enum NodeIdCategory
{
    NODE_INVISIBLE_CATEGORY = 0,
    NODE_VISIBLE_CATEGORY = 1 << 20,
};

char nameBuffer[256];
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

static void setupSceneAndCamera()
{
    coll = smgr->getSceneCollisionManager();

    scene::IDummyTransformationSceneNode* transformNode = smgr->addDummyTransformationSceneNode();
    arRootNode = smgr->addEmptySceneNode(transformNode);

    u32 loaderCount = smgr->getMeshLoaderCount();
    printf("getMeshLoaderCount: %d\n", loaderCount);
    for (u32 i = 0; i < loaderCount; ++i)
    {
        scene::IMeshLoader* loader = smgr->getMeshLoader(i);
        printf("----%s\n", loader->getDebugName());
    }

    // ref: https://github.com/Yikun/Design-On-Fingertips/blob/master/magicbookshow/irrAR/irrAR.cpp
    camera =(scene::CCameraSceneNode*)smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 100));
    smgr->setActiveCamera(camera);

#ifdef _IRR_WINDOWS_API_
    // XEffects
    effect = new EffectHandler(driver, smgr, driver->getScreenSize(), false, true);

    auto shadowDimen = 512;
    effect->addShadowLight(SShadowLight(1024, vector3df(-15, 30, -15), vector3df(5, 0, 5),
        video::SColor(255, 255, 255, 255), 20.0f, 60.0f, 30.0f * DEGTORAD));
#endif
}

static void createDriverAndSmgr(int width, int height, video::E_DRIVER_TYPE driverType)
{
    SIrrlichtCreationParameters params;
    params.DriverType = driverType;
    params.WindowSize.Width = width;
    params.WindowSize.Height = height;
    driver = video::createDriver(params, fs);
    testGLError("video::createDriver()");

    os::Timer::initTimer(true);

    smgr = new scene::CSceneManager(driver, fs, NULL, 0, NULL );
}

template <typename T>
T* getTypedPointer(long nodePtr)
{
    T* node = (T*)nodePtr;
    // TODO: check
    return node;
}

static scene::ISceneNode* addDummyNode(stringc name)
{
    scene::IDummyTransformationSceneNode* node = smgr->addDummyTransformationSceneNode(arRootNode);
    name.append(".dummy");
    node->setName(name);
    node->setID(getNewNodeId(NODE_INVISIBLE_CATEGORY));

    return node;
}

long Scene_addLightNode()
{
    static int counter = 0;
    sprintf(nameBuffer, "light#%d", counter++);
    // driver->setAmbientLight(video::SColorf(0.2f, 0.2f, 0.2f));
    scene::ILightSceneNode* node = smgr->addLightSceneNode(addDummyNode(nameBuffer));
    node->setID(getNewNodeId(NODE_INVISIBLE_CATEGORY));
    node->setName(nameBuffer);

    return (long)node;
}

void LightNode_setType(long nodePtr, LightType lightType)
{
    video::E_LIGHT_TYPE type = video::ELT_POINT;
    if (lightType == DirectionalLight)
    {
        type = video::ELT_DIRECTIONAL;
    }
    getTypedPointer<scene::ILightSceneNode>(nodePtr)->setLightType(type);
}

void LightNode_setRadius(long nodePtr, float radius)
{
    getTypedPointer<scene::ILightSceneNode>(nodePtr)->setRadius(radius);
}

void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float a)
{
    video::SLight& data = getTypedPointer<scene::ILightSceneNode>(nodePtr)->getLightData();
    data.DiffuseColor.set(r, g, b, a);
}

void Scene_initializeRenderer(int width, int height)
{
    printf("initializeRenderer()");

    // TODO: memory leak
    // if (driver == NULL)
    {
        createDriverAndSmgr(width, height, video::EDT_OGLES2);
    }
    setupSceneAndCamera();

    dimension2d<u32> dim(width, height);
    driver->OnResize(dim);
    testGLError("driver->OnResize()");
}

void Scene_clear()
{
    // printf("Scene_clear()");
    auto clr = video::SColor(255, 100, 100, 100);
	if (effect) effect->setClearColour(clr);
    driver->beginScene(true, true, clr);
    // driver->drawPixel(0, 0, video::SColor(255, 255, 0, 0));
    // driver->draw2DRectangleOutline(recti(10, 10, 100, 100));
}

void Scene_render()
{
    // printf("Scene_clear()");
    os::Timer::tick();
    // printf("fps: %d\n", driver->getFPS());

#if 0 //def _IRR_WINDOWS_API_
    effect->update();
#else
    smgr->drawAll();
#endif

    driver->endScene();
}

static void postProcessNode(scene::ISceneNode* node, const stringc name = "")
{
    if (node)
    {
        node->setID(getNewNodeId(NODE_VISIBLE_CATEGORY));
//            node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
        node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, true);
        if (!name.empty())
            node->setName(name);

        auto selector = smgr->createTriangleSelectorFromBoundingBox(node);
        node->setTriangleSelector(selector);
        selector->drop();
        auto it = node->getChildren().begin();
        for (; it != node->getChildren().end(); ++it)
            postProcessNode(*it);
    }
}

long Scene_loadScene(const char* sceneFileName)
{
    auto dummy = addDummyNode(sceneFileName);
    auto rootInSceneFile = smgr->addEmptySceneNode(dummy);
    if (smgr->loadScene(sceneFileName, NULL, rootInSceneFile))
    {
        postProcessNode(rootInSceneFile, sceneFileName);
        return (long)rootInSceneFile;
    }
    else
    {
        return 0;
    }
}

long Scene_getNodeFromName(const char* nodeName)
{
    scene::ISceneNode* startNode = 0;
    scene::ISceneNode* node = smgr->getSceneNodeFromName(nodeName, startNode);

    return (long)node;
}

long Scene_addCubeNode(float size)
{
    static int counter = 0;
    sprintf(nameBuffer, "cube#%d", counter++);

    scene::ISceneNode* node = smgr->addCubeSceneNode(size, addDummyNode(nameBuffer));
    postProcessNode(node, nameBuffer);

    return (long)node;
}

long Scene_addPlaneNode(float width, float height)
{
    static int counter = 0;
    sprintf(nameBuffer, "plane#%d_%.0fx%.0f", counter++, width, height);

    scene::IMesh* planeMesh = smgr->getGeometryCreator()->
                              createPlaneMesh(core::dimension2d<f32>(width, height));
    scene::ISceneNode* node = smgr->addMeshSceneNode(planeMesh, addDummyNode(nameBuffer));
    node->setRotation(core::vector3df(90, 0, 0));
    planeMesh->drop();

    postProcessNode(node, nameBuffer);

    return (long)node;
}

long Scene_addSphereNode(float radius)
{
    static int counter = 0;
    sprintf(nameBuffer, "sphere#%d", counter++);

    scene::ISceneNode* node = smgr->addSphereSceneNode(radius, 16, addDummyNode(nameBuffer));
    postProcessNode(node, nameBuffer);

    return (long)node;
}

long Scene_addTexture(const char* textureName)
{
    video::ITexture* texture = NULL;
    if (textureName)
    {
        texture = driver->getTexture(textureName);
        video::COGLES2Texture* es2tex = (video::COGLES2Texture*)texture;
        GLuint id = es2tex->getOpenGLTextureName();
        printf("tex#%d = %s\n", id, textureName);
    }

    return (long)texture;
}

#define CHECK_NODE_RETURN(nodePtr) \
    if (nodePtr == 0) \
        {\
        printf("Empty node.\n"); \
        return; \
        }

void Node_setLighting(long nodePtr, int enabled)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    node->setMaterialFlag(video::EMF_LIGHTING, enabled);
}

void Node_setTextureAt(long nodePtr, unsigned int mtrl, long texturePtr)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    video::ITexture* texture = (video::ITexture*)texturePtr;
    // printf("getMaterialCount: %d\n", node->getMaterialCount());

    if (mtrl < node->getMaterialCount())
        node->getMaterial(mtrl).setTexture(0, texture);
}

void Node_setTexture(long nodePtr, long texturePtr)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    video::ITexture* texture = (video::ITexture*)texturePtr;
    // printf("getMaterialCount: %d\n", node->getMaterialCount());

    node->setMaterialTexture(0, texture);
}

void Node_setSecondTextureAt(long nodePtr, unsigned int mtrl, long texturePtr)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    video::ITexture* texture = (video::ITexture*)texturePtr;
    // printf("getMaterialCount: %d\n", node->getMaterialCount());

    if (mtrl < node->getMaterialCount())
        node->getMaterial(mtrl).setTexture(1, texture);
}

void Node_setBillboard(long nodePtr, int isBillboard)
{
    CHECK_NODE_RETURN(nodePtr);
}

#define CHECK_ANIMATED_MESH_RETURN(nodePtr) \
    CHECK_NODE_RETURN(nodePtr);\
    auto node = getTypedPointer<scene::ISceneNode>(nodePtr);\
    if (node->getType() != scene::ESNT_ANIMATED_MESH)\
    {\
        printf("%s is not an animated mesh.\n", node->getName());\
        return;\
    }

void MeshNode_setAnimationFps(long nodePtr, float fps)
{
    CHECK_ANIMATED_MESH_RETURN(nodePtr);
    getTypedPointer<scene::IAnimatedMeshSceneNode>(nodePtr)->setAnimationSpeed(fps);
}

void MeshNode_setAnimationByName(long nodePtr, const char* animationName)
{
    CHECK_ANIMATED_MESH_RETURN(nodePtr);
    getTypedPointer<scene::IAnimatedMeshSceneNode>(nodePtr)->setAnimationByName(animationName);
}

void MeshNode_setAnimationLoop(long nodePtr, int isLoop)
{
    CHECK_ANIMATED_MESH_RETURN(nodePtr);
    getTypedPointer<scene::IAnimatedMeshSceneNode>(nodePtr)->setLoopMode(isLoop);
}

void MeshNode_setAnimationByIndex(long nodePtr, int index)
{
    CHECK_ANIMATED_MESH_RETURN(nodePtr);
    getTypedPointer<scene::IAnimatedMeshSceneNode>(nodePtr)->setAnimation(index);
}

void MeshNode_setAnimationByRange(long nodePtr, int start, int end)
{
    CHECK_ANIMATED_MESH_RETURN(nodePtr);
    getTypedPointer<scene::IAnimatedMeshSceneNode>(nodePtr)->setFrameLoop(start, end);
}

void Scene_setAnimationCallback(NodePtrFunctor cb)
{
    struct MyAnimationEndCallBack : public scene::IAnimationEndCallBack
    {
        MyAnimationEndCallBack(NodePtrFunctor cb)
        {
            mCallback = cb;
        }

        virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node)
        {
            mCallback((long)node);    
        }
        NodePtrFunctor mCallback;
    };

    scene::setGlobalAnimationEndCallback(new MyAnimationEndCallBack(cb));
}

void Scene_destroy()
{
    arRootNode->removeAll();

    if (v7 != NULL)
    {
        v7_destroy(v7);
    }
}

void Scene_removeNode(long nodePtr)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());
    delete transformNode;
}

long Scene_addMeshNode(const char* meshFileName)
{
    scene::IAnimatedMeshSceneNode* node = NULL;
    {
        printf("Start to load mesh: %s\n", meshFileName);
        scene::IAnimatedMesh* mesh = smgr->getMesh(meshFileName);
        if (mesh)
        {
            node = smgr->addAnimatedMeshSceneNode(mesh, addDummyNode(meshFileName));
            if (node)
            {
                postProcessNode(node, meshFileName);
                // node->setAnimation(0);
            }
            if (mesh->getMeshType() == scene::EAMT_MD2 ||
                mesh->getMeshType() == scene::EAMT_3DS)
            {
                // assimp-loaded mesh has different front / back settings
                // TODO: make it uniformed
                // node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
                // node->setMaterialFlag(video::EMF_FRONT_FACE_CULLING, false);
            }
        }
    }
    return (long)node;
}

static core::map<stringc, video::IImage*> imageFromFileMap;
long Scene_addImageFromFile(const char* imageFileName)
{
    auto kv = imageFromFileMap.find(imageFileName);
    if (kv)
    {
        return (long)kv->getValue();
    }
    video::IImage* img = driver->createImageFromFile(imageFileName);
    imageFromFileMap.insert(imageFileName, img);

    return (long)img;
}

static core::map<video::IImage*, video::ITexture*> textureFromImageMap;
long Scene_addTextureFromImage(long imagePtr)
{
    auto image = getTypedPointer<video::IImage>(imagePtr);
    auto kv = textureFromImageMap.find(image);
    if (kv)
    {
        return (long)kv->getValue();
    }

    auto texture = driver->addTexture("TextureFromImage", image);
    textureFromImageMap.insert(image, texture);
    image->drop();

    return (long)texture;
}

long Scene_getRootNode()
{
    return (long)arRootNode;
}

void Node_setVisible(long nodePtr, int visible)
{
    CHECK_NODE_RETURN(nodePtr);
    getTypedPointer<scene::ISceneNode>(nodePtr)->setVisible(visible);
}

void Node_setPosition(long nodePtr, float x, float y, float z)
{
    CHECK_NODE_RETURN(nodePtr);
    getTypedPointer<scene::ISceneNode>(nodePtr)->setPosition(vector3df(x, y, z));
    // printf("%.1f, %.1f, %.1f\n", x, y, z);
}

void Node_setRotation(long nodePtr, float x, float y, float z)
{
    CHECK_NODE_RETURN(nodePtr);
    getTypedPointer<scene::ISceneNode>(nodePtr)->setRotation(vector3df(x, y, z));
}

void Node_setScale(long nodePtr, float x, float y, float z)
{
    CHECK_NODE_RETURN(nodePtr);
    getTypedPointer<scene::ISceneNode>(nodePtr)->setScale(vector3df(x, y, z));
}

void Node_setParent(long nodePtr, long parentPtr)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::ISceneNode* parent = (scene::ISceneNode*)parentPtr;
    node->setParent(parent);
}

void Node_setModelMatrix(long nodePtr, const float* matrix)
{
    CHECK_NODE_RETURN(nodePtr);
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    scene::IDummyTransformationSceneNode* transformNode = (scene::IDummyTransformationSceneNode*)(node->getParent());
#if 0
    printf("Node_setModelMatrix");
    for (int i = 0; i < 4; ++i)
    {
        printf("%.1f %.1f %.1f %.1f", matrix[i * 4], matrix[i * 4 + 1], matrix[i * 4 + 2], matrix[i * 4 + 3]);
    }
#endif
    // IDummyTransformationSceneNode
    transformNode->getRelativeTransformationMatrix().setM(matrix);
}

void Camera_setViewMatrix(const float* matrix)
{
    matrix4 mat;
    mat.setM(matrix);
    camera->setViewMatrix(mat);
}

void Camera_setProjectionMatrix(const float* matrix)
{
    matrix4 mat;
    mat.setM(matrix);
    camera->setProjectionMatrix(mat);
}

long Scene_addEmptyTexture(int width, int height)
{
    dimension2du dim(width, height);
    video::ITexture* texture =  driver->addTexture(dim, "texture#", video::ECF_A8R8G8B8);
    return (long)texture;
}

void Texture_update(long texturePtr, const char* srcARGB8)
{
    video::ITexture* texture = (video::ITexture*)texturePtr;

    u8* dstData;
    dstData = (u8*)(texture->lock());
    if (dstData && srcARGB8)
    {
        int pixelCount = texture->getSize().Width * texture->getSize().Height;

        // TODO: optimize
        for (int i = 0; i < pixelCount; i++)
        {
            dstData[0] = srcARGB8[2];
            dstData[1] = srcARGB8[1];
            dstData[2] = srcARGB8[0];
            dstData[3] = srcARGB8[3];
            dstData += 4;
            srcARGB8 += 4;
        }
        texture->unlock();
    }
    else
    {
        printf("Camera's ITexture could not be written?\n");
    }
}

long Scene_addFullScreenTextureNode(long texturePtr, int rotationType)
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

void Texture_renderFullScreen(long texturePtr, int rotationType)
{
    dimension2du screenSize = driver->getScreenSize();
    video::ITexture* texture = (video::ITexture*)texturePtr;
    dimension2du imgSize = texture->getSize();

    f32 dstW = (f32)screenSize.Width;
    f32 dstH = (f32)screenSize.Height;
    f32 dstRatio = dstW / dstH;

    f32 srcX = 0;
    f32 srcY = 0;
    f32 srcW = (f32)imgSize.Width;
    f32 srcH = (f32)imgSize.Height;
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

void Texture_write(long texturePtr, const char* filename)
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

long Scene_pickNodeFromScreen(int x, int y)
{
    s32 idBitMask = NODE_VISIBLE_CATEGORY;
    bool bNoDebugObjects = false;

    auto ray = coll->getRayFromScreenCoordinates(position2di(x, y), camera);
#if 0
    auto hitNode = coll->getSceneNodeFromRayBB(ray, idBitMask, bNoDebugObjects, arRootNode);
#else
    vector3df hitPt;
    triangle3df hitTri;
    auto hitNode = coll->getSceneNodeAndCollisionPointFromRay(ray,
        hitPt, hitTri, idBitMask, arRootNode, bNoDebugObjects);
#endif
    
    if (hitNode)
    {
        printf("hit %s\n", hitNode->getName());
    }

    return (long)hitNode;
}

long Scene_pickNodeFromScreenPrecisely(int x, int y)
{
    s32 idBitMask = NODE_VISIBLE_CATEGORY;
    bool bNoDebugObjects = false;
    scene::ISceneNode* hitNode = coll->getSceneNodeFromScreenCoordinatesBB(
                                     position2di(x, y), idBitMask, bNoDebugObjects, arRootNode);

    if (hitNode)
    {
        printf("hit %s\n", hitNode->getName());
    }

    return (long)hitNode;
}

void Node_setMaterialType(long nodePtr, MaterialType materialType)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    u32 count = node->getMaterialCount();
    for (u32 mtrl = 0; mtrl < count; ++mtrl)
    {
        Node_setMaterialTypeAt(nodePtr, mtrl, materialType);
    }
}

void Node_setMaterialTypeAt(long nodePtr, unsigned int mtrl, MaterialType materialType)
{
    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;
    if (mtrl >= node->getMaterialCount()) return;

    video::E_MATERIAL_TYPE type = video::EMT_SOLID;
    switch (materialType)
    {
    case Solid: type = video::EMT_SOLID; break;
    case ColorAdd: type = video::EMT_TRANSPARENT_ADD_COLOR; break;
    case AlphaBlend: type = video::EMT_TRANSPARENT_ALPHA_CHANNEL; break;
    case NormalMap: type = video::EMT_NORMAL_MAP_SOLID; break;
    case LightMap: type = video::EMT_LIGHTMAP; break;
    default: break;
    }
    node->getMaterial(mtrl).MaterialType = type;
}

void MeshNode_setShadowMode(long nodePtr, ShadowMode mode)
{
	if (!effect) return;

    scene::ISceneNode* node = (scene::ISceneNode*)nodePtr;

    // TODO: cache & optimize
    effect->removeNodeFromShadow(node);
    E_FILTER_TYPE filterType = EFT_NONE;
    effect->addShadowToNode(node, filterType, (E_SHADOW_MODE)mode);
}

void Scene_setVisible(int visible)
{
    Node_setVisible(Scene_getRootNode(), visible);
}

void Scene_initializeFromDevice(long irrlichtDevice)
{
    auto device = (IrrlichtDevice*)irrlichtDevice;
    fs = device->getFileSystem();
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    setupSceneAndCamera();
}

void Scene_runScript(const char* scriptFileName)
{
    if (v7 == NULL)
    {
        v7 = v7_create();
    }
    auto file = fs->createAndOpenFile(scriptFileName);
    if (file == NULL) return;

    core::array<u8> scriptCode(file->getSize());
    file->seek(0);
    file->read(scriptCode.pointer(), file->getSize());

    v7_val_t result;
    v7_exec(v7, (const char*)scriptCode.pointer(), &result);
}
