#ifndef APUDPBROADCASTSET_H
#define APUDPBROADCASTSET_H
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
#include<mutex>
class ApUdpBroadcastSet{
public:
    ApUdpBroadcastSet();
    bool broadcastToDev(char* data,char *ssid, int nssid, char *pwd, int npwd, char *keytype, int nkey);
    bool broadcastRecycle(char* ssid,char* pwd,int mSec);
    void stopBroadcast();
    void setPauseBroadcast(bool bpause);
private:
    int config_my_sock_addr(char * ip);
    int createSocket();
    void sendDataToIp(int ip2,int ip3,char* data);
private:
    int m_sockfd;
    struct sockaddr_in m_sa;
    volatile bool m_bSendStop;
    volatile bool m_bPause;
    std::mutex m_broadcastMutex;
    std::mutex m_stopLock;

};


#endif // APUDPBROADCASTSET_H

