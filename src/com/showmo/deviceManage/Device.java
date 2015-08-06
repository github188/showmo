package com.showmo.deviceManage;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.table.DatabaseTable;
import com.showmo.R;
import com.showmo.dataDef.StruWifiConfig;
import com.showmo.ormlite.dao.impl.DeviceDaoImpl;

import android.R.integer;
import android.R.interpolator;
import android.content.Context;
import android.util.Log;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;
import ipc365.app.showmo.jni.JniDataDef.SDK_SEARCH;

@DatabaseTable(daoClass = DeviceDaoImpl.class, tableName = "tb_device")
public class Device implements Serializable ,Comparable<Device>{
	public static final String DEVICE_RENAME_ACTION="ipc365.app.showmo.DEVICE_RENAME_ACTION";

	@DatabaseField(id = true)
	private String UniqueId;

	@DatabaseField
	private int mCameraId;
	@DatabaseField
	private int mDeviceId;

	@DatabaseField
	private String mUuid;

	@DatabaseField
	private String mDeviceName;

	@DatabaseField
	private String mTinyImgFilePath;

	@DatabaseField
	private String mUseOwner;

	@DatabaseField
	private boolean mHaveNewAlarmInfo;//

	private boolean mOnlineState;

	private List<AlarmSwitch> mAlarmSwitchs;

	private boolean mSwitchStateIsValid=false;

	private boolean mUpgrading=false;//表示设备是否正在升级的标志位

	private int mDownUpgradePkgPos=0;//设备升级包下载进度，mUpgrading=true时有用

	private String mVersion;//"",null，表示是否有新固件版本可以升级的标志位。不为空则可以升级

	@DatabaseField
	private long mUseFreq=0;
	
	@Override
	protected Object clone() throws CloneNotSupportedException {
		// TODO Auto-generated method stub
		Device device=(Device) super.clone();
		device.mAlarmSwitchs=new ArrayList<Device.AlarmSwitch>();
		return device;
	}
	
	public Device() {
		// TODO Auto-generated constructor stub
	}
	public static String makeUniqueId(String username,int cameraId){
		return username+cameraId;
	}
	public Device(String user, int cameraId, int deviceId, String uuid,
			String deviceName, String imgFilename, String version) {
		mUseOwner = user;
		mCameraId = cameraId;
		mDeviceId = deviceId;
		mUuid = uuid;
		mDeviceName = deviceName;
		mTinyImgFilePath = imgFilename;
		mVersion = version;
		mOnlineState = false;
		UniqueId = mUseOwner + mCameraId;
		mAlarmSwitchs=new ArrayList<Device.AlarmSwitch>();
	}

	public int getmDownUpgradePkgPos() {
		return mDownUpgradePkgPos;
	}
	public void setmDownUpgradePkgPos(int mDownUpgradePkgPos) {
		this.mDownUpgradePkgPos = mDownUpgradePkgPos;
	}
	//	public Device(int m_cameraId, int m_deviceId, String m_uuid,
	//			String m_deviceName, String m_tinyImgFilePath, String m_version,
	//			String m_UseOwner, Boolean m_haveNewAlarmInfo, boolean m_onlineState) {
	//		super();
	//		this.mCameraId = m_cameraId;
	//		this.mDeviceId = m_deviceId;
	//		this.mUuid = m_uuid;
	//		this.mDeviceName = m_deviceName;
	//		this.mTinyImgFilePath = m_tinyImgFilePath;
	//		this.mVersion = m_version;
	//		this.mUseOwner = m_UseOwner;
	//		this.mHaveNewAlarmInfo = m_haveNewAlarmInfo;
	//		this.mOnlineState = m_onlineState;
	//		mAlarmSwitchs=new ArrayList<Device.AlarmSwitch>();
	//	}
	public void UseFreqIncrease(){
		mUseFreq++;
	}
	public void setFreq(long freq){
		mUseFreq = freq;
	}
	public long getFreq(){
		return mUseFreq;
	}
	public String getUniqueId() {
		return UniqueId;
	}

	public void setUniqueId(String uniqueId) {
		UniqueId = uniqueId;
	}

