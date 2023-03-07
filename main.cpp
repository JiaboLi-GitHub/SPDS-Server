#include <QtCore/QCoreApplication>
#include<qtcpserver.h>
#include<qthread.h>
#include<qsqldatabase.h>
#include<qdebug.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qfile.h>
#include <iostream>

#include"TcpServer.h"
#include"Email.h"
#include"MysqlServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //读取数据库配置文件
    MysqlServer::readConfiguration();
    MysqlServer mysqlServer = MysqlServer(1);

    //开启服务
    TcpServer *tcpServer=new TcpServer(new QObject(),8888);
    

    return a.exec();
}
