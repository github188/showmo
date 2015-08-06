
/*
* @file
*
* @path $(cspclient)
*
* @desc
*
* Copyright (c) Yesod Inc. 9.03.2009
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
*

*
*/


#ifndef __PROTO_H__NjjnddeuiuiuiooiioBJHSDMNMNwhduhqwdBCNASAKDJK78710230909KW_____
#define __PROTO_H__NjjnddeuiuiuiooiioBJHSDMNMNwhduhqwdBCNASAKDJK78710230909KW_____

#include "uu_types.h"

#if defined(_WIN32)
#pragma warning(disable : 4200)
#endif

//For protocol version 
#define   PROTOCOL_VERSION_MAJOR		105
#define   PROTOCOL_VERSION_REVISION      "$Rev: 4831 $"
#define   PROTOCOL_SET_HEADER_VER(pheader)   \
	do{  \
	unsigned short  maj=PROTOCOL_VERSION_MAJOR;\
	unsigned long  rev=0;\
	struct msg_header* __p_header__=(struct msg_header*)pheader;\
	__p_header__->ver=(maj<<16);\
	sscanf(PROTOCOL_VERSION_REVISION,"$Rev: %d", (int*)&rev);\
	__p_header__->ver|=(unsigned int)rev;\
}while(0)
////////////////////////////////////
#define MAX_GET_PARTNER_COUNT 10
#define MAX_CAMERA_NAME_SIZE		64
#define MAX_NICK_NAME_SIZE 32
#define LF_FACESIZE         32

#define SERVER_BROADCAT_ALIVE_TIMEOUT 3
/////////////////////////////////
//right mask defines security_center_admin
//////////////////////////////////////////////
#define		RIGHT_MASK_COMPANY_ADMIN				(1<<0)
#define		RIGHT_MASK_SECURITY_CENTER_ADMIN		(1<<1) 
#define		RIGHT_MASK_OPEN_VOICE					(1<<2) 
#define		RIGHT_MASK_PUBLIC_TALK					(1<<3) 
#define		RIGHT_MASK_PRIVATE_TALK					(1<<4)         

//by 20120802 增加云台控制权限
#define		RIGHT_MASK_CONTROL_DOME					(1<<5)
//by 20210802 增加对历史、下载视频界面的浏览权限
#define		RIGHT_MASK_HISTORY_VIEW					(1<<6)		
/////////////////////////////////////////////
///////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//by 20121205 增加路由类型
enum PEER_NAT_TYPE
{
	FINE_NAT,	//易于打洞成功的NAT
	BAD_NAT = 10,	//不易于打洞成功的NAT
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//by 20121221
enum ORDER_TYPE
{
	SNAP = 1,	//拍照
};
//////////////////////////////////////////////////////////////////////////

enum PEER_TYPE
{
	CAMERA_PEER,
	USER_PEER,//普通用户类型
	USER_SUPER_PEER,//用户超级节点类型
	MASTER_SERVER_SUPER_PEER,//服务器主超级节点类型
	SLAVE_SERVER_SUPER_PEER,//服务器主超级节点类型
	SERVER_BRIDGE_PEER,//服务器双线桥接类型
};

enum NET_TYPE
{
	NET_UNKNOWN,//未知
	NET_TEL,//电信
	NET_CNC,//网通
	NET_EDU,//教育网
	NET_CHIWALL,//长宽
	NET_TV,//有线通
};


enum FRAME_TYPE
{
	IFRAME,
	PFRAME,
	BFRAME,
	FRAME_TYPE_G711A=3,
	FRAME_JPEG = 4,
	FRAME_TYPE_PCMU =10,			
	FRAME_TYPE_PCMA =18,			
	FRAME_TYPE_G7231 =14,		
	FRAME_TYPE_G722 =19,			
	FRAME_TYPE_G728 =25,			
	FRAME_TYPE_G729 =28,			
	FRAME_TYPE_RAWAUDIO =29,	
	FRAME_TYPE_ADPCM =30,		
	FRAME_TYPE_ADPCM_HISI =31,
	FRAME_TYPE_AUDIO_HAIK = 32,	
	FRAME_TYPE_ALARM_JPEG =61,
	FRAME_TYPE_MP3 =106
};

enum CSP_ERRORS
{
	RESPONSE_OK =0,
	RESPONSE_FAILED=1,

	CSP_ERROR_SUCCESS,
	//user  			[100,200)
	CSP_ERROR_NAME_REPEATED = 100,
	CSP_ERROR_GET_USER_DETAIL,
	CSP_ERROR_USER_NAME_NOTEXIST,
	CSP_ERROR_UESR_NAME_NULL,
	CSP_ERROR_USER_KEY_WRONG,
	CSP_ERROR_USER_GET_DEVICE_LIST,

	CSP_ERROR_USER_TYPE_WRONG,	 //用户类型错误	请求测试账号,测试账号回收
	CSP_ERROR_USER_NO_DEMO_USER, //暂无测试账号	请求测试账号

	//custom  			[200,300)
	CSP_ERROR_GET_CUSTOMER_DETAIL = 200,
	CSP_ERROR_GET_CUSTOMER_GROUPS,
	CSP_ERROR_GET_CUSTOMER_LIST,
	CSP_ERROR_CUSTOM_GET_DEVICE_LIST,
	CSP_ERROR_CUSTOM_GET_USER_LIST,
	//register,login  	[300,400)
	CSP_ERROR_REGISTER_MSG_NULL = 300,
	CSP_ERROR_RESETKEY_MSG_NULL,
	CSP_ERROR_LOGIN_FAILED,
	//device     		[400,500)
	CSP_ERROR_GET_DEVICE_ID = 400,
	CSP_ERROR_ADD_DEVICE,
	CSP_ERROR_GET_DEVICE,
	CSP_ERROR_DEL_DEVICE,
	CSP_ERROR_GET_DEVICE_DETAIL,
	CSP_ERROR_DEVICE_GET_CAMERA_LIST,
	CSP_ERROR_DEVICE_GET_CUSTOM,
	CSP_ERROR_DEVICE_STATUS_NOTEXIST,
	CSP_ERROR_DEVICE_STATUS_ADDED,
	CSP_ERROR_DEVICE_STATUS_ABANDONED,
	CSP_ERROR_DEVICE_STATUS_WRONG,
	//camera   			[500,600)
	CSP_ERROR_GET_CAMERA_DETAIL = 500,
	CSP_ERROR_CAMERA_GET_CUSTOM,
	//terminaldevice   	[600,700)
	CSP_ERROR_TERMINAL_DEVICE_NULL = 600,
	CSP_ERROR_TERMINAL_DEVICE_EXISTED,
	CSP_ERROR_TERMINAL_DEVICE_NOTEXIST,
	CSP_ERROR_GET_TERMINAL_DEVICE,
	//terminalexception [700,800)
	CSP_ERROR_ADD_TERMINAL_EXCEPTION = 700,
	//verifycode  		[800,900)
	CSP_ERROR_SEND_VERIFYCODE = 800,
	CSP_ERROR_VERIFYCODE_NULL,
	CSP_ERROR_VERIFYCODE_WRONG,
	//app  				[900,1000)
	CSP_ERROR_GET_APPVERSION = 900,
	//MOD 2014.12.8

	//old				[1000,1100)
	CSP_ERROR_JOIN_DEPARTMENT = 1000,
	CSP_ERROR_GET_DEPARTMENTS,
	CSP_ERROR_DEL_EMPLOYEE,
	CSP_ERROR_GET_AUTH_CAMERAS,
	CSP_ERROR_GET_RIGHTS,
	CSP_ERROR_UPDATE_USER,
	CSP_ERROR_GET_ADMIN_FAILED,
	CSP_ERROR_ADMIN_NOT_ONLINE,
	CSP_ERROR_QUERY_FROM_STATUS_FAILED,

	//状态包代码		[1100,1200)
	CSP_STATE_START_LOOK_CAMERA_OK = 1100,
	CSP_STATE_GET_PARTNERS_FAILED,
	CSP_STATE_ASK_BAKFRAME_FAILED,
	CSP_STATE_ASK_INIT_BUF_FAILED,
	CSP_STATE_MGR_SIGN_IN_OK,
	CSP_STATE_MGR_SIGN_IN_FAILED,
	CSP_STATE_VD_SIGN_IN_OK,
	CSP_STATE_VD_SIGN_IN_FAILED,
	CSP_STATE_MGR_CLIENT_ONLINE_OK,
	CSP_STATE_MGR_CLIENT_ONLINE_FAILED,

	//other 			[1200,1300)
	CSP_ERROR_INVALID_SESSION_ID = 1200,
	
	CSP_ERROR_DOMAIN = 1250,
    CSP_ERROR_NO_INIT = 1500,
	CSP_ERROR_MGR_NOLINK = 1501,
	CSP_ERROR_VIDEO_NOLINK = 1502,
	CSP_ERROR_IPC_NOLINK = 1503,
	CSP_ERROR_LOGIN_NOLINK = 1504,
	CSP_ERROR_IPC_NOIP = 1505,
	CSP_ERROR_IPC_CONN = 1506,
	CSP_ERROR_LOGIN_CONN = 1507,

	CSP_ERROR_SOCKET_CREATE = 2000,
	CSP_ERROR_SOCKET_CONNECT = 2001,
	CSP_ERROR_SOCKET_BIND = 2002,
	CSP_ERROR_SEND_FAILED = 2003,
	CSP_ERROR_MEMORY_ERROR = 2004,
	CSP_ERROR_DEVICE_OFFLINE = 2005,
	CSP_ERROR_DATA_NOMATCH = 3000, 
	CSP_ERROR_ILLEGAL_PARAM = 4000,
	CSP_ERROR_TIMEOUT = 5000,
	CSP_ERROR_PICDOWNING = 6000,
	CSP_ERROR_LOGIN_REPEAT,
};

enum CSP_CAMERA_VISIBLE_LEVEL
{
	CSP_CAMERA_PUBLIC,
	CSP_CAMERA_PROTECTED,
	CSP_CAMERA_PRIVATE
};

enum CSP_CAMERA_ERRORS
{
	CSP_ERROR_NO_FRAME,
};

enum CONNECT_VIDEO_TYPE
{
	CONNECT_VIDEO_NORMAL,
	CONNECT_VIDEO_REALTIME,
	CONNECT_VIDEO_HISTORY,
	CONNECT_VIDEO_DOWNLOADFILE,	//下载文件
	CONNECT_VIDEO_REALTIME_ALARM,
};

enum PIC_PARAM_TYPE
{
	PIC_BRIGHTNESS = 0,
	PIC_CONTRAST,
	PIC_HUE,
	PIC_SATURATION,
};

enum VIDEO_FLOW_TYPE
{
	FLOW_VIDEO = 0,						//video flow
	FLOW_MUTI,							//multiple flow(both video and audio)
};

enum VIDEO_BITRATE_TYPE
{
	VIDEO_BITRATE_FIXED = 0,					//fixed bit rate
	VIDEO_BITRATE_VARIABLE,					//variable bit rate
};

enum VIDEO_BITRATE
{
	VIDEO_BITRATE_64 = 64,					//64kbps
	VIDEO_BITRATE_128 = 128,					//128kbps
	VIDEO_BITRATE_256 = 256,					//256kbps
	VIDEO_BITRATE_384 = 384,					//384kbps
	VIDEO_BITRATE_512 = 512,					//512kbps
	VIDEO_BITRATE_768 = 768,					//768kbps
	VIDEO_BITRATE_1024 = 1024,				//1Mbps
	VIDEO_BITRATE_1536 = 1536,				//1.5Mbps
	VIDEO_BITRATE_2048 = 2048,				//2Mbps
};

enum VIDEO_QUALITY
{
	VIDEO_QUALITY_BEST = 0,				//best video quality
	VIDEO_QUALITY_BETTER,				//better video quality
	VIDEO_QUALITY_GOOD,					//good video quality
	VIDEO_QUALITY_NORMAL,				//normal video quality
	VIDEO_QUALITY_BAD,					//bad video quality
	VIDEO_QUALITY_WORSE,				//worse video quality
};

enum VIDEO_FRAMERATE
{
	VIDEO_FRAMERATE_25 = 25,					//25f/s
	VIDEO_FRAMERATE_20 = 20,					//20f/s
	VIDEO_FRAMERATE_15 = 15,					//15f/s
	VIDEO_FRAMERATE_10 = 10,					//10f/s
	VIDEO_FRAMERATE_5 = 5,					//5f/s
	VIDEO_FRAMERATE_2 = 2,					//2f/s
	VIDEO_FRAMERATE_1 = 1,					//1f/s
};

enum HIST_LOOK_TYPE
{
	HISTORY_LOOK_NORMAL = 0,			//正常请求看历史视频
	HISTORY_LOOK_FORCE,				//强制看历史
};

enum HIST_DOWNLOAD_TYPE
{
	HISTORY_DOWNLOAD_NORMAL = 0,			//正常请求下载历史视频
	HISTORY_DOWNLOAD_FORCE,					//强制请求下载历史
};

enum HIST_LOOK_REPONSE
{
	HISTORY_LOOK_RESPONSE_SUCCESS = 0,			//请求成功
	HISTORY_LOOK_RESPONSE_ALREADY_OPEN,			//已经有人在看历史（不一定是同一camera）
    HISTORY_LOOK_RESPONSE_HASREALTIME,			//有人在看实时视频
	HISTORY_LOOK_RESPONSE_HASDOWNLOAD,			//有人在下载视频
	HISTORY_LOOK_RESPONSE_VS_UNREACHABLE,		//视频服务器连接失败
	HISTORY_LOOK_RESPONSE_HASALARM,				//有报警视频
};

enum CLOSE_VIDEO_REASON
{
	CLOSE_VIDEO_NORMAL = 0,			 //正常关闭
	CLOSE_VIDEO_HAS_HISTORY,		 //有人要看历史
	CLOSE_VIDEO_HAS_DOWNLOAD,		 //有人要下载历史
	CLOSE_VIDEO_HAS_ALARM,			//有报警视频
};

enum PUSH_DATA_RESPONSE
{
	PUSH_DATA_RESPONSE_NORMAL = 0,			 //正常关闭
	PUSH_DATA_RESPONSE_HAS_HISTORY,		 //有人要看历史
	PUSH_DATA_RESPONSE_HAS_DOWNLOAD,		 //有人要下载历史
	PUSH_DATA_RESPONSE_HAS_ALARM,			//有报警视频
};

enum CONNECT_VIDEO_RESPONSE
{
	CONNECT_VIDEO_RESPONSE_SUCCESS = 0,			 //成功
	CONNECT_VIDEO_RESPONSE_MORE_SONNODE,		 //子节点太多了，
	CONNECT_VIDEO_RESPONSE_NOCAMERA,		 //camera不存在
	CONNECT_VIDEO_RESPONSE_HAS_HISTORY,		//有人在看历史视频
	CONNECT_VIDEO_RESPONSE_HAS_DOWNLOAD,	//有人在下载视频
	CONNECT_VIDEO_RESPONSE_HAS_ALARM,	//有报警视频
};

enum ULUCU_DEVICE_TYPE
{
	DEVICE_TYPE_ULUCU = 0,
	DEVICE_TYPE_HAIS = 10,
	DEVICE_TYPE_FUHAN = 20,
	DEVICE_TYPE_HAIK = 30,
	DEVICE_TYPE_DAHUA = 40,
	DEVICE_TYPE_WEIQ = 50,	//by
	DEVICE_TYPE_HANB = 60,	//by 20120420
	DEVICE_TYPE_SZ100 = 70,	//by MZW 20120420
};

enum ULUCU_HTTP_FUNC_TYPE
{
	HTTP_QUERY_MEM = 0,		//空间询问
	HTTP_FILE_UP,			//文件上传
	HTTP_FILE_DOWN,			//文件下载
};

//////////////////////////////////////////////////////////////////////////
//by 20121225
//msg_header结构体的ver配置
//by 20130530 扩充版本数量
enum ULUCU_VERSION_TYPE
{
	SZ_100 = 100,
	PC_ENP_CLIENT = 200,	//企业版PC端	
	PC_HOME_CLIENT = 201,	//家庭版PC端
	WEB_CLIENT = 250,

