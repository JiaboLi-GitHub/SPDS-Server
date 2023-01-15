#pragma once
#include<qthread.h>
#include<qtcpsocket.h>
#include"CommonData.h"

extern QMutex sqlUser_mutex;

class TcpSocket :
    public QThread
{
public:
    TcpSocket(qintptr id,QObject* parent);
    ~TcpSocket();


private:
    void read();
    void verificationCode(QByteArray& byteArray);
    void TcpSocket::enroll(QByteArray& byteArray);

protected:
    void run();

private:
    QTcpSocket*     tcpSocket=NULL;     
    qintptr         socketDescriptor;       //套接字ID
    qint32          ipv4_int32;
    QString         ipv4_str;

   
//登录注册模块
private:
    QString userName;       //用户名
    QString mailAddress;    //邮箱号
    QString password;       //密码
    QString code;           //验证码

};

