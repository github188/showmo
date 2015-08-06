package com.showmo.util;

import com.showmo.base.ShowmoApplication;

import android.content.Context;
import android.os.Vibrator;

public class VibrateUtil {
	private static  VibrateUtil mVibrateUtil=new VibrateUtil();
	public static  VibrateUtil getInstance(){
		return mVibrateUtil;
	}
	Vibrator mVibrator;
	public VibrateUtil(){
		mVibrator=(Vibrator)ShowmoApplication.getInstance().getSystemService(Context.VIBRATOR_SERVICE);
	}
	public void vib(int ms){
		mVibrator.vibrate(ms);
	}
	
}
