package com.showmo.util;

import android.util.Log;
import android.view.View.MeasureSpec;
import android.view.ViewGroup.LayoutParams;

public class DisViewUtil {
	public static int getSpec(int size){
    	int spec=0;
    	if(size == LayoutParams.WRAP_CONTENT){
    		Log.e("measure", "WRAP_CONTENT");
    		spec=MeasureSpec.makeMeasureSpec(0,MeasureSpec.UNSPECIFIED);
    	}else if(size == LayoutParams.MATCH_PARENT){
    		Log.e("measure", "MATCH_PARENT");
    		spec=MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
    	}else{
    		Log.e("measure", ">0");
    		spec=MeasureSpec.makeMeasureSpec(size, MeasureSpec.EXACTLY);
    	}
    	return spec;
    	
    }
}
