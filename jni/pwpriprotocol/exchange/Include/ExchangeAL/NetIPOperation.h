#ifndef __EXCHANGE_AL_NETIP_OPERATION_H__
#define __EXCHANGE_AL_NETIP_OPERATION_H__

#include "ExchangeKind.h"
#include "MediaExchange.h"
#include "../Types/Defs.h"
#include "../Infra/Time.h"
#include <string>
#include <vector>

/// ������������
enum NetOperationErrorNo
{
	NET_OPERATION_COMMAND_INVALID = ERROR_BEGIN_NETOPERATION + 1,			///< ����Ϸ�
	NET_OPERATION_TALK_ALAREADY_START = ERROR_BEGIN_NETOPERATION + 2,		///< �Խ��Ѿ�����
	NET_OPERATION_TALK_NOT_START = ERROR_BEGIN_NETOPERATION + 3,			///< �Խ�δ����
	NET_OPERATION_UPGRADE_ALAREADY_START = ERROR_BEGIN_NETOPERATION + 10,   ///< �Ѿ���ʼ����
	NET_OPERATION_UPGRADE_NOT_START = ERROR_BEGIN_NETOPERATION + 11,		///< δ��ʼ����
	NET_OPERATION_UPGRADE_DATA_ERROR = ERROR_BEGIN_NETOPERATION + 12,		///< �������ݴ���
	NET_OPERATION_UPGRADE_FAILED = ERROR_BEGIN_NETOPERATION + 13,			///< ����ʧ��
	NET_OPERATION_UPGRADE_SUCCESS = ERROR_BEGIN_NETOPERATION + 14,			///< �����ɹ�
	NET_OPERATION_SETDEFAULT_FAILED = ERROR_BEGIN_NETOPERATION + 20,		///< ��ԭĬ��ʧ��
	NET_OPERATION_SETDEFAULT_REBOOT = ERROR_BEGIN_NETOPERATION + 21,		///< ��Ҫ�����豸
	NET_OPERATION_SETDEFAULT_VALIDATEERROR = ERROR_BEGIN_NETOPERATION + 22,	///< Ĭ�����÷Ƿ�
};

/// ���Ӷ���
enum MonitorAction
{
	MONITOR_ACTION_START,
	MONITOR_ACTION_STOP,
	MONITOR_ACTION_CLAIM,	/// ��ʶ������Ϣ,������ͨ����Щ��Ϣ������ý������
	MONITOR_ACTION_PAUSE,   /// ��ͣ
	MONITOR_ACTION_CONTINUE,   /// �ָ�
	MONITOR_ACTION_REQUEST,
	MONITOR_ACTION_NR
};

/// ����ģʽ
enum MonitorTansMode
{
	MONITOR_TRANSMODE_TCP,		///< TCP����
	MONITOR_TRANSMODE_UDP,		///< UDP����
	MONITOR_TRANSMODE_MCAST,	///< �ಥ
	MONITOR_TRANSMODE_RTP,		///< RTP����
	MONITOR_TRANSMODE_NR
};

/// ��ϱ���ģʽ
enum CombinType
{
	COMBIN_NONE,
	COMBIN_1,
	COMBIN_2,
	COMBIN_3,
	COMBIN_4,
	COMBIN_5,
	COMBIN_6,
	COMBIN_7,
	COMBIN_8,
	COMBIN_9,
	COMBIN_10,
	COMBIN_11,
	COMBIN_12,
	COMBIN_13,
	COMBIN_14,
	COMBIN_15,
	COMBIN_16,
	COMBIN_1_4,
	COMBIN_5_8,
	COMBIN_9_12,
	COMBIN_13_16,
	COMBIN_1_8,
	COMBIN_9_16,
	COMBIN_1_9,
	COMBIN_8_16,
	COMBIN_1_16,
	CONNECT_ALL = 0xFF
};

/// ���ӿ���
struct MonitorControl
{
	int iAction;		///< ���Ӷ���MonitorAction
	int iChannel;		///< ����ͨ��
	int iStreamType;	///< ������������
	int iTransMode;		///< ���Ӵ���ģʽ
	int iCombinType;	///< ��ϱ���ģʽ
};

