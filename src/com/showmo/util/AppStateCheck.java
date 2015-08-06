package com.showmo.util;

import java.util.List;

import com.showmo.base.ShowmoApplication;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

public class AppStateCheck {
	public static boolean isBackground(Context context) {
		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> appProcesses = activityManager
				.getRunningAppProcesses();
		for (RunningAppProcessInfo appProcess : appProcesses) {
			if (appProcess.processName.equals(context.getPackageName())) {
				if (appProcess.importance == RunningAppProcessInfo.IMPORTANCE_BACKGROUND) {
					LogUtils.i("screen",String.format("Background App:", appProcess.processName));
					return true;
				}else{
					LogUtils.i("screen",String.format("Foreground App:", appProcess.processName));
					return false;
				}
			}
		}
		return false;
	}
	public static boolean isActivityExist(Context context,Intent intent){
		PackageManager pManager=context.getPackageManager();
		ComponentName componentName=intent.resolveActivity(pManager);
		if (componentName !=null) {
			return true;
		}
		return false;
	}
	public static boolean isAppExist(String pkgName){
		if (!StringUtil.isNotEmpty(pkgName)) {
			return false;
		}
		else {
			try {
				ApplicationInfo info=ShowmoApplication.getInstance().getPackageManager()
						.getApplicationInfo(pkgName, PackageManager.GET_UNINSTALLED_PACKAGES);
			} catch (NameNotFoundException  e) {
				// TODO: handle exception
				return false;
			}
			
		}
		return true;
	}
}
