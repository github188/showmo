package com.showmo.activity.alarmInfo;

public interface CameraAlarmType {
	int ALARM_TYPE_OFFLINE = 0;
	int ALARM_TYPE_LOST_VIDEO = 1;
	int	ALARM_TYPE_MASK_VIDEO = 2;
	int	ALARM_TYPE_DETECTION_MOTION = 3;
	int	ALARM_TYPE_DISK_FULL = 4;
	int	ALARM_TYPE_RECORD_ABNORMAL = 5;
	int	ALARM_TYPE_LOG_LAMP_OFF = 6;          //Log灯箱断电 
	int	ALARM_TYPE_LED_SCREEN_OFF = 7;         //LED屏幕断电
	int	ALARM_TYPE_TALK_REQUEST = 8;           //店内通话请求
	int	ALARM_TYPE_LOG_LAMP_ON = 9;           //Log灯箱上电 
	int	ALARM_TYPE_LED_SCREEN_ON = 10;         //LED屏幕上电

	//by 录像丢失报警
	int ALARM_TYPE_LOST_RECORD = 11;

	int	ALARM_TYPE_ONLINE = 20;
	//by 无报警
	//20120518
	int	ALARM_TYPE_NULL = 100;

}
