#include "TcpServer.h"
#define toUTF8(str)  QString::fromLocal8Bit(str)
#include<iostream>

TcpServer::TcpServer(QObject* parent, int port)
	: QTcpServer(parent)
{
	thread_Count = 0;
	Ideal_Thread_Count = QThread::idealThreadCount();
	Max_Thread_Count = 2 * Ideal_Thread_Count;

	listen(QHostAddress::Any, port);

	std::cout << "����������" << std::endl;
	std::cout << "���߳�ID��" << QThread::currentThreadId() << std::endl;
	std::cout << "�����߳�����" << Ideal_Thread_Count << std::endl;
	std::cout << "����߳�����" << Max_Thread_Count << std::endl;
}

TcpServer::~TcpServer()
{
}

/*************************************************
Description: ��д�ӿ������ӵĺ���
	  Input: �׽���������
	 Return: ��
*************************************************/
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	if (thread_Count >= Max_Thread_Count)
	{
		return;
	}

	TcpSocket* tcpSocket = new TcpSocket(socketDescriptor);
	thread = new QThread();
	tcpSocket->moveToThread(thread);
	qRegisterMetaType<qintptr>("qintptr");
	connect(tcpSocket, &TcpSocket::disconnected, this, &TcpServer::tcpSocketDisconnect);
	connect(thread, &QThread::finished, tcpSocket, &QObject::deleteLater);
	thread->start();

	thread_Count++;
	tcpSockeMap[socketDescriptor] = tcpSocket;

	std::cout << "�յ�һ����������" << std::endl;
}

/*************************************************
Description: �׽��ֶϿ���ִ�еĲۺ���
	  Input: 
	 Return: ��
*************************************************/
void TcpServer::tcpSocketDisconnect(qintptr socketDescriptor)
{
	auto tcpSocket = tcpSockeMap[socketDescriptor];
	//�˳����߳�
//	tcpSocket->deleteLater();
	thread->quit();
	thread->wait();
	//thread->deleteLater();
	thread_Count--;
	tcpSockeMap.remove(socketDescriptor);

	qDebug() << u8"TcpServer::tcpSocketDisconnect�̣߳�"<< QThread::currentThreadId();
}
