#ifndef PACKPROTOCOLTYPE_H
#define PACKPROTOCOLTYPE_H
#include"pw_datatype.h"
#define MSG_HEADER_TAG 0xFFEEDDCC

#define MAX_UUID_COUNT 50
#define MAX_VERSIONFEATURE_LEN 256

#define PKG_LEN_HEAD 20
#define PKG_LEN_ERR_BODY 132

#define MSG_TYPE_USR_REG 10011
#define PKG_LEN_USR_REG 278
#define PKG_LEN_USR_REG_RET_BODY 8

#define MSG_TYPE_TEST_USR_EXIST 910011
#define PKG_LEN_USR_REG 278

#define MSG_TYPE_USR_LOGIN 10001
#define PKG_LEN_USR_LOGIN 156
#define PKG_LEN_USR_LOGIN_RET_BODY 156

#define MSG_TYPE_RESETPSW 10013
#define PKG_LEN_RESETPSW 212

#define MSG_TYPE_FORGETPSW 10014
#define PKG_LEN_FORGETPSW 212

#define MSG_TYPE_SENDVERIFY 10012
#define PKG_LEN_SENDVERIFY 84

#define MSG_TYPE_DEV_LOGIN 10005
#define PKG_LEN_DEV_LOGIN 90
//#define PKG_LEN_DEV_LOGIN_RET 28
#define MSG_TYPE_IPC_LOGIN 10002
#define PKG_LEN_IPC_LOGIN 26
//#define PKG_LEN_IPC_LOGIN_RET 28

#define MSG_TYPE_TERMINAL_INFO_ADD 10016
#define PKG_LEN_TERMINAL_INFO_ADD  792

#define MSG_TYPE_USER_DEV_ADD 10019
#define PKG_LEN_USER_DEV_ADD 216
#define PKG_LEN_USER_DEV_ADD_RET_BODY 0

#define MSG_TYPE_USER_DEV_GET_BYID 10020
#define PKG_LEN_USER_DEV_GET_BYID 24

#define MSG_TYPE_USER_DEV_GET_BYUUID 10021
#define PKG_LEN_USER_DEV_GET_BYUUID 24
#define PKG_LEN_USER_DEV_GET_BYUUID_RET_BODY MAX_UUID_COUNT*140

#define MSG_TYPE_APP_VERSTION 10022
#define PKG_LEN_APP_VERSTION 22
#define PKG_LEN_APP_VERSTION_RET_BODY 32+256

#define MSG_TYPE_IPC_UNBIND 10023
#define PKG_LEN_IPC_UNBIND 88

#define NAME_PASSWORD_LEN 64

enum PW_PHONE_TCPCLIENT_ERR{
    _INVALIDSOCKET                      =12300,//socket chuangjian shibai
    _CONNECTERR                         =12301,
    _WRITESOCKET_ERR                    =12302,
    _READSOCKET_ERR                     =12303,
    _MALLOC_ERR                         =12304,
    _TIMEOUT                            =12305,
    _SELECTSOCKET_ERR                   =12306
};

//一、通用
//1.所有请求包/返回包的包头：
typedef struct _msg_header//---------------------------------------------------20
{
    GUInt32 tag;				     	//包头标志
    GUInt32 cmd_id;					//包的类型id
    GUInt32 ver;					//协议版本号
    GUInt32 len;					//包的长度
    GUInt32 state;					//用在返回包，标示服务器对客户端的各种响应状态
}_msg_header;

//其中：
//tag = 0xFFEEDDCC；
//cmd_id = 10001；len = 156  //用户登录
//cmd_id = 10002；len = 26   //IPC登录
//cmd_id = 10005；len = 90   //设备登录

//cmd_id = 10011；len = 276  //用户注册
//cmd_id = 10012；len = 84   //发送验证码
//cmd_id = 10013；len = 212  //重设密码
//cmd_id = 10014；len = 212  //根据验证码重设密码
//cmd_id = 10016；len = 792  //终端设备增加
//cmd_id = 10017；len = 280  //终端设备查询
//cmd_id = 10018；len = 280  //终端设备异常信息增加
//cmd_id = 10019；len = 88   //用户IPC增加
//cmd_id = 10020；len = 24   //用户IPC查询(device_id)
//cmd_id = 10021；len = 24   //用户IPC查询(device_sn)
//cmd_id = 10022；len = 22   //用户APP版本查询
//cmd_id = 10023；len = 88   //用户IPC 解绑

