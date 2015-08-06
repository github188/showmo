
#ifndef PW_NET_SDK_H
#define PW_NET_SDK_H

#include "proto.h"
using namespace std;

#ifdef NETMODULE_EXPORTS
#ifdef WIN32
	#define PWNET_API  extern "C" __declspec(dllexport)
#else
	#define PWNET_API __attribute__((__stdcall))
#endif
#else
#ifdef WIN32
 #define PWNET_API  extern "C" __declspec(dllimport)   //VC 用
#else
 #define PWNET_API __attribute__((__stdcall))
#endif
#endif

#ifndef CALL_METHOD
#ifdef WIN32
	#define CALL_METHOD	__stdcall  //__cdecl
#else
    #define CALL_METHOD	__stdcall  
#endif
#endif

#define  MAX_FILE_NUM 16
#define  FRAME_MAX    32
#define  AUDIO_MAX    1024
#define  ONE_BLK_SIZE (1024-sizeof(msg_header)-sizeof(msg_data)) 
#define  ALARM_BLK_SIZE (1024-sizeof(msg_header)-sizeof(alarm_msg_data)) 
#define  AUDIO_BLK_SIZE  320 

enum MsgId
{
	UPDATE_DOWNPOS = 1000,
	UPDATE_FAILED = 1001,
	UPDATE_SUCCESS = 1002,
	DOWNLOAD_PIC_POS = 1003,		//下载图片进度
	DOWNLOAD_PIC_FAILED = 1004,		//下载图片失败
	DOWNLOAD_PIC_SUCCESS = 1005,    //下载图片成功
	PLAY_STOP = 1006,               //播放已停止 
};

struct SDK_CAMERA_UPDATE
{
	uint32 cameraid;				//camera id
	uint32 cmd;				        //cmd MsgId
	uint32 downpos;                 //升级下载进度 
};

struct SDK_CAMERA_STATE
{
	uint32 customid;				//camera所属客户id
	uint32 cameraid;				//camera id
};


struct SDK_TIME
{
	uint32 nYear;		//年
	uint32 nMonth;		//月
	uint32 nDay;		//日
	uint32 nHour;		//时
	uint32 nMinute;	//分
	uint32 nSecond;	//秒
};

// 录像文件返回结构体
struct SDK_REMOTE_FILE
{
	uint32  size;						// 文件大小
	char    sFileName[108];				// 文件名
	uint32  nFileType;				    // 文件类型 枚举：Remote_File_Type  
	SDK_TIME startTime;	                // 文件开始时间
	SDK_TIME endTime;	                // 文件结束时间
};

// 查询录像条件
struct SDK_SEARCH
{
	uint32		 nFileType;			// 文件类型 枚举：Remote_File_Type
	SDK_TIME     startTime;			// 开始时间
	SDK_TIME     endTime;			// 结束时间
};


struct SDK_REMOTE_SEARCH
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	SDK_SEARCH  findinfo; 
};

struct SDK_REMOTE_FILELIST
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	uint32 file_count;
	SDK_REMOTE_FILE fileinfo[MAX_FILE_NUM];
};


struct REMOTE_CONTROL
{
	uint32 Action;	        // 回放动作
	uint32 TransMode;	    // 传输模式
	char   sFileName[108];
	SDK_TIME stStartTime;	// 开始时间
	SDK_TIME stEndTime;		// 结束时间
	uint32   PlayMode;	    // 0
	uint32   Value;	        // 
};

struct SDK_REMOTE_CONTROL
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	REMOTE_CONTROL control;
};

struct REMOTE_POS_ASK
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	char   sFileName[108];
};

struct REMOTE_POS_ACK
{
	uint32 remote_pos;
};

/// 传输模式
enum Remote_TansMode
{
	SDK_TRANSMODE_TCP,		// TCP传输
	SDK_TRANSMODE_UDP,		// UDP传输
	SDK_TRANSMODE_MCAST,	// 多播
	SDK_TRANSMODE_RTP,		// RTP传输
	SDK_TRANSMODE_NR
};

enum  Remote_PlayBack_Action
{
	SDK_PLAY_BACK_PAUSE,		 // 暂停回放 
	SDK_PLAY_BACK_CONTINUE,	 	 // 继续回放  
	SDK_PLAY_BACK_FAST,	         // 加速回放 
	SDK_PLAY_BACK_SLOW,	         // 减速回放 
	SDK_PLAY_BACK_SEEK_PERCENT,  // 回放定位百分比 
};

