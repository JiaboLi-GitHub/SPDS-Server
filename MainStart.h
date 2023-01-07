#pragma once
#include <qobject.h>
#include"TcpServer.h"

class MainStart :
    public QObject
{
    Q_OBJECT
public:
    MainStart();
    ~MainStart();

    void f();

public:
    TcpServer* tcpServer=NULL;
};