/// �طŶ���
enum PlayBackAction
{
	PLAY_BACK_ACTION_START,		/// ��ʼ�ط� 
	PLAY_BACK_ACTION_STOP,		/// ֹͣ�ط�,���ر�����
	PLAY_BACK_ACTION_PAUSE,		/// ��ͣ�ط�
	PLAY_BACK_ACTION_CONTINUE,	/// �����ط�
	PLAY_BACK_ACTION_LOCATE,	/// �طŶ�λ
	PLAY_BACK_ACTION_EOF,		/// �ط��ļ�����,��PU֪ͨCU
	PLAY_BACK_ACTION_CLOSE,		/// �رջط�, ��Ͽ�����
	PLAY_BACK_ACTION_CLAIM,		/// ��ʶ������Ϣ,������ͨ����Щ��Ϣ������ý������
	PLAY_BACK_ACTION_DOWNLOADSTART,	/// ¼�����ؿ�ʼ
	PLAY_BACK_ACTION_DOWNLOADSTOP,	/// ¼�����ؽ���
	PLAY_BACK_ACTION_FAST,	    /// ���ٻط�
	PLAY_BACK_ACTION_SLOW,	    /// ���ٻط�
	PLAY_BACK_ACTION_REQUEST,	///�ط���������ע����
	PLAY_BACK_ACTION_DOWNLOAD_REQUEST,///������������ע����
	PLAY_BACK_ACTION_DOWNLOAD_PAUSE,	///������ͣ
	PLAY_BACK_ACTION_DOWNLOAD_CONTINUE,///���ؼ���
	PLAY_BACK_ACTION_NR
};

/// �ط�ģʽ
enum PlaybackMode
{
	PLAYBACK_BY_NAME,		///< ���ļ����ط�
	PLAYBACK_BY_TIME,		///< ��ʱ��ط�
	PLAYBACK_MODE_NR
};

/// �طſ���
struct PlayBackControl
{
	int iAction;	///< �طŶ���
	int iTransMode;	///< ����ģʽ
	std::string sFileName;
	SystemTime stStartTime;	///< ��ʼʱ��
	SystemTime stEndTime;		///< ����ʱ��
	int iPlayMode;	///< �ط�ģʽ����PlaybackMode
	int iValue;	    ///< ���ֻطŶ����в�������
};

/// ����ģʽ״̬
enum PtzControlSetPatternStatusTypes
{
	PTZ_PATTERN_STATUS_START,		///< ����ģʽ��ʼ
	PTZ_PATTERN_STATUS_STOP,		///< ����ģʽ����
	PTZ_PATTERN_STATUS_NR,
};

/// ģʽ״̬
enum PtzControlPatternTypes
{
	PTZ_PATTERN_RUN,		///< ����ģʽ
	PTZ_PATTERN_STOP,		///< ֹͣģʽ
	PTZ_PATTERN_CLEAR,		///< ���ģʽ
	PTZ_PATTERN_NR,
};

/// �˵�����
enum PtzMenuOperator
{
	PTZ_MENU_OPT_ENTER,		///< ����˵�
	PTZ_MENU_OPT_LEAVE,		///< �˳��˵�
	PTZ_MENU_OPT_OK,		///< ȷ��
	PTZ_MENU_OPT_CANCEL,	///< ȡ��
	PTZ_MENU_OPT_UP,		///< ��
	PTZ_MENU_OPT_DOWN,		///< ��
	PTZ_MENU_OPT_LEFT,		///< ��
	PTZ_MENU_OPT_RIGHT,		///< ��
	PTZ_MENU_OPT_NR,
};

