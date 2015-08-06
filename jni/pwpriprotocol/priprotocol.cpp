#include"priprotocol.h"
#include"APTcpConnect.h"
#include"ApUdpBroadcastset.h"
#include"APUdpBroadcast.h"
#include<string>
using namespace std;
ApUdpBroadcastSet set_broadcast;
APUdpBroadcast search_broadcast;
GBool PW_PRI_BroadcastToDev(char* data,char *ssid, int nssid, char *pwd, int npwd, char *keytype, int nkey){
   set_broadcast.broadcastToDev(data,ssid,nssid,pwd,npwd,keytype,nkey);
}
GBool PW_PRI_setPauseBroadcast(bool bpause){
	set_broadcast.setPauseBroadcast(bpause);
}
GBool PW_PRI_stopBroadcast(){
	set_broadcast.stopBroadcast();
}

GBool PW_PRI_BroadcastToDevRecycle(char *ssid,char *pwd,int mSec){
    return set_broadcast.broadcastRecycle(ssid,pwd,mSec);
}

GBool PW_PRI_SearchDeviceInLanWithTime(char mac[][32],int maxCount,int *nSearchDev,int searchSecTime){
    return search_broadcast.searchDevInLan(mac,maxCount,nSearchDev,searchSecTime);
}

GInt64 PW_PRI_DevLogin(char *ip){
    struct LoginRequest req;
    req.sUserName = "admin";
    req.sPassword = "nTBCS19C";
    req.iLoginType = LOGIN_TYPE_WEB;
    req.iEncryptType = PASSWORD_FLAG_MD5;
    std::string strConfigInfo;
    TExchangeAL<struct LoginRequest>::serizalConfig(req, strConfigInfo);
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    GInt64 LoginID = m_client->devLogin(ip, (char *)(strConfigInfo.data()));
    if(LoginID < 0){
        LoginID = m_client->devLogin(ip, (char *)(strConfigInfo.data()));
    }
    if(LoginID < 0){
        LoginID = m_client->devLogin(ip, (char *)(strConfigInfo.data()));
    }
   if(LoginID < 0){
       //m_iError=m_client->getLastErr();
       return -1;
   }
   else{
       return LoginID;
   }
}
GInt32 PW_PRI_GetLastError(){
    return  search_broadcast.getLastErr();
}

void PW_PRI_DevLogout(GInt64 LoginID){
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    m_client->devLogout(LoginID);
}

GBool PW_PRI_DevSetTimezone(GInt64 LoginID,int timezone){
    struct NetNTPConfig ntp;
    memset(&ntp,0,sizeof(struct NetNTPConfig));
    int sessionid = LoginID & 0xFFFFFFFF;
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    DefaultRequest defaultconfig;
    string strConfigInfo_;
    defaultconfig.uiSessionId = sessionid;
    TExchangeAL<DefaultRequest>::serizalConfig(defaultconfig, strConfigInfo_);

    bool bRet=m_client->devNtpGet(LoginID,(char*)strConfigInfo_.data(),(char *)(&ntp));
    if(!bRet){
        return false;
    }
    long msPerH=3600000;
    long timezoneArr[33]={    0,      1*msPerH,   2*msPerH,    3*msPerH,  (long)(3.5*msPerH), 4*msPerH,  (long)(4.5*msPerH),\
                          5*msPerH,   (long)(5.5*msPerH), (long)(5.75*msPerH), 6*msPerH,  (long)(6.5*msPerH),7*msPerH,   8*msPerH,  9*msPerH,\
                          (long)(9.5*msPerH), 10*msPerH,  11*msPerH,   12*msPerH, 13*msPerH,\
                          -1*msPerH,  -2*msPerH,  -3*msPerH,  (long)(-3.5*msPerH), -4*msPerH,  -5*msPerH,\
                          -6*msPerH,  -7*msPerH,  -8*msPerH,  -9*msPerH,   -10*msPerH, -11*msPerH, -12*msPerH};
    int indexParam=0;
    for(int i=0;i<19;i++){
        if(timezone==timezoneArr[i]){
            indexParam=i;
            break;
        }
        if(((timezone-timezoneArr[i]) < (timezoneArr[i+1]-timezoneArr[i])/2.0) && \
                ((timezone-timezoneArr[i+1])<0) && ((timezone-timezoneArr[i])>0)){
            indexParam=i;
            break;
        }
        if(((timezone-timezoneArr[i]) > (timezoneArr[i+1]-timezoneArr[i])/2.0) && \
                ((timezone-timezoneArr[i+1])<0) && ((timezone-timezoneArr[i])>0)){
            indexParam=i+1;
            break;
        }
    }
    for(int i=20;i<32;i++){
        if(timezone==timezoneArr[i]){
            indexParam=i;
            break;
        }
        if(((timezone-timezoneArr[i]) > (timezoneArr[i+1]-timezoneArr[i])/2.0) && \
                ((timezone-timezoneArr[i+1])>0) && ((timezone-timezoneArr[i])<0)){
            indexParam=i;
            break;
        }
        if(((timezone-timezoneArr[i]) < (timezoneArr[i+1]-timezoneArr[i])/2.0) && \
                ((timezone-timezoneArr[i+1])>0) && ((timezone-timezoneArr[i])<0)){
            indexParam=i+1;
            break;
        }
    }
    if(timezone >= 13*msPerH){
        indexParam=18;
    }
    if(timezone<=-12*msPerH){
        indexParam=31;
    }
    ntp.Enable=true;
    ntp.TimeZone=indexParam;
    std::string strConfigInfo;
    TExchangeAL<struct NetNTPConfig>::serizalConfig(sessionid, ntp, "NetWork.Ntp", strConfigInfo);
    bRet=m_client->devNtpSet(LoginID,(char*)strConfigInfo.data());
    if(!bRet){
        return false;
    }
    return true;
}

