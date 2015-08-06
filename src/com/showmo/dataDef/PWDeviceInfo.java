package com.showmo.dataDef;

import java.lang.String;

public class PWDeviceInfo {
	public PWDeviceInfo() {
		m_imgPath = "";
		m_uuid = "";
		m_devPsw = "";
		m_devName = "";
		m_cameraId = 0;
		m_onLine = false;
		m_newVersion = "";
	}
	public PWDeviceInfo(String imgPath, String uuid, String devPsw, 
			String devName,int cameraId, boolean onLine) {
		m_imgPath = imgPath;
		m_uuid = uuid;
		m_devPsw = devPsw;
		m_devName = devName;
		m_cameraId = cameraId;
		m_onLine = onLine;
		m_newVersion = "";
	}

	public PWDeviceInfo(String imgPath, String uuid, String devPsw, 
			String devName,int cameraId, boolean onLine, String newVersion) {
		m_imgPath = imgPath;
		m_uuid = uuid;
		m_devPsw = devPsw;
		m_devName = devName;
		m_cameraId = cameraId;
		m_onLine = onLine;
		m_newVersion = newVersion;
	}

	public String m_imgPath;
	public String m_uuid;
	public String m_devPsw;
	public String m_devName;
	public boolean m_onLine;
	public int m_cameraId;
	public String m_newVersion;
}
// Q_DECLARE_METATYPE(Device)
// class User
// {
// public:
// User(){}
// User(const QString &user, const QString &psw,int
// isRemPsw):m_user(user),m_psw(psw),m_isRemPsw(isRemPsw){;}
// User(const User& rhs){
// this->m_user=rhs.m_user;
// this->m_psw=rhs.m_psw;
// this->m_isRemPsw=rhs.m_isRemPsw;
// }
// QString m_user;
// QString m_psw;
// int m_isRemPsw;
// //![1]
// };
// Q_DECLARE_METATYPE(User)
// class AP{
// public:
// AP(const QString &ssid):m_ssid(ssid){;}
// QString m_ssid;
// };
// class APAddException{
// public:
// APAddException(const QString &user,const QString &uuid,const QString
// &devname,int state):\
// m_user(user),m_uuid(uuid),m_devname(devname),m_state(state){;}
// QString m_user;
// QString m_uuid;
// QString m_devname;
// int m_state;
// };
//
// class AlarmInfo{
// public:
// AlarmInfo(){
// m_clientId=0;
// m_recordId=0;
// m_deviceId=0;
// m_cameraId=0;
// m_channelId=0;
// m_alarmType=0;
// m_beginTime=0;
// m_endTime=0;
// m_alarmMode=0;
// m_alarmNode=0;
// m_ccid=0;
// }
// AlarmInfo(const AlarmInfo &info){
// m_clientId=info.m_clientId;
// m_recordId=info.m_recordId;
// m_deviceId=info.m_deviceId;
// m_cameraId=info.m_cameraId;
// m_channelId=info.m_channelId;
// m_alarmType=info.m_alarmType;
// m_beginTime=info.m_beginTime;
// m_endTime=info.m_endTime;
// m_alarmMode=info.m_alarmMode;
// m_alarmNode=info.m_alarmNode;
// m_ccid=info.m_ccid;
// m_devName=info.m_devName;
// m_uuid=info.m_uuid;
// transTime();
// //qDebug()<<"copy AlarmInfo ";
// }
// AlarmInfo(int clientId,int recordId,int deviceId,int cameraId,int
// channelId,int alarmType,\
// int beginTime,int endTime,int alarmMode,int alarmNode,int ccid,QString
// devName,QString uuid){
// m_clientId=clientId;
// m_recordId=recordId;
// m_deviceId=deviceId;
// m_cameraId=cameraId;
// m_channelId=channelId;
// m_alarmType=alarmType;
// m_beginTime=beginTime;
// m_endTime=endTime;
// m_alarmMode=alarmMode;
// m_alarmNode=alarmNode;
// m_ccid=ccid;
// m_devName=devName;
// m_uuid=uuid;
// transTime();
// }
// ~AlarmInfo(){}
// public:
// QString m_devName;
// QString m_uuid;
// int m_clientId;
// int m_recordId;
// int m_deviceId;
// int m_cameraId;
// int m_channelId;
// int m_alarmType;
// int m_beginTime;
// int m_endTime;
// int m_alarmMode;
// int m_alarmNode;
// int m_ccid;
// QString m_beginDT;
// QString m_endDT;
// void transTime(){
// time_t bTime=m_beginTime;
// time_t eTime=m_endTime;
// struct tm* bPtr=gmtime(&bTime);
// struct tm* ePtr=gmtime(&eTime);
//
// m_beginDT=QString(bPtr->tm_hour<=9?"0":"")+QString::number(bPtr->tm_hour)+":"+\
// QString(bPtr->tm_min<=9?"0":"")+QString::number(bPtr->tm_min)+":"+\
// QString(bPtr->tm_sec<=9?"0":"")+QString::number(bPtr->tm_sec);
// m_endDT=QString(ePtr->tm_hour<=9?"0":"")+QString::number(ePtr->tm_hour)+":"+\
// QString(ePtr->tm_min<=9?"0":"")+QString::number(ePtr->tm_min)+":"+\
// QString(ePtr->tm_sec<=9?"0":"")+QString::number(ePtr->tm_sec);
// }
// };
// Q_DECLARE_METATYPE(AlarmInfo)
// class SafeLevel{
// public://user---levelId---levelName----levelState-----loginLimit-----dataUpload
// SafeLevel(){}
// SafeLevel(QString name,int levelId,bool levelState,bool loginLimit,bool
// dataUpload):m_name(name),m_levelId(levelId),\
// m_levelState(levelState),m_loginLimit(loginLimit),m_dataUpload(dataUpload){}
// SafeLevel(const SafeLevel& rhs){
// m_name=rhs.m_name;
// m_levelId=rhs.m_levelId;
// m_levelState=rhs.m_levelState;
// m_loginLimit=rhs.m_loginLimit;
// m_dataUpload=rhs.m_dataUpload;
// }
// QString m_name;
// int m_levelId;
// bool m_levelState;
// bool m_loginLimit;
// bool m_dataUpload;
// };
//
// Q_DECLARE_METATYPE(SafeLevel)
// class Wifi{
// public:
// Wifi(){}
// Wifi(const QString &ssid, const int power, bool isLock):m_ssid(ssid),\
// m_power(power),m_isLock(isLock){;}
// Wifi(const Wifi& rhs){
// m_ssid=rhs.m_ssid;
// m_power=rhs.m_power;
// m_isLock=rhs.m_isLock;
// }
// QString m_ssid;
// int m_power;
// int m_isLock;
// };
// Q_DECLARE_METATYPE(Wifi)