/// ��̨������������
enum PtzOperationCommand
{
	PTZ_OPERATION_DIRECTION_UP,		///< ����
	PTZ_OPERATION_DIRECTION_DOWN,
	PTZ_OPERATION_DIRECTION_LEFT,
	PTZ_OPERATION_DIRECTION_RIGHT,
	PTZ_OPERATION_DIRECTION_LEFTUP,
	PTZ_OPERATION_DIRECTION_LEFTDOWN,
	PTZ_OPERATION_DIRECTION_RIGHTUP,
	PTZ_OPERATION_DIRECTION_RIGHTDOWN,
	PTZ_OPERATION_ZOOMWIDE,			///< �䱶 
	PTZ_OPERATION_ZOOMTILE,
	PTZ_OPERATION_FOCUSFAR,			///< �۽� 
	PTZ_OPERATION_FOCUSNEAR,
	PTZ_OPERATION_IRISLARGE,		///< ��Ȧ 
	PTZ_OPERATION_IRISSMALL,
	PTZ_OPERATION_ALARM,			///< �������� 
	PTZ_OPERATION_LIGHTON,			///< �ƹ⿪
	PTZ_OPERATION_LIGHTOFF,			///< �ƹ��
	PTZ_OPERATION_SETPRESET,		///< ����Ԥ�õ� 
	PTZ_OPERATION_CLEARPRESET,		///< ���Ԥ�õ� 
	PTZ_OPERATION_GOTOPRESET,		///< ת��Ԥ�õ� 
	PTZ_OPERATION_AUTOPANON,		///< ˮƽ��ʼ 
	PTZ_OPERATION_AUTOPANOFF,		///< ˮƽ���� 
	PTZ_OPERATION_SETLIMITLEFT,		///< ������߽� 
	PTZ_OPERATION_SETLIMITRIGHT,	///< �����ұ߽�
	PTZ_OPERATION_AUTOSCANON,		///< �Զ�ɨ�迪ʼ
	PTZ_OPERATION_AUTOSCANOFF,		///< �Զ�ɨ��ֹͣ 
	PTZ_OPERATION_ADDTOUR,			///< ����Ѳ���� 
	PTZ_OPERATION_DELETETOUR,		///< ɾ��Ѳ���� 
	PTZ_OPERATION_STARTTOUR,		///< ��ʼѲ�� 
	PTZ_OPERATION_STOPTOUR,			///< ����Ѳ�� 
	PTZ_OPERATION_CLEARTOUR,		///< ɾ��Ѳ�� 
	PTZ_OPERATION_POSITION,			///< ���ٶ�λ 
	PTZ_OPERATION_AUX,				///< �������� 
	PTZ_OPERATION_MENU,				///< ����˵�����
	PTZ_OPERATION_FLIP,				///< ��ͷ��ת
	PTZ_OPERATION_RESET,			///< ��̨��λ
	PTZ_OPERATION_OPT_NUM			///< �����ĸ��� 
};

/// ��̨��������
enum PtzAuxStatus
{
	PTZ_AUX_ON,		///< ��
	PTZ_AUX_OFF,	///< ��
	PTZ_AUX_NR,		
};

struct PTZControl
{
	int iCommand;				///< ��̨��������

	/// ��̨��������
	struct OperationParam
	{	
		int iChannel;	///< ��̨���Ƶ�ͨ����ͨ����0��ʼ
		int iStep;		///< ����
		int iPreset;	///< Ԥ�õ�
		int iTour;		///< ·�߱��
		int iPattern;	///< ģʽ,��ʼģʽ��ֹͣģʽ�����ģʽ
		int iMenuOpts;		///< �˵����������룬�˳���ȷ�ϣ�ȡ�����ϣ��£�����
		struct 
		{
			int iNumber;
			int iStatus;	/// ����״̬����PtzAuxStatus
		} AUX;				///< ��������
	} parameter;
};

/// ������������
enum MachineAction
{
	MACHINE_ACTION_REBOOT,		///< ��������
	MACHINE_ACTION_SHUTDOWN,	///< �رջ���
	MACHINE_ACTION_NR,
};

/// �������رղ���
struct MachineControl
{
	int iAction;	///< �������ƶ�������MachineAction
};

/// Ĭ�����ò���
struct DefaultConfigControl
{
	bool vDefaultConfig[DEFAULT_CFG_END];
};