	public int getmCameraId() {
		return mCameraId;
	}

	public void setmCameraId(int mCameraId) {
		this.mCameraId = mCameraId;
	}

	public int getmDeviceId() {
		return mDeviceId;
	}

	public void setmDeviceId(int mDeviceId) {
		this.mDeviceId = mDeviceId;
	}

	public String getmUuid() {
		return mUuid;
	}

	public void setmUuid(String mUuid) {
		this.mUuid = mUuid;
	}

	public String getmDeviceName() {
		return mDeviceName;
	}

	public void setmDeviceName(String mDeviceName) {
		this.mDeviceName = mDeviceName;
	}

	public String getmTinyImgFilePath() {
		return mTinyImgFilePath;
	}

	public void setmTinyImgFilePath(String mTinyImgFilePath) {
		this.mTinyImgFilePath = mTinyImgFilePath;
	}

	public String getmVersion() {
		return mVersion;
	}

	public void setmVersion(String mVersion) {
		this.mVersion = mVersion;
	}

	public String getmUseOwner() {
		return mUseOwner;
	}

	public void setmUseOwner(String mUseOwner) {
		this.mUseOwner = mUseOwner;
	}

	public boolean getHaveNewAlarmInfo() {
		return mHaveNewAlarmInfo;
	}

	public void setHaveNewAlarmInfo(boolean mHaveNewAlarmInfo) {
		this.mHaveNewAlarmInfo = mHaveNewAlarmInfo;
	}

	public boolean ismOnlineState() {
		return mOnlineState;
	}

	public synchronized void  setmOnlineState(boolean mOnlineState) {
		this.mOnlineState = mOnlineState;
	}

	public synchronized  List<AlarmSwitch> getmAlarmSwitchs() {
		return mAlarmSwitchs;
	}

	public synchronized void setAlarmSwitch(List<AlarmSwitch> mAlarmSwitchs) {
		this.mAlarmSwitchs=mAlarmSwitchs;
	}

	public boolean ismSwitchStateValid() {
		return mSwitchStateIsValid;
	}

	public void setmSwitchStateValid(boolean mSwitchStateIsValid) {
		this.mSwitchStateIsValid = mSwitchStateIsValid;
	}

	@Override
	public String toString() {
		return "Device [UniqueId=" + UniqueId + ", mCameraId=" + mCameraId
				+ ", mDeviceId=" + mDeviceId + ", mUuid=" + mUuid
				+ ", mDeviceName=" + mDeviceName + ", mTinyImgFilePath="
				+ mTinyImgFilePath + ", mVersion=" + mVersion + ", mUseOwner="
				+ mUseOwner + ", mHaveNewAlarmInfo=" + mHaveNewAlarmInfo
				+ ", mOnlineState=" + mOnlineState + "]";
	}
	//	public class AlarmSwitchs{
	//		
	//		public boolean motionDetect=false;
	//		
	//		public AlarmSwitchs(boolean motiondetect){
	//			motionDetect=motiondetect;
	//		}
	//	}
	public static class AlarmSwitch{
		public boolean value=false;
		public int cameraAlarmType;
		public int alarmNameResId=-1;
		public AlarmSwitch(int alarmType,boolean value){
			this.value=value;
			this.cameraAlarmType=alarmType;
			switch (alarmType) {
			case CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION:
				alarmNameResId=R.string.motion_detection;
				break;
			default:
				break;
			}
		}

	}
	public boolean ismUpgrading() {
		return mUpgrading;
	}
	public void setmUpgrading(boolean mUpgrading) {
		this.mUpgrading = mUpgrading;
	}

	@Override
	public int compareTo(Device another) {
		// TODO Auto-generated method stub
		if(this.mOnlineState == another.mOnlineState){//具有相同状态，则使用频率高的小于
			if(this.mUseFreq > another.mUseFreq){
				return -1;
			}else if (this.mUseFreq == another.mUseFreq) {
				return 0;
			}else{
				return 1;
			}
		}else{
			if(this.mOnlineState){//本实例在线，而另一个不在线，则小于否则大于
				return -1;
			}else{
				return 1;
			}
		}
	}
}
