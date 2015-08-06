package ipc365.app.showmo.jni;

import java.io.Serializable;
import java.util.HashMap;

import android.R.integer;
import android.R.interpolator;
import android.text.format.Time;
import android.util.Log;

public class JniDataDef {
	public interface OnRealdataCallBackListener {
		public int onDataCallBack(byte[] pBuffer, long lStreamType,
				long lFrameNum, long lbufsize, long dwUser);
	}
	public interface OnDebugDataCallbackListener{
		public void onDebugDataCallback(String debugmsg);
	}

	public interface OnMsgDataCallBackListener {
		public void onMsgDataCallBack(Object pBuffer, long lmsgid);
	}

	public static class TestClass {
		public int func(int a, int b) {
			Log.v("test"," a "+a+" b "+b);
			return a + b;
		}
	}
	public static class LightCtrl{
		public static int close=0;
		public static int open=1;
		public static int set=2;
	}
	public static class MSGID {
		public static final int UPDATE_MOBILE_INVITE_INFO = 20313;// 升级消息
		// Update_2_mobile_invite
		public static final int UPDATE_MOBILE_INVITE_ACK = 20314;// 升级消息响应Device_2_update_ftp_ack
		public static final int UPDATE_DEVICE_FTP_INFO = 20315;
		public static final int UPDATE_DEVICE_FTP_INFO_ACK = 20316;
		public static final int HIST_VIDEO_CAMERA_ACK_MSG = 20240;// 视频回放结束Remote_Message
		public static final int CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG = 20320;// 告警消息device_alarm_server_upload_msg
		//public static final int CAMERA_ALARM_DATA_UPLOAD_MSG = 20323; // 告警图片数据
		// string
		public static final int CLIENT_MGR_CAMERA_OFFLINE_MSG = 20206;// 设备下线User_2_mgr_disconn_device
		public static final int CLIENT_MGR_CAMERA_ONLINE_MSG = 20207;// 设备上线User_2_mgr_disconn_device
		public static final int MGR_OUTLINE = 80008;// mgr断线。
		public static final int UPDATE_DOWNPOS = 1000;              //设备正在升级
		public static final int	UPDATE_FAILED = 1001;           //设备升级失败
		public static final int	UPDATE_SUCCESS = 1002;          //设备升级成功
		public static final int	DOWNLOAD_PIC_POS = 1003;		//下载图片进度
		public static final int	DOWNLOAD_PIC_FAILED = 1004;		//下载图片失败
		public static final int	DOWNLOAD_PIC_SUCCESS = 1005;    //下载图片成功
		public static final int	 PLAY_STOP = 1006;//播放断线

	}

	public static class MSGBroadcastActions {
		public static final String DataKey="data";
		// 广播ACTION intent.getExtras().getSerializable("data")获取数据
		// 升级信息广播，附加数据对象类型Update_2_mobile_invite
		public static final String UPDATE_MOBILE_INVITE_INFO = "ipc365.app.shomo.UPDATE_MOBILE_INVITE_INFO";
		// 升级响应广播，附加数据对象类型Device_2_update_ftp_ack
		public static final String UPDATING_MOBILE_INVITE_ACK = "ipc365.app.shomo.UPDATING_MOBILE_INVITE_ACK";
		public static final String UPDATE_SUCCESS_MOBILE_INVITE_ACK = "ipc365.app.shomo.UPDATE_SUCCESS_MOBILE_INVITE_ACK";
		public static final String UPDATE_FAILE_MOBILE_INVITE_ACK = "ipc365.app.shomo.UPDATE_FAILE_MOBILE_INVITE_ACK";

