#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UENGINE_EXPORTS
    #ifdef _WIN32
        #define UENGINE_API __declspec(dllexport)
    #else
        #define UENGINE_API __attribute__ ((visibility("default")))
    #endif
#else
    #ifdef _WIN32
        #define UENGINE_API __declspec(dllimport)
    #else
        #define UENGINE_API 
    #endif
#endif

// Scene methods
UENGINE_API long Scene_addLightNode();
UENGINE_API long Scene_loadScene(const char* sceneFileName);

UENGINE_API long Scene_addMeshNode(const char* meshFileName);
UENGINE_API long Scene_addCubeNode(float size);
UENGINE_API long Scene_addPlaneNode(float width, float height);
UENGINE_API long Scene_addSphereNode(float radius);
UENGINE_API long Scene_addFullScreenTextureNode(long texturePtr, int rotationType);
UENGINE_API long Scene_addImageFromFile(const char* imageFileName);
UENGINE_API long Scene_addTextureFromImage(long imagePtr);

UENGINE_API long Scene_addTexture(const char* textureName);
UENGINE_API long Scene_addEmptyTexture(int width, int height);

UENGINE_API void Scene_initializeRenderer(int width, int height);
UENGINE_API void Scene_initializeFromDevice(long device);
UENGINE_API void Scene_destroy();
UENGINE_API void Scene_clear();
UENGINE_API void Scene_render();
UENGINE_API long Scene_getNodeFromName(const char* nodeName);
UENGINE_API void Scene_removeNode(long nodePtr);
UENGINE_API long Scene_getRootNode();
UENGINE_API long Scene_pickNodeFromScreen(int x, int y);
// TODO: implement it in wrapper
// long Scene_pickNodeFromScreenPrecisely(int x, int y);
UENGINE_API void Scene_setVisible(int visible);

typedef void (*NodePtrFunctor)(long nodePtr);
UENGINE_API void Scene_setAnimationCallback(NodePtrFunctor cb);

// Camera
UENGINE_API void Camera_setViewMatrix(const float* matrix);
UENGINE_API void Camera_setProjectionMatrix(const float* matrix);

// Node methods
typedef enum
{
    Solid,
    ColorAdd,
    AlphaBlend,
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

UENGINE_API void Node_setVisible(long nodePtr, int isVisible);
UENGINE_API void Node_setPosition(long nodePtr, float x, float y, float z);
UENGINE_API void Node_setRotation(long nodePtr, float x, float y, float z);
UENGINE_API void Node_setScale(long nodePtr, float x, float y, float z);
UENGINE_API void Node_setParent(long nodePtr, long parentPtr);
UENGINE_API void Node_setModelMatrix(long nodePtr, const float* matrix);
// TODO: implement it in engine
UENGINE_API void Node_setBillboard(long nodePtr, int isBillboard);
UENGINE_API void Node_setLighting(long nodePtr, int isLighting);
UENGINE_API void Node_setTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
UENGINE_API void Node_setTexture(long nodePtr, long texturePtr);
UENGINE_API void Node_setSecondTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
UENGINE_API void Node_setMaterialType(long nodePtr, MaterialType type);
UENGINE_API void Node_setMaterialTypeAt(long nodePtr, unsigned int mtrl, MaterialType type);

// MeshNode methods
UENGINE_API void MeshNode_setAnimationFps(long nodePtr, float fps);
UENGINE_API void MeshNode_setAnimationLoop(long nodePtr, int isLoop);
UENGINE_API void MeshNode_setAnimationByName(long nodePtr, const char* animationName);
UENGINE_API void MeshNode_setAnimationByIndex(long nodePtr, int index);
UENGINE_API void MeshNode_setAnimationByRange(long nodePtr, int start, int end);
UENGINE_API void MeshNode_setShadowMode(long nodePtr, ShadowMode mode);

// LightNode methods
typedef enum
{
    PointLight,
    DirectionalLight,
    LightType_EnumSize  = 0x7fffffff ///< \internal Force type int32_t.
} LightType;
UENGINE_API void LightNode_setType(long nodePtr, LightType lightType);
UENGINE_API void LightNode_setRadius(long nodePtr, float radius);
UENGINE_API void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float a);

// Texture methods
UENGINE_API void Texture_update(long texturePtr, const char* srcARGB8);
UENGINE_API void Texture_renderFullScreen(long texturePtr, int rotationType);
UENGINE_API void Texture_write(long texturePtr, const char* filename);

#ifdef __cplusplus
}
#endif