enum Remote_File_Type
{
	SDK_RECORD_ALL = 0,
	SDK_RECORD_ALARM = 1,	// 外部报警录像
	SDK_RECORD_DETECT,	    // 视频侦测录像
	SDK_RECORD_REGULAR,	    // 普通录像
	SDK_RECORD_MANUAL,	    // 手动录像
	SDK_PIC_ALL = 10,
	SDK_PIC_ALARM,		    // 外部报警抓拍
	SDK_PIC_DETECT,		    // 视频侦测抓拍
	SDK_PIC_REGULAR,         // 普通抓拍
	SDK_PIC_MANUAL,          // 手动抓拍
	SDK_TYPE_NUM,
};

struct SDK_APPLY_ACCOUNT
{
	uint16 user_type;
};

struct SDK_APPLY_ACCOUNTINFO
{
	char user_name[64];
	uint16 user_type;
};

struct SDK_REMOTE_MESSAGE   // 远程回放结束  HIST_VIDEO_CAMERA_ACK_MSG
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	uint32 id;
	uint32 option;
	uint32 value1;
	uint32 value2;
};

typedef struct tcframe_inner_blk_tags
{
	int  size;
	int  ask_count;  
}tcframe_inner_blk;

typedef struct tcframe_blk_array_tags
{
	unsigned int    frame_type;
	unsigned int    blk_received;  
	unsigned int    blk_sum;
	unsigned int    frame_size;
	unsigned int    frm_seq;
	unsigned int    frm_blk_seq;
	bool            blk_used;  
	bool            blk_all;
	bool            blk_push; 
	bool            blk_state[512];
	tcframe_inner_blk blks[512];  	
	char            *data;  
}tcframe_blk_array;

typedef struct tcaudio_inner_array_tags
{
	unsigned int    frm_size; 	
	char            data[160];  
}audio_inner_blk;

typedef struct tcaudioe_blk_array_tags
{
	unsigned int    frm_seq;
	bool            blk_used;
	unsigned int    frm_blk_seq; 
	bool            blkstate[15];  
	audio_inner_blk blkdata[15];
}audio_blk;


struct SDK_VOLUME_VALUE
{
	uint32 options;  //  开关选项;
	uint32 values;	 //  &options = 1 开  0 关	
	uint32 volume;   //  0-100 音量
	uint32 control;  //  静音
}; 

struct SDK_WIFI_VALUE
{
	unsigned int reserver1;
	unsigned int reserver2;
	unsigned int wifi_db;
	unsigned int local_ip;
	char   mac[18];
	char   version[16];
	char   currentssid[16];
};

enum NetWork
{
	Public_NET = 0,
	Local_NET ,
};
                                                                  
/***************************************************************************************************/



// 实时监视数据回调函数原形
typedef int   (*RealDataCallBack) (char *pBuffer,long lStreamType,long lFrameNum,long lbufsize,long dwUser); // lStreamType 0 iframe 1 pframe 4 jpeg

typedef void  (*errordatacallback) (char *pBuffer);

typedef void  (*msgdatacallback)(char *pBuffer,long lmsgid,long lexternid,long lbufsize,long dwUser);

// init
bool	PW_NET_Init();

// get last error
bool	PW_NET_GetLastError(unsigned long *dwError);

// get VerifyCode
bool	PW_NET_GetVerifyCode(send_verifycode_req* lpcode);

// check verify code
long    PW_NET_CheckVerifyCode(char *strUserName,char *strVerifyCode);// 0:验证码正确 1：验证码不存在  2：验证码不存在  -1：失败

// get app version
bool	PW_NET_GetAppVersion(int nDeviceType,query_app_version_ret *lpVersion); //0 ipc365; 1 iup365

// add terminal information
bool	PW_NET_AddTerminal(terminal_device_add_req *lpTerminal);

// reset password by old password
bool	PW_NET_ResetPassword(reset_password_req *lpByOldPass);

// reset password by verifycode
bool	PW_NET_ResetPasswordEx(reset_password_byverify_req *lpByVerify);

// ap get device online state
long	PW_NET_GetDeviceid(char *strUuid); // error <= 0

// change device name
bool    PW_NET_ModifyDevName(int nDeviceId,char *strName);

// device bilnd or not 
long	PW_NET_BindState(char *strUuid,char *strUser); // 0未绑定用户，1已被该用户绑定，2被其他用户绑定  -1 error

// add device
bool	PW_NET_AddDevice(client_device_add_req *lpAddDevice,client_device_add_ret *lpDevice);

