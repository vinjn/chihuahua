package com.uengine;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;

public final class MainActivity extends Activity implements Renderer {

	private UGraphics mScene;
	float[] modelMatrix = new float[16];
	float[] projMatrix = new float[16];

	/**
	 * Defines whether the activity is currently paused
	 */
	private boolean mActivityIsPaused;

	/**
	 * Main GLSufaceView in which everything is rendered
	 */
	private GLSurfaceView mSurfaceView;

	private long mCubeNode;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		DebugLog.w("onCreate");

		super.onCreate(savedInstanceState);

		mSurfaceView = null;
	}

	@Override
	protected void onPause() {
		DebugLog.w("onPause");

		super.onPause();

		if (mSurfaceView != null)
			mSurfaceView.onPause();

		mActivityIsPaused = true;
	}

	@Override
	protected void onResume() {
		DebugLog.w("onResume");

		super.onResume();

		mActivityIsPaused = false;

		if (mSurfaceView != null) {
			if (mSurfaceView.getParent() == null) {
				FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
						LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
				addContentView(mSurfaceView, params);
				mSurfaceView.setZOrderMediaOverlay(true);
			}

			mSurfaceView.onResume();
		}
	}

	@Override
	protected void onStart() {
		DebugLog.w("onStart");

		super.onStart();

		if (mScene == null) {
			mScene = new UGraphics(this);

			// Set empty content view
			setContentView(new FrameLayout(this));

			// Start camera only when the activity starts the first time (see
			// lifecycle:
			// http://developer.android.com/training/basics/activity-lifecycle/pausing.html)
			if (!mActivityIsPaused) {
				// startCamera();
			}

			// Create a new GLSurfaceView
			mSurfaceView = new GLSurfaceView(this);
			mSurfaceView.setEGLContextClientVersion(2); // glesv1 or glesv2
			mSurfaceView.setRenderer(this);
			mSurfaceView.setKeepScreenOn(true);
		}
	}

	@Override
	protected void onStop() {
		DebugLog.w("onStop");

		super.onStop();

		// Remove GLSurfaceView from the hierarchy because it has been destroyed
		// automatically
		if (mSurfaceView != null) {
			ViewGroup v = (ViewGroup) findViewById(android.R.id.content);
			v.removeAllViews();
			mSurfaceView = null;
		}
	}

	@Override
	protected void onDestroy() {
		DebugLog.w("onDestroy");

		super.onDestroy();
	}

	public boolean onTouchEvent(MotionEvent event) {
		// DebugLog.w(event.toString());
		if (event.getAction() == MotionEvent.ACTION_UP) {
			long hitNodePtr = UGraphics.Scene_pickNodeFromScreen(event.getX(),
					event.getY());
			if (hitNodePtr != 0 && hitNodePtr != mCubeNode) {
				UGraphics.MeshNode_setAnimationByName(hitNodePtr, "shock_down");
			}
		}

		return true;
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		DebugLog.w("onConfigurationChanged");

		// mScreenRotation = Screen.getRotation(this);
		// metaioSDK.setScreenRotation(mScreenRotation);
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public void onDrawFrame(GL10 gl) {

		UGraphics.Scene_clear();

		// mCameraImageRenderer.draw(gl, mScreenRotation);

		UGraphics.Scene_render();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if (height == 0)
			height = 1;

		DebugLog.w("onSurfaceChanged: " + width + ", " + height);
		// if (mScene == null) {

		UGraphics.Scene_resize(width, height);
		mCubeNode = UGraphics.Scene_addCubeNode(200);
		UGraphics.Node_setTexture(mCubeNode, UGraphics.Scene_addTexture("wall.jpg"));
		UGraphics.Node_setPosition(mCubeNode, 0, 0, 1000);
		final float kSize = 1000;
		for (int i = 0; i < 9; i++) {
			float x = (i / 3 - 1.0f) * kSize / 3;
			float y = (i % 3 - 1.0f) * kSize / 3;
			float z = (float) (Math.random() * kSize) - kSize / 2;
			float k = (float) (Math.random() * 0 + 3);
			long nodePtr = UGraphics.Scene_addMeshNode("metaioman.md2");
			UGraphics.Node_setTexture(nodePtr,
					UGraphics.Scene_addTexture("metaioman.png"));
			UGraphics.MeshNode_setAnimationByName(nodePtr, "idle");
			UGraphics.Node_setPosition(nodePtr, x, y, 0);
			UGraphics.Node_setRotation(nodePtr, 0, 0, z);
			UGraphics.Node_setScale(nodePtr, k, k, k);
		}

		long light = UGraphics.Scene_addLightNode();
		UGraphics.LightNode_setRadius(light, kSize);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		DebugLog.w("onSurfaceCreated");
	}
}
