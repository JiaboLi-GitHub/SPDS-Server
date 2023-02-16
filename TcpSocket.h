#pragma once
#include<qthread.h>
#include<qtcpsocket.h>
#include<qbytearray.h>
#include<qdatetime.h>
#include<qrunnable.h>
#include<qstack.h>
#include"CommonData.h"
#include"TcpData.h"
#include"MysqlConn.h"



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

protected:
    void run();

private:
    qintptr socketDescriptor;
    qint32        ipv4_int32;
    QString         ipv4_str;

//��¼ע��ģ��
    QString userName;       //�û���
    QString mailAddress;    //�����
    QString password;       //����
    QString code;           //��֤��
    QDateTime code_DateTme; //��֤������ʱ��

/*���ݿ�*/
    MysqlConn *mysqlConn;

/*�������*/
    const int SampleStack_Max=3;
};

