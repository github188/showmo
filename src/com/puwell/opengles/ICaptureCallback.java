package com.puwell.opengles;

import android.graphics.Bitmap;

public interface ICaptureCallback {
	void onSuccess(Bitmap bmp);
	void onFailured();
	void onProcess();
}
