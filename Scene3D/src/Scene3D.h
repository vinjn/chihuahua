#pragma once

typedef char s3dBool;   ///< Boolean type
#define s3dFalse 0      ///< s3dBool value of false.
#define s3dTrue  1      ///< s3dBool value of true.

#if defined(__cplusplus)
    #define S3D_CPP(x) x
#else
    #define S3D_CPP(x) /* Not C++ */
#endif

// Scene methods
long Scene_addLightNode();
long Scene_loadScene(const char* sceneFileName);

long Scene_addMeshNode(const char* meshFileName);
long Scene_addCubeNode(float size);
long Scene_addPlaneNode(float width, float height);
long Scene_addSphereNode(float radius);
long Scene_addFullScreenTextureNode(long texturePtr, int rotationType);
long Scene_addImageFromFile(const char* imageFileName);
long Scene_addTextureFromImage(long imagePtr);

long Scene_addTexture(const char* textureName);
long Scene_addEmptyTexture(int width, int height);

void Scene_initializeRenderer(int width, int height);
void Scene_destroy();
void Scene_clear();
void Scene_render();
long Scene_getNodeFromName(const char* nodeName);
void Scene_removeNode(long nodePtr);
long Scene_getRootNode();
long Scene_pickNodeFromScreen(int x, int y);
// TODO: implement it in wrapper
// long Scene_pickNodeFromScreenPrecisely(int x, int y);
void Scene_setVisible(s3dBool visible);

typedef void (*NodePtrFunctor)(long nodePtr);
void Scene_setAniamtionCallback(NodePtrFunctor cb);

// Camera
void Camera_setViewMatrix(const float* matrix);
void Camera_setProjectionMatrix(const float* matrix);

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
void Node_setVisible(long nodePtr, s3dBool visible);
void Node_setPosition(long nodePtr, float x, float y, float z);
void Node_setRotation(long nodePtr, float x, float y, float z);
void Node_setScale(long nodePtr, float x, float y, float z);
void Node_setParent(long nodePtr, long parentPtr);
void Node_setModelMatrix(long nodePtr, const float* matrix);
// TODO: implement it in engine
void Node_setBillboard(long nodePtr, s3dBool isBillboard);
void Node_setLighting(long nodePtr, s3dBool enabled);
void Node_setTexture(long nodePtr, long texturePtr);
void Node_setTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);
void Node_setMaterialType(long nodePtr, MaterialType materialType);

// MeshNode methods
void MeshNode_setAnimationFps(long nodePtr, float fps);
void MeshNode_setAnimationLoop(long nodePtr, s3dBool isLoop);
void MeshNode_setAnimationByName(long nodePtr, const char* animationName);
void MeshNode_setAnimationByIndex(long nodePtr, int index);
void MeshNode_setAnimationByRange(long nodePtr, int start, int end);

// LightNode methods
typedef enum
{
    PointLight,
    DirectionalLight,
    LightType_EnumSize  = 0x7fffffff ///< \internal Force type int32_t.
} LightType;
void LightNode_setType(long nodePtr, LightType lightType);
void LightNode_setRadius(long nodePtr, float radius);
void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float a);

// Texture methods
void Texture_update(long texturePtr, const char* srcARGB8);
void Texture_renderFullScreen(long texturePtr, int rotationType);
void Texture_write(long texturePtr, const char* filename);

