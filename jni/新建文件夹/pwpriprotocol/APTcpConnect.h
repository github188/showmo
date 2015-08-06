#ifndef APTCPCONNECT_H
#define APTCPCONNECT_H
//#include "proto.h"
#include "PackProtocolType.h"
#include<errno.h>
#include "./exchange/code/Include/ExchangeAL/Exchange.h"
#include "./exchange/code/Include/ExchangeAL/ManagerExchange.h"
#include "./exchange/code/Include/ExchangeAL/NetWorkExchange.h"
#include <mutex>
#ifdef __ANDROID__
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<unistd.h>
    #include<arpa/inet.h>
#elif __APPLE__
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<unistd.h>
    #include<arpa/inet.h>
#endif

#define APTCPCONNECT_DEV_LOGIN  1
#define APTCPCONNECT_DEV_SETWIFI  2
#define APTCPCONNECT_GET_DHCP 3
#define APTCPCONNECT_SET_DHCP 8
#define APTCPCONNECT_GET_NTP 4
#define APTCPCONNECT_SET_NTP 5
#define APTCPCONNECT_GET_OSD 6
#define APTCPCONNECT_SET_OSD 7
#define APTCPCONNECT_GET_ONLINE_STATUS 9
#define APTCPCONNECT_GET_WIFI_STATUS 10
#define APTCPCONNECT_SET_AP_SWAP 11
class CAPTcpConnect
{
public:
    static CAPTcpConnect *getInstance();
    long long devLogin(char *ip, char *in,int waitTime=10000);  //return -1失败
    void devLogout(long long LoginID,int waitTime=2000);
    bool devSetWifi(long long LoginID, char *in,int waitTime=2000);
    bool devDhcpGet(long long LoginID, char *in, char *out,int waitTime=2000);
    bool devDhcpSet(long long LoginID, char *in,int waitTime=2000);
    bool devNtpGet(long long LoginID,char * in,char *out,int waitTime=2000);
    bool devNtpSet(long long LoginID,char *in,int waitTime=2000);

    bool devOsdGet(long long LoginID,char * in,char *out,int waitTime=2000);
    bool devOsdSet(long long LoginID,char *in,int waitTime=2000);
    bool devOnlineStatus(long long LoginID,char *in,int waitTime=2000);
    int  devWifiStatus(long long LoginID, char *in, int waitTime=2000);
    bool devAPSwap(long long LoginID,char *in,int waitTime=2000);
    //bool dhcpConfig(long long LoginID,E_SDK_CONFIG_NET_DHCP,0,(char *)m_pDhcp,sizeof(PW_NET_DHCP_CONFIG_ALL),2500);

    int getLastErr()const;


private:
    CAPTcpConnect();
    bool interactWithDev(int clientSocket, int sessionID, void *in, void *out, int type,int waitTime=2000);
    int  create_connect(char *ip);

    int err;
    struct _msg_status err_rt;
    static CAPTcpConnect* m_instance;
    std::mutex m_mtx;
};




#endif // APTCPCONNECT_H
