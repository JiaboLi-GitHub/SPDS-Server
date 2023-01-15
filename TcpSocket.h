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
    qintptr         socketDescriptor;       //�׽���ID
    qint32          ipv4_int32;
    QString         ipv4_str;

   
//��¼ע��ģ��
private:
    QString userName;       //�û���
    QString mailAddress;    //�����
    QString password;       //����
    QString code;           //��֤��

};

