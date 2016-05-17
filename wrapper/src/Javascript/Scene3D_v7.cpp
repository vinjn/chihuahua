#include "../Scene3D.h"
#include "irrlicht.h"

extern "C"
{
    #include "../../../3rdparty/v7/v7.h"
}

using namespace ue;
using namespace core;

struct v7 *v7;
extern io::IFileSystem* fs;

void Scene_runScript(const char* scriptFileName);

namespace {

v7_err Scene_addLightNode(struct v7 *v7, v7_val_t *res)
{
    long result = ::Scene_addLightNode();
    *res = v7_mk_number(result);

    return V7_OK;
}

v7_err LightNode_setType(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    LightType lightType = (LightType)(int)v7_get_double(v7_arg(v7, 1));
    ::LightNode_setType(nodePtr, lightType);

    return V7_OK;
}

v7_err LightNode_setRadius(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float radius = v7_get_double(v7_arg(v7, 1));
    ::LightNode_setRadius(nodePtr, radius);

    return V7_OK;
}

v7_err LightNode_setDiffuseColor(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float r = v7_get_double(v7_arg(v7, 1));
    float g = v7_get_double(v7_arg(v7, 2));
    float b = v7_get_double(v7_arg(v7, 3));
    float a = v7_get_double(v7_arg(v7, 4));
    ::LightNode_setDiffuseColor(nodePtr, r, g, b, a);

    return V7_OK;
}

v7_err Scene_initializeRenderer(struct v7 *v7, v7_val_t *res)
{
    int width = v7_get_double(v7_arg(v7, 0));
    int height = v7_get_double(v7_arg(v7, 1));
    long windowHandle = v7_get_double(v7_arg(v7, 2));
    ApiType apiType = (ApiType)(int)v7_get_double(v7_arg(v7, 3));
    ::Scene_initializeRenderer(width, height, windowHandle, apiType);

    return V7_OK;
}

v7_err Scene_clear(struct v7 *v7, v7_val_t *res)
{
    unsigned char r = v7_get_double(v7_arg(v7, 0));
    unsigned char g = v7_get_double(v7_arg(v7, 1));
    unsigned char b = v7_get_double(v7_arg(v7, 2));
    unsigned char a = v7_get_double(v7_arg(v7, 3));    
    ::Scene_clear(r, g, b, a);

    return V7_OK;
}

v7_err Scene_render(struct v7 *v7, v7_val_t *res)
{
    ::Scene_render();

    return V7_OK;
}

static const char* toString(struct v7 *v7, unsigned long argIdx = 0)
{
    auto arg = v7_arg(v7, argIdx);
    size_t string_len;
    return v7_get_string(v7, &arg, &string_len);
}

v7_err Scene_loadScene(struct v7 *v7, v7_val_t *res)
{
    long result = ::Scene_loadScene(toString(v7));

    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Scene_getNodeFromName(struct v7 *v7, v7_val_t *res)
{
    long result = ::Scene_getNodeFromName(toString(v7));
    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Scene_addCubeNode(struct v7 *v7, v7_val_t *res)
{
    float size = v7_get_double(v7_arg(v7, 0));
    long result = ::Scene_addCubeNode(size);
    
    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Scene_addPlaneNode(struct v7 *v7, v7_val_t *res)
{
    float width = v7_get_double(v7_arg(v7, 0));
    float height = v7_get_double(v7_arg(v7, 1));
    long result = ::Scene_addPlaneNode(width, height);

    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Scene_addSphereNode(struct v7 *v7, v7_val_t *res)
{
    float radius = v7_get_double(v7_arg(v7, 0));
    long result = ::Scene_addSphereNode(radius);

    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Scene_addTexture(struct v7 *v7, v7_val_t *res)
{
    long result = ::Scene_addTexture(toString(v7));
    *res = v7_mk_number(result);
    return V7_OK;
}

v7_err Node_setLighting(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int enabled = v7_get_double(v7_arg(v7, 1));
    ::Node_setLighting(nodePtr, enabled);

    return V7_OK;
}

v7_err Node_setTextureAt(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    unsigned int mtrl = v7_get_double(v7_arg(v7, 1));
    long texturePtr = v7_get_double(v7_arg(v7, 2));
    ::Node_setTextureAt(nodePtr, mtrl, texturePtr);

    return V7_OK;
}

v7_err Node_setTexture(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    long texturePtr = v7_get_double(v7_arg(v7, 1));
    ::Node_setTexture(nodePtr, texturePtr);

    return V7_OK;
}

v7_err Node_setSecondTextureAt(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    unsigned int mtrl = v7_get_double(v7_arg(v7, 1));
    long texturePtr = v7_get_double(v7_arg(v7, 2));
    ::Node_setSecondTextureAt(nodePtr, mtrl, texturePtr);

    return V7_OK;
}

v7_err Scene_setAnimationCallback(struct v7 *v7, v7_val_t *res)
{
    // TODO:

    return V7_OK;
}

v7_err Node_setBillboard(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int isBillboard = v7_get_double(v7_arg(v7, 1));
    ::Node_setBillboard(nodePtr, isBillboard);

    return V7_OK;
}

v7_err MeshNode_setAnimationFps(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float fps = v7_get_double(v7_arg(v7, 1));
    ::MeshNode_setAnimationFps(nodePtr, fps);

    return V7_OK;
}

v7_err MeshNode_setAnimationByName(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    const char* animationName = toString(v7, 1);
    ::MeshNode_setAnimationByName(nodePtr, animationName);
 
    return V7_OK;
}

v7_err Scene_runScript(struct v7 *v7, v7_val_t *res)
{
    const char* scriptFileName = toString(v7, 0);
    ::Scene_runScript(scriptFileName);

    return V7_OK;
}

v7_err MeshNode_setAnimationLoop(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int isLoop = v7_get_double(v7_arg(v7, 1));
    ::MeshNode_setAnimationLoop(nodePtr, isLoop);

    return V7_OK;
}

v7_err MeshNode_setAnimationByIndex(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int index = v7_get_double(v7_arg(v7, 1));
    ::MeshNode_setAnimationByIndex(nodePtr, index);

    return V7_OK;
}

v7_err MeshNode_setAnimationByRange(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int start = v7_get_double(v7_arg(v7, 1));
    int end = v7_get_double(v7_arg(v7, 2));
    ::MeshNode_setAnimationByRange(nodePtr, start, end);

    return V7_OK;
}

v7_err Scene_destroy(struct v7 *v7, v7_val_t *res)
{
    ::Scene_destroy();

    return V7_OK;
}

v7_err Scene_removeNode(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    ::Scene_removeNode(nodePtr);

    return V7_OK;
}

v7_err Scene_addMeshNode(struct v7 *v7, v7_val_t *res)
{
    const char* meshFileName = toString(v7);
    ::Scene_addMeshNode(meshFileName);

    return V7_OK;
}

v7_err Scene_addImageFromFile(struct v7 *v7, v7_val_t *res)
{
    const char* imageFileName = toString(v7);
    ::Scene_addImageFromFile(imageFileName);

    return V7_OK;
}

v7_err Scene_addTextureFromImage(struct v7 *v7, v7_val_t *res)
{
    long imagePtr = v7_get_double(v7_arg(v7, 0));
    ::Scene_addTextureFromImage(imagePtr);

    return V7_OK;
}

v7_err Scene_getRootNode(struct v7 *v7, v7_val_t *res)
{
    long result = ::Scene_getRootNode();

    *res = v7_get_double(result);
    return V7_OK;
}

v7_err Node_setVisible(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    int visible = v7_get_double(v7_arg(v7, 1));
    ::Node_setVisible(nodePtr, visible);

    return V7_OK;
}

v7_err Node_setPosition(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float x = v7_get_double(v7_arg(v7, 1));
    float y = v7_get_double(v7_arg(v7, 2));
    float z = v7_get_double(v7_arg(v7, 3));
    ::Node_setPosition(nodePtr, x, y, z);

    return V7_OK;
}

v7_err Node_setRotation(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float x = v7_get_double(v7_arg(v7, 1));
    float y = v7_get_double(v7_arg(v7, 2));
    float z = v7_get_double(v7_arg(v7, 3));
    ::Node_setRotation(nodePtr, x, y, z);

    return V7_OK;
}

v7_err Node_setScale(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    float x = v7_get_double(v7_arg(v7, 1));
    float y = v7_get_double(v7_arg(v7, 2));
    float z = v7_get_double(v7_arg(v7, 3));
    ::Node_setScale(nodePtr, x, y, z);

    return V7_OK;
}

v7_err Node_setParent(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    long parentPtr = v7_get_double(v7_arg(v7, 1));
    ::Node_setParent(nodePtr, parentPtr);

    return V7_OK;
}

static const float* toMatrix(struct v7 *v7, unsigned long argIdx = 0)
{
    static float matrix[16];
    v7_val_t val = v7_arg(v7, 1);
    auto arrayLength = v7_array_length(v7, val);
    //assert(arrayLength == 16);
    for (auto i = 0; i < arrayLength; i++)
    {
        matrix[i] = v7_get_double(v7_array_get(v7, val, i));
    }
    return matrix;
}

v7_err Node_setModelMatrix(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));

    ::Node_setModelMatrix(nodePtr, toMatrix(v7, 1));

    return V7_OK;
}

v7_err Camera_setViewMatrix(struct v7 *v7, v7_val_t *res)
{
    ::Camera_setViewMatrix(toMatrix(v7, 0));

    return V7_OK;
}

v7_err Camera_setProjectionMatrix(struct v7 *v7, v7_val_t *res)
{
    ::Camera_setProjectionMatrix(toMatrix(v7, 0));

    return V7_OK;
}

v7_err Scene_addEmptyTexture(struct v7 *v7, v7_val_t *res)
{
    long width = v7_get_double(v7_arg(v7, 0));
    long height = v7_get_double(v7_arg(v7, 1));
    long result = ::Scene_addEmptyTexture(width, height);

    *res = v7_get_double(result);
    return V7_OK;
}

v7_err Texture_update(struct v7 *v7, v7_val_t *res)
{
    long texturePtr = v7_get_double(v7_arg(v7, 0));
    const char* srcARGB8 = toString(v7, 1);
    ::Texture_update(texturePtr, srcARGB8);

    return V7_OK;
}

v7_err Scene_addFullScreenTextureNode(struct v7 *v7, v7_val_t *res)
{
    long texturePtr = v7_get_double(v7_arg(v7, 0));
    int rotationType = v7_get_double(v7_arg(v7, 1));
    long result = ::Scene_addFullScreenTextureNode(texturePtr, rotationType);

    *res = v7_get_double(result);
    return V7_OK;
}

v7_err Texture_renderFullScreen(struct v7 *v7, v7_val_t *res)
{
    long texturePtr = v7_get_double(v7_arg(v7, 0));
    int rotationType = v7_get_double(v7_arg(v7, 1));
    ::Texture_renderFullScreen(texturePtr, rotationType);

    return V7_OK;
}

v7_err Texture_write(struct v7 *v7, v7_val_t *res)
{
    long texturePtr = v7_get_double(v7_arg(v7, 0));
    const char* filename = toString(v7, 1);
    ::Texture_write(texturePtr, filename);

    return V7_OK;
}

v7_err Scene_pickNodeFromScreen(struct v7 *v7, v7_val_t *res)
{
    int x = v7_get_double(v7_arg(v7, 0));
    int y= v7_get_double(v7_arg(v7, 1));
    long result = ::Scene_pickNodeFromScreen(x, y);

    *res = v7_get_double(result);
    return V7_OK;
}

v7_err Scene_pickNodeFromScreenPrecisely(struct v7 *v7, v7_val_t *res)
{
    // TODO:
    return Scene_pickNodeFromScreen(v7, res);
}

v7_err Node_setMaterialType(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    MaterialType materialType = (MaterialType)(int)v7_get_double(v7_arg(v7, 1));
    ::Node_setMaterialType(nodePtr, materialType);

    return V7_OK;
}

v7_err Node_setMaterialTypeAt(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    unsigned int mtrl = v7_get_double(v7_arg(v7, 1));
    MaterialType materialType = (MaterialType)(int)v7_get_double(v7_arg(v7, 2));
    ::Node_setMaterialTypeAt(nodePtr, mtrl, materialType);

    return V7_OK;
}

v7_err MeshNode_setShadowMode(struct v7 *v7, v7_val_t *res)
{
    long nodePtr = v7_get_double(v7_arg(v7, 0));
    ShadowMode mode = (ShadowMode)(int)v7_get_double(v7_arg(v7, 1));
    ::MeshNode_setShadowMode(nodePtr, mode);

    return V7_OK;
}

v7_err Scene_setVisible(struct v7 *v7, v7_val_t *res)
{
    int visible = v7_get_double(v7_arg(v7, 0));
    ::Scene_setVisible(visible);

    return V7_OK;
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
