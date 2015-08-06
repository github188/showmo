package com.showmo.widget;

import android.content.Context;
import android.graphics.PixelFormat;
import android.view.Gravity;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;

public class MyWindowManager {
	private static FloatWindowSmallView smallWindow;
	private static LayoutParams smallWindowParams;
	private static WindowManager mWindowManager;

	public static void createSmallWindow(Context context) {
		WindowManager windowManager = getWindowManager(context);
		//int screenWidth = windowManager.getDefaultDisplay().getWidth();
		int screenHeight = windowManager.getDefaultDisplay().getHeight();
		if (smallWindow == null) {
			smallWindow = new FloatWindowSmallView(context);
			if (smallWindowParams == null) {
				smallWindowParams = new LayoutParams();
				smallWindowParams.type = LayoutParams.TYPE_PHONE;
				smallWindowParams.format = PixelFormat.RGBA_8888;
				smallWindowParams.flags = LayoutParams.FLAG_NOT_TOUCH_MODAL
						| LayoutParams.FLAG_NOT_FOCUSABLE;
				//smallWindowParams.gravity = Gravity.LEFT | Gravity.TOP;
				smallWindowParams.gravity =Gravity.CENTER_HORIZONTAL;
				smallWindowParams.width = FloatWindowSmallView.viewWidth;
				smallWindowParams.height = LayoutParams.WRAP_CONTENT;
				//smallWindowParams.x = screenWidth;
				smallWindowParams.y = screenHeight / 4;
			}
			smallWindow.setParams(smallWindowParams);
			windowManager.addView(smallWindow, smallWindowParams);
		}
	}

	public static void removeSmallWindow(Context context) {
		if (smallWindow != null) {
			WindowManager windowManager = getWindowManager(context);
			windowManager.removeView(smallWindow);
			smallWindow = null;
		}
	}

	private static WindowManager getWindowManager(Context context) {
		if (mWindowManager == null) {
			mWindowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		}
		return mWindowManager;
	}
}
