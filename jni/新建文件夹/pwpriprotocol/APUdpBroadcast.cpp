#include"APUdpBroadcast.h"
//#include<QDebug>
//#include "ExchangeAL/Exchange.h"
//#include "ExchangeAL/ManagerExchange.h"
//#include "ExchangeAL/NetWorkExchange.h"
//#include "ExchangeAL/ExchangeKind.h"

#include "./exchange/code/Include/ExchangeAL/Exchange.h"
#include "./exchange/code/Include/ExchangeAL/ManagerExchange.h"
#include "./exchange/code/Include/ExchangeAL/NetWorkExchange.h"
#include "./exchange/code/Include/ExchangeAL/ExchangeKind.h"
#include<fcntl.h>
#include<time.h>
#include<math.h>
#include"jniutil.h"
#include"priptoto.h"
#define MSG_HEAD_LEN   (sizeof(DVRIP_MSG_HEAD_T))
typedef struct netipmsg_h
{
    uchar		HeadFlag;				/* head flag = 0xFF */
    uchar		Reserved0;				/* reserved0 */
    uchar		Reserved1;				/* reserved1 */
    uchar		Reserved2;				/* reserved2 */
    uchar		Version;				/* version  */
    uchar		Reserved3;				/* reserved3 */
    uchar		Reserved4;				/* reserved4 */  //addby yxj 2014-1-11	0x5a
    uchar		Reserved5;				/* reserved5 */  //addby yxj 2014-1-11	0xa5
    uint		SID;					/* session ID */
    uint		Seq;					/* sequence number */
    ushort		MsgId;			        /* mesage id */
    union
    {
        struct
        {
            uchar	TotalPacket;		/* total packet */
            uchar	CurPacket;			/* current packet */
        }c;

        struct
        {
            uchar	Channel;			/* channel */
            uchar	EndFlag;			/* end flag */
        }m;
    };
    uint		DataLen;			    /* data len */
}DVRIP_MSG_HEAD_T;
APUdpBroadcast::APUdpBroadcast(){
    m_udpSocket=-1;
}

APUdpBroadcast::~APUdpBroadcast(){
    ;
}
int APUdpBroadcast::getLastErr(){
    int error=this->err;
    this->err=0;
    return error;
}

int APUdpBroadcast::create_udp_broadcast_socket(){
    m_udpSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    int opt=1;
    setsockopt(m_udpSocket,SOL_SOCKET,SO_BROADCAST,(char *)&opt,sizeof(opt));

    int x=fcntl(m_udpSocket,F_GETFL,0);              // Get socket flags
    fcntl(m_udpSocket,F_SETFL,x | O_NONBLOCK);   // Add non-blocking flag
    if(m_udpSocket<0){
        err=Broadcast_SocketCreateErr;
        return -1;
    }
    struct sockaddr_in addrLocal;
    addrLocal.sin_family=AF_INET;
    addrLocal.sin_addr.s_addr=INADDR_ANY;
    addrLocal.sin_port=htons(34569);
    if(bind(m_udpSocket, (struct sockaddr *) &addrLocal, sizeof(sockaddr_in)) < 0)
    {
        m_udpSocket=-1;
        err=Broadcast_SocketBindErr;
        return -1;
    }


    return m_udpSocket;
}
/*
 *鎼滅储灞�鍩熺綉鍐呰澶�
 * mac[][32]鎼滅储鍒扮殑璁惧 maxCount鏈�澶ц澶囨暟閲忥紝nCount鎼滅储鍒扮殑璁惧鏁伴噺锛宻earchSec鎼滅储鏃堕棿
 *
 * */
