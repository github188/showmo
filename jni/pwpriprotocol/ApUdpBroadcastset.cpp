#include"ApUdpBroadcastset.h"
#include<thread>
#include<chrono>
#include<stdlib.h>
#include"jniutil.h"
#define MAXBUF 1024
#define PORT 60000

#define VERSION "0.0.5"

char g_ssid[33] = "softtest";
char g_key[65] = "12345678";
char SERV_HOST_ADDR_0[100] = "239.0.0.254";

#define UIDLENGTH 8
using namespace std;
char nonce[16+16];
char* cnonce=nonce;
char* snonce=&nonce[16];

char *p_cnonce=nonce;
char *p_snonce=&nonce[16];

char arowp_sack[]="ACK -- Welcome to IOE World!!! CHEERS!!";
char arowp_nack[]="NACK-- You are blocked out!!";

#define NONCE_LEN 16
#define MIC_SIZE 16
unsigned char IV[MIC_SIZE];

unsigned char Mic[MIC_SIZE];
unsigned char wapiQosIcd[48];
unsigned char wapiIcd[32];
unsigned long KeyExt[32], ICDExt[32];

#define ascii_valid_value	256
#define ssidkey_len_valid_value	64

#define value_4bit_len	16

#define k_start		0
#define k_len		(k_start + 1)
#define k_ssid		(k_len + 1)
#define k_key		(k_ssid + 1)
#define k_stop		(k_key + 1)
#define k_v		    (k_stop + 1)
#define k_v_end		(k_v + value_4bit_len)

int SERV_UDP_PORT = 6444;
int ApUdpBroadcastSet::createSocket(){
    m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int flag=1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&flag, sizeof(flag));
}
int ApUdpBroadcastSet::config_my_sock_addr(char * ip){
    m_sa.sin_addr.s_addr = inet_addr(ip);
}
ApUdpBroadcastSet::ApUdpBroadcastSet(){
    createSocket();
    memset((void*)&m_sa,0,sizeof(m_sa));
    m_sa.sin_family = AF_INET;
    m_sa.sin_port = htons(SERV_UDP_PORT);
}

void ApUdpBroadcastSet::sendDataToIp(int ip2,int ip3,char* data){
    char ip[16] = {0};
    sprintf(ip, "239.%d.%d.254", ip2, ip3);
    config_my_sock_addr(ip);
    int ret = sendto(m_sockfd, data, sizeof(data), 0, (struct sockaddr *)&m_sa, sizeof(m_sa));
    std::this_thread::sleep_for(std::chrono::milliseconds(35));
}
bool ApUdpBroadcastSet::broadcastToDev(char* data,char *ssid, int nssid, char *pwd, int npwd, char *keytype, int nkey){
    if(m_sockfd<0){
        createSocket();
    }
    if(m_sockfd<0){
        return false;
    }
    int offset;
    int ssidlen = nssid;
    int pwdlen = npwd;
    int keylen = nkey;
    int totallen = ssidlen+pwdlen+keylen+4;

    offset=3;
    //閹鏆辨惔锟�
    sendDataToIp(offset,totallen,data);

    offset++;

    //SSID
    for(int i=0; i<nssid;i++) {
        sendDataToIp(offset,ssid[i],data);
        offset++;
    }
    sendDataToIp(offset,10,data);
    offset++;

    //鐎靛棛鐖�
    for(int i=0; i<npwd;i++) {
        sendDataToIp(offset,pwd[i],data);
        offset++;
    }
    sendDataToIp(offset,10,data);
    offset++;
    //閸旂姴鐦戠猾璇茬��
    for(int i=0; i<nkey;i++) {
        sendDataToIp(offset,keytype[i],data);
        offset++;
    }
    sendDataToIp(offset,10,data);

    return true;
}
void ApUdpBroadcastSet::setPauseBroadcast(bool bpause){
   m_bPause=bpause;
   LOGE("ApUdpBroadcastSet::setPauseBroadcast");
}
void ApUdpBroadcastSet::stopBroadcast(){
    m_bSendStop=true;
    LOGE("ApUdpBroadcastSet::stopBroadcas");
}
void printErr(int para){
	//LOGE("broadcast data value:%d",para);
	if(para<0){
		LOGE("broadcast data err");
	}
}
volatile int SendPriod=15;

bool ApUdpBroadcastSet::broadcastRecycle(char* ssid,char* pwd,int mSec){
    m_bSendStop = false;
    m_bPause=false;
    int flag = 1, ssidlen, pwdlen, keylen, totallen,i,offset;
    size_t ret;
    char data[] = "";
    char keytype[] = "wpa2";
    struct sockaddr_in sa;
    bzero((char*)&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(6444);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
    	return false;
    }
    int nRet = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&flag, sizeof(flag));

    ssidlen = strlen(ssid);
    pwdlen = strlen(pwd);
    keylen = strlen(keytype);
    totallen = ssidlen+pwdlen+keylen+4;
    int uSleep = SendPriod*1000;//mSec*1000/totallen;

    while(!m_bSendStop) {
        if(!m_bPause){
            char ip[16] = {0};
            offset=3;
            //閹鏆辨惔锟�
            sprintf(ip, "239.%d.%d.254", offset, totallen);
            sa.sin_addr.s_addr = inet_addr(ip);
            ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
            printErr(ret);
            offset++;
            usleep(uSleep);

            //SSID
            for(i=0; i<ssidlen;i++) {
                sprintf(ip, "239.%d.%d.254", offset, ssid[i]);
                sa.sin_addr.s_addr = inet_addr(ip);
                ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
                printErr(ret);
                offset++;
                usleep(uSleep);
            }
            sprintf(ip, "239.%d.10.254", offset);   //閸欐垿锟斤拷/n
            sa.sin_addr.s_addr = inet_addr(ip);
            ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
            offset++;
            usleep(uSleep);
            printErr(ret);
            //鐎靛棛鐖�
            for(i=0; i<pwdlen;i++) {
                sprintf(ip, "239.%d.%d.254", offset, pwd[i]);
                sa.sin_addr.s_addr = inet_addr(ip);
                ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
                printErr(ret);
                offset++;
                usleep(uSleep);
            }
            sprintf(ip, "239.%d.10.254", offset);   //閸欐垿锟斤拷/n
            sa.sin_addr.s_addr = inet_addr(ip);
            ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
            printErr(ret);
            offset++;
            usleep(uSleep);

            //閸旂姴鐦戠猾璇茬��
            for(i=0; i<keylen;i++) {
                sprintf(ip, "239.%d.%d.254", offset, keytype[i]);
                sa.sin_addr.s_addr = inet_addr(ip);
                ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
                printErr(ret);
                offset++;
                usleep(uSleep);
            }
            sprintf(ip, "239.%d.10.254", offset);   //閸欐垿锟斤拷/n
            sa.sin_addr.s_addr = inet_addr(ip);
            ret = sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&sa, sizeof(sa));
            printErr(ret);
            usleep(uSleep);
            LOGE("ApUdpBroadcastSet::broadcastToDev OVER");
        }else{
        	LOGE("ApUdpBroadcastSet::broadcastToDev pause recycle");
            usleep(50000);
        }
    }
    close(sockfd);
    return true;
}