/// �����Խ�����
enum TalkControlTypes
{
	TALK_CONTROL_TYPES_START,	///< ��ʼ�Խ�
	TALK_CONTROL_TYPES_STOP,	///< ֹͣ�Խ�
	TALK_CONTROL_TYPES_CLAIM,		/// ��ʶ������Ϣ,������ͨ����Щ��Ϣ������ý������
	TALK_CONTROL_TYPES_REQUEST,
	TALK_CONTROL_TYPES_NR,
};

/// �����Խ�
struct TalkControl
{
	int iAction;
	AudioInFormatConfig afAudioFormat;
};

/// �洢�豸��������
enum StorageDeviceControlTypes
{
	STORAGE_DEVICE_CONTROL_SETTYPE,		///< ��������
	STORAGE_DEVICE_CONTROL_RECOVER,		///< �ָ�����
	STORAGE_DEVICE_CONTROL_PARTITIONS,	///< ��������
	STORAGE_DEVICE_CONTROL_CLEAR,		///< �������
	STORAGE_DEVICE_CONTROL_NR,
};

/// ���������������
enum StorageDeviceClearTypes
{
	STORAGE_DEVICE_CLEAR_DATA,			///< ���¼������
	STORAGE_DEVICE_CLEAR_PARTITIONS,	///< �������
	STORAGE_DEVICE_CLEAR_NR,
};

/// �洢�豸����
struct StorageDeviceControl
{
	int iAction;	///< ��enum StorageDeviceControlTypes
	int iSerialNo;	///< �������к�
	int iPartNo;    ///< ������
	int iType;		///< enum StorageDeviceClearTypes����DRIVER_TYPE
	int iPartSize[2/*MAX_DRIVER_PER_DISK*/];	///< ���������Ĵ�С
};

/// ��־��������
enum LogControlActionTypes
{
	LOG_CONTROL_REMOVEALL,		/// ɾ�������������־
	LOG_CONTROL_NR,
};

/// ��־����
struct LogControl
{
	int iAction;
};

/// ������������
enum UpgradeActionTypes
{
	UPGRADE_ACTION_TYPES_START,		///< ��ʼ����
	UPGRADE_ACTION_TYPES_ABORT,		///< ��ֹ����
	UPGRADE_ACTION_TYPES_NR
};

/// ��������
enum UpgradeTypes
{
	UPGRADE_TYPES_SYSTEM,	///< ����ϵͳ
	UPGRADE_TYPES_NR,
};

/// ϵͳ����
struct UpgradeControl
{
	int iAction;
	int iType;
};

/// ������Ϣ��ȡ
struct UpgradeInfo
{
	std::string strSerial;
	std::string strHardware;
	std::string strVendor;
	uint uiLogoArea[2];
};

/// �ļ���ѯ����
struct SearchCondition
{
	int iChannel;			///< ¼��ͨ��������
	char sType[24];			///< ��ѯ�ļ�����
	char sEvent[32];			///< ��ѯ�¼�����
	uint uiDriverTypeMask;		///< ��ѯ��������������
	SystemTime stBeginTime;	///< ��ѯ��ʼʱ��
	SystemTime stEndTime;		///< ��ѯ����ʱ��
};

enum 
{
	MAX_SEARCHED_FILES = 64,	///< һ�����Ĳ�ѯ�ļ�����
};

/// �ļ��б���Ϣ
struct FileList
{
	int iNumFiles;		///< �ļ�����
	struct FileInfo
	{
		int iDiskNo;	///< �����������
		int iSerialNo;	///< �������к�
		uint uiFileLength;			///< �ļ����ȣ���KΪ��λ
		char sFileName[108];		///< �ļ���
		SystemTime stBeginTime;	///< �ļ���ʼʱ��
		SystemTime stEndTime;		///< �ļ�����ʱ��
	} Files[MAX_SEARCHED_FILES];
};

