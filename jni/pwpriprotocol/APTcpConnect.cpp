#include "APTcpConnect.h"
#include <fcntl.h>

#define IPC_PORT    34567

CAPTcpConnect * CAPTcpConnect::m_instance=NULL;
CAPTcpConnect *CAPTcpConnect::getInstance()
{
    if(m_instance == NULL) {
        m_instance = new CAPTcpConnect();
    }

    return m_instance;
}

CAPTcpConnect::CAPTcpConnect()
{

}

int CAPTcpConnect::getLastErr()const{
    return err;
}

int CAPTcpConnect::create_connect(char *ip){
    int clientSocket=socket(AF_INET,SOCK_STREAM,0);
    int x=fcntl(clientSocket,F_GETFL,0);              // Get socket flags
    fcntl(clientSocket,F_SETFL,x | O_NONBLOCK);   // Add non-blocking flag
    if(clientSocket<0){
        err=_INVALIDSOCKET;
        return -1;
    }
    struct sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(34567);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    int c=connect(clientSocket,(const struct sockaddr*)&servaddr,sizeof(struct sockaddr_in));
    while(c<0){
        if(errno==EINPROGRESS){//ack ing
            break;
        }else{
            err=_CONNECTERR;
            return -1;
        }
    }
    return clientSocket;
}

long long CAPTcpConnect::devLogin(char *ip, char *in,int waitTime)
{
    int clientSocket = create_connect(ip);
    if(clientSocket<0){
        err=_INVALIDSOCKET;
        return -1;
    }

    int iSessionID = 0;
    if(!interactWithDev(clientSocket, 0, (void*)in,(void*)&iSessionID,APTCPCONNECT_DEV_LOGIN,waitTime)) {
        close(clientSocket);
        return -1;
    }
    long long lRes = clientSocket;
    return (lRes<<32|iSessionID);
}

void CAPTcpConnect::devLogout(long long LoginID,int waitTime)
{
    int clientSocket = LoginID>>32;
    if(clientSocket>=0) {
        close(clientSocket);
    }
}

bool CAPTcpConnect::devSetWifi(long long LoginID, char *in,int waitTime)
{
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,APTCPCONNECT_DEV_SETWIFI,waitTime);
}
bool CAPTcpConnect::devDhcpGet(long long LoginID, char *in, char *out,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,(void *)out,APTCPCONNECT_GET_DHCP,waitTime);
}

bool CAPTcpConnect::devDhcpSet(long long LoginID, char *in,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,APTCPCONNECT_SET_DHCP,waitTime);
}

bool CAPTcpConnect::devNtpGet(long long LoginID,char * in,char *out,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,(void *)out,APTCPCONNECT_GET_NTP,waitTime);
}

bool CAPTcpConnect::devNtpSet(long long LoginID,char *in,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,APTCPCONNECT_SET_NTP,waitTime);
}

bool CAPTcpConnect::devOsdGet(long long LoginID,char * in,char *out,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,(void *)out,APTCPCONNECT_GET_OSD,waitTime);
}

bool CAPTcpConnect::devOsdSet(long long LoginID,char *in,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,APTCPCONNECT_SET_OSD,waitTime);
}

bool CAPTcpConnect::devOnlineStatus(long long LoginID,char *in,int waitTime){
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,\
                           APTCPCONNECT_GET_ONLINE_STATUS,waitTime);
}

int CAPTcpConnect::devWifiStatus(long long LoginID, char *in, int waitTime)
{
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    int iStatus = 0;
    if(!interactWithDev(clientSocket, sessionID, (void*)in, (void*)&iStatus,APTCPCONNECT_GET_WIFI_STATUS,waitTime)) {
        iStatus = -1;
    }
    return iStatus;
}

bool CAPTcpConnect::devAPSwap(long long LoginID,char *in,int waitTime)
{
    int clientSocket = LoginID>>32;
    int sessionID = LoginID & 0xFFFFFFFF;
    return interactWithDev(clientSocket, sessionID, (void*)in,NULL,APTCPCONNECT_SET_AP_SWAP,waitTime);
}

