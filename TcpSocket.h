#pragma once
#include<qthread.h>
#include<qtcpsocket.h>

class TcpSocket :
    public QThread
{
public:
    TcpSocket(qintptr id,QObject* parent);
    ~TcpSocket();

private:
    void read();

protected:
    void run();

private:
    QTcpSocket* tcpSocket=NULL;
    qintptr socketDescriptor;
};

