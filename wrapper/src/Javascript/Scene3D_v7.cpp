#include "../Scene3D.h"
#include "irrlicht.h"
#include "../../../3rdparty/v7/v7.c"

using namespace ue;
using namespace core;

struct v7 *v7;
extern io::IFileSystem* fs;

void Scene_runScript(const char* scriptFileName);

namespace {

v7_val_t Scene_addLightNode(struct v7 *v7)
{
    long result = ::Scene_addLightNode();
    return v7_create_number(result);
}

v7_val_t LightNode_setType(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    LightType lightType = (LightType)(int)v7_to_number(v7_arg(v7, 1));
    ::LightNode_setType(nodePtr, lightType);

    return 0;
}

v7_val_t LightNode_setRadius(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float radius = v7_to_number(v7_arg(v7, 1));
    ::LightNode_setRadius(nodePtr, radius);

    return 0;
}

v7_val_t LightNode_setDiffuseColor(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float r = v7_to_number(v7_arg(v7, 1));
    float g = v7_to_number(v7_arg(v7, 2));
    float b = v7_to_number(v7_arg(v7, 3));
    float a = v7_to_number(v7_arg(v7, 4));
    ::LightNode_setDiffuseColor(nodePtr, r, g, b, a);

    return 0;
}

v7_val_t Scene_initializeRenderer(struct v7 *v7)
{
    int width = v7_to_number(v7_arg(v7, 0));
    int height = v7_to_number(v7_arg(v7, 1));
    int apiType = v7_to_number(v7_arg(v7, 2));
    ::Scene_initializeRenderer(width, height, apiType);

    return 0;
}

v7_val_t Scene_clear(struct v7 *v7)
{
    unsigned char r = v7_to_number(v7_arg(v7, 0));
    unsigned char g = v7_to_number(v7_arg(v7, 1));
    unsigned char b = v7_to_number(v7_arg(v7, 2));
    unsigned char a = v7_to_number(v7_arg(v7, 3));    
    ::Scene_clear(r, g, b, a);

    return 0;
}

v7_val_t Scene_render(struct v7 *v7)
{
    ::Scene_render();

    return 0;
}

static const char* toString(struct v7 *v7, unsigned long argIdx = 0)
{
    auto arg = v7_arg(v7, 0);
    size_t string_len;
    return v7_to_string(v7, &arg, &string_len);
}

v7_val_t Scene_loadScene(struct v7 *v7)
{
    long result = ::Scene_loadScene(toString(v7));

    return v7_create_number(result);
}

v7_val_t Scene_getNodeFromName(struct v7 *v7)
{
    long result = ::Scene_getNodeFromName(toString(v7));
    return v7_create_number(result);
}

v7_val_t Scene_addCubeNode(struct v7 *v7)
{
    float size = v7_to_number(v7_arg(v7, 0));
    long result = ::Scene_addCubeNode(size);
    
    return v7_create_number(result);
}

v7_val_t Scene_addPlaneNode(struct v7 *v7)
{
    float width = v7_to_number(v7_arg(v7, 0));
    float height = v7_to_number(v7_arg(v7, 1));
    long result = ::Scene_addPlaneNode(width, height);

    return v7_create_number(result);
}

v7_val_t Scene_addSphereNode(struct v7 *v7)
{
    float radius = v7_to_number(v7_arg(v7, 0));
    long result = ::Scene_addSphereNode(radius);

    return v7_create_number(result);
}

v7_val_t Scene_addTexture(struct v7 *v7)
{
    long result = ::Scene_addTexture(toString(v7));
    return v7_create_number(result);
}

v7_val_t Node_setLighting(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int enabled = v7_to_number(v7_arg(v7, 1));
    ::Node_setLighting(nodePtr, enabled);

    return 0;
}

v7_val_t Node_setTextureAt(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    unsigned int mtrl = v7_to_number(v7_arg(v7, 1));
    long texturePtr = v7_to_number(v7_arg(v7, 2));
    ::Node_setTextureAt(nodePtr, mtrl, texturePtr);

    return 0;
}

v7_val_t Node_setTexture(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    long texturePtr = v7_to_number(v7_arg(v7, 1));
    ::Node_setTexture(nodePtr, texturePtr);

    return 0;
}

v7_val_t Node_setSecondTextureAt(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    unsigned int mtrl = v7_to_number(v7_arg(v7, 1));
    long texturePtr = v7_to_number(v7_arg(v7, 2));
    ::Node_setSecondTextureAt(nodePtr, mtrl, texturePtr);

    return 0;
}

v7_val_t Scene_setAnimationCallback(struct v7 *v7)
{
    // TODO:

    return 0;
}

v7_val_t Node_setBillboard(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int isBillboard = v7_to_number(v7_arg(v7, 1));
    ::Node_setBillboard(nodePtr, isBillboard);

    return 0;
}

v7_val_t MeshNode_setAnimationFps(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float fps = v7_to_number(v7_arg(v7, 1));
    ::MeshNode_setAnimationFps(nodePtr, fps);

    return 0;
}

v7_val_t MeshNode_setAnimationByName(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    const char* animationName = toString(v7, 1);
    ::MeshNode_setAnimationByName(nodePtr, animationName);
 
    return 0;
}

v7_val_t Scene_runScript(struct v7 *v7)
{
    const char* scriptFileName = toString(v7, 0);
    ::Scene_runScript(scriptFileName);

    return 0;
}

v7_val_t MeshNode_setAnimationLoop(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int isLoop = v7_to_number(v7_arg(v7, 1));
    ::MeshNode_setAnimationLoop(nodePtr, isLoop);

    return 0;
}

v7_val_t MeshNode_setAnimationByIndex(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int index = v7_to_number(v7_arg(v7, 1));
    ::MeshNode_setAnimationByIndex(nodePtr, index);

    return 0;
}

v7_val_t MeshNode_setAnimationByRange(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int start = v7_to_number(v7_arg(v7, 1));
    int end = v7_to_number(v7_arg(v7, 2));
    ::MeshNode_setAnimationByRange(nodePtr, start, end);

    return 0;
}

v7_val_t Scene_destroy(struct v7 *v7)
{
    ::Scene_destroy();

    return 0;
}

v7_val_t Scene_removeNode(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    ::Scene_removeNode(nodePtr);

    return 0;
}

v7_val_t Scene_addMeshNode(struct v7 *v7)
{
    const char* meshFileName = toString(v7);
    ::Scene_addMeshNode(meshFileName);

    return 0;
}

v7_val_t Scene_addImageFromFile(struct v7 *v7)
{
    const char* imageFileName = toString(v7);
    ::Scene_addImageFromFile(imageFileName);

    return 0;
}

v7_val_t Scene_addTextureFromImage(struct v7 *v7)
{
    long imagePtr = v7_to_number(v7_arg(v7, 0));
    ::Scene_addTextureFromImage(imagePtr);

    return 0;
}

v7_val_t Scene_getRootNode(struct v7 *v7)
{
    long result = ::Scene_getRootNode();

    return v7_to_number(result);
}

v7_val_t Node_setVisible(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    int visible = v7_to_number(v7_arg(v7, 1));
    ::Node_setVisible(nodePtr, visible);

    return 0;
}

v7_val_t Node_setPosition(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float x = v7_to_number(v7_arg(v7, 1));
    float y = v7_to_number(v7_arg(v7, 2));
    float z = v7_to_number(v7_arg(v7, 3));
    ::Node_setPosition(nodePtr, x, y, z);

    return 0;
}

v7_val_t Node_setRotation(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float x = v7_to_number(v7_arg(v7, 1));
    float y = v7_to_number(v7_arg(v7, 2));
    float z = v7_to_number(v7_arg(v7, 3));
    ::Node_setRotation(nodePtr, x, y, z);

    return 0;
}

v7_val_t Node_setScale(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    float x = v7_to_number(v7_arg(v7, 1));
    float y = v7_to_number(v7_arg(v7, 2));
    float z = v7_to_number(v7_arg(v7, 3));
    ::Node_setScale(nodePtr, x, y, z);

    return 0;
}

v7_val_t Node_setParent(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    long parentPtr = v7_to_number(v7_arg(v7, 1));
    ::Node_setParent(nodePtr, parentPtr);

    return 0;
}

static const float* toMatrix(struct v7 *v7, unsigned long argIdx = 0)
{
    static float matrix[16];
    v7_val_t val = v7_arg(v7, 1);
    auto arrayLength = v7_array_length(v7, val);
    //assert(arrayLength == 16);
    for (auto i = 0; i < arrayLength; i++)
    {
        matrix[i] = v7_to_number(v7_array_get(v7, val, i));
    }
    return matrix;
}

v7_val_t Node_setModelMatrix(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));

    ::Node_setModelMatrix(nodePtr, toMatrix(v7, 1));

    return 0;
}

