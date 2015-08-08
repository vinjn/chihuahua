#pragma once

namespace Scene3D
{
    long addPointLight(float lightRadius);

    void resize(int width, int height);

    void destroyScene();

    void clear();

    void render();

    long getNodeFromName(const char* nodeName);
    long loadScene(const char* sceneFileName);

    long addCubeNode(float size);
    long addPlaneNode(float width, float height);
    long addSphereNode(float radius);

    long getTexture(const char* textureName);

    void removeNode(long nodePtr);
    long addMeshNode(const char* meshName);
    long getRootNode();

    //
    void setNodeVisible(long nodePtr, bool visible);
    void setNodePosition(long nodePtr, float x, float y, float z);
    void setNodeRotation(long nodePtr, float x, float y, float z);
    void setNodeScale(long nodePtr, float x, float y, float z);
    void setNodeParent(long nodePtr, long parentPtr);
    void setNodeModelMatrix(long nodePtr, const float* matrix);
    void setNodeLighting(long nodePtr, bool enabled);
    void setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);

    void setNodeAnimationFps(long nodePtr, float fps);
    void setNodeAnimation(long nodePtr, const char* animationName);
    void setNodeAnimationLoop(long nodePtr, bool isLoop);
    void setNodeAnimationIndex(long nodePtr, int index);
    void setNodeAnimationStartEnd(long nodePtr, int start, int end);

    //
    void setViewMatrix(const float* matrix);
    void setProjectionMatrix(const float* matrix);

    long addTexture(int width, int height);
    void updateTexture(long texturePtr, const char* srcData);
    long addFullScreenTextureNode(long texturePtr, int rotationType);
    void drawFullScreenTexture(long texturePtr, int rotationType);
    void writeTexture(long texturePtr, const char* filename);

    long pickNodeFromScreen(int x, int y);    
}

