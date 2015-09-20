// Copyright 2007-2014 metaio GmbH. All rights reserved.
package com.uengine.gfx;

import java.io.File;
import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.metaio.example_custom_renderer.BuildConfig;
import com.metaio.example_custom_renderer.R;
import com.metaio.sdk.MetaioDebug;
import com.metaio.sdk.SensorsComponentAndroid;
import com.metaio.sdk.jni.CameraVector;
import com.metaio.sdk.jni.ECAMERA_TYPE;
import com.metaio.sdk.jni.ERENDER_SYSTEM;
import com.metaio.sdk.jni.ESCREEN_ROTATION;
import com.metaio.sdk.jni.IMetaioSDKAndroid;
import com.metaio.sdk.jni.IMetaioSDKCallback;
import com.metaio.sdk.jni.ImageStruct;
import com.metaio.sdk.jni.MetaioSDK;
import com.metaio.sdk.jni.TrackingValues;
import com.metaio.sdk.jni.TrackingValuesVector;
import com.metaio.tools.Screen;
import com.metaio.tools.SystemInfo;
import com.metaio.tools.io.AssetsManager;

public final class MainActivity extends Activity implements Renderer {

	private UGraphics mGraphics;
	private long mCubeNode;
	private long mMeshNode;
	float[] modelMatrix = new float[16];
	float[] projMatrix = new float[16];

	/**
	 * Defines whether the activity is currently paused
	 */
	private boolean mActivityIsPaused;

	/**
	 * Camera image renderer which takes care of differences in camera image and
	 * viewport aspect ratios
	 */
	private CameraImageRenderer mCameraImageRenderer;

	/**
	 * metaio SDK instance
	 */
	private IMetaioSDKAndroid metaioSDK;

	/**
	 * metaio SDK callback handler
	 */
	private MetaioSDKCallbackHandler mSDKCallback;

	/**
	 * Whether the metaio SDK null renderer is initialized
	 */
	private boolean mRendererInitialized;

	/**
	 * Current screen rotation
	 */
	private ESCREEN_ROTATION mScreenRotation;

	/**
	 * Sensors component
	 */
	private SensorsComponentAndroid mSensors;

	/**
	 * Main GLSufaceView in which everything is rendered
	 */
	private GLSurfaceView mSurfaceView;

	/**
	 * Load native libs required by the Metaio SDK
	 */
	protected void loadNativeLibs() throws UnsatisfiedLinkError {
		IMetaioSDKAndroid.loadNativeLibs();
		MetaioDebug.log(
				Log.INFO,
				"MetaioSDK libs loaded for " + SystemInfo.getDeviceABI()
						+ " using "
						+ com.metaio.sdk.jni.SystemInfo.getAvailableCPUCores()
						+ " CPU cores");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		MetaioDebug.log(Log.INFO, "onCreate");

		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		try {
			// Load native libs
			loadNativeLibs();

			// Create metaio SDK instance by passing a valid signature
			metaioSDK = MetaioSDK.CreateMetaioSDKAndroid(this, getResources()
					.getString(R.string.metaioSDKSignature));
			if (metaioSDK == null) {
				throw new Exception("Unsupported platform!");
			}

			// Register Metaio SDK callback
			mSDKCallback = new MetaioSDKCallbackHandler();
			metaioSDK.registerCallback(mSDKCallback);

			// Register sensors component
			mSensors = new SensorsComponentAndroid(getApplicationContext());
			metaioSDK.registerSensorsComponent(mSensors);

		} catch (Exception e) {
			MetaioDebug.log(Log.ERROR, "Error creating Metaio SDK!");
			MetaioDebug.printStackTrace(Log.ERROR, e);
			finish();
			return;
		}

		try {
			// Enable metaio SDK log messages based on build configuration
			MetaioDebug.enableLogging(BuildConfig.DEBUG);

			// Extract all assets and overwrite existing files if debug build
			AssetsManager.extractAllAssets(getApplicationContext(),
					BuildConfig.DEBUG);
		} catch (IOException e) {
			MetaioDebug.log(Log.ERROR, "Error extracting application assets!");
			MetaioDebug.printStackTrace(Log.ERROR, e);
		}

		mSurfaceView = null;
		mRendererInitialized = false;
	}