// repeat add device
bool	PW_NET_AddDeviceEx(client_device_add_req *lpAddDevice,client_device_add_ret *lpDevice);

// delete device
bool	PW_NET_DeleteDevice(char *strUuid);

// account 
long	PW_NET_VerityAccount(char *strUser);  // 0 not exist  1 exist  -1 failure

// apply test account
bool	PW_NET_ApplyTestAccount(int nDeviceType,SDK_APPLY_ACCOUNTINFO *lpAccount);// 0 ipc365 1 xiaomo

// delete apply test account
bool	PW_NET_DeleteApplyAccount(SDK_APPLY_ACCOUNTINFO *lpAccount);

// sign up server
bool	PW_NET_SignUp(client_register_2_login *lpRegister); 

// online state
bool	PW_NET_OnLineState(int nCamera_id); // 0 error 1 success

// manager server
bool	PW_NET_Mgr_SignIn();

// disconnect manager server 
bool	PW_NET_Mgr_DisConnect(int nCamera_id);


// login loginserver
bool	PW_NET_Login(client_2_login_req *lpLogin);

// logout device
bool	PW_NET_Logout();

// clean 
bool    PW_NET_Cleanup();

// video   realplay
bool	PW_NET_StartRealPlay(int nCamera_id ,int *netType);

// set stream  type
bool	PW_NET_SetStreamType(int nCamera_id,int nSteramType);// 0 main 1 sub  2jpeg

// set video data callback 
bool	PW_NET_SetRealDataCallBack(RealDataCallBack cbRealData,long dwUser);

// set msg data callback
bool	PW_NET_SetMsgDataCallBack(msgdatacallback cbMsgData,long dwUser);

// start receive alarm
bool	PW_NET_SetAlarmState(int nCamera_id,int nalarmtype,bool state);

// get alarm state
long	PW_NET_GetAlarmState(int nCamera_id,int nalarmtype); // 0 关  1开 -1 失败

// get alarm pic
long	PW_NET_GetAlarmPic(int nCamera_id,int nalarmtype,int nalarmcode,int nccid,int nstarttime,int nendtime); // -1 failure  0 no pic  1 succcess  

// release buf
void	PW_NET_ReleaseBuf(char *strBuf);

// stop realplay
bool	PW_NET_StopRealPlay(int nCamera_id);

// sound
bool	PW_NET_Audio(int nCamera_id,bool state);

// get device list
bool	PW_NET_GetDeviceList(int nDeviceType,int* nDeviceNum,char *lpDeviceList);// 0:ipc365 1:xiaomo   client_device_query_ret *lpDeviceList);

// print log
bool	PW_NET_Log(bool bPrint);

// get talk state
long	PW_NET_GetTalkState(int nCamera_id); // 0 close 1 open -1 failed

// talk ctrl
bool	PW_NET_TalkCtrl(int nCamera_id,bool bstate);//state: 1 start 0 close      

// send talk data
bool	PW_NET_SendTalkData(int nCamera_id,char *strTalk ,long lTalkSize);

// dubug test
bool	PW_NET_SetDebugCallBack(errordatacallback func);

// upgrade
bool	PW_NET_Upgrade(int nCamera_id ,char *strVersion); 

// light
bool	PW_NET_BrightCtrl(int nCamera_id,int nBrightness,int nCtrl); // 0:close 1: open 2:set

// get light
bool	PW_NET_Brightness(int nCamera_id,int *nBrightness);

// get volume
bool    PW_NET_GetVolume(int nCamerai_d,SDK_VOLUME_VALUE *strOutputVolume);

// set volume
bool    PW_NET_SetVolume(int nCamerai_d,SDK_VOLUME_VALUE *strInputVolume);

// get wifi
bool    PW_NET_GetWifiValue(int nCamerai_d,SDK_WIFI_VALUE *strOutputWifi);

/************************************************************************/                                                                     
/************************************************************************/

// remote file
bool	PW_NET_SearchRemoteFile(int nCamera_id,SDK_SEARCH* lpFindInfo, char *lpFileData, int *findcount);

// remote play
bool	PW_NET_PlayBack(int nCamera_id,SDK_REMOTE_FILE *lpPlayBackFile, RealDataCallBack fDownLoadDataCallBack, long dwDataUser);

// remote play control
bool	PW_NET_PlayBackControl(int nCamera_id,int ctrl,int ctrlvalue);

// stop remote play
bool	PW_NET_StopPlayBack(int nCamera_id);


#endif
