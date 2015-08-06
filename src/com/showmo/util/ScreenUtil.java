package com.showmo.util;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class ScreenUtil {
	public static class ScreenInfo{
		public int width;
		public int height;
		public float density;//ÆÁÄ»ÃÜ¶È£¨ÏñËØ±ÈÀý£©
		public float densityDpi;//ÆÁÄ»ÃÜ¶È£¨dpi£©
		public float xdpi;
		public float ydpi;
		@Override
		public String toString() {
			return "ScreenInfo [width=" + width + ", height=" + height + ", density=" + density + ", densityDpi="
					+ densityDpi + ", xdpi=" + xdpi + ", ydpi=" + ydpi + "]";
		}
		
	}
	public static ScreenInfo getScreenInfo(Context context){
		DisplayMetrics dm=new DisplayMetrics();
		dm=context.getResources().getDisplayMetrics();
		ScreenInfo info=new ScreenInfo();
		info.width=dm.widthPixels;
		info.height=dm.heightPixels;
		info.density=dm.density;
		info.densityDpi=dm.densityDpi;
		info.xdpi=dm.xdpi;
		info.ydpi=dm.ydpi;
		return info;
	}
}