	@Override
	protected void onPause() {
		MetaioDebug.log(Log.INFO, "onPause");

		super.onPause();

		if (mSurfaceView != null)
			mSurfaceView.onPause();

		mActivityIsPaused = true;
		metaioSDK.pause();
	}

	@Override
	protected void onResume() {
		MetaioDebug.log(Log.INFO, "onResume");

		super.onResume();

		metaioSDK.resume();
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
		MetaioDebug.log(Log.INFO, "onStart");

		super.onStart();

		if (metaioSDK != null) {
			MetaioDebug.log(Log.INFO, "metaioSDK != null");

			// Set empty content view
			setContentView(new FrameLayout(this));

			// Start camera only when the activity starts the first time (see
			// lifecycle:
			// http://developer.android.com/training/basics/activity-lifecycle/pausing.html)
			if (!mActivityIsPaused) {
				startCamera();
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
		MetaioDebug.log(Log.INFO, "onStop");

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
		MetaioDebug.log(Log.INFO, "onDestroy");

		super.onDestroy();

		if (metaioSDK != null) {
			metaioSDK.delete();
			metaioSDK = null;
		}

		if (mSDKCallback != null) {
			mSDKCallback.delete();
			mSDKCallback = null;
		}

		if (mSensors != null) {
			mSensors.registerCallback(null);
			mSensors.release();
			mSensors.delete();
			mSensors = null;
		}
	}

	int animIdx = 0;
	boolean isAnimationCompleted = false;

	public boolean onTouchEvent(MotionEvent event) {
		// MetaioDebug.log(Log.INFO, event.toString());
		if (event.getAction() == MotionEvent.ACTION_UP) {
			long hitNode = UGraphics.Scene_pickNodeFromScreen(event.getX(),
					event.getY());
			if (hitNode == mMeshNode) {
				if (isAnimationCompleted) {
					isAnimationCompleted = false;
					UGraphics
							.MeshNode_setAnimationByName(hitNode, "shock_down");
					UGraphics.MeshNode_setAnimationLoop(hitNode, false);
				}
			}
		}

		return true;
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		MetaioDebug.log(Log.INFO, "onConfigurationChanged");

		mScreenRotation = Screen.getRotation(this);
		metaioSDK.setScreenRotation(mScreenRotation);
		super.onConfigurationChanged(newConfig);
	}

	/**
	 * Start camera. Override this to change camera index or resolution
	 */
	protected void startCamera() {
		final CameraVector cameras = metaioSDK.getCameraList();
		if (cameras.size() > 0) {
			// Start the first camera by default
			com.metaio.sdk.jni.Camera camera = cameras.get(0);

			// Disable YUV image pipeline to easily handle RGB images in
			// CameraImageRenderer. If the
			// see-through
			// constant setting is active, we don't need this (YUV pipeline is
			// faster!).
			camera.setYuvPipeline(false);

			metaioSDK.startCamera(camera);
		}
	}

	@Override
	public void onDrawFrame(GL10 gl) {

		metaioSDK.requestCameraImage();

		// Note: The metaio SDK itself does not render anything here because we
		// initialized it with
		// the NULL renderer. This call is necessary to get the camera image and
		// update tracking.
		metaioSDK.render();

		UGraphics.Scene_clear();

		mCameraImageRenderer.draw(gl, mScreenRotation);

		final TrackingValues trackingValues = metaioSDK.getTrackingValues(1);

		if (trackingValues.isTrackingState()) {
			// MetaioDebug.log(Log.DEBUG, "onTracked");
			metaioSDK.getTrackingValues(1, modelMatrix, false, true);
			metaioSDK.getProjectionMatrix(projMatrix, true,
					ECAMERA_TYPE.ECT_RENDERING_MONO);
			UGraphics.Node_setModelMatrix(mMeshNode, modelMatrix);
			UGraphics.Camera_setProjectionMatrix(projMatrix);
			float k = (float) (Math.random() * 360);
			UGraphics.Node_setRotation(mCubeNode, k, k, k);
			UGraphics.Scene_setVisible(true);
		} else {
			UGraphics.Scene_setVisible(false);
		}

		UGraphics.Scene_render();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if (height == 0)
			height = 1;

		MetaioDebug.log(Log.INFO, "onSurfaceChanged: " + width + ", " + height);
		// if (mScene == null) {
		mGraphics = new UGraphics(this);

		UGraphics.Scene_initializeRenderer(width, height);
		mCubeNode = UGraphics.Scene_addCubeNode(200);
		UGraphics.Node_setTexture(mCubeNode, UGraphics
				.Scene_addTextureFromImage(UGraphics
						.Scene_addImageFromFile("wall.jpg")));
		UGraphics.Node_setPosition(mCubeNode, 0, 0, 1000);
		final float kSize = 1000;

		float z = (float) (Math.random() * kSize) - kSize / 2;
		float k = (float) (Math.random() * 0 + 3);
		if (true) {
			mMeshNode = UGraphics.Scene_addMeshNode("metaioman.md2");
			UGraphics.Node_setTexture(mMeshNode, UGraphics
					.Scene_addTextureFromImage(UGraphics
							.Scene_addImageFromFile("metaioman.png")));
		} else {
			mMeshNode = UGraphics.Scene_addMeshNode("dog_color3.ply");
			UGraphics.Node_setScale(mMeshNode, 100, 100, 100);
		}

		UGraphics.MeshNode_registerCallback(mMeshNode,
				new UGraphics.MeshNodeCallback() {
					public void onAnimationCompleted(long nodePtr) {
						DebugLog.w("Animation completed");
						animIdx = (animIdx + 1) % 6;
						UGraphics.MeshNode_setAnimationByIndex(mMeshNode,
								animIdx);
					}
				});
		UGraphics.Node_setLighting(mMeshNode, false);
		UGraphics.MeshNode_setAnimationByName(mMeshNode, "idle");
		UGraphics.MeshNode_setAnimationLoop(mMeshNode, false);
		UGraphics.Node_setPosition(mMeshNode, 0, 0, 0);
		UGraphics.Node_setRotation(mMeshNode, 0, 0, z);
		UGraphics.Node_setScale(mMeshNode, k, k, k);

		long lightNode = UGraphics.Scene_addLightNode();
		UGraphics.LightNode_setRadius(lightNode, kSize);

		// }

		mCameraImageRenderer = new CameraImageRenderer();

		if (metaioSDK != null) {
			metaioSDK.resizeRenderer(width, height);
		} else {
			MetaioDebug.log(Log.ERROR, "Metaio SDK not yet created");
		}
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		MetaioDebug.log(Log.INFO, "onSurfaceCreated");

		if (!mRendererInitialized) {
			mScreenRotation = Screen.getRotation(this);

			// Set up custom rendering (metaio SDK will only do tracking and not
			// render any objects
			// itself)
			metaioSDK.initializeRenderer(0, 0, mScreenRotation,
					ERENDER_SYSTEM.ERENDER_SYSTEM_NULL);

			mRendererInitialized = true;
		}
	}

	final class MetaioSDKCallbackHandler extends IMetaioSDKCallback {
		@SuppressWarnings("unused")
		@Override
		public void onNewCameraFrame(ImageStruct cameraFrame) {
			if (mCameraImageRenderer != null) {
				mCameraImageRenderer.updateFrame(cameraFrame);
			}
		}

		@Override
		public void onSDKReady() {
			MetaioDebug.log(Log.INFO, "onSDKReady");

			// Load desired tracking configuration when the SDK is ready
			final File trackingConfigFile = AssetsManager.getAssetPathAsFile(
					getApplicationContext(), "TrackingData_MarkerlessFast.xml");
			if (trackingConfigFile == null
					|| !metaioSDK.setTrackingConfiguration(trackingConfigFile)) {
				MetaioDebug.log(Log.ERROR,
						"Failed to set tracking configuration");
			}
		}

		@Override
		public void onTrackingEvent(TrackingValuesVector trackingValues) {
			for (int i = 0; i < trackingValues.size(); ++i) {
				final TrackingValues v = trackingValues.get(i);
				MetaioDebug.log("Tracking state for COS "
						+ v.getCoordinateSystemID() + " is " + v.getState());
			}
		}
	}
}