v7_val_t Camera_setViewMatrix(struct v7 *v7)
{
    ::Camera_setViewMatrix(toMatrix(v7, 0));

    return 0;
}

v7_val_t Camera_setProjectionMatrix(struct v7 *v7)
{
    ::Camera_setProjectionMatrix(toMatrix(v7, 0));

    return 0;
}

v7_val_t Scene_addEmptyTexture(struct v7 *v7)
{
    long width = v7_to_number(v7_arg(v7, 0));
    long height = v7_to_number(v7_arg(v7, 1));
    long result = ::Scene_addEmptyTexture(width, height);

    return v7_to_number(result);
}

v7_val_t Texture_update(struct v7 *v7)
{
    long texturePtr = v7_to_number(v7_arg(v7, 0));
    const char* srcARGB8 = toString(v7, 1);
    ::Texture_update(texturePtr, srcARGB8);

    return 0;
}

v7_val_t Scene_addFullScreenTextureNode(struct v7 *v7)
{
    long texturePtr = v7_to_number(v7_arg(v7, 0));
    int rotationType = v7_to_number(v7_arg(v7, 1));
    long result = ::Scene_addFullScreenTextureNode(texturePtr, rotationType);

    return v7_to_number(result);
}

v7_val_t Texture_renderFullScreen(struct v7 *v7)
{
    long texturePtr = v7_to_number(v7_arg(v7, 0));
    int rotationType = v7_to_number(v7_arg(v7, 1));
    ::Texture_renderFullScreen(texturePtr, rotationType);

    return 0;
}

