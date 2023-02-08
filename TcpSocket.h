#pragma once
#include<qthread.h>
#include<qtcpsocket.h>
#include<qbytearray.h>
#include<qdatetime.h>
#include<qrunnable.h>
#include"CommonData.h"
#include"TcpData.h"


extern QMutex sqlUser_mutex;

class TcpSocket :
    public QTcpSocket
{
Q_OBJECT

public:
    TcpSocket(qintptr id);
    ~TcpSocket();

signals:
    void disconnected(qintptr socketDescriptor);

private slots:
    void read();
    void disconnect();

private:
    void write(QByteArray& byteArray);
    void response(TcpData::ResponseType type, QMap<QString, QString>& data);
    void verificationCode(QByteArray& byteArray);
    void enroll(QByteArray& byteArray);
    void logIn(QByteArray& byteArray);

protected:
    void run();

private:
    qint32          ipv4_int32;
    QString         ipv4_str;

//登录注册模块
private:
    QString userName;       //用户名
    QString mailAddress;    //邮箱号
    QString password;       //密码
    QString code;           //验证码
    QDateTime code_DateTme; //验证码生成时间

};