GBool PW_PRI_DevSetOSD(GInt64 LoginID,char * osdName){
    struct CONFIG_VIDEOWIDGET  osd;
    memset(&osd,0,sizeof(struct CONFIG_VIDEOWIDGET));
    int sessionid = LoginID & 0xFFFFFFFF;
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    DefaultRequest questConfig;
    questConfig.uiSessionId = sessionid;
    questConfig.sName = "AVEnc.VideoWidget.[0]";
    string getConfigInfo;
    TExchangeAL<DefaultRequest>::serizalConfig(questConfig, getConfigInfo);
    bool bret=m_client->devOsdGet(LoginID,(char*)getConfigInfo.data(),(char*)(&osd));
    if(!bret){
        return false;
    }

    strcpy(osd.ChannelName.strName,osdName);
    std::string strGetConfigInfo;

    TExchangeAL<struct CONFIG_VIDEOWIDGET>::serizalConfig(sessionid, osd, "AVEnc.VideoWidget.[0]", strGetConfigInfo);
    bret=m_client->devOsdSet(LoginID,(char*)(strGetConfigInfo.data()));
    if(!bret){
        return false;
    }
    return true;
}

GBool PW_PRI_DevSetWifi(GInt64 LoginID, LPPW_PHONE_BL_WIFI_CONFIG config){
    struct NetWifiConfig req;
    req.bEnable = config->bEnable;
    req.strSSID = config->sSSID;
    req.nChannel = config->nChannel;
    req.strNetType = config->sNetType;
    req.strEncrypType = config->sEncrypType;
    req.strAuth = config->sAuth;
    req.nKeyType = config->nKeyType;
    req.strKeys = config->sKeys;
    memcpy(req.HostIP.c, config->HostIP, sizeof(IPAddress));
    memcpy(req.Submask.c, config->Submask, sizeof(IPAddress));
    memcpy(req.Gateway.c, config->Gateway, sizeof(IPAddress));

    int sessionid = LoginID & 0xFFFFFFFF;
    std::string strConfigInfo;
    TExchangeAL<struct NetWifiConfig>::serizalConfig(sessionid, req, "NetWork.Wifi", strConfigInfo);
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    m_client->devSetWifi(LoginID, (char *)(strConfigInfo.data()));
    return true;
}

GBool PW_PRI_isDevConnectedInApModel(GInt64 LoginID){
    int sessionid = LoginID & 0xFFFFFFFF;

    struct DefaultRequest defaultconfig;
    defaultconfig.sName = "KeepAlive";
    defaultconfig.uiSessionId = sessionid;

    std::string strConfigInfo;
    TExchangeAL<struct DefaultRequest>::serizalConfig(defaultconfig, strConfigInfo);
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    return m_client->devOnlineStatus(LoginID, (char *)(strConfigInfo.data()));
}

GInt32 PW_PRI_GetConnectedRouterStatus(GInt64 LoginID){
    int sessionid = LoginID & 0xFFFFFFFF;

    struct DefaultRequest defaultconfig;
    defaultconfig.sName = "NetWork.WifiStatus";
    defaultconfig.uiSessionId = sessionid;

    std::string strConfigInfo;
    TExchangeAL<struct DefaultRequest>::serizalConfig(defaultconfig, strConfigInfo);
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    return m_client->devWifiStatus(LoginID, (char *)(strConfigInfo.data()));
}

GBool PW_PRI_DevSetDHCP(GInt64 LoginID, GBool bOpen){
    struct NetDHCPConfigAll dhcp;
    memset(&dhcp,0,sizeof(struct NetDHCPConfigAll));
    int sessionid = LoginID & 0xFFFFFFFF;
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    DefaultRequest defaultconfig;
    string strConfigInfo_;
    defaultconfig.uiSessionId = sessionid;
    defaultconfig.sName = "NetWork.NetDHCP";
    TExchangeAL<DefaultRequest>::serizalConfig(defaultconfig, strConfigInfo_);

    bool bRet=m_client->devDhcpGet(LoginID,(char*)strConfigInfo_.data(),(char *)(&dhcp));
    if(!bRet){
        return false;
    }
    dhcp.vNetDHCPConfig[3].bEnable = bOpen;
    sprintf(dhcp.vNetDHCPConfig[3].ifName, "eth3");
    std::string strConfigInfo;
    TExchangeAL<struct NetDHCPConfigAll>::serizalConfig(sessionid, dhcp, "NetWork.DHCP", strConfigInfo);
    bRet=m_client->devDhcpSet(LoginID,(char*)strConfigInfo.data());
    if(!bRet){
        return false;
    }
    return true;
}

GBool PW_PRI_DevAlive(GInt64 LoginID){

}

GBool PW_PRI_DevAPSwap(GInt64 LoginID){
    int sessionid = LoginID & 0xFFFFFFFF;

    struct NetAPSwapWifi req;
    req.nAPSwapWifi = 1;
    std::string strConfigInfo;
    TExchangeAL<struct NetAPSwapWifi>::serizalConfig(sessionid, req, "NetWork.APSwapWifi", strConfigInfo);
    CAPTcpConnect *m_client = CAPTcpConnect::getInstance();
    return m_client->devAPSwap(LoginID, (char *)(strConfigInfo.data()));

}
