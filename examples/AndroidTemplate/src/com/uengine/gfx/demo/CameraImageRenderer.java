// Copyright 2015-2015 u-engine.com
// All rights reserved.
package com.uengine.gfx.demo;

import java.nio.ByteBuffer;

import javax.microedition.khronos.opengles.GL10;

import android.util.Log;

import com.metaio.sdk.jni.ESCREEN_ROTATION;
import com.metaio.sdk.jni.ImageStruct;
import com.uengine.gfx.jni;

public final class CameraImageRenderer {
	private static final String TAG = "CameraImageRenderer";

	private int mCameraH;

	private int mCameraW;

	private boolean mInitialized = false;

	private boolean mMustUpdateTexture = false;

	private long mTexturePtr;

	private ByteBuffer mCameraRgb;

	private boolean mTextureInitialized = false;

	final private boolean mIsNodeMode = false;

	public void draw(GL10 gl, ESCREEN_ROTATION screenRotation) {
		if (!mInitialized)
			return;

		if (mMustUpdateTexture) {
			if (!mTextureInitialized) {
				// Allocate camera image texture once with 2^n dimensions
				mTexturePtr = jni.Scene_addEmptyTexture(mCameraW, mCameraH);
				Log.e(TAG, "Init camera texture: " + mCameraW + ", " + mCameraH);

				if (mIsNodeMode) {
					long node = jni.Scene_addFullScreenTextureNode(mTexturePtr,
							screenRotation.ordinal());
					// UGraphics.Scene_setNodeTexture(node, mTexturePtr);
					// UGraphics.Scene_setNodeRotation(node, 180, 180, 180);
				}
				mTextureInitialized = true;
			}

			byte[] jImageData = mCameraRgb.array();
			jni.Texture_update(mTexturePtr, jImageData);
			mMustUpdateTexture = false;
		}

		if (!mIsNodeMode) {
			jni.Texture_renderFullScreen(mTexturePtr, screenRotation.ordinal());
		}
	}

	private void init(int cameraImageWidth, int cameraImageHeight) {
		mCameraRgb = ByteBuffer.allocateDirect(cameraImageWidth
				* cameraImageHeight * 4);

		mInitialized = true;
	}

	public void updateFrame(ImageStruct frame) {
		final int frameWidth = frame.getWidth();
		final int frameHeight = frame.getHeight();

		switch (frame.getColorFormat()) {
		case ECF_RGBA8:
			if (!mInitialized)
				init(frameWidth, frameHeight);

			if (!frame.getOriginIsUpperLeft()) {
				Log.e(TAG, "Unimplemented: camera image upside-down");
				return;
			}

			mCameraRgb.rewind();
			frame.copyBufferToNioBuffer(mCameraRgb);
			mCameraRgb.rewind();

			break;

		default:
			Log.e(TAG, "Unimplemented color format " + frame.getColorFormat());
			return;
		}

		mMustUpdateTexture = true;

		mCameraW = frameWidth;
		mCameraH = frameHeight;
	}
}