	IOS_CLIENT = 300,
	IOS_CLIENT_2 = 301,
	IOS_CLIENT_3 = 302,
	IOS_CLIENT_4 = 303,
	IOS_CLIENT_5 = 304,
	IOS_CLIENT_6 = 305,
	IOS_CLIENT_7 = 306,
	IOS_CLIENT_8 = 307,
	IOS_CLIENT_9 = 308,
	IOS_CLIENT_10 = 309,

	ANDROID_CLIENT = 350,
	ANDROID_CLIENT_2 = 351,
	ANDROID_CLIENT_3 = 352,
	ANDROID_CLIENT_4 = 353,
	ANDROID_CLIENT_5 = 354,
	ANDROID_CLIENT_6 = 355,
	ANDROID_CLIENT_7 = 356,
	ANDROID_CLIENT_8 = 357,
	ANDROID_CLIENT_9 = 358,
	ANDROID_CLIENT_10 = 359,

	IPAD_CLIENT = 400,
	IPAD_CLIENT_2 = 401,
	IPAD_CLIENT_3 = 402,
	IPAD_CLIENT_4 = 403,
	IPAD_CLIENT_5 = 404,
	IPAD_CLIENT_6 = 405,
	IPAD_CLIENT_7 = 406,
	IPAD_CLIENT_8 = 407,
	IPAD_CLIENT_9 = 408,
	IPAD_CLIENT_10 = 409,
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//by 20121226
//文件类型
//////////////////////////////////////////////////////////////////////////

#pragma pack(4)
///通讯中用到基本结构体
typedef struct active_member_s
{
	uint32 user_id;
	uint32 right_mask;
	uint32 manage_server_ip;
}active_member_s;

typedef struct active_camera_s
{
	uint32 camera_id;
	uint32 visible_level;
	uint32 manage_server_ip;
}active_camera_s;

typedef struct msg_p2p_s
{
	uint32 client_id;
	uint32 device_id;
	uint32 session[4];	//用于局域网登陆
}msg_p2p_client_2_device_signin;

typedef struct custom_s
{
	uint32 custom_id;
	char br_name[64];
}custom_s;

typedef struct client_s
{
	uint32 client_id;					//节点标识
	uint32 manager_server_ip;			//所连接的管理服务器ip
	uint16 peer_type;
	uint16 net_type;
}client_s;

typedef struct camera_s
{
	uint32 camera_id;					//camera标识id
	uint32 group_id;					//camera所属分组id
	uint32	online;								//camera在线状态
	uint32 manage_server_ip;			//所在管理服务器地址
	uint16 manage_server_port;		//所在管理服务器端口
	uint32 status_server_ip;			//所在状态服务器地址
	uint16 status_server_port;		//所在状态服务器端口
	uint32 video_server_ip;			//所在视频中转服务器地址
	uint16 video_server_port;		//所在视频服务器端口
	uint16 visible;							//可见性
	uint16 history_support;						//是否支持历史//是否有硬盘
	uint16 realtime_support;					//是否支持实时
	uint16 ptctrl_support;					//是否支持云台控制
	char camera_name[MAX_CAMERA_NAME_SIZE];	//Camera的显示名称
	uint16 pickup_support;					//是否支持实时通话
	uint16 speaker_support;					//是否支持实时通话
	uint16 device_type;//设备类型           use_ipc
}camera_s;

struct query_device_id_req
{
	char device_sn[64];
};

struct query_device_id_ret
{
	uint32 device_id;			//设备ID号
};

struct client_device_query_isadded_req
{ 	
	uint32 custom_id;	    //用户id
	char device_sn[64];    //摄像头uuid
};

struct client_device_query_isadded_ret
{
	uint32 status;   //0未绑定用户，1已被该用户绑定，2被其他用户绑定     
};

struct client_device_query_isadded_ret_v2
{
	uint32 status;  	 //0未绑定用户，1已被该用户绑定，2被其他用户绑定     
	char user_name[64];   //新增：原用户
};

typedef struct camera_group_s
{
	uint32 group_id;					//分组id
	uint32 custom_id;				//
	uint32 parent_id;					//父分组id
	uint16 level;								//分组层级
	char group_name[64];					//分组名称
}camera_group_s;


typedef struct user_s
{
	uint32 user_id;					//用户标识id
	uint32 depart_id;					//所属部门id
	uint32 custom_id;				//
	uint32 right_mask;				//用户拥有的权限
	char user_name[32];						//用户名
	char real_name[64];						//真实名称
	char nick_name[32];						//用户昵称
}user_s;

typedef struct right_s
{
	char right_name[96];
}right_s;



typedef struct admin_s
{
	uint32 admin_id;					//请求的管理员标识id
	uint32 manage_server_ip;			//请求管理员所在管理服务器的ip
}admin_s;

typedef struct _PROBE
{
	int packet_id;
	unsigned int send_time;
	unsigned int recv_time;
	int is_send;
	char data[1024];
}PROBE;


typedef struct _ISPTYPE
{
	unsigned int nettype;
	char ipstr[20];
}ISPTYPE;

//by 通知客户端历史视频的最大帧最小帧
typedef struct camera_seq_param_s
{
	uint32 i_min_seq;					//请求的管理员标识id
	uint32 i_max_seq;			//请求管理员所在管理服务器的ip
}camera_seq_param_s;

typedef struct tagUUFont
{
	uint32				m_clrColor;				//字体颜色
	uint16				m_bSize;				//字体大小
	uint16				m_bStyle;				//字体风格
	char				m_szName[LF_FACESIZE];	//字体名称
}font_s;


typedef struct  tag_alarm_record_s
{
	uint32    id;
	uint32 	 camera_id;
	uint32    alarm_type;
	uint32	 alarm_level;
	char	 alarm_msg[32];
	char	 alarm_time[32];
	uint32	 has_process;
	char	 process_time[32];
	uint32	 process_id;
	char	 process_result[64];
}alarm_record_s;

//by 20120710 回送给camera的接收报告
struct msg_p2p_data_report
{
	uint32 camera_id;
	uint32 father_id;	//发送数据方
	uint32 start_block_seq; //开始记录的小包号
	uint32 data_report;     //32个小包的接收状态
};

#define PACKET_HEAD_FLAG 0xFFEEDDCC

//消息类型定义
enum msgType
{
	MSG_BEGIN =0,

	KEEP_ALIVE_MSG =1,
	ERROR_MSG =2,
	KEEP_ALIVE_ACK_MSG =3,
	KEEP_ALIVE_CLUSTER_MSG= 4,

	SERVER_BROADCAST_ALIVE_MSG=500,
	SERVER_FORWARD_MSG        =501,
	///登录消息
	/***************************************************************************/
	MSG_LOGIN_BEGIN = 10000,
	
	CLIENT_LOGIN_MSG =10001,
	CAMERA_LOGIN_MSG =10002,
	LOGIN_CHALLENGE_MSG =10003,
	LOGIN_CHALLENGE_ACK_MSG =10004,
	DEVICE_LOGIN_MSG =10005,
	DEVICE_LOGIN_NEW_MSG =10006,

	//////////////////////////////////////////////////////////////////////////
	//by 20121227 
	//新版本提醒消息
	QUERY_NEW_VERSION_MSG =10007,
	QUERY_NEW_VERSION_ACK_MSG =10008,
	//////////////////////////////////////////////////////////////////////////

	/*************login server and status server ****************/
	QUERY_LOGIN_SERVER_MSG = 10009,
	LOGIN_DI_SERVER_MSG = 10010,
	CLIENT_REGISTER_MSG = 10011,
	SEND_VERIFYCODE_MSG = 10012,
	RESET_PASSWORD_MSG = 10013,
	RESET_PASSWORD_BYVERIFY_MSG = 10014,
	QUERY_IPC_ONLINE_MSG = 10015,
	ADD_TERMINAL_DEVICE_MSG = 10016,
	QUERY_TERMINAL_DEVICE_MSG = 10017,
	ADD_TERMINAL_EXCEPTION_MSG = 10018,
	CLIENT_DEVICE_ADD_MSG = 10019,
	CLIENT_DEVICE_QUERY_MSG =10020,
	CLIENT_DEVICE_QUERY_2_MSG =10021,
	QUERY_APP_VERSION_MSG = 10022,
	CLIENT_DEVICE_UNBUNDLE_MSG = 10023,
	CLIENT_DEVICE_ID = 10024,
	CLIENT_DEVICE_ADD_2_MSG = 10025,
	CLIENT_DEVICE_QUERY_ISADDED_MSG = 10027,
	CLIENT_APPLY_TEST_ACCOUNT_MSG= 10028,
	CLIENT_APPLY_TEST_ACCOUNT_CTL_MSG = 10029,
	CLIENT_APPLY_SALT_MSG  =  10030,
	CLIENT_REGISTER_CHECK_MSG = 10031,
	CLIENT_MOD_DEVICENAME_MSG = 10032,

	CLIENT_CHECK_VERIFYCODE_MSG = 10038,
	/****************************************************************************/


	MSG_MGR_SERVER_BEGIN = 20000,
	/******************************在线状态服务器与管理服务器之间消息************/


	II_MGR_CLIENT_ONLINE_MSG = 20001,
	II_MGR_CLIENT_OFFLINE_MSG = 20002,
	II_MGR_CAMERA_ONLINE_MSG =20003,
	II_MGR_CAMERA_OFFLINE_MSG = 20004,
	II_MGR_GET_PARTNER_LIST_MSG = 20005,
	II_MGR_ACTIVE_CAMERA_LIST_MSG = 20006,
	II_MGR_ACTIVE_USER_LIST_MSG = 20007,
	II_MGR_CLIENT_ADD_TO_CAMERA_GROUP_MSG = 20008,
	II_MGR_CLIENT_DEL_FROM_CAMERA_GROUP_MSG = 20009,
	II_MGR_GET_PARTNER_LIST_COUNT_MSG = 20010,
	II_MGR_UPDATE_MANAGE_SERVER_ADDR_MSG = 20011,
	II_MGR_QUERY_ADMIN_MSG = 20012,
	II_MGR_JOIN_DEPARTMENT_MSG = 20013,
	II_MGR_AUTH_OF_CAMERA_FOR_USER_MSG = 20014,
	II_MGR_CHECK_RIGHT_OF_USER_FOR_CAMERA_MSG = 20015,
	II_MGR_MGR_ONLINE_MSG = 20016,
	II_LOGIN_QUERY_SERVER_MSG = 20017,
	MSG_II_MGR_END = 20018,
	/***************************************************************************/


	/****************************节点与管理服务器之间的通讯消息 ***************/

	PEER_MGR_SIGN_IN_MSG = 20200,						//签入
	CLIENT_MGR_SIGN_OUT_MSG = 20201,

	PEER_MGR_PARTNERS_MSG = 20202,					//伙伴节点ID
	PEER_MGR_P2P_CALL_MSG = 20203,
	PEER_MGR_P2P_CALL_ACK_MSG = 20204,

	CLIENT_MGR_PARTNER_LIST_COUNT_MSG = 20205,		//获取摄像头的观看信息(人数信息)


	CLIENT_MGR_CAMERA_OFFLINE_MSG = 20206,
	CLIENT_MGR_CAMERA_ONLINE_MSG = 20207,
	CLIENT_MGR_CLIENT_OFFLINE_MSG = 20208,
	CLIENT_MGR_CLIENT_ONLINE_MSG = 20209,
	CLIENT_MGR_ACTIVE_CAMERA_LIST_MSG = 20210,	//得到在线的camera信息
	CLIENT_MGR_ACTIVE_USER_LIST_MSG = 20211,
	CLIENT_MGR_JOIN_DEPARTMENT_MSG = 20212,
	CLIENT_MGR_JOIN_DEPARTMENT_ACK_MSG = 20213,
	CLIENT_MGR_MEMBER_JOIN_DEPARTMENT_BROADCAST_MSG = 20214,
	CLIENT_MGR_REQUEST_AUTH_OF_CAMERA_MSG = 20215,
	CLIENT_MGR_REQUEST_AUTH_OF_CAMERA_ACK_MSG = 20216,
	CLIENT_MGR_ADD_FRIEND_COMPANY_MSG = 20217,
	CLIENT_MGR_ADD_FRIEND_COMPANY_ACK_MSG = 20218,
	CLIENT_MGR_STOP_LOOK_CAMERA_MSG = 20219,
	CAMERA_MGR_SIGN_IN = 20220,
	CLIENT_MGR_START_LOOK_CAMERA_MSG = 20221,

	WANT_LOOK_RT_VIDEO_MSG = 20222, //观看实时视频协商协议
	WANT_LOOK_RT_VIDEO_ACK_MSG = 20223,//观看实时视频协商回复协议

	PEER_MGR_SIGN_OUT_MSG = 20224,						//签
	CAMERA_MGR_SIGN_OUT = 20225,

