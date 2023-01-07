#include "TcpSocket.h"
#include<qbytearray.h>
#include"TcpData.h"
#include"MessageJson.h"
#include"Email.h"

TcpSocket::TcpSocket(qintptr id, QObject* parent):
	socketDescriptor(id), QThread(parent)
{
	tcpSocket = new QTcpSocket();
	tcpSocket->setSocketDescriptor(id);
	connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::read);
}

TcpSocket::~TcpSocket()
{
	tcpSocket->deleteLater();
}

void TcpSocket::run()
{
	exec();
}

void TcpSocket::read()
{
	QByteArray byteArray = tcpSocket->readAll();
	TcpData::RequestType type = MessageJson::requestType(byteArray);
	if (type == TcpData::RequestType::VerificationCode)
	{
		QString mailAddress = MessageJson::getMailAddress(byteArray);
		auto email = new Email();
		email->connect();
		email->sendMail(mailAddress, "6666");
	}
}

