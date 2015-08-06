package com.showmo.alarmManage;

import java.io.Serializable;

import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.table.DatabaseTable;
import com.showmo.R;
import com.showmo.activity.alarmInfo.CameraAlarmType;
import com.showmo.base.ShowmoApplication;
import com.showmo.ormlite.dao.impl.AlarmDaoImpl;
import com.showmo.util.TimeUtil;

@DatabaseTable(daoClass = AlarmDaoImpl.class, tableName = "tb_alarms")
public class Alarm implements Serializable {
	public final static int ChNoTFCard=1;
	@DatabaseField
	private int alarmCode;
	@DatabaseField
	private int clientId;
	@DatabaseField(id = true)
	private int recordId;
	@DatabaseField
	private int deviceId;
	@DatabaseField
	private int cameraId;
	@DatabaseField
	private int channelNo;
	@DatabaseField
	private int alarmType;
	@DatabaseField
	private int alarmMode;
	@DatabaseField
	private int ccid;
	@DatabaseField
	private long beginTime;
	@DatabaseField
	private long endTime;
	@DatabaseField
	private String mImgPath;
	@DatabaseField
	private boolean mImgDownloading;
	
	private int mImgDownloadPos;
	
	public void setmImgDownloadPos(int mImgDownloadPos) {
		this.mImgDownloadPos = mImgDownloadPos;
	}
	public int getmImgDownloadPos() {
		return mImgDownloadPos;
	}
	public Alarm() {
	}

	public Alarm(int clientId, int recordId, int deviceId, int cameraId,
			int channelNo, int alarmType, int alarmMode, long begintime,
			long endtime, int alarmNode, int ccid) {
		super();
		this.clientId = clientId;
		this.recordId = recordId;
		this.deviceId = deviceId;
		this.cameraId = cameraId;
		this.channelNo = channelNo;
		this.alarmType = alarmType;
		this.alarmMode = alarmMode;
		this.alarmCode = alarmNode;
		this.ccid = ccid;
		this.beginTime = begintime;
		this.endTime = endtime;
		this.mImgPath = "";
		this.mImgDownloading = false;
	}

	// public Alarm( int recordId,
	// int alarmType, int alarmMode) {
	// super();
	// this.recordId = recordId;
	// this.alarmType = alarmType;
	// this.alarmMode = alarmMode;
	// }
	//
	// public Alarm(int recordId, int alarmType, int alarmMode, long beginTime,
	// long endTime) {
	// super();
	// this.recordId = recordId;
	// this.alarmType = alarmType;
	// this.alarmMode = alarmMode;
	// this.beginTime = beginTime;
	// this.endTime = endTime;
	// }

	public int getClientId() {
		return clientId;
	}

	public void setClientId(int clientId) {
		this.clientId = clientId;
	}

	public int getRecordId() {
		return recordId;
	}

	public void setRecordId(int recordId) {
		this.recordId = recordId;
	}

	public int getDeviceId() {
		return deviceId;
	}

	public void setDeviceId(int deviceId) {
		this.deviceId = deviceId;
	}

	public int getCameraId() {
		return cameraId;
	}

	public void setCameraId(int cameraId) {
		this.cameraId = cameraId;
	}

	public int getChannelNo() {
		return channelNo;
	}

	public void setChannelNo(int channelNo) {
		this.channelNo = channelNo;
	}

	public int getAlarmType() {
		return alarmType;
	}

	public void setAlarmType(int alarmType) {
		this.alarmType = alarmType;
	}

	public int getAlarmMode() {
		return alarmMode;
	}

	public void setAlarmMode(int alarmMode) {
		this.alarmMode = alarmMode;
	}

	public int getAlarmCode() {
		return alarmCode;
	}

	public void setAlarmCode(int alarmCode) {
		this.alarmCode = alarmCode;
	}

	public int getCcid() {
		return ccid;
	}

	public void setCcid(int ccid) {
		this.ccid = ccid;
	}

	public void setmImgDownloading(boolean mImgDownloading) {
		this.mImgDownloading = mImgDownloading;
	}

	public boolean getmImgDownloading() {
		return mImgDownloading;
	}

	public String getmImgPath() {
		return mImgPath;
	}
	public void setmImgPath(String mImgPath) {
		this.mImgPath = mImgPath;
	}

	public long getBeginTime() {
		return beginTime;
	}

	public void setBeginTime(long beginTime) {
		this.beginTime = beginTime;
	}

	public long getEndTime() {
		return endTime;
	}

	public void setEndTime(long endTime) {
		this.endTime = endTime;
	}

	// test
	@Override
	public String toString() {
		return "Alarm [alarmCode=" + alarmCode + ", deviceId=" + deviceId
				+ ", alarmType=" + alarmType + ", alarmMode=" + alarmMode
				+ ", beginTime=" + beginTime + ", endTime=" 
				+ TimeUtil.format(endTime) +",imgpath="+mImgPath
				+",RecordID="+recordId+"]";
	}

	public static String getAlarmTypeName(int alarmType) {
		String str = null;
		switch (alarmType) {
		case CameraAlarmType.ALARM_TYPE_DETECTION_MOTION:
			str = ShowmoApplication.getInstance().getString(
					R.string.motion_detection);
			break;
		case CameraAlarmType.ALARM_TYPE_OFFLINE:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_LOST_VIDEO:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_MASK_VIDEO:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_DISK_FULL:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_RECORD_ABNORMAL:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_LOG_LAMP_OFF:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_LED_SCREEN_OFF:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_TALK_REQUEST:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_LOG_LAMP_ON:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		case CameraAlarmType.ALARM_TYPE_LED_SCREEN_ON:
			str=ShowmoApplication.getInstance().getString(R.string.motion_offline);
			break;
		default:
			break;
		}

		return str;

	}

}