	GET_HIST_VIDEO_FILE_LIST_MSG = 20226, //获取历史视频文件列表
	HIST_VIDEO_FILE_LIST_MSG = 20227, //历史视频文件列表
	WANT_LOOK_HIST_VIDEO_MSG = 20228, //观看历史视频协商协议
	WANT_LOOK_HIST_VIDEO_ACK_MSG = 20229,//观看历史视频协商回复协议
	PT_MOVE_LEFT_MSG = 20230,//云台左移
	PT_MOVE_RIGHT_MSG = 20231,//云台右移
	PT_MOVE_UP_MSG = 20232,//云台上移
	PT_MOVE_DOWN_MSG = 20233,//云台下移
	PT_FOCUS_NEAR_MSG = 20234,//近焦
	PT_FOCUS_FAR_MSG = 20235,//远焦
	PT_MOVE_STOP_MSG = 20236, //停止移动
	MGR_CLIENT_VD_HIST_START_LOOK_VIDEO_MSG = 20237,//告诉中继服务器做好观看历史视频准备
	MGR_CLIENT_VD_HIST_STOP_LOOK_VIDEO_MSG = 20238,//告诉中继停止观看历史视频
	//MGR_CAMERA_VD_ASK_PUSH_HIST_VIDEO_MSG = 20239,
 	HIST_VIDEO_CAMERA_ACK_MSG = 20240,
	MGR_CLIENT_VD_HIST_CONTROL_VIDEO_MSG = 20241,
	///////For download file
	WANT_DOWNLOAD_HIST_FILE_MSG = 20242,
	WANT_DOWNLOAD_HIST_FILE_ACK_MSG = 20243,
	/////////For security center 
	MGR_GET_CAMERA_NETINFO_MSG = 20244,
	MGR_GET_CAMERA_NETINFO_ACK_MSG = 20245,
	MGR_SET_CAMERA_NET_ISPTYPE_MSG = 20246,
	MGR_GET_DEVICE_INFO_MSG = 20247,
	MGR_GET_DEVICE_INFO_ACK_MSG = 20248,
	CLIENT_MGR_SET_LM_PARAM=20249,
	MGR_GET_MOTION_DETECTION_PARAM_MSG = 20250,
	MGR_GET_MOTION_DETECTION_PARAM_ACK_MSG =20251,
	MGR_SET_MOTION_DETECTION_PARAM_MSG = 20252,
	MGR_GET_LOST_VIDEO_PARAM_MSG = 20253,
	MGR_GET_LOST_VIDEO_PARAM_ACK_MSG = 20254,
	MGR_SET_LOST_VIDEO_PARAM_MSG = 20255,
	MGR_GET_MASK_VIDEO_PARAM_MSG = 20256,
	MGR_GET_MASK_VIDEO_PARAM_ACK_MSG = 20257,
	MGR_SET_MASK_VIDEO_PARAM_MSG = 20258,
	MGR_SET_PRESET_POINT_MSG = 20259,
	MGR_SET_CRUISE_PATH_MSG = 20260,
	MGR_GET_CRUISE_PATH_MSG = 20261,
	MGR_GET_CRUISE_PATH_ACK_MSG = 20262,
	MGR_START_CRUISE_PATH_MSG = 20263,
	MGR_STOP_CRUISE_PATH_MSG = 20264,
	MGR_SET_YUANTAI_MODE_MSG = 20265,
	MGR_GET_YUANTAI_MODE_MSG = 20266,
	MGR_GET_YUANTAI_MODE_ACK_MSG = 20267,
	MGR_GET_SYSTEM_TIME_MSG =20268,
	MGR_GET_SYSTEM_TIME_ACK_MSG = 20269,
	//MGR_SET_ALARM_UPLOAD_MSG = 20270,
	
	MGR_SET_REALTIME_STREAM_MSG = 20271,
	MGR_GET_REALTIME_STREAM_MSG = 20272,
	MGR_GET_REALTIME_STREAM_ACK_MSG = 20273,

	MGR_CAMERA_REBOOT_MSG = 20274,

	MGR_GET_RUNTIME_AUDIO_MSG = 20275,
	MGR_GET_RUNTIME_AUDIO_ACK_MSG = 20276,
	MGR_SET_RUNTIME_AUDIO_MSG   = 20277,
	////////////////////////////////////////////
	//Add messages for vca purpose
	//////////////////////////////////////////////
	CAMERA_2_MGR_VCA_DATA_REPORT = 20278,
	CAMERA_2_MGR_VCA_DATA_REPORT_ACK = 20279,
	
	DEVICE_MGR_SIGN_IN = 20280,
	DEVICE_MGR_SIGN_OUT = 20281,
	
	CLIENT_MGR_GET_LM_PARAM  =20282,
	CLIENT_MGR_LM_PARAM_ACK_MSG  =20283,
	MGR_STOP_TALK_MSG  =20284,
	/////////////////////////////////////////////////////
	//switch alarm param 
	/////////////////////////////////////////////////////
	MGR_SET_SWITCH_ALARM_PARAM_MSG  =20285,
	MGR_GET_SWITCH_ALARM_PARAM_MSG  =20286,
	MGR_GET_SWITCH_ALARM_PARAM_ACK_MSG  =20287,
	MGR_SET_SWITCH_DEVICE_MSG  =20288,
	MGR_GET_SWITCH_DEVICE_PARAM_MSG  =20289,
	MGR_GET_SWITCH_DEVICE_PARAM_ACK_MSG  =20290,
	/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//by 20121212
	CLIENT_2_MGR_SNAPSHOT_MSG =20291,
	MGR_2_CAMERA_SNAPSHOT_MSG =20292,
	CAMERA_2_HTTP_QUERY_FILE_UP_MSG =20293,
	HTTP_2_CAMERA_QUERY_FILE_UP_ACK_MSG =20294,
	CAMERA_2_MGR_FILE_UP_ACK_MSG =20295,
	MGR_2_CLIENT_SNAPSHOT_ACK_MSG =20296,
	CLIENT_MGR_GET_CLOUD_HOME_FILE_LIST_ACK_MSG =20297,
	CLIENT_MGR_SET_CAMERA_TIMING_RECORD_PARA_MSG =20298,
	//CLIENT_MGR_SET_CAMERA_ALARM_NOTE_MSG =20299,
	//CLIENT_MGR_GET_CAMERA_ALARM_NOTE_MSG =20300,
	CLIENT_MGR_SET_CAMERA_CRUISE_MODE_MSG =20301,

	PEER_NEW_MGR_P2P_CALL_MSG =20302,

	CLIENT_MGR_SET_CAMERA_RECORD_MSG =20303,

	//////////////////////////////////////////////////////////////////////////
	//alarm msg
	CLIENT_MGR_SET_CAMERA_ALARM_NOTE_MSG =20304,
	CLIENT_MGR_GET_CAMERA_ALARM_NOTE_MSG =20305,
	CLIENT_MGR_GET_CAMERA_ALARM_NOTE_ACK_MSG =20306,
	
	MGR_GET_USER_DISCONN_DEVICE_MSG = 20309,

	MGR_CAMERA_ALARM_ACK_MSG = 20310,

	PEER_MGR_P2P_CALL_LOCAL_MSG =20311,
	PEER_MGR_P2P_CALL_LOCAL_ACK_MSG =20312,
	UPDATE_MOBILE_INVITE_INFO =20313,
	UPDATE_MOBILE_INVITE_ACK =20314,
	UPDATE_DEVICE_FTP_INFO =20315,
	UPDATE_DEVICE_FTP_INFO_ACK =20316,

	CLIENT_MGR_SET_VIDEO_PARAM = 20317,
	
	CAMERA_ALARM_INFO_UPLOAD_MSG =20319,	
	CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG =20320,
	CLIENT_ACK_SERVER_ALARM_INFO_MSG =20321,
	CLIENT_MGR_GET_CAMERA_ALARM_DATA_MSG =20322,
	CAMERA_ALARM_DATA_UPLOAD_MSG =20323,
	CLIENT_MGR_GET_VOL_PARAM = 20324,
	CLIENT_MGR_VOL_PARAM = 20325,
    CLIENT_MGR_GET_DEVICE_MDATA = 20326,
	CLIENT_MGR_DEVICE_MDATA = 20327,
	MGR_DEVICE_RELOGIN = 20328,
	CLIENT_MGR_SET_VOL_PARAM = 20329,


	//////////////////////////////////////////////////////////////////////////

	I_CHAT_CONTENT_MSG = 20400,							//聊天内容消息
	I_CHAT_SHAKE_MSG = 20401,							//聊天震动消息

	/****************************************************************************/




	/*******************************管理服务器之间通讯消息 **********************/
	MGR_MGR_P2P_CALL_MSG = 20800,
	MGR_MGR_P2P_CALL_ACK_MSG = 20801,
	MGR_MGR_CAMERA_OFFLINE_MSG= 20802,
	MGR_MGR_CLIENT_OFFLINE_MSG= 20803,
	MGR_MGR_CAMERA_ONLINE_MSG= 20804,
	MGR_MGR_CLIENT_ONLINE_MSG= 20805,
	MGR_MGR_ASK_CLIENT_CONNECT_MSG= 20806,
	MGR_MGR_JOIN_DEPARTMENT_MSG= 20807,
	MGR_MGR_JOIN_DEPARTMENT_ACK_MSG= 20808,
	MGR_MGR_JOIN_DEPARTMENT_BROADCAST_MSG= 20809,
	MGR_MGR_REQUEST_AUTH_OF_CAMERA_MSG= 20810,
	MGR_MGR_REQUEST_AUTH_OF_CAMERA_ACK_MSG= 20811,
	MGR_MGR_ADD_FRIEND_COMPANY_MSG= 20812,
	MGR_MGR_ADD_FRIEND_COMPANY_ACK_MSG= 20813,
	MGR_MGR_START_LOOK_CAMERA_MSG= 20814,
	MGR_MGR_STOP_LOOK_CAMERA_MSG= 20815,

	CAMERA_GET_PICTURE_PARAM =20816,
	CAMERA_GET_PICTURE_PARAM_ACK =20817,
	CAMERA_SET_PICTURE_PARAM =20818,

	CAMERA_GET_VIDEO_PARAM =20819,
	CAMERA_GET_VIDEO_PARAM_ACK =20820,
	CAMERA_SET_VIDEO_PARAM =20821,

	/****************************************************************************/
	MSG_MGR_SERVER_END = 21000,


	/******************************客户端与数据服务器通讯消息********************/
	MSG_DATA_SERVER_BEGIN = 30000,
	CLIENT_DATA_GET_COMPANY_GROUP_CAMERA_INFO_MSG = 30001,
	CLIENT_DATA_GET_COMPANY_GROUPS_MSG = 30002,
	CLIENT_DATA_GET_COMPANY_CAMERAS_MSG = 30003,
	CLIENT_DATA_GET_DEPARTMENT_USER_INFO_MSG = 30004,
	CLIENT_DATA_GET_DEPARTMENTS_MSG = 30005,
	CLIENT_DATA_GET_DEPART_USERS_MSG = 30006,
	CLIENT_DATA_GET_USER_DETAIL_INFO_MSG = 30007,
	CLIENT_DATA_GET_COMPANY_DETAIL_INFO_MSG = 30008,
	CLIENT_DATA_GET_CAMERA_DETAIL_INFO_MSG = 30009,
	CLIENT_DATA_UPDATE_USER_INFO_MSG = 30010,
	CLIENT_DATA_GET_USER_RIGHT_MSG = 30011,
	CLIENT_DATA_GET_FRIEND_COMPANY_MSG = 30012,
	CLIENT_DATA_DEL_EMPLOYEE_MSG = 30013,
	CLIENT_DATA_DEL_FRIEND_COMPANY_MSG = 30014,
	CLIENT_DATA_GET_AUTH_CAMERAS_MSG = 30015,
	CLIENT_DATA_SEARCH_COMPANY_MSG = 30016,
	CLIENT_DATA_GET_RECOMMEND_COMPANY_MSG = 30017,
	CLIENT_DATA_GET_CUSTOMER_LIST_MSG = 30018,
	CLIENT_DATA_GET_OTHER_COMPANY_GROUPS_MSG = 30019,
	CLIENT_DATA_GET_OTHER_COMPANY_CAMERAS_MSG = 30020,
	CLIENT_DATA_GET_OTHER_COMPANY_GROUP_CAMERA_INFO_MSG =30021,
	CLIENT_DATA_GET_UNPROCESS_ALARM_MSG = 30022,
	CLIENT_DATA_GET_UNPROCESS_ALARM_ACK_MSG = 30023,
	CLIENT_DATA_GET_ALL_ALARM_MSG = 30024,
	CLIENT_DATA_GET_ALL_ALARM_ACK_MSG = 30025,
	CLIENT_DATA_UPDATE_ALARM_PROCESSED_MSG = 30026,
	CLIENT_DATA_GET_SECURITY_CENTER_CAMERA_MSG = 30027,
	CLIENT_DATA_GET_SECURITY_CENTER_CAMERA_ACK_MSG = 30028,
	CLIENT_2_INFOSVR_QUERY_VCA_DATA = 30029,
	CLIENT_DATA_GET_COMPANY_EVENT = 30030,
	CLIENT_DATA_GET_COMPANY_EVENT_ACK = 30031,
	
	//by 20130529 infoserver
	CLIENT_DATA_GET_LOG_CONTENT_MSG =30051, //获得最后一次操作记录
	CLIENT_DATA_GET_LOG_CONTENT_MSG_ACK = 30052,
    CLIENT_INFO_SET_CAMERA_ALARM_NOTE_MSG = 30053,
    CLIENT_INFO_GET_CAMERA_ALARM_NOTE_MSG = 30054,
    CLIENT_INFO_GET_CAMERA_ALARM_NOTE_MSG_ACK = 30055,

	//
	MSG_DATA_SERVER_END = 30056,
	/****************************************************************************/

	P2P_BEGIN = 40000,

	P2P_STUN_MSG = 40001,
	P2P_STUN_ACK_MSG = 40002,
	P2P_INIT_PARAM_MSG = 40003,
	P2P_ASK_DATA_MSG =40004,
	P2P_DATA_MSG = 40005,
	P2P_ASK_BUFFERMAP_MSG = 40006,
	P2P_BUFFERMAP_MSG = 40007,
	P2P_ASK_PUSHDATA = 40008,
	P2P_STOP_PUSHDATA = 40009,
	P2P_STUN_ACK_ACK_MSG = 40010,
	P2P_KEEP_ALIVE_MSG	= 40011,
	P2P_KEEP_ALIVE_ACK_MSG	= 40012,
	P2P_CONNECT_VIDEO_MSG	= 40013,
	P2P_CONNECT_VIDEO_ACK_MSG	= 40014,
	P2P_CLOSE_VIDEO_MSG	= 40015,
	P2P_GET_INIT_PARAM_MSG = 40016,
	P2P_HIST_INIT_PARAM_MSG = 40017,
	P2P_ASK_HIST_DATA_MSG =40018,
	P2P_HIST_DATA_MSG = 40019,
	P2P_DATA_ACK_MSG = 40020,
	P2P_ASK_MULTIPLE_DATA_MSG = 40021,

	P2P_ASK_PUSHDATA_ACK = 40022,
	P2P_AUDIO_DATA_MSG = 40023,
	
	P2P_ASK_MULTIPLE_HIST_DATA_MSG = 40024,

