package com.hiscene;

import android.util.Log;

public class DebugLog {
	private static final String LOGTAG = "DebugLog";

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
}
