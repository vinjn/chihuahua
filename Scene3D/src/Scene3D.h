#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef char s3dBool;   ///< Boolean type
#define s3dFalse 0      ///< s3dBool value of false.
#define s3dTrue  1      ///< s3dBool value of true.

#ifdef S3D_EXPORTS
    #ifdef _WIN32
        #define S3D_API __declspec(dllexport)
    #else
        #define S3D_API __attribute__ ((visibility("default")))
    #endif
#else
    #ifdef _WIN32
        #define S3D_API __declspec(dllimport)
    #else
        #define S3D_API 
    #endif
#endif

// Scene methods
S3D_API long Scene_addLightNode();
S3D_API long Scene_loadScene(const char* sceneFileName);

S3D_API long Scene_addMeshNode(const char* meshFileName);
S3D_API long Scene_addCubeNode(float size);
S3D_API long Scene_addPlaneNode(float width, float height);
S3D_API long Scene_addSphereNode(float radius);
S3D_API long Scene_addFullScreenTextureNode(long texturePtr, int rotationType);
S3D_API long Scene_addImageFromFile(const char* imageFileName);
S3D_API long Scene_addTextureFromImage(long imagePtr);

S3D_API long Scene_addTexture(const char* textureName);
S3D_API long Scene_addEmptyTexture(int width, int height);

S3D_API void Scene_initializeRenderer(int width, int height);
S3D_API void Scene_initializeFromDevice(long device);
S3D_API void Scene_destroy();
S3D_API void Scene_clear();
S3D_API void Scene_render();
S3D_API long Scene_getNodeFromName(const char* nodeName);
S3D_API void Scene_removeNode(long nodePtr);
S3D_API long Scene_getRootNode();
S3D_API long Scene_pickNodeFromScreen(int x, int y);
// TODO: implement it in wrapper
// long Scene_pickNodeFromScreenPrecisely(int x, int y);
S3D_API void Scene_setVisible(s3dBool visible);

typedef void (*NodePtrFunctor)(long nodePtr);
S3D_API void Scene_setAnimationCallback(NodePtrFunctor cb);

// Camera
S3D_API void Camera_setViewMatrix(const float* matrix);
S3D_API void Camera_setProjectionMatrix(const float* matrix);

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
S3D_API void Node_setVisible(long nodePtr, s3dBool visible);
S3D_API void Node_setPosition(long nodePtr, float x, float y, float z);
S3D_API void Node_setRotation(long nodePtr, float x, float y, float z);
S3D_API void Node_setScale(long nodePtr, float x, float y, float z);
S3D_API void Node_setParent(long nodePtr, long parentPtr);
S3D_API void Node_setModelMatrix(long nodePtr, const float* matrix);
// TODO: implement it in engine
S3D_API void Node_setBillboard(long nodePtr, s3dBool isBillboard);
S3D_API void Node_setLighting(long nodePtr, s3dBool enabled);
S3D_API void Node_setTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
S3D_API void Node_setTexture(long nodePtr, long texturePtr);
S3D_API void Node_setSecondTextureAt(long nodePtr, unsigned int mtrl, long texturePtr);
S3D_API void Node_setMaterialType(long nodePtr, MaterialType type);
S3D_API void Node_setMaterialTypeAt(long nodePtr, unsigned int mtrl, MaterialType type);

// MeshNode methods
S3D_API void MeshNode_setAnimationFps(long nodePtr, float fps);
S3D_API void MeshNode_setAnimationLoop(long nodePtr, s3dBool isLoop);
S3D_API void MeshNode_setAnimationByName(long nodePtr, const char* animationName);
S3D_API void MeshNode_setAnimationByIndex(long nodePtr, int index);
S3D_API void MeshNode_setAnimationByRange(long nodePtr, int start, int end);

// LightNode methods
typedef enum
{
    PointLight,
    DirectionalLight,
    LightType_EnumSize  = 0x7fffffff ///< \internal Force type int32_t.
} LightType;
S3D_API void LightNode_setType(long nodePtr, LightType lightType);
S3D_API void LightNode_setRadius(long nodePtr, float radius);
S3D_API void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float a);

// Texture methods
S3D_API void Texture_update(long texturePtr, const char* srcARGB8);
S3D_API void Texture_renderFullScreen(long texturePtr, int rotationType);
S3D_API void Texture_write(long texturePtr, const char* filename);

#ifdef __cplusplus
}
#endif