int APUdpBroadcast::searchDevInLan(char mac[][32],int maxCount,int* nCount,int searchSec)
{
    if(m_udpSocket<0){
        if(create_udp_broadcast_socket()<0){
        	LOGE("create_udp_broadcast_socket err:%d",errno);
            return -1;
        }
    }
    DVRIP_MSG_HEAD_T head;
    memset(&head, 0, sizeof(head));
    head.HeadFlag = 0xFF;
    head.MsgId = 11530;
    head.Reserved4 =0x5a;
    head.Reserved5 = 0xa5;

    struct sockaddr_in  addrTo;
    memset(&addrTo, 0, sizeof(sockaddr_in));
    addrTo.sin_family = AF_INET;
    addrTo.sin_addr.s_addr =INADDR_BROADCAST;
    addrTo.sin_port = htons(34569);

    fd_set write_flags;
    int selectRet;
    timeval m_waitTime;
    m_waitTime.tv_sec=1;
    m_waitTime.tv_usec=0;
    FD_ZERO(&write_flags);
    FD_SET(m_udpSocket,&write_flags);
    selectRet=select(m_udpSocket+1,NULL,&write_flags,NULL,&m_waitTime);
    if(selectRet>0)
    {
        if(FD_ISSET(m_udpSocket, &write_flags))
        {
            int iRet=sendto(m_udpSocket,(char*)(&head),sizeof(DVRIP_MSG_HEAD_T),0,(sockaddr*)&addrTo,sizeof(sockaddr_in));
            //qDebug()<<"sendto af "<<iRet<<" "<<errno;
        }
    }
    if(selectRet>0)
    {
        if(FD_ISSET(m_udpSocket, &write_flags))
        {
            int iRet=sendto(m_udpSocket,(char*)(&head),sizeof(DVRIP_MSG_HEAD_T),0,(sockaddr*)&addrTo,sizeof(sockaddr_in));
            //qDebug()<<"sendto af "<<iRet<<" "<<errno;
        }
    }
    if(selectRet>0)
    {
        if(FD_ISSET(m_udpSocket, &write_flags))
        {
            int iRet=sendto(m_udpSocket,(char*)(&head),sizeof(DVRIP_MSG_HEAD_T),0,(sockaddr*)&addrTo,sizeof(sockaddr_in));
            //qDebug()<<"sendto af "<<iRet<<" "<<errno;
        }
    }
    else if(selectRet==0)
    {
        err=Broadcast_Timeout;
        LOGE("broadcast sendto timeout err:%d",errno);
        // qDebug()<<"select socket err timeout"<<errno;
        return -1;
    }

    char *buffer;
    buffer=(char*)malloc(sizeof(char)*1024);
    memset(buffer,0,sizeof(char)*1024);

    fd_set read_flags;
    FD_ZERO(&read_flags);
    FD_SET(m_udpSocket,&read_flags);
    timeval selectWaitTime;

    *nCount=0;
    time_t beginTime;
    time_t endTime;
    time(&beginTime);
    long secBegin=beginTime%1000;
    time(&endTime);
    long secEnd=endTime%1000;
    while(fabs((endTime-beginTime))<searchSec)//灞�鍩熺綉鍐呮湁鏈煡鐨勮澶囨暟锛岄�氳繃澶氭鎼滅储鏉ヨ幏鍙�
    {
        selectWaitTime.tv_sec=0;
        selectWaitTime.tv_usec=500000;//瓒呮椂鏃堕棿鍗婄
        selectRet=select(m_udpSocket+1,&read_flags,NULL,NULL,&selectWaitTime);
        if(selectRet>0) {
            if(FD_ISSET(m_udpSocket, &read_flags))
            {
                int iRetLen=recv(m_udpSocket,buffer,1024,0);
                // qDebug()<<"recvfrom af "<<iRetLen<<errno;
                if(iRetLen>0){
                    DVRIP_MSG_HEAD_T head;
                    memcpy(&head, buffer, sizeof(DVRIP_MSG_HEAD_T));
                    // qDebug()<<"DVRIP_MSG_HEAD_T "<<head.MsgId<<head.HeadFlag<< head.Reserved4<<head.Reserved5;
                    if(head.MsgId==11531)
                    {
                        int nRet = 0;
                        NetCommonConfig ConfigRes;
                        memset(&ConfigRes, 0, sizeof(NetCommonConfig));
                        std::string strkey = getConfigName(CFG_NETCOMMON);
                        uint nSession = 0;
                        TExchangeAL<NetCommonConfig>::parseConfig(buffer+sizeof(DVRIP_MSG_HEAD_T), strkey, nSession,nRet, ConfigRes);
                        if(100==nRet)
                        {
                            bool macIsAlreadyExist=false;
                            for(int i=0;i<=(*nCount);i++)
                            {
                                if(strcmp(mac[i],ConfigRes.sMac)==0)//鍚屼竴涓猰ac鍦板潃涓嶉噸澶嶈褰�
                                {
                                    macIsAlreadyExist=true;
                                    break;
                                }
                            }
                            if(!macIsAlreadyExist){
                                strcpy(mac[*nCount],ConfigRes.sMac);
                                (*nCount)++;
                                if(*nCount>=maxCount){
                                    close(m_udpSocket);
                                    free(buffer);
                                    return *nCount;
                                }
                            }

                        }
                    }
                }
            }
        } else if(selectRet<0) {
            //LOGE("broadcast recv selectRet:%d",selectRet);
        } else {
           // LOGE("broadcast recv selectRet:%d",selectRet);
        }
        time(&endTime);
    }
    free(buffer);
    return *nCount;
}