//��ʱ��β�ѯ
struct SearchByTime
{
	int nHighChannel;			///< 33~64¼��ͨ��������
	int nLowChannel;			///< 1~32¼��ͨ��������
	char sType[24];			///< ��ѯ�ļ�����
	char sEvent[32];			///< ��ѯ�¼�����
	SystemTime stBeginTime;	    ///< ��ѯ��ʼʱ��
	SystemTime stEndTime;		///< ��ѯ����ʱ��
	int    iSync;               ///< �Ƿ���Ҫͬ��
};


//ÿ��ͨ����¼����Ϣ
struct SearchByTimeInfo
{
	int iChannel;			    ///< ¼��ͨ����
	///< ¼���¼��720���ֽڵ�5760λ����ʾһ���е�1440����
	///< 0000:��¼�� 0001:F_COMMON 0002:F_ALERT 0003:F_DYNAMIC 0004:F_CARD 0005:F_HAND
	uchar cRecordBitMap[720];
};

struct SearchByTimeResult
{
	int nInfoNum;			          ///< ͨ����¼���¼��Ϣ����
	SearchByTimeInfo ByTimeInfo[N_SYS_CH];    ///< ͨ����¼���¼��Ϣ
};

/// ��־��ѯ����
enum LOG_SEARCH_KIND
{
	LOG_SEARCH_KIND_TYPE_ALL,		///< ������־����
	LOG_SEARCH_KIND_SYSTEM,			///< ϵͳ��־
	LOG_SEARCH_KIND_TYPE_CONFIG,	///< ������־
	LOG_SEARCH_KIND_TYPE_STORAGE,	///< �洢��־
	LOG_SEARCH_KIND_TYPE_ALAEM,		///< ������־
	LOG_SEARCH_KIND_TYPE_RECORD,	///< ¼����־
	LOG_SEARCH_KIND_TYPE_ACCOUNT,	///< �û�����
	LOG_SEARCH_KIND_TYPE_FILE,		///< �ʼ۲���
	LOG_SEARCH_KIND_TYPE_NR,
};

/// ��־��ѯ����
struct LogSearchCondition
{
	int iType;					///< ��־����,��LOG_SEARCH_KIND
	int iLogPosition;			///< ���ϴβ�ѯ�Ľ���ʱ����־ָ��
	SystemTime stBeginTime;	///< ��ѯ��־��ʼʱ��
	SystemTime stEndTime;		///< ��ѯ��־����ʱ��
};

enum 
{
	MAX_RETURNED_LOGLIST = 128,	///< һ�β�ѯ����������־����
};

struct LogList
{
	int iNumLog;
	struct LogItem
	{
		char sType[24];	///< ��־����
		char sUser[32];	///< ��־�û�
		char sData[68];	///< ��־����
		SystemTime stLogTime;	///< ��־ʱ��
		int iLogPosition;
	} Logs[MAX_RETURNED_LOGLIST];
};

/// ʱ�䶯������
enum EventActionTypes
{
	EVENT_ACTION_START  = 0,		// �¼���ʼ
	EVENT_ACTION_STOP,				// �¼�����
	EVENT_ACTION_CONFIG,			// �¼����ñ仯������Ϊ���ղ���
	EVENT_ACTION_LATCH,				// �¼���ʱ����������Ϊ�������
	EVENT_ACTION_NR,
};

/// �����¼���
enum EventCodeTypes
{
	EVENT_CODE_INIT = 0,
	EVENT_CODE_LOCAL_ALARM = 1,
	EVENT_CODE_NET_ALARM,
	EVENT_CODE_MANUAL_ALARM,
	EVENT_CODE_VIDEO_MOTION,
	EVENT_CODE_VIDEO_LOSS,
	EVENT_CODE_VIDEO_BLIND,
	EVENT_CODE_VIDEO_TITLE,
	EVENT_CODE_VIDEO_SPLIT,
	EVENT_CODE_VIDEO_TOUR,
	EVENT_CODE_STORAGE_NOT_EXIST,
	EVENT_CODE_STORAGE_FAILURE,
	EVENT_CODE_LOW_SPACE,
	EVENT_CODE_NET_ABORT,
	EVENT_CODE_COMM,
	EVENT_CODE_STORAGE_READ_ERROR,
	EVENT_CODE_STORAGE_WRITE_ERROR,
	EVENT_CODE_NET_IPCONFLICT,
	EVENT_CODE_ALARM_EMERGENCY,
	EVENT_CODE_DEC_CONNECT,
	EVENT_CODE_NR,
};

