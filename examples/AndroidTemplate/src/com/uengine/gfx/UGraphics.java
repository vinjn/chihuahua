package com.uengine.gfx;

import android.content.Context;
import android.content.res.AssetManager;

public class UGraphics {

	static {
		System.loadLibrary("UGraphics");
	}
	private final Context context;

	public UGraphics(Context context) {
		this.context = context;
		AssetManager amgr = context.getAssets();
		String internalDataPath = context.getFilesDir().getAbsolutePath();
		Scene_initializeFileSystem(internalDataPath, amgr);
	}

	private native void Scene_initializeFileSystem(String jInternalDataPath,
			AssetManager amgr);

	// Scene methods
	public static native long Scene_addLightNode();

	public static native long Scene_loadScene(String sceneFileName);

	public static native long Scene_addMeshNode(String meshName);

	public static native long Scene_addCubeNode(float size);

	public static native long Scene_addPlaneNode(float width, float height);

	public static native long Scene_addSphereNode(float radius);

	public static native long Scene_addTexture(String textureName);

	public static native long Scene_addImageFromFile(String imageFileName);

	public static native long Scene_addTextureFromImage(long imagePtr);

	public static native long Scene_addEmptyTexture(int width, int height);

	public static native void Scene_initializeRenderer(int width, int height);

	public static native void Scene_destroy();

	public static native void Scene_clear();

	public static native void Scene_render();

	public static native long Scene_getNodeFromName(String nodeName);

	public static native void Scene_removeNode(long nodePtr);

	public static native long Scene_getRootNode();

	public static native long Scene_pickNodeFromScreen(int x, int y);

	public static long Scene_pickNodeFromScreen(float x, float y) {
		return Scene_pickNodeFromScreen((int) x, (int) y);
	}

	public static native void Scene_setVisible(boolean visible);

	public static native long Scene_addFullScreenTextureNode(long texturePtr,
			int rotationType);

	// Camera
	public static native void Camera_setViewMatrix(float[] matrix);

	public static native void Camera_setProjectionMatrix(float[] matrix);

	// Node methods
	// for Node_setMaterialType()
	public static final int Solid = 0;
	public static final int ColorAdd = 1;
	public static final int AlphaBlend = 2;
	public static final int NormalMap = 3;
	public static final int LightMap = 4;

	public static native void Node_setVisible(long nodePtr, boolean visible);

	public static native void Node_setPosition(long nodePtr, float x, float y,
			float z);

	public static native void Node_setRotation(long nodePtr, float x, float y,
			float z);

	public static native void Node_setScale(long nodePtr, float x, float y,
			float z);

	public static native void Node_setParent(long nodePtr, long parentPtr);

	public static native void Node_setModelMatrix(long nodePtr, float[] matrix);

	// TODO: implement it
	public static native void Node_setBillboard(long nodePtr,
			boolean isBillboard);

	public static native void Node_setLighting(long nodePtr, boolean enabled);

	public static native void Node_setTexture(long nodePtr, long texturePtr);

	public static native void Node_setTextureAtLayer(long nodePtr,
			int textureLayer, long texturePtr);

	public static native void Node_setMaterialType(long nodePtr,
			int materialType);

	// MeshNode methods
	public static native void MeshNode_setAnimationFps(long nodePtr, float fps);

	public static native void MeshNode_setAnimationLoop(long nodePtr,
			boolean isLoop);

	public static native void MeshNode_setAnimationByName(long nodePtr,
			String animationName);

	public static native void MeshNode_setAnimationByIndex(long nodePtr,
			int index);

	public static native void MeshNode_setAnimationByRange(long nodePtr,
			int start, int end);

	public interface MeshNodeCallback {
		public void onAnimationCompleted(long nodePtr);
	}

	public static native void MeshNode_registerCallback(long nodePtr,
			MeshNodeCallback callback);

	// LightNode methods
	// for LightNode_setType()
	public static final int PointLight = 0;
	public static final int DirectionalLight = 1;

	public static native void LightNode_setType(long nodePtr, int lightType);

	public static native void LightNode_setRadius(long nodePtr, float radius);

	public static native void LightNode_setDiffuseColor(long nodePtr, float r,
			float g, float b, float a);

	// Texture methods
	public static native void Texture_update(long texturePtr, byte[] srcARGB8);

	public static native void Texture_renderFullScreen(long texturePtr,
			int rotationType);

	public static native void Texture_write(long texturePtr, String filename);

}