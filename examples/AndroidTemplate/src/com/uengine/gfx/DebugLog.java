package com.uengine.gfx;

import android.util.Log;

public class DebugLog {
	private static final String LOGTAG = "Scene3D-java";

	public static final void wtf(String nMessage) {
		Log.wtf(LOGTAG, nMessage);
	}

	public static final void e(String nMessage) {
		Log.e(LOGTAG, nMessage);
	}

	public static final void w(String nMessage) {
		Log.w(LOGTAG, nMessage);
	}

	public static final void d(String nMessage) {
		Log.d(LOGTAG, nMessage);
	}

	public static final void i(String nMessage) {
		Log.i(LOGTAG, nMessage);
	}

	public static void printStackTrace(Exception paramException) {
		if (paramException != null) {
			e(paramException.getMessage());
			e(Log.getStackTraceString(paramException));
		}
	}

}