struct AlarmInfo
{
	int nChannel;
	int iEvent;
	int iStatus;
	SystemTime SysTime;
};


/// ����ֵ, ����������
enum NetKeyBoardValue
{
	NET_KEY_0, NET_KEY_1, NET_KEY_2, NET_KEY_3, NET_KEY_4, NET_KEY_5, NET_KEY_6, NET_KEY_7, NET_KEY_8, NET_KEY_9,
	NET_KEY_10, NET_KEY_11, NET_KEY_12, NET_KEY_13, NET_KEY_14, NET_KEY_15, NET_KEY_16, NET_KEY_10PLUS,
	NET_KEY_UP = 20, NET_DOWN, NET_LEFT, NET_RIGHT, NET_KEY_SHIFT, NET_KEY_PGUP, NET_KEY_PGDN, NET_KEY_RET, NET_KEY_ESC, NET_KEY_FUNC,
	NET_KEY_PLAY, NET_KEY_BACK, NET_KEY_STOP, NET_KEY_FAST, NET_KEY_SLOW, NET_KEY_NEXT, NET_KEY_PREV,
	NET_KEY_REC = 40, NET_KEY_SEARCH, NET_KEY_INFO, NET_KEY_ALARM, NET_KEY_ADDR, NET_KEY_BACKUP, NET_KEY_SPLIT, NET_KEY_SPLIT1, NET_KEY_SPLIT4, NET_KEY_SPLIT8,
	NET_KEY_SPLIT9, NET_KEY_SPLIT16, NET_KEY_SHUT, NET_KEY_MENU,NET_KEY_SPLIT25,NET_KEY_SPLIT36,
	NET_KEY_PTZ = 60, NET_KEY_TELE, NET_KEY_WIDE, NET_KEY_IRIS_SMALL, NET_KEY_IRIS_LARGE, NET_KEY_FOCUS_NEAR, NET_KEY_FOCUS_FAR, NET_KEY_BRUSH, NET_KEY_LIGHT, NET_KEY_SPRESET,
	NET_KEY_GPRESET, NET_KEY_DPRESET, NET_KEY_PATTERN, NET_KEY_SCAN, NET_KEY_AUTOTOUR, NET_KEY_AUTOPAN,
};

/// ����״̬
enum NetKeyBoardState
{
	NET_KEYBOARD_KEYDOWN,	// ��������
	NET_KEYBOARD_KEYUP,		// �����ɿ�
};

struct NetKeyBoardData
{
	int iValue;
	int iState;
};

/// ���籨��
struct NetAlarmInfo
{
	int iEvent;   //�澯�¼����ͣ�Ŀǰδʹ��
	int iState;   //ÿbit��ʾһ��ͨ��,bit0:��һͨ��,0-�ޱ��� 1-�б���, ��������
};

/// �����ֶ�ץͼ
struct NetSnap
{
	int iChannel;  //ͨ����
};

enum TransCommOpr
{
	TRANS_COMM_START,
	TRANS_COMM_STOP,
};

enum TransCommType
{
	TRANS_COMM_RS232,
	TRANS_COMM_RS485,
};

struct TransparentComm
{
	int iTCommType;
	int iTCommOpr;
};

enum UpLoadDataType  //�ϴ���������
{
	VEHICLE_INFO,	//������Ϣ
};

struct UpLoadData		//�ϴ�����
{
	int InfoType;
};

enum NatStatusType
{
	NAT_STATUS_DISENABLE,
	NAT_STATUS_PROBING,
	NAT_STATUS_CONNECTING,
	NAT_STATUS_CONNECTED,
};

struct NatStatusInfo
{
	int	iNatStatus;
	std::string NatInfoCode;
};

#endif