	P2P_DATA_REPORT = 40026,		//by 20120710 回送给camera的接收报告
	P2P_SET_PUB_AUDIO_MSG = 40027,
	P2P_CLIENT_DEVICE_SIGNIN = 40028,

	P2P_END = 40030,

	/***************************************************************************/
	PEER_PUNCH_GET_PUBLIC_ADDR = 50000,
	UDP_EXPLORESERVER = 50001,
	UDP_EXPLORESERVER_ACK = 50002,
	STATE_RESPONSE_MSG = 51000,
	NET_TYPE_PROBE_MSG = 52000,

	PEER_VD_BEGIN_MSG = 60000,
	/***************************************************************************/
	PEER_VD_SIGN_IN_MSG = 60001,
	CLIENT_VD_RT_START_LOOK_CAMERA_MSG = 60002,//客户端告诉中继做好观看实时视频准备
	CLIENT_VD_RT_STOP_LOOK_CAMREA_MSG = 60003,//告诉中继停止观看实时视频
	CAMERA_VD_ASK_PUSH_VIDEO_MSG = 60004,//中继要camera推送实时视频流
	CAMERA_VD_ASK_STOP_VIDEO_MSG = 60005,//中继让camera停止发送视频
 	CLIENT_VD_HIST_START_LOOK_VIDEO_MSG = 60006,//告诉中继服务器做好观看历史视频准备
 	CLIENT_VD_HIST_STOP_LOOK_VIDEO_MSG = 60007,//告诉中继停止观看历史视频
 	CAMERA_VD_ASK_PUSH_HIST_VIDEO_MSG = 60008,//中继要求camera发送历史视频流
 	CAMERA_VD_ASK_STOP_HIST_VIDEO_MSG = 60009,//中继要求camera停发历史视频流
 	CLIENT_VD_PAUSE_PUSH_VIDEO_MSG = 60010,//中继要求camera暂停发送实时视频
	CLIENT_VD_RESTART_PUSH_VIDEO_MSG = 60011,//中继要求camera重新发送实时视频
	HIST_VIDEO_MSG = 60012,//历史视频数据流
	CLIENT_VD_CAMERA_RT_HELLO_MSG = 60013,
	HIST_INIT_PARAM_MSG		=  60014,        // Init param for history video
	HIST_SKIP_VIDEO_MSG = 60015,
	/***************************************************************************/
	/************ For download  hist file*************************/
	CAMERA_VD_DOWNLOAD_HIST_DATA_MSG  = 60016,
	CAMERA_VD_DOWNLOAD_HIST_DATA_ACK_MSG  = 60017,
	CAMERA_VD_DOWNLOAD_HIST_FILE_FINISHED_MSG	= 60018,
	
    PEER_VD_SIGN_OUT_MSG = 60019,

	PEER_VD_END_MSG = 60020,

//	CAMERA_SEQ_PARAM_MSG = 60019,

	ID_BEGIN_MSG = 70000,
	PEER_GET_RAND_ID_MSG = 70001,

	ID_END_MSG = 70008,

	CAMERA_CONTROL_BEGIN_MSG = 71000,
	CAMERA_REBOOT_MSG = 71001,
	CAMERA_REMOTE_LOG_START_MSG = 71002,
	CAMERA_REMOTE_LOG_STOP_MSG = 71003,
	CAMERA_CONTROL_END_MSG = 71004,

	//track
	TK_BEGIN_MSG = 80000,
	/**************************************************************************/
	CLIENT_TK_START_LOOK_CAMERA_MSG = 80001,
	CLIENT_TK_STOP_LOOK_CAMERA_MSG = 80002,
	CLIENT_TK_GET_PARTNERS_MSG = 80003,
	CLIENT_TK_GET_PARTNER_COUNT_MSG = 80004,
	CLIENT_TK_P2P_CALL_MSG = 80005,
	CLIENT_TK_P2P_CALL_ACK_MSG = 80006,
	/***************************************************************************/
	TK_END_MSG = 80007,
	MGR_OUTLINE = 80008,

	MSG_END
};

///通讯包头，包体的定义

typedef struct msg_header
{
	uint32 tag;						//包头标志
	uint32 cmd_id;					//包的类型id
	uint32 ver;						//协议版本号
	uint32 len;						//包的长度
	uint32 state;					//用在返回包，标示服务器对客户端的各种响应状态
}msg_header;

struct msg_status
{
	int32 err_code;
	char err_msg[128];
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//client与camera和登录服务器
//client与camera和登录服务器采用tcp的短连接

struct client_mgr_set_lm_param
{
	uint32 device_id;
	uint32 user_id;
	uint32 luminous;   //0--100
	uint32 reserver;
};


struct msg_keep_alive
{
	uint32 peer_id;
	uint32 camera_id;	//by
};

struct msg_keep_alive_ack
{
	uint32 src_peerid;
	uint32 camera_id;	//by
};

// msg between login and status ; 
struct msg_login_server_info
{
	int magic;
	int mgrs;
	
};

//register ;
struct client_register_2_login
{
	char user_name[64];
	char pass[64];
	char verifycode[64];    //验证码
	char device_sn[64];
	uint16 user_type;  //用户类型
};

struct client_register_check_ret
{
	uint32 status;  //1存在;0不存在
};

//
struct login_2_client_reg_ret
{
        uint32 device_id;
        uint32 camera_id;
};

//online query;
struct client_online_query_2_login
{
	uint32 custom_id;
	uint32 user_id;
	uint32 device_id;
	uint32 camera_id;
};


///客户端登录服务器的请求包

struct login_challenge_ack
{
	char	challenge[20];
};

struct client_2_login_req
{
	char user_name[64];
	char pass[64];
	uint32 net_type;
	uint32 padding; // 0:Android   1: ios
};

struct client_2_login_req_v2
{
	char user_name[64];
	char pass[128];
	uint32 net_type;
	char session_id[128];
	uint32 mgrsvr_addr;
	uint32 public_cnc_ip;
	uint32 public_tel_ip;
};

struct login_2_client_ret_ver101
{
        uint32 user_id;
        uint32 custom_id;
};

///客户端登录服务器的返回包
struct login_2_client_ret
{
	uint32 user_id;
	uint32 manage_server_ip;
	uint16 manage_server_port;
	uint32 custom_id;
	uint32 right_mask;//权限表
	uint16 custom_type;
	uint16 net_type; //网络类型，比如电信，网通，长宽等
	char session_id[128];
	uint32 is_admin;
};

//QUERY_NEW_VERSION_MSG
struct query_new_version_ret
{
	//by 版本更新提示
	char ver_NO[20];
	char ver_detail[200];
};

///客户端登录服务器的返回包
struct login_2_client_ver_ret
{
	uint32 user_id;
	uint32 manage_server_ip;
	uint16 manage_server_port;
	uint32 custom_id;
	uint32 right_mask;//权限表
	uint16 custom_type;
	uint16 net_type; //网络类型，比如电信，网通，长宽等
	char session_id[128];
	uint32 is_admin;
	//by 版本更新提示
	char ver_NO[20];
	char ver_detail[200];
};

///camera登录服务器的请求包
struct camera_2_login_req
{
	uint32 camera_id;
	uint16 net_type;//本地检测出的网络类型
};

struct device_2_login_req
{
	uint32 device_id;
	uint16 net_type;//本地检测出的网络类型
	char   device_sn[64];
};

///camera登录服务器的请求包
struct login_2_camera_ret
{
	uint32 custom_id;
	uint32 manage_server_ip;
	uint16 manage_server_port;	
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	char session_id[128];
};

//add by chengweishan 2014.11.17 密码恢复功能包体
//请求发验证码
struct send_verifycode_req
{
	char user_name[64];
};

struct client_check_verifycode_req
{
	char user_name[64];
	char verifycode[64];    //验证码
	char notused[32];	   //暂时未用到
};

struct client_check_verifycode_ret
{
	uint32 status;	  //  0:验证码正确   1:验证码不存在  2: 验证码错误 
};

//请求重设密码
struct reset_password_req
{
 	char user_name[64];
 	char oldpass[64];
 	char newpass[64];
};
//根据验证码重设密码请求
struct reset_password_byverify_req
{
	char user_name[64];
 	char verifycode[64];
	char newpass[64];
};

//add by chengweishan 2014.11.19 终端设备包体
//终端设备添加
struct terminal_device_add_req
{ 
	uint32 user_id;
	char dev_uuid[256];
 	char dev_model[256];
 	char dev_osversion[256];
};

//终端设备查询
struct terminal_device_query_req
{ 	
	uint32 user_id;
 	char dev_uuid[256];
};

//终端设备查询返回
struct terminal_device_query_ret
{ 
	uint32 dev_id;
};

//终端设备异常信息添加
struct terminal_exception_add_req
{ 
	uint32 dev_id;
 	char exception[256];
};

//用户添加设备
struct client_device_add_req
{ 	
	uint32 custom_id;	
	char device_sn[64];  //摄像头uuid
	char device_name[64];  // add
	char device_passwd[64];  //add
};

//返回为成功时的包体：
struct client_device_add_ret
{
   uint32 device_id;      //从redis（从camera）
   uint32 camera_id;
};


struct device_sn_name_passwd
{
	char device_sn[12];
	char device_name[64];
	char device_passwd[64];
};
//用户查询设备
struct client_device_query_req
{ 	
	uint32 user_id;	 //moded
};

struct client_device_query_v2_req
{ 	
	uint32 user_id;
	uint32 device_type;	//增加设备类型区分   0:ipc365,1:showmo
};

struct device_id_sn_name_passwd
{
	uint32 device_id;     
	char device_sn[12];    //摄像头uuid
	char device_name[32];  
	char device_passwd[32];
};

//返回为成功时的包体：(device_id)
struct client_device_query_ret
{
    device_id_sn_name_passwd device[0]; 
};
//返回为成功时的包体：(device_sn)
struct client_device_query_2_ret
{ 
	struct device_sn_name_passwd device_info[0];  //华南
};

//用户设备解绑
struct client_device_unbundle_req
{ 	
	uint32 custom_id;	
	char device_sn[64];  //摄像头uuid  
};

//app版本查询
struct query_app_version_req
{ 	
	uint16 user_type;	// 0 表示ipc365; 1表示iup365
};

struct query_app_version_ret
{ 	
	char version[32];
	char feature[256];	
};
//////////////////////////////

//////////////////////////////////////////////////////////////////////////
//http服务器业务类型
struct  http_fuction
{
	uint32 func_type;		//http服务器业务类型
	char http_url[50];		//http服务器页面地址
};
//by 20121214 新的camera登录服务器的请求包
struct login_2_camera_ver_100_ret
{
	uint32 custom_id;
	uint32 manage_server_ip;
	uint32 status_server_ip;
	uint32 video_server_ip;
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	char session_id[128];
	uint32 http_server_ip;		//http服务器ip
	uint16 http_server_port;	//http服务器port
	uint16 func_count;   //设备能接入的camera 数目
	struct http_fuction http_func_list[0];		
};
//////////////////////////////////////////////////////////////////////////

struct  device_camera_port
{
	uint32   camera_id;
	uint32   port_id;
};

struct login_2_device_ret
{
	uint32 custom_id;
	uint32 group_id;			//分组ID号
	uint32 device_id;			//设备ID号
	uint32 manage_server_ip;
	uint32 video_server_ip;
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	uint16 camera_num;   //设备能接入的camera 数目
	struct device_camera_port camera_id_list[0];
};

struct new_login_2_device_ret
{
	uint32 custom_id;
	uint32 group_id;			//分组ID号
	uint32 device_id;			//设备ID号
	uint32 manage_server_ip;
	uint32 video_server_ip;
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	uint16 camera_num;   //设备能接入的camera 数目
	//by 增加文件服务器的上传地址和port
	uint32 file_server_ip;
	uint16 file_server_port;

	struct device_camera_port camera_id_list[0];
};
//////////////////////////////////////////////////////////////////////////////////////////////////////
//在线管理服务器与全局索引信息服务器
//在线管理服务与全局索引信息服务器保持tcp的常连接，在线管理服务器转发客户端向索引信息服务器的请求命令。


///客户端离线
struct mgr_2_ii_client_offline
{
	uint32 custom_id;				//用户所属客户id
	uint32 client_id;					//用户标识id
	uint32 camera_id;					//如果正在观看视频，则该字段有值
	uint32 peer_type;
	uint32 net_type;
};

///客户端上线
struct mgr_2_ii_client_online
{
	uint32 custom_id;				//用户所属客户id
	uint32 client_id;					//用户标识id
	uint32 manage_server_ip;			//客户端所在管理服务器地址
	uint32 right_mask;				//权限
	uint16 net_type;
	uint16 peer_type;
};

struct mgr_2_ii_camera_offline
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//camera标识id
};

struct mgr_2_ii_camera_online
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//camera标识id
	uint32 manage_server_ip;			//camera所在管理服务器地址
	uint32 net_type;
	uint32 group_id;                    //camera所属分组
	uint16 visible_level;						//可见级别
};


///请求在线camera列表
struct mgr_2_ii_active_camera_list
{
	uint32 custom_id;				//id
	uint32 camera_id;					//摄像头id
	uint32 cfd;								//客户端与管理服务器的链接描述符
};

///返回同一个客户下的在线camera列表
struct ii_2_mgr_active_camera_list
{
	uint32 cfd;								//客户端与管理服务器的链接描述符
	uint32 custom_id;				//id
	uint16 camera_count;			//Camera的个数
	active_camera_s camera_arr[0];					//Camera列表数组
};


///请求同一个客户下的在线user列表
struct mgr_2_ii_active_user_list
{
	uint32 cfd;								//客户端与管理服务器的链接描述符
	uint32 custom_id;				//客户id
};

///返回同一个客户下的在线user列表
struct ii_2_mgr_active_user_list
{
	uint32 cfd;								//客户端与管理服务器的链接描述符
	uint32 custom_id;				//客户id
	uint16 user_count;				//Camera的个数
	active_member_s user_arr[0];				//Camera列表数组
};

///获取camera相关的列表的请求包
struct mgr_2_ii_partner_list
{
	uint32 camera_id;					//目的camera_id
	uint32 client_count;				//请求的个数
	uint32 net_type;
	uint32 peer_type;
	uint32 cfd;								//客户端的socket描述符
	uint32 mgr_svr_ip;
};


///请求camera相关的client 列表的返回包
struct ii_2_mgr_partner_list
{
	uint32 cfd;								//客户端的socket描述符
	uint32 camera_custom_id;				//camera所属客户id
	uint32 camera_id;					//相关的camera的标识Id
	uint16 client_count;				//客户端的个数
	client_s client_array[0];				//客户端数组的首地址
};


///加入在线观看camera的分组的请求包
struct mgr_2_ii_client_add_to_camera_group
{
	uint32 camera_id;//希望观看的camera_id
	uint32 client_id;
	uint16 net_type;
	uint16 peer_type;
	uint32 camera_stat_svr_ip;
	uint32 mgr_svr_ip;
};

///从camera分组虫删除的请求包
struct mgr_2_ii_del_client_from_camera_group
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//目的camera_id
	uint32 client_id;					//待删除的clientid
	uint16 net_type;
	uint16 peer_type;
};

