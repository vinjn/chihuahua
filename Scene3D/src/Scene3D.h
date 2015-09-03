#pragma once

// Scene methods
long Scene_addLightNode();
long Scene_addMeshNode(const char* meshName);
long Scene_addCubeNode(float size);
long Scene_addPlaneNode(float width, float height);
long Scene_addSphereNode(float radius);
long Scene_loadScene(const char* sceneFileName);

void Scene_resize(int width, int height);
void Scene_destroy();
void Scene_clear();
void Scene_render();
long Scene_getNodeFromName(const char* nodeName);
void Scene_removeNode(long nodePtr);
long Scene_getRootNode();
long Scene_pickNodeFromScreen(int x, int y);

long Scene_addTexture(const char* textureName);
long Scene_addEmptyTexture(int width, int height);
long Scene_addFullScreenTextureNode(long texturePtr, int rotationType);

// Camera
void Camera_setViewMatrix(const float* matrix);
void Camera_setProjectionMatrix(const float* matrix);

// Node methods
void Node_setVisible(long nodePtr, bool visible);
void Node_setPosition(long nodePtr, float x, float y, float z);
void Node_setRotation(long nodePtr, float x, float y, float z);
void Node_setScale(long nodePtr, float x, float y, float z);
void Node_setParent(long nodePtr, long parentPtr);
void Node_setModelMatrix(long nodePtr, const float* matrix);

// MeshNode methods
enum MaterialType
{
    // for MeshNode_setMaterialType()
    Solid,
    ColorAdd,
    AlphaBlend,
    NormalMap,
    LightMap,
};    

void MeshNode_setLighting(long nodePtr, bool enabled);
void MeshNode_setTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);
void MeshNode_setMaterialType(long nodePtr, MaterialType materialType);
void MeshNode_setAnimationFps(long nodePtr, float fps);
void MeshNode_setAnimation(long nodePtr, const char* animationName);
void MeshNode_setAnimationLoop(long nodePtr, bool isLoop);
void MeshNode_setAnimationIndex(long nodePtr, int index);
void MeshNode_setAnimationStartEnd(long nodePtr, int start, int end);
// TODO: implement it
void MeshNode_setBillboard(long nodePtr, bool isBillboard);

// LightNode methods
enum LightType
{
    // for LightNode_setType()
    PointLight,
    DirectionalLight,
};
void LightNode_setType(long nodePtr, LightType lightType);
void LightNode_setRadius(long nodePtr, float radius);
void LightNode_setDiffuseColor(long nodePtr, float r, float g, float b, float b);

// Texture methods
void Texture_update(long texturePtr, const char* srcARGB8);
void Texture_renderFullscreen(long texturePtr, int rotationType);
void Texture_write(long texturePtr, const char* filename);

