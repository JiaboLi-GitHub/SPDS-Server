#pragma once
#include<qtcpserver.h>
#include<qtcpsocket.h>
#include<qmap.h>
#include"TcpSocket.h"

class TcpServer :
    public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject* parent, int port);
    ~TcpServer();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

private:
    void tcpSocketDisconnect(qintptr socketDescriptor);

private:
    QMap<qintptr, TcpSocket*> tcpSockeMap;
    int thread_Count;
    QThread* thread;

private:
    int Ideal_Thread_Count;
    int Max_Thread_Count;
};

