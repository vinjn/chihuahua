#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CHIHUAHUA_EXPORTS
    #ifdef _WIN32
        #define CHIHUAHUA_API __declspec(dllexport)
    #else
        #define CHIHUAHUA_API __attribute__ ((visibility("default")))
    #endif
#else
    #ifdef _WIN32
        #define CHIHUAHUA_API __declspec(dllimport)
    #else
        #define CHIHUAHUA_API 
    #endif
#endif

// Scene methods
CHIHUAHUA_API long Scene_addLightNode();
CHIHUAHUA_API long Scene_loadScene(const char* sceneFileName);

// TODO: error check?
CHIHUAHUA_API void Scene_runScript(const char* scriptFileName);

CHIHUAHUA_API long Scene_addMeshNode(const char* meshFileName);
CHIHUAHUA_API long Scene_addCubeNode(float size);

CHIHUAHUA_API long Scene_addPlaneNode(float width, float height);
CHIHUAHUA_API long Scene_addSphereNode(float radius);
CHIHUAHUA_API long Scene_addFullScreenTextureNode(long texturePtr, int rotationType);
CHIHUAHUA_API long Scene_addImageFromFile(const char* imageFileName);
CHIHUAHUA_API long Scene_addTextureFromImage(long imagePtr);

CHIHUAHUA_API long Scene_addTexture(const char* textureName);
CHIHUAHUA_API long Scene_addEmptyTexture(int width, int height);

typedef enum
{
    API_OPENGL,
    API_OPENGL_ES,
    API_D3D9,
    API_D3D11,
    API_D3D12,
    API_METAL,
    API_VULKAN,
    API_COUNT,
} ApiType;
CHIHUAHUA_API void Scene_initializeRenderer(int width, int height, long windowHandle, ApiType apiType);
CHIHUAHUA_API void Scene_destroy();
CHIHUAHUA_API void Scene_clear(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
CHIHUAHUA_API void Scene_render();
CHIHUAHUA_API long Scene_getNodeFromName(const char* nodeName);
CHIHUAHUA_API void Scene_removeNode(long nodePtr);
CHIHUAHUA_API long Scene_getRootNode();
CHIHUAHUA_API long Scene_pickNodeFromScreen(int x, int y);
// TODO: implement it in wrapper
// long Scene_pickNodeFromScreenPrecisely(int x, int y);
CHIHUAHUA_API void Scene_setVisible(int visible);

typedef void (*NodePtrFunctor)(long nodePtr);
CHIHUAHUA_API void Scene_setAnimationCallback(NodePtrFunctor cb);

// Camera
CHIHUAHUA_API void Camera_setViewMatrix(const float* matrix);
CHIHUAHUA_API void Camera_setProjectionMatrix(const float* matrix);

// Node methods
typedef enum
{
    Solid,
    ColorAdd,
    Transparent,
    NormalMap,
    LightMap,
    MaterialType_EnumSize  = 0x7fffffff ///< \internal Force type int32_t.
} MaterialType;

typedef enum
{
    Shadow_ReceiveOnly,
    Shadow_CastOnly,
    Shadow_Both,
    Shadow_Off,
} ShadowMode;

CHIHUAHUA_API void Node_setVisible(long nodePtr, int isVisible);
CHIHUAHUA_API void Node_setPosition(long nodePtr, float x, float y, float z);
CHIHUAHUA_API void Node_setRotation(long nodePtr, float x, float y, float z);
CHIHUAHUA_API void Node_setScale(long nodePtr, float x, float y, float z);
CHIHUAHUA_API void Node_setParent(long nodePtr, long parentPtr);
CHIHUAHUA_API void Node_setModelMatrix(long nodePtr, const float* matrix);
// TODO: implement it in engine
CHIHUAHUA_API void Node_setBillboard(long nodePtr, int isBillboard);
CHIHUAHUA_API void Node_setLighting(long nodePtr, int isLighting);
CHIHUAHUA_API void Node_setTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
CHIHUAHUA_API void Node_setTexture(long nodePtr, long texturePtr);
CHIHUAHUA_API void Node_setSecondTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
CHIHUAHUA_API void Node_setMaterialType(long nodePtr, MaterialType type);
CHIHUAHUA_API void Node_setMaterialTypeAt(long nodePtr, unsigned int mtrl, MaterialType type);

// MeshNode methods
CHIHUAHUA_API void MeshNode_setAnimationFps(long nodePtr, float fps);
CHIHUAHUA_API void MeshNode_setAnimationLoop(long nodePtr, int isLoop);
CHIHUAHUA_API void MeshNode_setAnimationByName(long nodePtr, const char* animationName);
CHIHUAHUA_API void MeshNode_setAnimationByIndex(long nodePtr, int index);
CHIHUAHUA_API void MeshNode_setAnimationByRange(long nodePtr, int start, int end);
CHIHUAHUA_API void MeshNode_setShadowMode(long nodePtr, ShadowMode mode);

// LightNode methods
typedef enum
{
    PointLight,
    DirectionalLight,
    LightType_EnumSize  = 0x7fffffff ///< \internal Force type int32_t.
} LightType;
CHIHUAHUA_API void LightNode_setType(long nodePtr, LightType lightType);
CHIHUAHUA_API void LightNode_setRadius(long nodePtr, float radius);
CHIHUAHUA_API void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float a);

// Texture methods
CHIHUAHUA_API void Texture_update(long texturePtr, const char* srcARGB8);
CHIHUAHUA_API void Texture_renderFullScreen(long texturePtr, int rotationType);
CHIHUAHUA_API void Texture_write(long texturePtr, const char* filename);

#ifdef __cplusplus
}
#endif