///获取正在查看同一个camera的人数的请求包
struct mgr_2_ii_partner_list_count
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//目的camera_id
	uint32 cfd;								//客户端的socket描述符
};

///返回正在查看同一个camera的人数的请求包
struct ii_2_mgr_partner_list_count
{
	uint32 camera_custom_id;				//camera所属客户id
	uint32 camera_id;					//相关的camera的标识Id
	uint32 client_list_count;			//查看同一个camera的人数
	uint32 cfd;								//客户端的socket描述符
};

///删除索引服务器上指定的camera分组
struct mgr_2_ii_del_camera_group
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;				//离线camera_id
};

///在索引服务器上增加一个camera分组
struct mgr_2_ii_add_camera_group
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//上线camera id
	uint32 manage_server_ip;
};

///更新节点在索引服务器中存储的在线管理服务器地址
struct mgr_2_ii_update_manage_server_addr
{
	uint32 new_ip;					//目标服务器ip
	uint16 new_port;				//目标服务器端口
	uint32 client_id;				//Client节点标识
	uint32 camera_id;				//Client正在观看的camera标识
};

///索引服务器中未发现camera分组，则认为camera离线，返回camera离线包给管理服务器
struct ii_2_mgr_camera_offline
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//离线camera的标识id
};


///管理服务器向索引服务器请求客户的特定用户信息
struct mgr_2_ii_query_admin
{
	uint32 custom_id;				//请求的客户的标识id
	uint32 right_mask;				//请求的管理员权限值
};

// mgr to status , inform online ; clm ;
struct mgr_2_status_online
{
	uint32 public_cnc_ip;
	uint32 public_tel_ip;
	uint32 local_ip;
	uint32 padding;
};

///状态服务器返回给管理服务器请求的管理员
struct ii_2_mgr_admin
{
	uint16 count;
	admin_s arr[0];
};


///用户加入部门成功的在状态服务器的注册包
struct mgr_2_ii_join_department
{
	client_s me;
};


///在状态服务器登记为某用户授权成功观看某摄像头的信息包
struct mgr_2_ii_add_auth_of_camera_for_user
{
	uint32 custom_id;				//摄像头所属客户id
	uint32 camera_id;					//请求观看的camera id
	uint32 user_id;					//请求观看的用户id
};

///在状态服务器删除某用户观看某摄像头授权的信息包
struct mgr_2_ii_del_auth_of_camera_for_user
{
	uint32 custom_id;				//摄像头所属客户id
	uint32 camera_id;					//请求观看的camera id
	uint32 admin_id;					//执行删除操作的用户id
	uint32 user_id;					//请求观看的用户id
};

///管理服务器检查用户是否有权限查看摄像头
struct mgr_2_ii_check_right_of_user_for_camera
{
	uint32 custom_id;				//camera所属客户的id
	uint32 camera_id;					//camera id
	uint32 user_id;					//请求用户的id
};

///状态服务器返回给管理服务器该用户是否可以观看摄像头
struct ii_2_mgr_right_of_user_for_camera
{
	uint16 is_can_look;						//是否可看标志
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//客户端与在线管理服务器
///节点（client）登入在线管理服务器包
//CLIENT_MGR_SIGN_IN_MSG,
struct peer_2_mgr_sign_in
{
	uint16 net_type;
	uint16 peer_type;
	uint32 custom_id;				//所属客户id
	uint32 client_id;					//节点的标识id
	uint32 right_mask;
	uint32 company_stat_server_ip;	//所属客户所在状态服务器ip
	char session_id[128];					//验证session标识id
};

struct peer_2_mgr_sign_out
{
	uint16 net_type;
	uint16 peer_type;
	uint32 custom_id;				//所属客户id
	uint32 client_id;					//节点的标识id
	uint32 company_stat_server_ip;	//所属客户所在状态服务器ip
	char session_id[128];					//验证session标识id
};

//获取伙伴节点列表请求包
struct msg_ask_partner_list
{
//	uint32 camera_custom_id;				//camera所属客户id
	uint32 camera_id;					//目的camera_id
	uint32 camera_status_server_ip;	//camera所在的状态服务器地址
	uint16 net_type;
	uint32 partner_count;				//请求的个数
};

///返回包
struct msg_partner_list_ack
{
	uint32 camera_custom_id;				//camera所属客户id
	uint32 camera_id;					//camera id
	uint32 client_count;				//客户端的个数
	client_s client_array[0];				//客户端数组的首地址
};

//通过管理服务器节点之间建立p2p连接的协议包
struct msg_p2p_call
{
	uint16 net_type;			//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;		//对方节点id
	uint32 son_id;	//本节点id
	uint32 son_public_udp_ip;			//自己的公网ip
	uint16 son_public_udp_port;		//自己公网port
	uint32 son_local_ip;				//自己私网ip
	uint16 son_local_port;			//自己私网port

	//以下两个字段用以管理服务器之间消息转发
	uint32 father_manage_server_ip;	//父节点所在的managerserver ip
	uint32 son_manage_server_ip;		//在服务器填写
};

//PEER_MGR_P2P_CALL_LOCAL_MSG
struct msg_p2p_call_local
{
	uint16 net_type;			//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;		//对方节点id
	uint32 son_id;	//本节点id
	uint32 son_public_udp_ip;			//自己的公网ip
	uint16 son_public_udp_port;		//自己公网port
	uint32 son_local_ip;				//自己私网ip
	uint16 son_local_port;			//自己私网port

	//以下两个字段用以管理服务器之间消息转发
	uint32 father_manage_server_ip;	//父节点所在的managerserver ip
	uint32 son_manage_server_ip;		//在服务器填写
};


//PEER_NEW_MGR_P2P_CALL_MSG
struct msg_new_p2p_call
{
	uint16 net_type;			//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;		//对方节点id
	uint32 son_id;	//本节点id
	uint32 son_public_udp_ip;			//自己的公网ip
	uint16 son_public_udp_port;		//自己公网port
	uint32 son_local_ip;				//自己私网ip
	uint16 son_local_port;			//自己私网port

	//以下两个字段用以管理服务器之间消息转发
	uint32 father_manage_server_ip;	//父节点所在的managerserver ip
	uint32 son_manage_server_ip;		//在服务器填写
	uint32 nat_type;	//路由器类型
};

//让目标节点向自己打洞的返回包
struct msg_p2p_call_ack
{
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;					//对方节点id
	uint32 son_id;					//本节点id
	uint32 father_public_udp_ip;		//对方的公网ip
	uint16 father_public_udp_port;	//对方公网port
	uint32 father_local_ip;			//对方私网ip
	uint16 father_local_port;		//对方私网port
	uint32 son_manage_server_ip;		//son 节点所在的manage server ip
	uint32 father_manage_server_ip; //在服务器填写
};

//PEER_MGR_P2P_CALL_LOCAL_ACK_MSG =20312
struct msg_p2p_call_local_ack
{
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;					//对方节点id
	uint32 son_id;					//本节点id
	uint32 father_public_udp_ip;		//对方的公网ip
	uint16 father_public_udp_port;	//对方公网port
	uint32 father_local_ip;			//对方私网ip
	uint16 father_local_port;		//对方私网port
	uint32 son_manage_server_ip;		//son 节点所在的manage server ip
	uint32 father_manage_server_ip; //在服务器填写
	
	uint32 session[4];	//用于局域网登陆
};

struct client_mod_devicename_req
{ 
	uint32 user_id; 
	uint32 device_id;
	char   device_name[32]; 
};



/*----------------------------------------------------------------------*/

///通知相关客户端camera离线的包
struct mgr_2_client_camera_offline
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//离线camera id
};

///通知相关客户端camera上线的包
struct mgr_2_client_camera_online
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;				//上线camera id
};
///通知同属一个客户的客户端client离线的包
struct mgr_2_client_client_offline
{
	uint32 custom_id;				//client所属客户id
	uint32 client_id;					//离线client id
};
///通知相关客户端camera上线的包
struct mgr_2_client_client_online
{
	uint32 custom_id;				//client所属客户id
	uint32 client_id;					//离线client id
};
///获取正在查看同一个camera的人数的请求包
struct client_2_mgr_partner_list_count
{
	uint32 camera_custom_id;				//camera所属客户id
	uint32 camera_id;					//目的camera_id
	uint32 camera_status_server_ip;	//camera所在的状态服务器地址
};
///返回正在查看同一个camera的人数的请求包
struct mgr_2_client_partner_list_count
{
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//查看的cameraid
	uint32 client_list_count;			//查看同一个camera的人数
};

///请求同一个客户下的在线camera列表
struct client_2_mgr_active_camera_list
{
	uint32 custom_id;				//客户id
	uint32 company_status_server_ip;	//客户所在状态服务器地址
};

///返回同一个客户下的在线camera列表
struct mgr_2_client_active_camera_list
{
	uint32 custom_id;				//客户id
	uint16 camera_count;			//Camera的个数
	active_camera_s camera_arr[0];					//Camera列表数组
};
///请求同一个客户下的在线user列表
struct client_2_mgr_active_user_list
{
	uint32 custom_id;				//客户id
	uint32 company_status_server_ip;	//客户所在状态服务器地址
};

///返回同一个客户下的在线user列表
struct mgr_2_client_active_user_list
{
	uint32 custom_id;				//客户id
	uint16 user_count;				//Camera的个数
	active_member_s user_arr[0];				//Camera列表数组
};

struct client_mgr_get_lm_param
{
	uint32 device_id;
	uint32 user_id;
	uint32 type;   
	uint32 reserver;
};

struct client_mgr_lm_param_ack
{
	uint32 device_id;
	uint32 user_id;
	uint32 luminous;   //0--100
	uint32 reserver;
};





struct client_mgr_start_look_camera
{
	uint32 camera_id;//希望观看的camera_id
	uint32 client_id;
	uint16 net_type;
	uint16 peer_type;
	uint32 camera_stat_svr_ip;
	uint32 mgr_svr_ip;
};

struct client_mgr_stop_look_camera
{
	uint32 camera_custom_id;
	uint32 camera_status_svr_ip;
	uint32 camera_id;
	uint32 client_id;
};

struct client_mgr_want_look_video
{
	uint32 camera_id;
	uint32 client_id;
};

struct client_mgr_want_look_video_ack
{
	uint32 camera_id;
	uint32 client_id;
	uint32 camera_vd_svr_ip;
};

struct client_vd_rt_start_look_camera
{
	uint32 camera_id;//希望观看的camera_id
	uint32 client_id;//请求观看视频的客户端id
};

struct client_vd_rt_stop_look_camera
{
	uint32 camera_id;//所观看的camera_id
	uint32 client_id;//请求观看视频的客户端id
};

struct client_vd_camera_rt_hello
{
	uint32 sour_id;//源节点id
	uint32 dest_id;//目标节点id
};

struct camera_vd_rt_ask_push_video
{
	uint32 camera_id;
};

struct camera_vd_rt_ask_stop_video
{
	uint32 camera_id;
};



///请求观看特定摄像头的授权
struct client_mgr_req_auth_of_camera
{
	uint32 custom_id;					//摄像头所属客户id
	uint32 camera_id;					//摄像头id
	uint32 user_id;					//请求的用户id
	uint32 status_server_ip;			//客户组对应的状态服务器地址
	uint32 user_manage_server_ip;		//用户所属管理服务器ip
	char req_msg[64];						//请求消息语
	char user_name[32];						//请求的用户名
};



///管理员发送授权允许的包给管理服务器
struct client_mgr_req_auth_of_camera_ack
{
	uint32 custom_id;				//摄像头所属客户id
	uint32 camera_id;					//摄像头id
	uint32 user_id;					//请求的用户id
	uint32 status_server_ip;			//客户组对应的状态服务器地址
	uint32 user_manage_server_ip;		//请求用户所属管理服务器地址
	uint16 state;								//成功与否的标志 0 失败，1 成功
	char user_name[32];						//请求的用户名
};

struct client_2_mgr_forbid_auth_of_camera
{
	uint32 user_id;					//用户标识id
	uint32 user_manage_server_ip;		//请求用户所属管理服务器地址
};


///删除摄像头上的授权用户
struct client_2_mgr_del_auth_of_camera
{
	uint32 custom_id;				//摄像头所属客户
	uint32 camera_id;					//摄像头id
	uint32 admin_id;					//执行删除功能的管理员id
	uint32 user_id;					//即将被删除授权的用户id
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//camera与在线管理服务器

///节点（camera）登入在线管理服务器包
struct device_2_mgr_sign_in
{
	uint32 device_id;					//节点的标识id
	uint32 custom_id;				    //
	uint32 group_id;					//camera所属分组
	uint16 net_type;
	uint16   visible_level;					//camera可见级别
	char   session_id[128];					//验证session标识id
	int32		camera_num;						//本设备的camera数目
	uint32  camera_id_list[0];              //本设备camera_id列表，
};

struct device_2_mgr_sign_out
{
	uint32 device_id;					//节点的标识id
	uint32 custom_id;				    //0 = camera,1 = client
	uint32 group_id;					//camera所属分组
	uint16 net_type;
	char session_id[128];					//验证session标识id
	int32		camera_num;						//本设备的camera数目
	uint32  camera_id_list[0];              //本设备camera_id列表，
};

struct camera_2_mgr_sign_in
{
	uint32 camera_id;					//节点的标识id

