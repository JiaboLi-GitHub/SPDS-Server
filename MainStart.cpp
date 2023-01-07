#include "MainStart.h"
MainStart::MainStart()
{
	tcpServer = new TcpServer(this, 8888);
	connect(tcpServer, &QTcpServer::newConnection, this, &MainStart::f);
		
}
MainStart::~MainStart()
{

}

void MainStart::f()
{
	qDebug() << "f";
}