		public static final String UPDATE_DEVICE_FTP_INFO = "ipc365.app.shomo.UPDATE_DEVICE_FTP_INFO";
		public static final String UPDATE_DEVICE_FTP_INFO_ACK = "ipc365.app.shomo.UPDATE_DEVICE_FTP_INFO_ACK";
		// 回放结束广播，附加数据对象类型Remote_Message
		public static final String HIST_VIDEO_CAMERA_ACK_MSG = "ipc365.app.shomo.HIST_VIDEO_CAMERA_ACK_MSG";
		// 告警信息广播，附加数据对象类型Device_alarm_server_upload_msg
		public static final String CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG = "ipc365.app.shomo.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG";
		// 设备上下线广播，附加数据对象类型User_2_mgr_disconn_device
		public static final String CLIENT_MGR_CAMERA_ONLINE_STATE_CHANGE_MSG = "ipc365.app.shomo.CLIENT_MGR_CAMERA_ONLINE_CHANGE_MSG";
		//告警图片下载,附加数据对象类型alarm_data_state 
		public static final String CAMERA_ALARM_DATA_UPLOAD_MSG="ipc365.app.shomo.CAMERA_ALARM_DATA_UPLOAD_MSG";
		public static final String CAMERA_ALARM_DATA_DOWLOAD_fai_MSG="ipc365.app.shomo.CAMERA_ALARM_DATA_DOWLOAD_fai_MSG";
		public static final String CAMERA_ALARM_DATA_DOWLOAD_POS="ipc365.app.shomo.CAMERA_ALARM_DATA_DOWLOAD_POS";
	}

	public static class CAMERA_ALARM_TYPE {
		public final static int ALARM_TYPE_OFFLINE = 0;
		public final static int ALARM_TYPE_LOST_VIDEO = 1;
		public final static int ALARM_TYPE_MASK_VIDEO = 2;
		public final static int ALARM_TYPE_DETECTION_MOTION = 3;
		public final static int ALARM_TYPE_DISK_FULL = 4;
		public final static int ALARM_TYPE_RECORD_ABNORMAL = 5;
		public final static int ALARM_TYPE_LOG_LAMP_OFF = 6; // Log灯箱断电
		public final static int ALARM_TYPE_LED_SCREEN_OFF = 7; // LED屏幕断电
		public final static int ALARM_TYPE_TALK_REQUEST = 8; // 店内通话请求
		public final static int ALARM_TYPE_LOG_LAMP_ON = 9; // Log灯箱上电
		public final static int ALARM_TYPE_LED_SCREEN_ON = 10; // LED屏幕上电
		// by 录像丢失报警
		public final static int ALARM_TYPE_LOST_RECORD = 11;
		public final static int ALARM_TYPE_ONLINE = 20;
		// by 无报警
		// 20120518
		public final static int ALARM_TYPE_NULL = 100;
	};


	public static class alarm_data_state implements Serializable{
		public int recordId;
		public int state; //-1 失败 1成功
		public int cameraId;
	}
	public static class alarm_data_download implements Serializable {
		public String alarmImgFilename;
		public int recordId;
		public int state;//0失败 1成功
	}
	public static class alarm_data_download_progress implements Serializable {
		public int recordId;
		public int pos;//进度
	}

	public static class User_2_mgr_disconn_device implements Serializable {
		public int device_id;
		public int user_id;

		@Override
		public String toString() {
			return "User_2_mgr_disconn_device [device_id=" + device_id
					+ ", user_id=" + user_id + "]";
		}

	}

	public static class Update_2_mobile_invite implements Serializable {
		public int deviceId;
		public int customId;
		public String softwareVersion;
		public int reserver1;
		public int reserver2;

		@Override
		public String toString() {
			return "Update_2_mobile_invite [deviceId=" + deviceId
					+ ", customId=" + customId + ", softwareVersion="
					+ softwareVersion + ", reserver1=" + reserver1
					+ ", reserver2=" + reserver2 + "]";
		}
	}
	public static class UpGradeErrcode{
		public static int FTP_Login_Failed = 1;
		public static int Download_TimeOut = 2;
		public static int FTP_File_Start_Failed = 3;
	}
	public static class SDK_CAMERA_UPDATE implements Serializable {
		public int cameraid;
		public int cmd;//MSGID.UPDATE_DOWNPOS 进度 // MSGID.UPDATE_FAILED升级失败 //MSGID.UPDATE_SUCCESS成功
		public int downpos;
		public int errcode;
		@Override
		public String toString() {
			return "SDK_CAMERA_UPDATE [cameraid=" + cameraid + ", cmd=" + cmd
					+ "]";
		}

	}

	public static class Remote_Message implements Serializable {
		public int cameraId;
		public int clientId;
		public int customId;
		public int deviceId;
		public int id;
		public int option;
		public int value1;
		public int value2;

		@Override
		public String toString() {
			return "Remote_Message [cameraId=" + cameraId + ", clientId="
					+ clientId + ", customId=" + customId + ", deviceId="
					+ deviceId + ", id=" + id + ", option=" + option
					+ ", value1=" + value1 + ", value2=" + value2 + "]";
		}

	}