//2.所有返回结果为失败的包体：
struct _msg_status
{
    GInt32 err_code;
    char err_msg[128];    // -------------------------------------------------------------------------132
};

//二、各个服务相应的包体
//1.用户注册：
//请求：

struct _client_register_2_login
{
    char user_name[64];
    char pass[64];
    char verifycode[64];    //验证码
    char device_sn[64];  //摄像头uuid
    GUInt16 user_type;
};
//注：
//其中，若user_name不为空，pass为空则表示进行用户名是否存在的验证：返回包头状态为0表示用户不存在，无包体；用户存在返回错误包体（msg_status）。
//其中，device_sn若为空，表示只进行用户注册；device_sn不为空，表示进行用户注册、设备添加。
//返回为成功时的包体：
struct _login_2_client_reg_ret
{
        GUInt32 device_id;      //从redis（从camera）
        GUInt32 camera_id;
};

//2.用户登陆
//请求：
struct _client_2_login_req
{
    char user_name[64];
    char pass[64];
    GUInt32 net_type;
    GUInt32 padding;
};

//返回为成功时的包体：
struct _login_2_client_ret
{
    GUInt32 user_id;          //从user_id
    GUInt32 manage_server_ip;
    GUInt16 manage_server_port;
    GUInt32 custom_id;
    GUInt32 right_mask;//权限表
    GUInt16 custom_type;
    GUInt16 net_type; //网络类型，比如电信，网通，长宽等
    char session_id[128];
    GUInt32 is_admin;
};

//3.发验证码
//请求：
struct _send_verifycode_req
{
    char user_name[64];
};
//返回为成功时的包体：无

//4.重设密码
//请求：
struct _reset_password_req
{
    char user_name[64];
    char oldpass[64];
    char newpass[64];
};
//返回为成功时的包体：无

//5.根据验证码重设密码
//请求：
struct _reset_password_byverify_req
{
    char user_name[64];
    char verifycode[64];
    char newpass[64];
};
//返回为成功时的包体：无

//6.设备登陆
//请求：
struct _device_2_login_req
{
    GUInt32 device_id;
    GUInt16 net_type;//本地检测出的网络类型
    char   device_sn[64];
};
//返回为成功时的包体：
struct _login_2_device_ret
{
    GUInt32 custom_id;
    GUInt32 group_id;			//分组ID号
    GUInt32 device_id;			//设备ID号
    GUInt32 manage_server_ip;
    GUInt32 video_server_ip;
    GUInt16 net_type;//网络类型，比如电信，网通，长宽等
    GUInt16 camera_num;   //设备能接入的camera 数目
    //struct device_camera_port camera_id_list[0];
};

//7.IPC登陆
//请求：
struct _camera_2_login_req
{
    GUInt32 camera_id;
    GUInt16 net_type;//本地检测出的网络类型
};

//返回为成功时的包体：
struct _login_2_camera_ret
{
    GUInt32 custom_id;
    GUInt32 manage_server_ip;
    GUInt16 manage_server_port;
    GUInt16 net_type;//
    char session_id[128];
};

//8.查询ipc是否上线
//请求：
struct _client_online_query_2_login
{
    GUInt32 custom_id;
    GUInt32 user_id;
    GUInt32 device_id;
    GUInt32 camera_id;
};

//返回：未定

//9.终端设备添加
struct _terminal_device_add_req
{
    GUInt32 user_id;
    char dev_uuid[256];
    char dev_model[256];
    char dev_osversion[256];
};
//返回为成功时的包体：无

//10.终端设备查询
struct _terminal_device_query_req
{

    GUInt32 user_id;
    char dev_uuid[256];
};
//成功时返回：
struct _terminal_device_query_ret
{
    GUInt32 dev_id;
};