v7_val_t Texture_write(struct v7 *v7)
{
    long texturePtr = v7_to_number(v7_arg(v7, 0));
    const char* filename = toString(v7, 1);
    ::Texture_write(texturePtr, filename);

    return 0;
}

v7_val_t Scene_pickNodeFromScreen(struct v7 *v7)
{
    int x = v7_to_number(v7_arg(v7, 0));
    int y= v7_to_number(v7_arg(v7, 1));
    long result = ::Scene_pickNodeFromScreen(x, y);

    return v7_to_number(result);
}

v7_val_t Scene_pickNodeFromScreenPrecisely(struct v7 *v7)
{
    // TODO:
    return Scene_pickNodeFromScreen(v7);
}

v7_val_t Node_setMaterialType(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    MaterialType materialType = (MaterialType)(int)v7_to_number(v7_arg(v7, 1));
    ::Node_setMaterialType(nodePtr, materialType);

    return 0;
}

v7_val_t Node_setMaterialTypeAt(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    unsigned int mtrl = v7_to_number(v7_arg(v7, 1));
    MaterialType materialType = (MaterialType)(int)v7_to_number(v7_arg(v7, 2));
    ::Node_setMaterialTypeAt(nodePtr, mtrl, materialType);

    return 0;
}

v7_val_t MeshNode_setShadowMode(struct v7 *v7)
{
    long nodePtr = v7_to_number(v7_arg(v7, 0));
    ShadowMode mode = (ShadowMode)(int)v7_to_number(v7_arg(v7, 1));
    ::MeshNode_setShadowMode(nodePtr, mode);

    return 0;
}