	uint32 custom_id;				//0 = camera,1 = client
	uint16 net_type;
	int visible_level;						//camera可见级别
	char session_id[128];					//验证session标识id
};

struct camera_2_mgr_sign_out
{
	uint32 camera_id;					//节点的标识id
	uint32 custom_id;				//0 = camera,1 = client
	uint16 net_type;
	char session_id[128];					//验证session标识id
};
//////////////////////////////////////////////////////////////////////////////////////////////////////
//camera与在线管理服务器
struct peer_vd_sign_in
{
	unsigned int peer_id;					//节点的标识id
	char session_id[128];					//	验证session标识id
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//在线管理服务器与在线管理服务器

///让目标节点向自己打洞的返回包的转发包
//让目标节点向自己打洞的请求包
struct mgr_mgr_p2p_call
{
	uint32 father_id;		//对方节点id
	uint32 son_id;	//本节点 id
	uint32 son_public_udp_ip;			//自己的公网ip
	uint16 son_public_udp_port;		//自己公网port
	uint32 son_local_ip;				//自己私网ip
	uint16 son_local_port;			//自己私网port
	uint32 ret_dest_mgr_ip;
	uint32 source_key;							//源节点标识字
};

//让目标节点向自己打洞的返回包
struct mgr_mgr_p2p_call_ack
{
	uint32 father_id;					//对方节点id
	uint32 son_id;					//本节点 id
	uint32 father_public_udp_ip;		//对方的公网ip
	uint16 father_public_udp_port;	//对方公网port
	uint32 father_local_ip;			//对方私网ip
	uint16 father_local_port;		//对方私网port
	uint32 source_key;							//源节点标识字
};

///向其他管理服务器发CAMERA离线消息
struct mgr_mgr_camera_offline
{
	uint32 custom_id;				//所属客户的标识id
	uint32 camera_id;					//离线节点的标示id
};


///向其他管理服务器发CLIENT离线消息
struct mgr_mgr_client_offline
{
	uint32 camera_id;				//所属camera的标示id
	uint32 client_id;				//离线client的标示id
	uint32 custom_id;			//客户端所属客户id
};

struct  device_runtime_audio 
{
	uint32   camera_id;
	uint32   client_id;        
	uint32   type;      //1
	uint32   params[3];       // 0: disable   1: enable 
};

///向其他管理服务器发CAMERA上线消息
struct mgr_mgr_camera_online
{
	uint32 custom_id;			//所属客户的标识id
	uint32 camera_id;				//上线camera的标示id
};


///向其他管理服务器发CLIENT上线消息
struct mgr_mgr_client_online
{
	uint32 client_id;				//上线client的标示id
	uint32 custom_id;				//客户端所属客户id
};


///向其他管理服务器发观看视频消息
struct mgr_mgr_start_look_camera
{
	uint32 client_id;				//观看camera的client id
	uint32 camera_id;				//所观看的camera id
};

///向其他管理服务器发停止观看视频消息
struct mgr_mgr_stop_look_camera
{
	uint32 client_id;				//停止观看camera的client id
	uint32 camera_id;				//停止所观看的camera id
};

///要求服务器通知某些节点连接指定服务器
struct mgr_mgr_ask_client_connect
{
	uint32 dest_ip;					//目标服务器ip
	uint16 dest_port;				//目标服务器端口
	uint32 client_count;				//通知的节点个数
	client_s client_arr[0];					//通知的节点详细信息
};



///观看摄像头请求授权转发包
struct mgr_mgr_req_auth_of_camera
{
	uint32 user_id;					//请求用户标识id
	uint32 custom_id;				//camera所属客户id
	uint32 camera_id;					//请求观看的camera_id
	uint32 user_manage_server_ip;		//请求用户所在管理服务器ip
	uint32 status_server_ip;			//所属客户分部的状态服务器
	uint32 admin_id;					//管理员id
	char req_msg[64];						//请求消息语
	char user_name[32];						//请求用户的用户名
};

///为某用户观看摄像头授权成功与否的状态包
struct mgr_mgr_auth_of_camera_status
{
	int16 status;								//状态
	uint32 user_id;					//请求用户的id
	uint32 source_key;							//源节点标识字
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
//节点间消息定义
// p2pstun,打洞消息
struct msg_p2p_stun
{
	uint32 camera_id;
	uint32 son_id;
	uint32 father_id;
};

// p2pstunack,打洞应答消息
struct msg_p2p_stun_ack
{
	uint32 camera_id;
	uint32 son_id;
	uint32 father_id;
};

// p2pstunack,打洞应答消息
struct msg_p2p_stun_ack_ack
{
	uint32 camera_id;
	uint32 son_id;
	uint32 father_id;
};

typedef struct pano_info
{
	long lWidth;
	long lHeight;
	long lCenterX_1024;
	long lCenterY_1024;
	long lRadius_1024;
	long lPanoType;
	long lPanoTilt;
};

//初始化参数,父节点向子节点推送消息
struct msg_init_param //初始化客户端buffer 边界消息
{
	uint32 camera_id;
	uint32 min_block_seq;//帧块最小边界
	uint32 max_block_seq;//帧块最大边界
	uint32 device_type;		//OEM device type
	uint16 decode_head_size;
	char   decode_head[0];
};// buffermap消息,推送消息

struct msg_buffermap
{
	uint32 camera_id;
	uint32 father_id;//推送buffermap的节点
	uint32 son_id;//接收buffermap节点
	uint32 begin_block_seq;
	uint32 end_block_seq;
	uint32 map_data_size;
	char   map_data[0];
};

//请求推送数据
struct msg_ask_pushdata
{
	uint32 camera_id;
	uint32 peer_id;//请求推送数据的节点id
	int		video_type;   //请求推送数据video type
};

struct msg_ask_pushdata_ack
{
	uint32 camera_id;
	uint32 peer_id;//请求推送数据的节点id
	int	   result;  //0: Success  1: Has look history 2: Has dwonload history file 3.has look alarm video
};

struct msg_stop_pushdata
{
	uint32 camera_id;
	uint32 peer_id;
};

//请求数据
struct msg_ask_data
{
	uint32 camera_id;
	uint32  frm_block_seq;//小包序号
	uint32 son_id;//请求数据方
};
//数据
struct msg_data
{
	uint32 camera_id;
	uint32 father_id;	//发送数据方
	uint16 frm_type;			//帧类型,0 = i ,1= p
	uint32 frm_seq;
	uint32 frm_block_seq;
	uint16 inner_block_seq;		//从0开始编号到frm_block_count -1
	uint16 frm_block_count;
	uint32 data_size;		   //
	char data[0]; //数据
};

struct msg_p2p_data_ack
{
	uint32 camera_id;
	uint32 src_clientid;
	uint32 dst_clientid;
	int	   video_type;
	int	   last_recv_packets;
};
//keepalive
struct  msg_p2p_keepalive
{
	uint32   src_id;
	uint32	 dst_id;
};

//for  P2P_CONNECT_VIDEO_MSG
struct  msg_p2p_connect_video
{
	uint32	 src_peerid;
	uint32   dst_peerid;
	uint16		 net_type;
	uint16		 video_type;               //video type : realtime or history or normal
	uint32   camera_id;
};
//for  P2P_CONNECT_VIDEO_ACK_MSG
struct  msg_p2p_connect_video_ack
{
	uint32	 src_peerid;
	uint32   dst_peerid;
	uint16		 net_type;
	uint32   camera_id;
	uint16		 video_type;			//video type : realtime or history or normal
	int32		 response;             //0 : success  1: failed,more son nodes, 2: failed, no camera video.
};

//for  P2P_CLOSE_VIDEO_MSG
struct  msg_p2p_close_video
{
	uint32	 src_peerid;
	uint32   dst_peerid;
	uint16	 net_type;
	uint32   camera_id;
	uint16	 video_type;			//video type : realtime or history or normal
	int32      close_reason;          // reason for close this video , 0 : normal
};

struct client_mgr_get_vol_param
{
	uint32 device_id;
	uint32 user_id;
};

struct client_mgr_vol_param
{
	uint32 device_id;
	uint32 user_id;
	uint32 options;  // 开关选项;
	uint32 values;	 //  &options = 1 开  0 关	
	uint32 volume;   // 0-100 音量
	uint32 control;   //静音 
};

struct client_mgr_get_device_mdata
{
	uint32 device_id;
	uint32 user_id;
};

struct client_mgr_device_mdata
{
	unsigned int device_id;
	unsigned int user_id;
	unsigned int wifi_db;
	unsigned int local_ip;
	char   mac[18];
	char   version[16];
	char   currentssid[16];

};

struct mgr_device_relogin
{
	uint32 device_id;
	uint32 user_id;
};

//for P2P_SET_PUB_AUDIO_MSG
struct  msg_p2p_set_audio
{
        uint32   src_peerid;
        uint32   dst_peerid;
        uint16   net_type;
        uint16   type;
        uint32   camera_id;
        uint32   value;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
//client与数据服务器

///获取客户客户列表
struct client_2_data_customer_list
{
	uint32 read_line_start;//读取的起始位置
	uint32 read_line_count;//读取的行数
};

//返回的客户列表
struct data_2_client_customer_list
{
	uint32 customer_count;
	custom_s customers[0];
};

///根据custom_id获取客户的所有camera分组信息以及camera信息
struct client_2_data_company_group_camera_info
{
	uint32 custom_id;					//目标客户标示id
	uint32 client_id;					//user client id
	uint16 is_recommend;					//是否是获取推荐视频 0 = 推荐 1 = 其它
};

///服务器发送客户camera分组信息到客户端
struct data_2_client_company_groups
{
	uint32 custom_id;				//客户的标示id
	uint32 status_server_ip;			//客户所在状态服务器
	uint16 camera_group_count;					//camera分组数量
	char brand_name[64];					//品牌名称
	camera_group_s groups[0];				//分组信息数组
};

///服务器发送客户camera列表信息到客户端
struct data_2_client_company_cameras
{
	uint32 custom_id;				//客户的标示id
	uint32 camera_count;						//camera个数
	camera_s arr[0];						//camera信息数组
};

struct  client_alarm_process_msg
{
	uint32   record_id;	//add
	uint32   status;    //alarm_msg process(for user)   1:APP已接收告警;2:用户忽略;3:用户查看
};

struct  device_alarm_server_upload_msg
{
	uint32   client_id;	   //custom_id
	uint32   record_id;
	uint32   device_id;
	uint32   camera_id;
	uint32   channel_no;         //
	uint32   alarm_type;         //报警类型 enum  CAMERA_ALARM_TYPE
	uint32   begin_time;		 //报警开始时间
	uint32   end_time;           //报警结束时间 
	uint32   alarm_mode;         // 0x01,图片  0x02 视频
	uint32   alarm_code;	     //报警编号
	uint32   ccid;               //硬盘分区
};

struct client_mgr_set_camera_alarm_param
{
	uint32 	nstate;			// 0:close 1:open
	uint32	reserver[3];    // 保留参数
};


struct client_mgr_set_camera_alarm_note_msg
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	uint32 alarm_type;
	uint32 channel_no;
	client_mgr_set_camera_alarm_param param;
};

struct client_mgr_get_camera_alarm_note_msg  
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	uint32 alarm_type;
	uint32 channel_no; 
};

struct client_mgr_get_camera_alarm_note_ack_msg 
{
	uint32 camera_id;
	uint32 client_id;
	uint32 custom_id;
	uint32 device_id;
	uint32 alarm_type;
	uint32 channel_no;
	client_mgr_set_camera_alarm_param   param;
};

struct client_mgr_get_camera_alarm_data_msg 
{
	uint32   camera_id;
	uint32   client_id;
	uint32   custom_id;
	uint32   device_id;
	uint32   channel_no;
	uint32   alarm_type;
	uint32   begin_time;		//报警开始时间
	uint32   end_time;           //报警结束时间 
	uint32   alarm_mode;         // 0x01,图片  0x02 视频
	uint32   alarm_code;	//报警编号
	uint32   ccid;          //硬盘分区
};

struct  device_alarm_upload_msg
{
	uint32   device_id;
	uint32   camera_id;
	uint32   channel_no;         
	uint32   alarm_type;         //报警类型 enum  CAMERA_ALARM_TYPE
	uint32   begin_time;	     //报警开始时间
	uint32   end_time;           //报警结束时间 
	uint32   alarm_mode;         //0x01 图片  0x02 视频
	uint32   alarm_code;	     //报警编号
	uint32   ccid;               //硬盘分区
};

struct alarm_msg_data
{
	uint32 camera_id;
	uint32 father_id;	//发送数据方
	uint16 frm_type;			//帧类型,0 = i ,1= p  ,报警FRAME_TYPE_ALARM_JPEG =61; 
	uint32 frm_seq;
	uint32 frm_block_seq;
	uint16 inner_block_seq;		//从0开始编号到frm_block_count -1
	uint16 frm_block_count;
	uint32 data_size;				//包含报警编号长度;
	uint32 alarm_code;	//报警编号
	char   data[0]; //数据
};


///返回客户员工列表的信息包
struct data_2_client_users
{
	uint32 custom_id;				//客户的标识id
	uint32 user_count;							//客户员工数量
	user_s	arr[0];							//员工信息数组
};

///请求用户详细信息包
struct client_2_data_user_detail
{
	uint32 user_id;					//用户标识id
	char user_name[32];						//用户名
};


///返回用户详细信息包
struct data_2_client_user_detail
{
	uint32 user_id;					//用户id
	char user_name[32];						//用户名
	char real_name[64];						//用户真实姓名
	char phone[20];							//电话号码
	char email[64];							//email地址
	char nick_name[32];						//用户昵称
};

///请求客户详细信息包
struct client_2_data_company_detail
{
	uint32 custom_id;				//客户的标识id
};

///返回客户详细信息包
struct data_2_client_company_detail
{
	uint32 custom_id;				//客户的标识id
	uint16 is_authorized;						//是否授权
	char company_name[128];					//客户的企业名称
	char company_code[32];					//企业代码
	char brand_name[64];					//品牌名称
	char addr[128];							//客户地址
	char phone[20];							//客户电话
	char email[64];							//email地址
	char coment[500];						//客户描述
};

///请求camera详细信息的包
struct client_2_data_camera_detail
{
	uint32 camera_id;					//camera标识id
};

///返回camera详细信息的包
struct data_2_client_camera_detail
{
	uint32 camera_id;					//camera标识id
	uint32 video_rate;							//视频码率
	uint32 custom_id;				//所属客户id
	uint32 group_id;					//所属组id
	int16 visible;							//可视性
	uint32 made_date;							//出厂日期
	uint16 online;								//在线状态
	uint16 history_support;						//是否支持历史
	uint16 realtime_support;					//是否支持实时
	uint16 ptctrl_support;					//是否支持云台控制
	char position[64];						//camera位置
};

///更新用户的个人详细信息到服务器
struct client_2_data_update_user_info
{
	uint32 user_id;					//用户标识id
	char real_name[64];						//真实名称
	char passwd[64];						//密码
	char phone[20];							//电话号码
	char nick_name[32];						//昵称
};

///返回更新用户的成功与否状态包
struct data_2_client_update_user_info
{
	uint16 status;								//更新状态
};

///查看用户权限的包
struct client_2_data_user_right
{
	uint32 user_id;					//用户标识id
};

///返回用户权限包
struct data_2_client_user_right
{
	uint16 right_count;						//权限个数
	right_s arr[0];							//权限描述数组
};



struct client_2_data_alarm
{
	uint32	camera_id;
	uint32	client_id;
	uint32  alarm_total_count;			//total alarm count 
	uint32	alarm_count;				//alarm count for this packet
	alarm_record_s  alarm_rec[0];
};

struct client_2_data_get_alarm
{
	uint32	camera_id;
	uint32	client_id;
	uint32  begin_alarm_id;     // First it is 0, 
	uint32  req_count;			//request alarm count per packet
	uint32  only_processed;     //0: all alarm records 1: Only get processed alarm record 
	char	begin_date[32];     //'2011-07-14 14:25:03'  
	char	end_date[32];
};

/////////////////////////////////////////////////////
//////////////////////////////////////////
struct msg_get_public_addr
{
	uint32 peer_id;
};

struct msg_public_addr_ack
{
	uint32 peer_id;
	uint32 public_ip;
	uint16 public_port;
};


//获取随机client_id
struct msg_req_client_id
{
	uint16 net_type;
};

struct msg_client_id_ack
{
	uint32 client_id;
	uint32 mgr_svr_ip;
	uint32 stat_svr_ip;
};

//远程重启camera
struct msg_camera_reboot
{
	uint32 camera_id;//需要重启的camera
	uint32 client_id;//重启者的id
};

//开启远程日志
struct msg_camera_remote_log_start
{
	uint32 camera_id;//需要打开远程日志的camera
	uint32 client_id;//控制者client_id
	uint32 log_svr_ip;//远程日志服务器的ip
	uint16 log_svr_port;//远程日志服务器的port
};

struct msg_camera_remote_log_stop
{
	uint32 camera_id;//需要关闭远程日志的camera
	uint32 client_id;//控制者client_id
};

struct msg_want_look_rt_video
{
	uint32 camera_id;
	uint32 client_id;
};

struct msg_want_look_rt_video_ack
{
	uint32 camera_id;
	uint32 client_id;
	uint32 camera_vd_svr_ip;//这个是camera在登录之后返回的中继服务器地址
};

struct hist_video_file
{
	uint32 file_size;
	uint32 begin_time;
	uint32 end_time;
};


struct msg_want_look_hist_video
{
	uint32 camera_id;
	uint32 client_id;
	uint32 File_begin_time;
	uint32 file_end_time;
	uint16 look_type;              // 0:normal  1: forcely
};


struct msg_want_look_hist_video_ack
{
	uint32 camera_id;
	uint32 client_id;
	uint32 File_begin_time;
	uint32 file_end_time;
	uint16 result;		// 0: success  1: fail, other hist has been open 2: fail,other realtime is looking
};

struct msg_pt_move_left
{
	uint32 camera_id;
	uint32 client_id;
};

struct msg_pt_move_right
{
	uint32 camera_id;
	uint32 client_id;
};


struct msg_pt_move_up
{
	uint32 camera_id;
	uint32 client_id;
};


struct msg_pt_move_down
{
	uint32 camera_id;
	uint32 client_id;
};

struct msg_pt_foucus_near
{
	uint32 camera_id;
	uint32 client_id;
};

struct msg_pt_foucus_far {
	uint32 camera_id;
	uint32 client_id;
};

struct msg_pt_move_stop {
	uint32 camera_id;
	uint32 client_id;
};

struct client_vd_hist_start_look_video
{
	uint32 client_id;
	uint32 camera_id;
	uint32 begin_time;
	uint32 end_time;
	uint32 begin_frame;     //开始帧号
};

struct client_vd_hist_stop_look_video
{
	uint32 client_id;
	uint32 camera_id;
	uint32 begin_time;
	uint32 end_time;
};


struct client_vd_ask_push_hist_video
{
	uint32 camera_id;
	uint32 begin_time;
	uint32 end_time;
	uint32 begin_frame;   //开始帧号
};

struct client_vd_ask_stop_hist_video
{
	uint32 camera_id;
	uint32 begin_time;
	uint32 end_time;
};


struct client_vd_pause_push_video
{
	uint32 client_id;
	uint16 camera_group_count;
	uint32 camera_id[0];
};

struct client_vd_restart_push_video
{
	uint32 client_id;
	uint16 camera_group_count;
	uint32 camera_id[0];
};

struct hist_skip_video
{
	uint32 camera_id;
	uint32 client_id;
	uint32 start_frame;
};

struct msg_hist_video
{
	uint32 camera_id;
	uint32 begin_time;
	uint32 end_time;
	uint32 cur_frame;
	uint32 max_frames;
	uint16 frm_type;			//帧类型,0 = i ,1= p
	uint32 frm_seq;
	uint32 frm_block_seq;
	uint16 inner_block_seq;		//从0开始编号到frm_block_count -1
	uint16 frm_block_count;
	uint32 data_size;				//
	char data[0]; //数据
};

//用于得到和设置camera图像参数
struct  msg_get_picture_param
{
	uint32 	 camera_id;
	uint32   src_clientid;
	uint32   dst_clientid;
};

//CAMERA_SET_PICTURE_PARAM
struct  msg_picture_param
{
	uint32  camera_id;
	uint32  src_clientid;
	uint32  dst_clientid;
	int		brightness;		//图像亮度
	int		contrast;			//图像对比度
	int		hue;				//图像色调
	int		saturation;		//图像饱和度
	int	acute;				//图像锐度 <---- by 20121211
};

//////////////////////////////////////////////////////////////////////////

struct  msg_get_video_param
{
	uint32 	 camera_id;
	uint32   src_clientid;
	uint32   dst_clientid;
};
//////////////////////////////////////////////////////////////////////////

struct  msg_video_param
{
	uint32    camera_id;
	uint32    src_clientid;
	uint32    dst_clientid;
	int		  flow_type;			//流类型，video流或者video/audio流
	int    	  bitrate_type;		//码流比特率类型，可变的或固定的
	int   	  bitrate;				//码流比特率
	int		  video_quality;		//视频质量
	int		  frame_rate;			//视频的帧速率
};

struct  msg_get_init_param
{
	uint32		camera_id;
	uint32		src_clientid;
	uint32		dst_clientid;
	uint16		video_type;           //指是realtime,normal,history
	uint16      padding;
};

struct msg_want_download_hist_file
{
	uint32 camera_id;
	uint32 client_id;
	uint16 download_type;              // 0:normal  1: forcely
};

struct msg_want_download_hist_file_ack
{
	uint32   camera_id;
	uint32   client_id;
	uint32   result;		//0: 成功，即enum HIST_LOOK_REPONSE值，见下面。
	uint32   camera_vd_svr_ip;//这个是camera在登录之后返回的中继服务器地址
};

struct client_vd_start_download_hist_file
{
	uint32 camera_id;
	uint32 client_id;
	uint32 begin_time;      //
	uint32 end_time;
	uint32 begin_frame;     //开始帧号
};

struct client_vd_stop_download_hist_file
{
	uint32 camera_id;
	uint32 client_id;
	uint32 begin_time;
	uint32 end_time;
};

struct client_vd_download_hist_file_finished
{
	uint32 camera_id;
	uint32 client_id;
	uint32 begin_time;
	uint32 end_time;
	uint32 last_frame_timestamp;  //最后一帧的timestamp,防止消息先到，而数据后到的情况
};

struct msg_vd_download_hist_data_ack
{
	uint32  camera_id;
	uint32  client_id;
	uint32  last_frameid;
};

struct  msg_ask_multiple_data
{
	uint32   msg_id;       //消息的ID号，自然增长的整数值
	uint32   camera_id;
	uint32   client_id;
	uint16	 ask_packet_num;    //本消息包含的请求个数。
    struct msg_ask_data   ask_data[0];
};

///track//////////////////////////////////////////////////////////////////////
struct client_tk_start_look_camera
{
	uint32 camera_id;//希望观看的camera_id
	uint32 client_id;
	uint16 net_type;
	uint16 peer_type;
};

///从camera分组虫删除的请求包
struct client_tk_stop_look_camera
{
	uint32 camera_id;					//目的camera_id
	uint32 client_id;					//待删除的clientid
	uint16 net_type;
	uint16 peer_type;
};

struct client_tk_get_partner_list
{
	uint32 camera_id;					//目的camera_id
	uint32 client_count;				//请求的个数
	uint16 net_type;
	uint16 peer_type;
};


///请求camera相关的client 列表的返回包
struct client_tk_partner_list_ack
{
	uint32 camera_id;					//相关的camera的标识Id
	uint16 client_count;				//客户端的个数
	client_s client_array[0];				//客户端数组的首地址
};

///获取正在查看同一个camera的人数的请求包
struct client_tk_get_partner_list_count
{
	uint32 camera_id;					//目的camera_id
};

///返回正在查看同一个camera的人数的请求包
struct client_tk_partner_list_count_ack
{
	uint32 camera_id;					//相关的camera的标识Id
	uint32 client_list_count;			//查看同一个camera的人数
};

struct client_tk_p2p_call
{
	uint16 net_type;			//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;		//对方节点id
	uint32 son_id;	//本节点id
	uint32 son_public_udp_ip;			//自己的公网ip
	uint16 son_public_udp_port;		//自己公网port
	uint32 son_local_ip;				//自己私网ip
	uint16 son_local_port;			//自己私网port
};

//让目标节点向自己打洞的返回包
struct client_tk_p2p_call_ack
{
	uint16 net_type;//网络类型，比如电信，网通，长宽等
	uint16 peer_type;
	uint32 camera_id;
	uint32 father_id;					//对方节点id
	uint32 son_id;					//本节点id
	uint32 father_public_udp_ip;		//对方的公网ip
	uint16 father_public_udp_port;	//对方公网port
	uint32 father_local_ip;			//对方私网ip
	uint16 father_local_port;		//对方私网port
};


////////////////////////////////////////////////
///im

struct msg_chat_content
{
	uint32 src_user_id;								//源用户id
	uint32 dest_user_id;							//目标用户id
	char   src_nick_name[MAX_NICK_NAME_SIZE];		//源用户名称
	uint32	send_time;								//聊天内容发送日期
	font_s	m_tFont;								//内容字体
	uint16 content_size;							//聊天信息大小
	char contents[0];								//聊天内容
} ;

struct msg_chat_shake
{
	uint32 src_user_id;
	uint32 dest_user_id;
};

//////////////////////////////////////////
///For security center 
/////////////////////////////////////////////
struct get_camera_netinfo
{
	uint32    camera_id;
	uint32	  client_id;
};

struct get_camera_netinfo_ack
{
	uint32    camera_id;
	uint32	  client_id;
	uint32    public_ipaddr;
	uint32	  local_ipaddr;
	uint32	  gateway_ipaddr;
	uint32    net_isp_type;
	uint32    net_isp_type_mode;  //0:auto probe 1:tel 2:cnc
};

struct client_apply_salt_req
{ 
	char user_name[64];
	uint16 user_type;  
};

struct client_apply_salt_ret
{
	char salt[16];
};

struct set_camera_net_isptype
{
	uint32    camera_id;
	uint32	  client_id;
	uint32    net_isp_type;
};

struct get_device_info
{
	uint32    camera_id;
	uint32	  client_id;
};

struct get_device_info_ack
{
	uint32    camera_id;
	uint32	  client_id;
	char	  version_hardware[16];
	char	  version_software[16];
	char	  version_oem_sdk[16];
	uint32	  public_ipaddr;
	uint32	  local_ipaddr;
	char	  mac_addr[24];
	uint32	  free_mem_size;
	uint32	  free_flash_size;
	uint32	  has_disk;
};

enum  CAMERA_ALARM_TYPE
{
	ALARM_TYPE_OFFLINE,
	ALARM_TYPE_LOST_VIDEO,
	ALARM_TYPE_MASK_VIDEO,
	ALARM_TYPE_DETECTION_MOTION,
	ALARM_TYPE_DISK_FULL,
	ALARM_TYPE_RECORD_ABNORMAL,
	ALARM_TYPE_LOG_LAMP_OFF,           //Log灯箱断电 
	ALARM_TYPE_LED_SCREEN_OFF,         //LED屏幕断电
	ALARM_TYPE_TALK_REQUEST,           //店内通话请求
	ALARM_TYPE_LOG_LAMP_ON,           //Log灯箱上电 
	ALARM_TYPE_LED_SCREEN_ON,         //LED屏幕上电
	
