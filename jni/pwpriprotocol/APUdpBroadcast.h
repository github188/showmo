#ifndef APUDPBROADCAST_H
#define APUDPBROADCAST_H
#include "PackProtocolType.h"
#include<errno.h>


#ifdef __ANDROID__
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<unistd.h>
    #include<arpa/inet.h>
    #include<netdb.h>
#elif __APPLE__
    #include<sys/socket.h>
    #include<netinet/in.h>
    #include<sys/types.h>
    #include<unistd.h>
    #include<arpa/inet.h>
#endif

class APUdpBroadcast{
public:
    APUdpBroadcast();
    ~APUdpBroadcast();
    int create_udp_broadcast_socket();
    int searchDevInLan(char mac[][32],int maxLen,int* nCount,int searchSec=1);
    int getLastErr();
private:
    int m_udpSocket;
    int err;
};

#endif // APUDPBROADCAST_H

