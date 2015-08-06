
package com.showmo.userManage;

import java.util.ArrayList;

import android.R.integer;

import com.showmo.deviceManage.Device;

import ipc365.app.showmo.jni.JniDataDef.ClientDeviceQueryRet;

public interface IUserBehavior {
	
	public Device bindDevice(String devName,String uuid,int device);
	public boolean unbindDevice(Device dev);
	public boolean unbindDevice(String UniqueId);
	public void sortDevice();
}
