#include "TcpServer.h"
#define toUTF8(str)  QString::fromLocal8Bit(str)

TcpServer::TcpServer(QObject* parent, int port)
	: QTcpServer(parent)
{
	listen(QHostAddress::Any, port);
	qDebug() << toUTF8("启动服务器");
}

TcpServer::~TcpServer()
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocket* tcpSocket = new TcpSocket(socketDescriptor, this);
	tcpSocket->start();
	tcpSockeMap[socketDescriptor] = tcpSocket;
	qDebug() << toUTF8("收到一个连接请求");
}
