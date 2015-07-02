package com.hiscene;

import android.content.Context;
import android.content.res.AssetManager;

public class Scene3D {

	static {
		System.loadLibrary("Irrlicht");
	}
	private final Context context;

	public Scene3D(Context context) {
		this.context = context;
		AssetManager amgr = context.getAssets();
		String internalDataPath = context.getFilesDir().getAbsolutePath();
		initializeFileSystem(internalDataPath, amgr);
	}

	// general
	public static native void clear();
	private native void initializeFileSystem(String jInternalDataPath, AssetManager amgr);
	public static native void resize(int width, int height);
	public static native void render();
	public static native void destroyScene();

	// node methods
	public static native long addMeshNode(String jMeshName);
	public static native long addCubeNode(float size);
	public static native long addPlaneNode(float width, float height);
	public static native long addSphereNode(float radius);
	public static native void removeNode(long nodePtr);
	public static native long getRootNode();

	public static native void setNodePosition(long nodePtr, float x, float y, float z);
	public static native void setNodeRotation(long nodePtr, float x, float y, float z);
	public static native void setNodeScale(long nodePtr, float x, float y, float z);
	public static native void setNodeVisible(long nodePtr, boolean visible);
	public static native void setNodeParent(long nodePtr, long parentPtr);

	public static void setRootNodeVisible(boolean visible)
	{
		setNodeVisible(getRootNode(), visible);
	}
	
	public static native void setNodeTextureAtLayer(long nodePtr, int textureLayer, long texturePtr);
	public static void setNodeTexture(long nodePtr, long texturePtr)
	{
		setNodeTextureAtLayer(nodePtr, 0, texturePtr);
	}
	
	public static native void setNodeAnimationFps(long nodePtr, float fps);
	public static native void setNodeAnimation(long nodePtr, String jAnimationName);
	public static native void setNodeAnimationIndex(long nodePtr, int index);
	public static native void setNodeAnimationStartEnd(long nodePtr, int start, int end);
	public static native void setNodeAnimationLoop(long nodePtr, boolean isLoop);
	// interaction methods
	public static long pickNodeFromScreen(float x, float y)
	{
		return pickNodeFromScreen((int)x, (int)y);
	}
	
	public static native long pickNodeFromScreen(int x, int y);
	
	// matrix methods
	public static native void setNodeModelMatrix(long nodePtr, float[] jMatrix);
	public static native void setViewMatrix(float[] jMatrix);
	public static native void setProjectionMatrix(float[] jMatrix);

	// light methods
	public static native void addPointLight(float lightRadius);
	
	// texture methods
	public static native long addTexture(int width, int height);
	public static native long getTexture(String jFileName);
	public static native long addFullScreenTextureNode(long texturePtr, int rotationType);
	public static native void updateTexture(long texturePtr, byte[] jImageData);
	public static native void drawFullScreenTexture(long texturePtr, int rotationType);
	public static native void writeTexture(long texturePtr, String jFilename);
}