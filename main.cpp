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
#include"MessageJson.h"
#include"MysqlConn.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //读取数据库配置文件
    MysqlConn::readMysqlConfiguration();

    //MysqlConn mysql = MysqlConn(1);
  //  mysql.run("select * from user");

    //qDebug()<<"";

    //开启服务
    TcpServer *tcpServer=new TcpServer(new QObject(),8888);
    
    return a.exec();
}