	public static class Device_alarm_server_upload_msg implements Serializable {
		public int clientId;
		public int recordId;
		public int deviceId;
		public int cameraId;
		public int channelNo;//0代表没有TF卡，用户下载提示没有TF卡
		public int alarmType;// 报警类型 enum CAMERA_ALARM_TYPE
		public long beginTime;
		public long endTime;
		public int alarmMode;// 0x01,图片 0x02 视频
		public int alarmCode; // 报警编号
		public int Ccid;// 硬盘分区

		@Override
		public String toString() {
			return "Device_alarm_server_upload_msg [clientId=" + clientId
					+ ", recordId=" + recordId + ", deviceId=" + deviceId
					+ ", cameraId=" + cameraId + ", channelNo=" + channelNo
					+ ", alarmType=" + alarmType + ", beginTime=" + beginTime
					+ ", endTime=" + endTime + ", alarmMode=" + alarmMode
					+ ", alarmCode=" + alarmCode + ", Ccid=" + Ccid + "]";
		}

	}
	public static class VolumeTypes
	{
		public static int STREAM_TYPE= 0x00000001;//视频开启关闭播报	
	};
	public static class DeviceVolumState implements Serializable{
		public boolean bSwitch;
		public int type;
	}

	public static class BindStateInfo{
		public String deviceCurUser;
		public int state;
	}
	public static class QueryAppVersionRet {
		public String version;
		public String feature;
	}

	public static class TerminalDeviceAddReq {
		public int user_id;
		public String dev_uuid;
		public String dev_model;
		public String dev_osversion;
	}

	public static class ResetPasswordReq {
		public String user_name;
		public String oldpass;
		public String newpass;
	}

	public static class ResetPasswordByVerifyReq {

		public ResetPasswordByVerifyReq(String user_name, String verifycode,
				String newpass) {
			super();
			this.user_name = user_name;
			this.verifycode = verifycode;
			this.newpass = newpass;
		}

		public String user_name;
		public String verifycode;
		public String newpass;
	}

	// 用户添加设备
	public static class ClientDeviceAddReq {
		public long custom_id;
		public String device_sn; // 摄像头uuid
		public String device_name; // add
		public String device_passwd; // add
	}

	// 返回为成功时的包体：
	public static class ClientDeviceAddRet {
		public long device_id; // 从redis（从camera）
		public long camera_id;
	}

	public static class SDK_APPLY_ACCOUNTINFO {
		public String user_name;
		public int user_type;
	}
	public static class SDK_WIFI_VALUE implements Serializable{
		public int reserver1;
		public int reserver2;
		public int wifi_db;
		public String local_ip;
		public String   mac;
		public String   version;
		public String   currentssid;
		@Override
		public String toString() {
			return "SDK_WIFI_VALUE [reserver1=" + reserver1 + ", reserver2=" + reserver2 + ", wifi_db=" + wifi_db
					+ ", local_ip=" + local_ip + ", mac=" + mac + ", version=" + version + ", currentssid="
					+ currentssid + "]";
		}

	}

	public static class ClientRegister2Login {

		public ClientRegister2Login(String user_name, String pass,
				String verifycode, String device_sn, int user_type) {
			super();
			this.user_name = user_name;
			this.pass = pass;
			this.verifycode = verifycode;
			this.device_sn = device_sn;
			this.user_type = user_type;
		}

		public String user_name;
		public String pass;
		public String verifycode; // 验证码
		public String device_sn;
		public int user_type; // 用户类型
	}

	public static class ClienteLoginReq {
		public String user_name;
		public String pass;
		public int net_type;
		public int padding;
	}

	public static class PanoInfo {
		public long lWidth;
		public long lHeight;
		public long lCenterX_1024;
		public long lCenterY_1024;
		public long lRadius_1024;
		public long lPanoType;
		public long lPanoTilt;
	}

	public static class MsgInitParam // 初始化客户端buffer 边界消息
	{
		public long camera_id;
		public long min_block_seq;// 帧块最小边界
		public long max_block_seq;// 帧块最大边界
		public long device_type; // OEM device type
		public long decode_head_size;
		public char decode_head;
	}// buffermap消息,推送消息

	public static class ClientDeviceQueryRet {
		public long device_id;
		public String device_sn; // 摄像头uuid
		public String device_name;
		public String device_passwd;
	}