bool CAPTcpConnect::interactWithDev(int clientSocket, int sessionID, void *in, void *out, int type,int waitTime){
    std::unique_lock<std::mutex> lck(m_mtx);
    char *rbufheader = NULL;
    char *rbufbody = NULL;
    char *buf = NULL;
    int rlen=0;
    int wlen=0;
    char *cur=buf;
    char *rcur=rbufheader;
    int bufLen=0;
    int headlen=sizeof(struct ap_header);
    int rbufLen_body=0;
    fd_set read_flags;
    fd_set write_flags;
    int selectRet;
    timeval m_waitTime;
    m_waitTime.tv_sec=waitTime/1000;
    m_waitTime.tv_usec=(waitTime%1000)*1000;
    struct ap_header header={0};
    memset(&header, 0xA5, sizeof(struct ap_header));
    bool bRes = false;
    header.HeadFlag = 0xFF;
    header.Reserved4 = 0x5A;
    header.Reserved5 = 0xA5;

    switch (type){
    case APTCPCONNECT_DEV_LOGIN:
        header.MsgId = 11000;
        header.DataLen = strlen((char *)in);
        bufLen = header.DataLen+headlen;
        break;
    case APTCPCONNECT_DEV_SETWIFI:
    case APTCPCONNECT_SET_DHCP:
    case APTCPCONNECT_SET_NTP:
    case APTCPCONNECT_SET_OSD:
    case APTCPCONNECT_SET_AP_SWAP:
        header.MsgId = 11040;
        header.SID = sessionID;
        header.DataLen = strlen((char *)in);
        bufLen = header.DataLen+headlen;
        break;
    case APTCPCONNECT_GET_DHCP:
    case APTCPCONNECT_GET_NTP:
    case APTCPCONNECT_GET_OSD:
    case APTCPCONNECT_GET_WIFI_STATUS:
        header.MsgId = 11042;
        header.SID = sessionID;
        header.DataLen = strlen((char *)in);
        bufLen = header.DataLen+headlen;
        break;
    case APTCPCONNECT_GET_ONLINE_STATUS:
        header.MsgId = 11006;
        header.SID = sessionID;
        header.DataLen = strlen((char *)in);
        bufLen = header.DataLen+headlen;
    default:
        break;
    }

    rbufheader=(char *)malloc(headlen);
    buf=(char *)malloc(bufLen);
    if(rbufheader==NULL || buf==NULL){
        err=_MALLOC_ERR;

        return false;
    }
    memset(rbufheader,0,headlen);
    memset(buf,0,bufLen);
    cur=buf;
    memcpy(cur,(char*)&header,sizeof(struct ap_header));
    cur+=sizeof(struct ap_header);
    memcpy(cur,(char*)in,bufLen-sizeof(struct ap_header));

    FD_ZERO(&write_flags);
    FD_SET(clientSocket,&write_flags);
    selectRet=select(clientSocket+1,NULL,&write_flags,NULL,&m_waitTime);
    if(selectRet<0){
        err=_SELECTSOCKET_ERR;
        goto ERREXIT;
    }else if(selectRet==0){
        err=_TIMEOUT;
        goto ERREXIT;
    }
    do{
        wlen =write(clientSocket,buf,bufLen);
        if(wlen==-1){
            if(errno==EINTR)
            {
                continue;
            } else{
                err=_WRITESOCKET_ERR;
                goto ERREXIT;
            }
        }
        break;
    }while(1);

    FD_ZERO(&read_flags);
    FD_SET(clientSocket,&read_flags);
    selectRet=select(clientSocket+1,&read_flags,NULL,NULL,&m_waitTime);
    if(selectRet<0){
        err=_SELECTSOCKET_ERR;
        goto ERREXIT;
    }else if(selectRet==0){
        err=_TIMEOUT;
        goto ERREXIT;
    }

    do{
        rlen =read(clientSocket,rbufheader,headlen);  //读取包头，获取包体长度
        if(rlen==-1){
            if(errno==EINTR)
            {
                continue;
            }
            else{
                err=_READSOCKET_ERR;
                goto ERREXIT;
            }
        }
        rcur=rbufheader;
        rbufLen_body = ((struct ap_header *)rcur)->DataLen;
        rbufbody=(char *)malloc(rbufLen_body);
        memset(rbufbody,0,rbufLen_body);
        rlen =read(clientSocket,rbufbody,rbufLen_body);  //读取包体

        if(rlen==-1){
            if(errno==EINTR)
            {
                continue;
            }else{
                err=_READSOCKET_ERR;
                goto ERREXIT;
            }
        }
        break;
    }while(1);
    rcur=rbufbody;
    switch (type) {
    case APTCPCONNECT_DEV_LOGIN:
    {
        if(rlen > 0) {
            struct LoginResponse ret;
            memset(&ret, 0, sizeof(struct LoginResponse));
            std::string temp = rcur;
            TExchangeAL<struct LoginResponse>::parseConfig(temp, ret);

            if(ret.iRet == 100)
                bRes = true;

            *((int *)out) = ret.uiSessionId;
        }
    }
        break;
    case APTCPCONNECT_GET_WIFI_STATUS:
    {
        if(rlen > 0) {
            unsigned int nSession = 0;
            int nRet = 0;
            struct NetWifiStatus ret;
            std::string temp = rcur;
            std::string name = "NetWork.WifiStatus";
            //TExchangeAL<struct NetWifiStatus>::parseConfig(temp, ret);
            TExchangeAL<struct NetWifiStatus>::parseConfig(temp, name, nSession, nRet, ret);
            if(nRet == 100) {
                bRes = true;
                *((int *)out) = ret.nWifiStatus;
            }
        }
    }
        break;
    case APTCPCONNECT_SET_DHCP:
    case APTCPCONNECT_DEV_SETWIFI:
    case APTCPCONNECT_SET_NTP:
    case APTCPCONNECT_GET_ONLINE_STATUS:
    case APTCPCONNECT_SET_AP_SWAP:
    {
        if(rlen > 0) {
            struct DefaultResponse ret;
            std::string temp = rcur;
            TExchangeAL<struct DefaultResponse>::parseConfig(temp, ret);
            if(ret.iRet == 100 || ret.iRet == 603)
                bRes = true;
        }
    }
        break;
    case APTCPCONNECT_GET_NTP:
    {
        if(rlen>0){
            NetNTPConfig ret;//=(struct NetNTPConfig*)out;
            std::string temp = rcur;
            TExchangeAL<struct NetNTPConfig>::parseConfig(temp,ret);
            strncpy((char*)out,(char*)(&ret),sizeof(NetNTPConfig));
           bRes = true;
        }
    }
        break;
    case APTCPCONNECT_GET_OSD:
    {
        if(rlen>0){
            CONFIG_VIDEOWIDGET ret;//=(struct CONFIG_VIDEOWIDGET*)out;
            std::string temp = rcur;

            TExchangeAL<struct CONFIG_VIDEOWIDGET>::parseConfig(temp, ret);


            strncpy((char*)out,(char*)(&ret),sizeof(CONFIG_VIDEOWIDGET));

            bRes = true;
        }
    }
        break;
    case APTCPCONNECT_GET_DHCP:
    {
        if(rlen>0){
            struct NetDHCPConfigAll ret;//=(struct CONFIG_VIDEOWIDGET*)out;
            std::string temp = rcur;

            TExchangeAL<struct NetDHCPConfigAll>::parseConfig(temp, ret);
            strncpy((char*)out,(char*)(&ret),sizeof(struct NetDHCPConfigAll));
            bRes = true;
        }
    }
        break;
    default:
        break;
    }
    if(rbufheader)  free(rbufheader);
    if(rbufbody)  free(rbufbody);
    if(buf)  free(buf);
    return bRes;

ERREXIT:
    if(rbufheader)  free(rbufheader);
    if(rbufbody)  free(rbufbody);
    if(buf)  free(buf);
    return false;
}