v7_val_t Scene_setVisible(struct v7 *v7)
{
    int visible = v7_to_number(v7_arg(v7, 0));
    ::Scene_setVisible(visible);

    return 0;
}

v7_val_t Scene_initializeFromDevice(struct v7 *v7)
{
    long irrlichtDevice = v7_to_number(v7_arg(v7, 0));
    ::Scene_initializeFromDevice(irrlichtDevice);

    return 0;
}

}

void bindSceneAPIs()
{
#define BIND_API(function) v7_set_method(v7, v7_get_global(v7), #function, &function)
    BIND_API(Scene_addLightNode);
    BIND_API(Scene_loadScene);
    BIND_API(Scene_runScript);
    BIND_API(Scene_addMeshNode);
    BIND_API(Scene_addCubeNode);
    BIND_API(Scene_addPlaneNode);
    BIND_API(Scene_addSphereNode);
    BIND_API(Scene_addFullScreenTextureNode);
    BIND_API(Scene_addImageFromFile);
    BIND_API(Scene_addTextureFromImage);
    BIND_API(Scene_addTexture);
    BIND_API(Scene_addEmptyTexture);
    BIND_API(Scene_initializeRenderer);
    BIND_API(Scene_initializeFromDevice);
    BIND_API(Scene_destroy);
    BIND_API(Scene_clear);
    BIND_API(Scene_render);
    BIND_API(Scene_getNodeFromName);
    BIND_API(Scene_removeNode);
    BIND_API(Scene_getRootNode);
    BIND_API(Scene_pickNodeFromScreen);
    BIND_API(Scene_setVisible);
    BIND_API(Scene_setAnimationCallback);
    BIND_API(Camera_setViewMatrix);
    BIND_API(Camera_setProjectionMatrix);
    BIND_API(Node_setVisible);
    BIND_API(Node_setPosition);
    BIND_API(Node_setRotation);
    BIND_API(Node_setScale);
    BIND_API(Node_setParent);
    BIND_API(Node_setModelMatrix);
    BIND_API(Node_setBillboard);
    BIND_API(Node_setLighting);
    BIND_API(Node_setTextureAt);
    BIND_API(Node_setTexture);
    BIND_API(Node_setSecondTextureAt);
    BIND_API(Node_setMaterialType);
    BIND_API(Node_setMaterialTypeAt);
    BIND_API(MeshNode_setAnimationFps);
    BIND_API(MeshNode_setAnimationLoop);
    BIND_API(MeshNode_setAnimationByName);
    BIND_API(MeshNode_setAnimationByIndex);
    BIND_API(MeshNode_setAnimationByRange);
    BIND_API(MeshNode_setShadowMode);
    BIND_API(LightNode_setType);
    BIND_API(LightNode_setRadius);
    BIND_API(LightNode_setDiffuseColor);
    BIND_API(Texture_update);
    BIND_API(Texture_renderFullScreen);
    BIND_API(Texture_write);
#undef BIND_API
}

void Scene_runScript(const char* scriptFileName)
{
    if (v7 == NULL)
    {
        v7 = v7_create();
        bindSceneAPIs();
    }
    auto file = fs->createAndOpenFile(scriptFileName);
    if (file == NULL) return;

    core::array<c8> scriptCode(file->getSize());
    file->seek(0);
    file->read(scriptCode.pointer(), file->getSize());
    file->drop();

    v7_val_t result;
    v7_exec(v7, (const char*)scriptCode.pointer(), &result);
}