	//by 录像丢失报警
	ALARM_TYPE_LOST_RECORD,

	ALARM_TYPE_ONLINE = 20,
	//by 无报警
	//20120518
	ALARM_TYPE_NULL = 100,
};

struct camera_alarm
{
	uint32		camera_id;
	uint32		alarm_type;
	uint32      alarm_id;         //id of alarm record in database
	char		alarm_msg[32];
};

struct forward_camera_alarm
{
        uint32          admin_id;
        uint32          camera_id;
        uint32          alarm_type;
        uint32          alarm_id;         //id of alarm record in database
        char            alarm_msg[32];
};

struct alarm_2_mgr_alarm_msg
{
        int forward_alarm_num;
        struct forward_camera_alarm fcamera_alarm[0];
};

struct server_login_alarm_req
{
        unsigned int server_ip;
        unsigned int port;
};

struct camera_alarm_ack_msg
{
        uint32 camera_id;
        uint32 alarm_id;
        uint32 ack_msg;
        uint32 padding;
};


struct get_motion_detection_param
{
	uint32		camera_id;
	uint32		client_id;
};

struct motion_detection_param
{
	uint32		camera_id;
	uint32		client_id;
	uint32		begin_time;	//开始时间，分钟
	uint32		end_time;		//结束时间，分钟
	uint32		delay;      
	uint32 		trigger_record;
	uint32		trigger_alarm;
	uint32		detection_sense;   //移动侦测灵敏度:0-不检测;1-5灵敏度,值越大越灵敏
	uint32		flag_buzz;		//是否开启本地蜂鸣。0: 关 1：开
};

enum  MOTION_DETECTION_SENSE
{
	MOTION_DETECTION_STOP=0,
	MOTION_DETECTION_SENSE_1,
	MOTION_DETECTION_SENSE_2,
	MOTION_DETECTION_SENSE_3,
	MOTION_DETECTION_SENSE_4,
	MOTION_DETECTION_SENSE_5,
};

struct get_lost_video_param
{
	uint32		camera_id;
	uint32		client_id;
};

struct lost_video_param
{
	uint32		camera_id;
	uint32		client_id;
	uint32		begin_time;	//开始时间，分钟
	uint32		end_time;		//结束时间，分钟
	uint32		delay;      
	uint32 		trigger_record;
	uint32		trigger_alarm;
	uint32		flag_buzz;		//是否开启本地蜂鸣。0: 关 1：开
};

struct get_mask_video_param
{
	uint32		camera_id;
	uint32		client_id;
};

struct mask_video_param
{
	uint32		camera_id;
	uint32		client_id;
	uint32		begin_time;	//开始时间，分钟
	uint32		end_time;		//结束时间，分钟
	uint32		delay;      
	uint32 		trigger_record;
	uint32		trigger_alarm;
	uint32		flag_buzz;		//是否开启本地蜂鸣。0: 关 1：开
};

struct set_preset_point
{
	uint32  camera_id;
	uint32  client_id;
	uint32  preset_no;
	uint32  preset_flag;	//0:add 1: del  2: to 
};



struct  start_stop_cruise_path
{
	uint32	 camera_id;
	uint32	 client_id;
	uint32   cruise_path_no;
};

struct  set_yuntai_mode
{
	uint32	camera_id;
	uint32	client_id;
	uint32  yuntai_mode;
	uint32	mode_param;
};

enum  YUNTAI_MODE
{
	YUNTAI_MODE_USER,
	YUNTAI_MODE_CRUISE,
	YUNTAI_MODE_WATCH,
};

struct  get_system_time
{
	uint32	 camera_id;
	uint32   system_time;
};

struct  set_alarm_upload
{
	uint32	camera_id;
	uint32   client_id;
	uint32   is_alarm_upload;   //0: 关闭 1：开启
};

struct  switch_alarm_param
{
	uint32   camera_id;
	uint32   channel_no;        //通道号
	uint32   begin_time;		//报警开始时间
	uint32   end_time;           //报警结束时间 
	uint32   alarm_type;         //报警类型
	uint32   alarm_mode;         //指明高低电平，0：低电平 1：高电平
};

struct  set_switch_alarm_param
{
	uint32   camera_id;
	uint32   client_id;              //发送方
	struct   switch_alarm_param    switch_param;  //     
};

struct  get_switch_alarm_param
{
	uint32   camera_id;
	uint32   client_id;              //发送方
};

struct  get_switch_alarm_param_ack
{
	uint32   camera_id;
	uint32   client_id;              //发送方
	uint32   count;				//  指明下列参数数组的个数。
	struct   switch_alarm_param    switch_param[0];  // 
};

struct  set_switch_device
{
	uint32   camera_id;			//在设备端并不会严格指定 设备端是通过channel_no判断
	uint32   client_id;         //发送方
	uint32   channel_no;        //通道号 从0开始
	uint32   device_type;         //开关设备类型  1：LED灯，2：LCD屏幕
	uint32   switch_mode;         //指明开关，0：关 1：开
	//by 20120518
	//提供操作的自动性 使之可以在自动模式下 的某个时段内有效
	uint32   auto_mode;			//自动状态 0：手动控制 1：自动控制 2：清除控制
	uint32   begin_time;		//操作开始时间
	uint32   end_time;          //操作结束时间 
};

//////////////////////////////////////////////////////////////////////////
//by 
//20120518
struct  get_switch_device_param
{
	uint32   camera_id;
	uint32   client_id;              //发送方
};

struct  get_switch_device_param_ack
{
	uint32   camera_id;
	uint32   client_id;              //发送方
	uint32   count;				//  指明下列参数数组的个数。
	struct   set_switch_device    switch_device[0];  // 
};

struct  realtime_stream_type
{
	uint32	 camera_id;
	uint32   client_id;
	uint32   stream_type;   //0: 主码流 1：子码流1
};

struct  set_runtime_audio 
{
	uint32   camera_id;
	int		 audio_enable;          // 0: disable   1: enable 
};

////////////////////////////////////
struct camera_2_mgr_vca_data_report
{
	uint32 camera_id;    //请求上传的camera id
	uint32 vca_id;       //绑定的智能传感器
	char   begin_time[24];  //开始时间，格式为"2011-11-11 11:11:11"的字符串  
	char   end_time[24];  //截止时间，格式为"2011-11-11 11:11:11"的字符串
	uint32 val;          //数值
};

struct mgr_2_camera_vca_data_report_ack
{
	uint32 camera_id;     //请求上传的camera id
	uint32 vca_id;        //绑定的智能传感器
};

struct client_2_infosvr_query_vca_data
{
	uint32 vca_id;            //绑定的智能传感器名称
	uint32 val;					//统计数值
	char   begin_time[24];    //开始时间，格式为"2011-11-11 11:11:11"的字符串  
	char   end_time[24];      //截止时间，格式为"2011-11-11 11:11:11"的字符串
};

struct client_2_infosvr_query_vca_data_list
{
	uint32 client_id;         //请求的客户端
	uint32 camera_id;         //请求的camera id
	uint32 query_num;         //请求标示号码
	uint32 counts;            //数据总数
	char   begin_time[24];    //开始时间，格式为"2011-11-11 11:11:11"的字符串  
	char   end_time[24];      //截止时间，格式为"2011-11-11 11:11:11"的字符串
	struct client_2_infosvr_query_vca_data data_list[0];  //请求包首地址
};

///used by  broadcast server alive message
struct  server_alive_msg
{
	int		server_type;   // login manage status info etc.
	unsigned int	last_alive_msg_time; // time of last alive message reveived
	int     is_alive;           //
	unsigned int  srv_ipaddr;   // ip address of server
	char	srv_c_ipaddr[24];   
	int		srv_port;           //server port
	int     start_time;         //time of server startup
	unsigned int  clients_num;  // client number of the server
	unsigned int  camera_num;   // camera number of the server
	int		work_thread_num;    // number of working threads of server
};


////////////////////////
struct  server_forward_msg
{
	unsigned int    dstination_id;          // perhaps is clientid or cameraid or device id etc.
	unsigned int    cmd_id;					// forward message id 
	int			    msg_body_len;   
	char			msg_body[0];
};


////////////////////////////////////////////////
//structures for talk purpose
struct  mgr_talk_msg
{
	uint32    camera_id;
	uint32    src_clientid;		// sender of message
	uint32    dst_clientid;     // dstination of message
	uint32    custom_id;       // company id of camera 
	uint32    reason;           //结束时，可指明原因。 0:normal  1: 拒绝 2: 参数不匹配 3: private talk
};

struct  mgr_talk_ack_msg
{
	uint32    camera_id;
	uint32    src_clientid;		// sender of message
	uint32    dst_clientid;     // dstination of message
	uint32    custom_id;       // company id of camera
	uint32    result;           //0: success   1: has talked to other
	uint32    audio_frm_type;    //audio frame type of support
};

struct  p2p_audio_data_msg
{
	uint32 src_clientid;	   //发送数据方
	uint32 dst_clientid;       //dstination of message
	uint32 block_id;
	char data[0]; //数据
};

//////////////////////////////////////////////////////////////////////////
//by
//从服务器上通过CLIENT_DATA_GET_COMPANY_EVENT得到该客户支持的事件，相关结构体如下：
struct get_company_event_list
{
	uint32 custom_id;
};

//接收时CLIENT_DATA_GET_COMPANY_EVENT_ACK
struct  event_describtion
{
	uint32   event_id;
	char     event_name[32];
};
struct company_event_list_ret
{
	uint32 custom_id;
	uint16 event_num;   // event数目
	struct event_describtion event_list[0];
};

//////////////////////////////////////////////////////////////////////////
//by 20121211
//关联业务询问
//CLIENT_DATA_GET_COMPANY_ACTIVED_DEVICE
//CLIENT_DATA_GET_COMPANY_ACTIVED_DEVICE_ACK
struct client_2_data_active_device
{
	uint32 	custom_id;
	uint32 	active_device;//0:none 1:actived some device
};

//CLIENT_DATA_CHANGE_CAMERA_NAME
//客户端向数据服务器发送更名请求
struct  client_2_data_change_camera_name
{
	uint32 	user_id;
	uint32 	camera_id;
	char	camera_name[32];
};

//CLIENT_MGR_SET_CAMERA_ALARM_NOTE_MSG
//CLIENT_MGR_GET_CAMERA_ALARM_NOTE_MSG
//CLIENT_MGR_GET_CAMERA_ALARM_NOTE_ACK_MSG
//MGR_GET_CAMERA_ALARM_NOTE_MSG
//MGR_GET_CAMERA_ALARM_NOTE_ACK_MSG

//struct client_mgr_set_camera_alarm_note_msg
//{
//	uint32	user_id;
//	uint32 	camera_id;
//	uint32 	note_switch;	//0:close 1:open
//	uint32	begin_time;		//开始时间，分钟
//	uint32	end_time;		//结束时间，分钟
//};



//CLIENT_MGR_SET_CAMERA_CRUISE_MODE_MSG
struct client_camera_cruise_mode_msg
{
	uint32	user_id;
	uint32 	camera_id;
	uint32 	cruise_mode;//0:horizonal cruise 1:vertical cruise
};

//CLIENT_DATA_SUMBIT_LOG_CONTENT
struct client_2_data_submit_log_content_msg
{
	uint32 		user_id;
	uint32 		log_time;//日志时间
	char	 	log_type[32];//日志类型 1.登录 2.操作等
	char		log_content[64];//日志内容
};
//CLIENT_DATA_GET_LOG_CONTENT_MSG
//by zbiao  支持家庭版手机客户端获得用户操作记录
/////////////////////////////////////////
struct client_2_data_get_log_content_msg
{
	uint32 		user_id;	
	char	 	log_type[32];//日志类型 1.登录 2.操作等
};
struct user_log_content
{
	char		username[32];
	char		log_content[64];	//日志内容
	uint32 	log_time;	//日志操作时间
};


struct client_2_data_get_log_content_msg_ack
{
	char	 	log_type[32];	//日志类型 1.登录 2.操作等
	uint32	usercnt;
	user_log_content userLog[32];
};

////////////////////////////////////////////////////
struct timing_record_time_list
{	
	uint32	begin_time;		//开始时间，分钟
};

//CLIENT_MGR_SET_CAMERA_TIMING_RECORD_PARA_MSG
struct client_camera_timing_record_para
{
	uint32	user_id;
	uint32 	camera_id;
	uint32 	timing_record_switch;//0:close 1:open
	uint32 	record_length;//录像时长，单位为秒
	uint32	time_count;//设置个数
	struct 	timing_record_time_list  time_list[0];//录像开始时间
};

//CLIENT_DATA_GET_HOME_FILE_LIST_MSG
struct client_2_data_get_home_file_list_msg
{
	uint32 	user_id;
	uint32 	camera_id;
	uint32  req_count;
	uint32  start_file_id;
	uint32 	query_s_date;	//查询起始日期
	uint32  query_e_date;	//查询终止日期
	uint32 	query_type;		//0:全部文件 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
};

struct home_file
{	
	char		file_name[32];
	uint32		file_size;
	uint32		file_time;
	uint32		file_id;	//文件的唯一标示符
	char		file_url[200];	//文件路径
};

//CLIENT_MGR_GET_CLOUD_HOME_FILE_LIST_ACK_MSG
struct client_2_data_get_home_file_list_ack_msg
{
	uint32 	query_type;		//0:全部文件 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	uint32 	file_count;		//0:无文件
	struct 	home_file files[0];
};



//CLIENT_DATA_EDIT_HOME_FILE_MSG
struct client_2_data_edit_home_file
{
	uint32	user_id;
	uint32 	query_type;//1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	uint32  file_id;
	char	old_file_name[32];
	char	new_file_name[32];
	uint32  edit_type;//0:change file name 1:delete file
};

//快照消息
//CLIENT_2_MGR_SNAPSHOT_MSG
struct  client_2_mgr_snapshot_msg
{
	uint32   camera_id;
	uint32   client_id;	//by 20121225 请求拍照的客户端
};
//MGR_2_CAMERA_SNAPSHOT_MSG
struct mgr_2_camera_snapshot_msg
{
	uint32   camera_id;
	uint32   client_id;	//by 20121225 请求拍照的客户端
};
//设备上传文件询问消息
//CAMERA_2_HTTP_QUERY_FILE_UP_MSG
struct camera_2_http_query_file_up_msg
{
	uint32   camera_id;
	uint32   file_type;	//文件类型 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	uint32   file_size;  
};

//设备上传文件询问应答消息
//HTTP_2_CAMERA_QUERY_FILE_UP_ACK_MSG
struct http_2_camera_query_file_up_ack_msg
{
	uint32   camera_id;
	uint32   file_type;	//文件类型 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	/*
	返回值：
	0.有效确认 
	1.无此权限
	2.设备离线
	3.空间已满
	*/
	uint32   ack_msg;	
};

//CAMERA_2_MGR_FILE_UP_ACK_MSG
//设备上传文件回馈消息
struct camera_2_mgr_file_up_ack_msg
{
	uint32   camera_id;
	uint32   client_id;	//by 20121225 请求拍照的客户端
	uint32   file_type;	//文件类型 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	uint32   file_id;	//文件ID 设备上传http成功后需要将该文件的id通知管理服务器
	uint32	 ack_msg;	//可扩充的消息备注
	char	 file_url[200];	//文件路径
};

//MGR_2_CLIENT_SNAPSHOT_ACK_MSG
struct mgr_2_client_file_up_ack_msg
{
	uint32   camera_id;
	uint32   client_id;	//by 20121225 请求拍照的客户端
	uint32   file_type;	//文件类型 1.报警图片 2.报警视频 3.定时图片 4.定时视频 5.快照图片
	uint32   file_id;	//文件ID 设备上传http成功后需要将该文件的id通知管理服务器
	uint32	 ack_msg;	//可扩充的消息备注
	char	 file_url[200];	//文件路径
};

//报警信息
//CAMERA_ALARM_MSG
struct camera_alarm_msg
{
	uint32 camera_id;
	uint32 alarm_type;	//报警类型
};

//命令类型
struct client_camera_common_order
{
	uint32  order_type;
	uint32	user_id;
	uint32 	camera_id;
};

//UPDATE_MOBILE_INVITE_INFO
struct update_2_mobile_invite
{
	uint32 device_id;
	uint32 custom_id;
	char   software_version[16];
	uint32 reserver1;
	uint32 reserver2;
};

//UPDATE_MOBILE_INVITE_ACK
struct mobile_2_update_invite_ack
{
	uint32 device_id;
	uint32 custom_id;
	uint32 status;       // 1 msg received; 2 agree to update ; 
	char   software_version[16];
	uint32 reserver1;
	uint32 reserver2;
};



//UPDATE_DEVICE_FTP_INFO
struct update_2_device_ftp_info
{
	uint32 device_id;
	uint32 custom_id;
	uint32 task_id;
	char   username[16];
	char   password[16];
	char   pathname[128];
	char   md5[32];
	uint32 reserver1;
	uint32 reserver2;
};

//UPDATE_DEVICE_FTP_INFO_ACK
struct device_2_update_ftp_ack
{
	uint32 device_id;
	uint32 custom_id;
	uint32 task_id;
	uint32 status;   // 1 msg received; 2 update packet downloaded; 4 update failed ; 8 update succeed
	uint32 reason;
	uint32 reserver1;
	uint32 reserver2;
};

struct user_2_mgr_disconn_device
{
	uint32 device_id;
	uint32 user_id;
};

//CLIENT_MGR_SET_VIDEO_PARAM=20217
struct client_mgr_set_video_param
{
	uint32 device_id;
	uint32 user_id;
	uint32 channel;
	uint32 video_type;
	uint32 reservers[2];
};

////////////////////////////////////////////////

#pragma pack()

#endif//__PROTO_H__