	public static class SDK_SEARCH {

		public int nFileType;// 文件类型 枚举：Remote_File_Type;
		public Time startTime;
		public Time endTime;
	}
	public static class  Remote_PlayBack_Action
	{
		public static final int SDK_PLAY_BACK_PAUSE=0;		 // 暂停回放 
		public static final int SDK_PLAY_BACK_CONTINUE=1;		 	 // 继续回放  
		public static final int SDK_PLAY_BACK_FAST=2;		         // 加速回放 
		public static final int SDK_PLAY_BACK_SLOW=3;		         // 减速回放 
		public static final int SDK_PLAY_BACK_SEEK_PERCENT=4;	  // 回放定位百分比 
	};
	public static class Remote_File_Type {
		public static final int SDK_RECORD_ALL = 0;
		public static final int SDK_RECORD_ALARM = 1; // 外部报警录像
		public static final int SDK_RECORD_DETECT = 2; // 视频侦测录像
		public static final int SDK_RECORD_REGULAR = 3; // 普通录像
		public static final int SDK_RECORD_MANUAL = 4; // 手动录像
		public static final int SDK_PIC_ALL = 10;
		public static final int SDK_PIC_ALARM = 11; // 外部报警抓拍
		public static final int SDK_PIC_DETECT = 12; // 视频侦测抓拍
		public static final int SDK_PIC_REGULAR = 13; // 普通抓拍
		public static final int SDK_PIC_MANUAL = 14; // 手动抓拍
		public static final int SDK_TYPE_NUM = 15;
	};

	public static class SDK_REMOTE_FILE {
		public int size;
		public String sFileName;
		public int nFileType;
		public Time startTime;
		public Time endTime;
	}

	public static class Broadcast_wifi_info{
		public String ssid;
		public String psw;
		public String keytype;
		public String data;
		public Broadcast_wifi_info(String ssid,String psw,String keyType,String data){
			this.ssid=ssid;
			this.psw=psw;
			this.keytype=keyType;
			this.data=data;
		}
	}

	public static class PW_PHONE_BL_WIFI_CONFIG {
		public boolean bEnable;
		public int[] sSSID; // 36 bit SSID Number
		public int nChannel; // channel
		public int[] sNetType; // Infra, Adhoc
		public int[] sEncrypType; // NONE, WEP, TKIP, AES
		public int[] sAuth; // OPEN, SHARED, WEPAUTO, WPAPSK, WPA2PSK, WPANONE,
		// WPA, WPA2
		public int nKeyType; // 0:Hex 1:ASCII

		public int[] sKeys; // 128bit
		public int[] HostIP; // /< 4bit host ip
		public int[] Submask; // /< 4bit netmask
		public int[] Gateway; // /< 4bit gateway
	}

	public static class FrameSize {
		public int nWidth;
		public int nHeight;
	}

	// public static class PWPA_CAM_POSTURE{
	// boolean bEnable;
	//
	// float fViewAngle;
	// float fMinDistance;
	// float fMaxDistance;
	//
	// float fPan;
	// float fTilt;
	// float fRotate;
	//
	// float fPosiX;
	// float fPosiY;
	// float fPosiZ;
	// };
	// public static class PWPA_POSTURE{
	// byte bNeedRotate;
	// byte bNeedTranslate;
	// byte bNeedScale;
	// byte byExtern;
	// float[] fRotate; // XYZ // Old version dRotateTilt = dRotate[0]
	// float[] fTranslate; // XYZ
	// float[] fScale; // XYZ
	// };
	// public static class
	//
	// public static class_PWPA_MEM_TEX_CFG
	// {
	// String pstrTextureName;
	// LPGVM_SCREEN pstPrivateTexture;
	// };
	// public static class PW_PATTERN_OUT{
	// float[] fBackColor; // GRBA:0~1
	// PWPA_CAM_POSTURE stCamState;
	// PWPA_POSTURE stWorldPosture;
	//
	// int lTextureExCount;
	// PWPA_MEM_TEX_CFG *pstTextureExList;
	// int lGroupCount;
	// const PW_PATTERN_GROUP* pstPatternGroup;
	//
	// /*-- pstPatternEx point to a 'PW_PATTERN_GROUP' struct added
	// provisionally.
	// Will be drawn at last that ignore 'dwPriority'.
	// --*/
	// const GVoid* phPatternAddEx;
	// }
}
