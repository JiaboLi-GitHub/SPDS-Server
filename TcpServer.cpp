#include "TcpServer.h"

TcpServer::TcpServer(QObject* parent, int port)
	: QTcpServer(parent)
{
	listen(QHostAddress::Any, port);
	qDebug() << "isListening:" << this->isListening();
}

TcpServer::~TcpServer()
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocket* tcpSocket = new TcpSocket(socketDescriptor, this);
	tcpSocket->start();
	tcpSockeMap[socketDescriptor] = tcpSocket;
}
