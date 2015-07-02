package com.hiscene;

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

	private Scene3D mScene;
	float[] modelMatrix = new float[16];
	float[] projMatrix = new float[16];

	/**
	 * Defines whether the activity is currently paused
	 */
	private boolean mActivityIsPaused;

	/**
	 * Whether the metaio SDK null renderer is initialized
	 */
	private boolean mRendererInitialized;

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
		mRendererInitialized = false;
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
			mScene = new Scene3D(this);

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
			long hitNodePtr = Scene3D.pickNodeFromScreen(event.getX(),
					event.getY());
			if (hitNodePtr != 0 && hitNodePtr != mCubeNode) {
				Scene3D.setNodeAnimation(hitNodePtr, "shock_down", true);
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

		Scene3D.clear();

		// mCameraImageRenderer.draw(gl, mScreenRotation);

		Scene3D.render();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if (height == 0)
			height = 1;

		DebugLog.w("onSurfaceChanged: " + width + ", " + height);
		// if (mScene == null) {

		Scene3D.resize(width, height);
		mCubeNode = Scene3D.addCubeNode(200);
		Scene3D.setNodeTexture(mCubeNode, Scene3D.getTexture("wall.jpg"));
		Scene3D.setNodePosition(mCubeNode, 0, 0, 1000);
		final float kSize = 1000;
		for (int i = 0; i < 9; i++) {
			float x = (i / 3 - 1.0f) * kSize / 3;
			float y = (i % 3 - 1.0f) * kSize / 3;
			float z = (float) (Math.random() * kSize) - kSize / 2;
			float k = (float) (Math.random() * 0 + 3);
			long nodePtr = Scene3D.addMeshNode("metaioman.md2");
			Scene3D.setNodeTexture(nodePtr, Scene3D.getTexture("metaioman.png"));
			Scene3D.setNodeAnimation(nodePtr, "idle", true);
			Scene3D.setNodePosition(nodePtr, x, y, 0);
			Scene3D.setNodeRotation(nodePtr, 0, 0, z);
			Scene3D.setNodeScale(nodePtr, k, k, k);
		}

		Scene3D.addPointLight(kSize);
		// }
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		DebugLog.w("onSurfaceCreated");

		// if (!mRendererInitialized) {
		// mScreenRotation = Screen.getRotation(this);
		//
		// // Set up custom rendering (metaio SDK will only do tracking and not
		// // render any objects
		// // itself)
		// metaioSDK.wnitializeRenderer(0, 0, mScreenRotation,
		// ERENDER_SYSTEM.ERENDER_SYSTEM_NULL);
		//
		// mRendererInitialized = true;
		// }
	}
}
