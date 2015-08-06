package com.showmo.util;

import android.content.Context;
import android.util.TypedValue;

public class HexTrans {
	public static int dip2px(Context context,float dipValue){
//	     final float scale=context.getResources().getDisplayMetrics().density;
//	     return (int)(dipValue*scale+0.5f);
		float dp=TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dipValue, context.getResources().getDisplayMetrics());
		return (int)dp;
	}

	public static int px2dp(Context context,float pxValue){
//	    final float scale = context.getResources().getDisplayMetrics().density; 
//	    return (int)(pxValue/scale+0.5f);
	    float px=TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_PX,pxValue, context.getResources().getDisplayMetrics());
	    return (int)px;
	}

}
