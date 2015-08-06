package com.showmo.deviceManage;

import com.showmo.dataDef.StruWifiConfig;

public class DeviceAddUtils {
	private Device mDevice;
	public DeviceAddUtils(Device dev){
		mDevice=dev;
	}
	
	public Device getmDevice() {
		return mDevice;
	}

	public void setmDevice(Device mDevice) {
		this.mDevice = mDevice;
	}
	long loginDevice(String ip){
		return 0;
	}
	void logoutDevice(long loginId){
		return;
	}
	boolean setDevTimezone(long loginId,int timezone){
		return false;
	}
	boolean setDevOsd(long loginId, String osdname){
		return false;
	}
	boolean setDhcp(long loginId,boolean bOpen){
		return false;
	}
	boolean setDevWifi(long loginId,StruWifiConfig wifi){
		return false;
	}
	
}
