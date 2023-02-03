#include "TcpServer.h"
#define toUTF8(str)  QString::fromLocal8Bit(str)
#include<iostream>

TcpServer::TcpServer(QObject* parent, int port)
	: QTcpServer(parent)
{
	listen(QHostAddress::Any, port);
	std::cout << "����������" << std::endl;
}

TcpServer::~TcpServer()
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	TcpSocket* tcpSocket = new TcpSocket(socketDescriptor, this);
	tcpSocket->start();
	tcpSockeMap[socketDescriptor] = tcpSocket;
	std::cout << "�յ�һ����������" << std::endl;
}
