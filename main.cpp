#include <QtCore/QCoreApplication>
#include"TcpServer.h"
#include<qtcpserver.h>
#include<qthread.h>
#include"MainStart.h"


#include"Email.h"
//#pragma comment(lib,"D:\\QT5.12.9\\5.12.9\\msvc2017_64\\lib\\SMTPEmail\\SMTPEmail.lib")

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TcpServer *tcpServer=new TcpServer(new QObject(),8888);
    MainStart* mainStart = new MainStart();

    return a.exec();
}