//11.终端设备异常信息添加
struct _terminal_exception_add_req
{
    GUInt32 dev_id;
    char exception[256];
};
//返回为成功时的包体：无
//12.用户添加设备
struct _client_device_add_req
{
    GUInt32 custom_id;	  	//注意只有主账号才能添加
    char device_sn[64];    //摄像头uuid
    char device_name[64];  // add
    char device_passwd[64];  //add
};

//返回为成功时的包体：
struct _client_device_add_ret
{
   GUInt32 device_id;      //从redis（从camera）
   GUInt32 camera_id;
};
//13.用户设备查询
struct _client_device_query_req
{
    GUInt32 user_id;		//moded
};

//返回为成功时的包体：(device_id)
struct _client_device_query_ret
{
   GUInt32 device_id[MAX_UUID_COUNT];
};
//返回为成功时的包体：(device_sn)
struct _device_sn_name_passwd
{
    char device_sn[12];
    char device_name[64];
    char device_passwd[64];
};
struct _client_device_query_2_ret
{
    struct _device_sn_name_passwd dev_info[MAX_UUID_COUNT];   //moded
};
//其中：


//14. 用户设备解绑
struct _client_device_unbundle_req
{
GUInt32 custom_id;
char device_sn[64];  //摄像头uuid  // or device_id
};
//返回为成功时的包体：无

//15.APP版本信息
struct _client_device_unbundle_ret
{
GUInt16 user_type;	// 0 表示ipc365; 1表示iup365
};

//返回为成功时的包体：
struct _query_app_version_ret
{
char version[32];
char feature[MAX_VERSIONFEATURE_LEN];
};

#include <string>
//消息头部：
struct ap_header
{
    GUInt8		HeadFlag;				/* head flag = 0xFF */
    GUInt8		Reserved0;				/* reserved0 */
    GUInt8		Reserved1;				/* reserved1 */
    GUInt8		Reserved2;				/* reserved2 */
    GUInt8		Version;				/* version */
    GUInt8		Reserved3;				/* reserved3 */
    GUInt8		Reserved4;				/* reserved4 = 0x5A*/
    GUInt8		Reserved5;				/* reserved5 = 0xA5*/
    GUInt32		SID;					/* session ID */
    GUInt32		Seq;					/* sequence number */
    GUInt16		MsgId;					/* mesage id */
    union
    {
        struct
        {
            GUInt8	TotalPacket;		/* total packet */
            GUInt8	CurPacket;			/* current packet */
        }c;

        struct
        {
            GUInt8	Channel;			/* channel */
            GUInt8	EndFlag;			/* end flag */
        }m;
    };
    GUInt32		DataLen;				/* data len */
};

struct ap_dev_login_req
{
    std::string sUserName;		///< 登陆名字
    std::string sPassword;		///< 登陆密码
    int iEncryptType;			///< 密码加密方式，加密或者不加密
    int iLoginType;				///< 登陆方式
};

/// 登陆请求响应   消息号11001
struct ap_dev_login_ret
{
    int iChannelNum;		///< 机器路数
    int iAliveInterval;		///< 保活周期(s)
    int iRet;				///< 返回值
    int iDeviceType;		///< 设备类型
    GUInt32 uiSessionId;	///< 会话ID
    int iExtraChannel;		///< 扩展通道数
};
// NTP

typedef union {	//IP addr
    unsigned char	c[4];
    unsigned short	s[2];
    unsigned int	l;
}ip_address;
struct remote_server_config
{
    char ServerName[NAME_PASSWORD_LEN];	///< 服务名
    ip_address ip;						///< IP地址
    int Port;							///< 端口号
    char UserName[NAME_PASSWORD_LEN];		///< 用户名
    char Password[NAME_PASSWORD_LEN];		///< 密码
    bool Anonymity;							///< 是否匿名登录
};

struct net_ntp_config
{
    ///< 是否开启
    bool Enable;
    ///< PPPOE服务器
    remote_server_config Server;
    ///< 更新周期
    int UpdatePeriod;
    ///< 时区
    int TimeZone;
};



#endif // PACKPROTOCOLTYPE_H
