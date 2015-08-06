#ifndef PRIPROTOCOL_H
#define PRIPROTOCOL_H
#include"pw_datatype.h"
#define PW_NET_IW_ENCODING_TOKEN_MAX 128
typedef struct
{
    GBool bEnable;
    GChar sSSID[36];								// SSID Number
    GInt nChannel;								// channel
    GChar sNetType[32];							// Infra, Adhoc
    GChar sEncrypType[32];						// NONE, WEP, TKIP, AES
    GChar sAuth[32];								// OPEN, SHARED, WEPAUTO, WPAPSK, WPA2PSK, WPANONE, WPA, WPA2
    GInt  nKeyType;								// 0:Hex 1:ASCII
    GChar sKeys[PW_NET_IW_ENCODING_TOKEN_MAX];
    GUChar HostIP[4];		///< host ip
    GUChar Submask[4];		///< netmask
    GUChar Gateway[4];		///< gateway
}PW_PHONE_BL_WIFI_CONFIG, *LPPW_PHONE_BL_WIFI_CONFIG;

GInt64 PW_PRI_DevLogin(char *ip);
void PW_PRI_DevLogout(GInt64 LoginID);
GBool PW_PRI_DevSetTimezone(GInt64 LoginID,int timezone);
GBool PW_PRI_DevSetOSD(GInt64 LoginID,char * osdName);
GBool PW_PRI_DevSetWifi(GInt64 LoginID, LPPW_PHONE_BL_WIFI_CONFIG config);
GBool PW_PRI_isDevConnectedInApModel(GInt64 LoginID);
GInt32 PW_PRI_GetConnectedRouterStatus(GInt64 LoginID);
GBool PW_PRI_DevSetDHCP(GInt64 LoginID, GBool bOpen);
GBool PW_PRI_DevAlive(GInt64 LoginID);
GBool PW_PRI_DevAPSwap(GInt64 LoginID);
GInt32 PW_PRI_GetLastError();

GBool PW_PRI_BroadcastToDev(char* data,char *ssid, int nssid, char *pwd, int npwd, char *keytype, int nkey);
void PW_PRI_StopBroadcastInfo();
GBool PW_PRI_SearchDeviceInLanWithTime(char mac[][32],int maxCount,int *nSearchDev,int searchSecTime=1);
GBool PW_PRI_BroadcastToDevRecycle(char *ssid,char *pwd,int mSec);
GBool PW_PRI_setPauseBroadcast(bool bpause);
GBool PW_PRI_stopBroadcast();

#endif // PRIPROTOCOL_H

