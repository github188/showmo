package com.showmo.util;

import com.showmo.R;

import android.app.Activity;
/**
 * 过长动画工具类
 * @author Terry
 *
 */
public class AnimUtil {
	
	public static void slideInFromRight(Activity activity ) {
		activity.overridePendingTransition(R.anim.slide_in_from_right, R.anim.slide_out_to_left);
	}
	
	public static void slideInFromLeft(Activity activity ) {
		activity.overridePendingTransition(R.anim.slide_in_from_left, R.anim.slide_out_to_right);
	}
	
	
}
