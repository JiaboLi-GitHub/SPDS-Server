#include "TcpServer.h"
#include"MysqlServer.h"

#define toUTF8(str)  QString::fromLocal8Bit(str)
#include<iostream>

TcpServer::TcpServer(QObject* parent, int port)
	: QTcpServer(parent)
{
	//注册类型
	qRegisterMetaType<qintptr>("qintptr");

	thread_Count = 0;
	Ideal_Thread_Count = QThread::idealThreadCount();
	Max_Thread_Count = 2 * Ideal_Thread_Count;

	listen(QHostAddress::Any, port);

	std::cout << "启动服务器" << std::endl;
	std::cout << "主线程ID：" << QThread::currentThreadId() << std::endl;
	std::cout << "理想线程数：" << Ideal_Thread_Count << std::endl;
	std::cout << "最大线程数：" << Max_Thread_Count << std::endl;

}

TcpServer::~TcpServer()
{
}

/*************************************************
Description: 重写接口新连接的函数
	  Input: 套接字描述符
	 Return: 空
*************************************************/
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	if (thread_Count >= Max_Thread_Count)
	{
		return;
	}

	auto tcpSocket = new TcpSocket(socketDescriptor);
	auto thread = new QThread();
	tcpSocket->moveToThread(thread);
	connect(tcpSocket, &TcpSocket::disconnected, this, &TcpServer::tcpSocketDisconnect);
	connect(thread, &QThread::finished, tcpSocket, &QObject::deleteLater);
	tcpSockeMap[socketDescriptor] = tcpSocket;
	threadMap[socketDescriptor] = thread;
	thread_Count++;
	thread->start();

	std::cout << "收到一个连接请求" << std::endl;
}

/*************************************************
Description: 套接字断开所执行的槽函数
	  Input: 
	 Return: 空
*************************************************/
void TcpServer::tcpSocketDisconnect(qintptr socketDescriptor)
{
	auto thread = threadMap[socketDescriptor];
	thread->quit();
	thread->wait();
	thread->deleteLater();
	thread_Count--;
	tcpSockeMap.remove(socketDescriptor);
	threadMap.remove(socketDescriptor);
}