#ifndef PRIPTOTO_H
#define PRIPTOTO_H
enum BroadcastErr{
    Broadcast_SocketCreateErr=110001,//socket建立失败
    Broadcast_SocketBindErr=110002,
    Broadcast_Timeout=110003//socket发送数据超时
};


#endif // PRIPTOTO_H

