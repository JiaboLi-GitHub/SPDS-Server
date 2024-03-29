#pragma once
#include<qthread.h>
#include<qtcpsocket.h>
#include<qbytearray.h>
#include<qdatetime.h>
#include<qrunnable.h>
#include<qstack.h>
#include"CommonData.h"
#include"TcpData.h"
#include"MysqlServer.h"

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
    void response(TcpData::ResponseType type, QMap<QString, QString>& data);
    void verificationCode(QByteArray& byteArray);
    void enroll(QByteArray& byteArray);
    void logIn(QByteArray& byteArray);
    void detectionSave(QByteArray& byteArray);
    void detectionRead();
    void userQuit();
    QString tokenBuilder(QString mailAddress);

protected:
    void run();

private:
    qintptr socketDescriptor;
    qint32        ipv4_int32;
    QString         ipv4_str;

//登录注册模块
    QString userName;       //用户名
    QString mailAddress;    //邮箱号
    QString password;       //密码
    int code;           //验证码
    QDateTime code_DateTme; //验证码生成时间

/*数据库*/
    MysqlServer *mysqlServer;

/*常量*/
    const int TOKENLEN=512;           //token长度
    const int SampleStack_Max=3;
};

