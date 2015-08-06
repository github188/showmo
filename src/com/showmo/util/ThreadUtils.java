package com.showmo.util;

import com.showmo.base.ShowmoApplication;

public class ThreadUtils {
	public static boolean isMainThread(){
		return Thread.currentThread().getId()==ShowmoApplication.getInstance().getMainLooper().getThread().getId();
	}
}
