#include "TcpSocket.h"
#include<qbytearray.h>
#include<qrandom.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qmutex.h>

#include"TcpData.h"
#include"MessageJson.h"
#include"Email.h"
#include"CommonData.h"

#include<qdebug.h>
#include<qhostaddress.h>



TcpSocket::TcpSocket(qintptr id, QObject* parent):
	socketDescriptor(id), QThread(parent)
{
	tcpSocket = new QTcpSocket();
	tcpSocket->setSocketDescriptor(id);

	//获取用户IP地址
	this->ipv4_int32 = tcpSocket->peerAddress().toIPv4Address();
	this->ipv4_str = QHostAddress(ipv4_int32).toString();
	
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
	TcpData::RequestType type = MessageJson::getRequestType(byteArray);

	switch (type)
	{
	case TcpData::RequestType::VerificationCode:
		verificationCode(byteArray);
		break;
	case TcpData::RequestType::Enroll_Request:
		enroll(byteArray);
		break;
	default:
		break;
	}
}

void TcpSocket::write(QByteArray& byteArray)
{
	tcpSocket->write(byteArray);
}

/*************************************************
Description: 处理客户端发起的获取验证码请求
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::verificationCode(QByteArray &byteArray)
{
	//获取邮箱号
	QMap<QString,QString> requestData= MessageJson::getRequestData(byteArray);
	QString mailAddress_tmp = requestData["mailAddress"];

	Email email;
	if (email.connect())
	{
		int number = QRandomGenerator::global()->bounded(1000, 9999);
		QString code_tmp = QString::number(number);
		if (email.sendMail(mailAddress_tmp, code_tmp))
		{
			this->mailAddress = mailAddress_tmp;
			this->code = code_tmp;
		}
	}
}

/*************************************************
Description: 处理客户端发起的获取验证码请求
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::enroll(QByteArray& byteArray)
{
	//获取请求注册数据
	QMap<QString, QString> requestData = MessageJson::getRequestData(byteArray);
	QString    userName_tmp =  requestData["userName"];
	QString mailAddress_tmp =  requestData["mailAddress"];
	QString    password_tmp =  requestData["password"];
	QString        code_tmp =  requestData["code"];

	QMap<QString, QString> data;
	QString responseTypeStr = "Enroll_Request";
	data.insert(responseTypeStr, QString::number(TcpData::Enroll_Correct));

	//对比验证码
	if (code_tmp != code)
	{
		data[responseTypeStr] = QString::number(TcpData::Code_Error);
		response(TcpData::Enroll_Response, data);
		return;
	}

	//锁住SQL中user表
	QMutexLocker locker(&CommonData::sqlUser_mutex);

	//片段账号是否已经注册
	QString query = "select * from user where mailAddress = '[mailAddress]'";
	query.replace("[mailAddress]", mailAddress_tmp);
	QSqlQuery sqlQuery;
	sqlQuery.exec(query);
	if (!sqlQuery.next())
	{
		data[responseTypeStr] = QString::number(TcpData::Exist_Error);
		response(TcpData::Enroll_Response,data);
		return;
	}


	if (mailAddress == mailAddress_tmp && code == code_tmp)
	{
		//插入数据库user表
		query = "insert into user values ('[mailAddress]',\
							 			  '[userName]',\
						  				  '[password]',\
										  '[dateTime]',\
										  '[endDateTime]',\
										  '[ip]')";

		QDateTime dateTime= QDateTime::currentDateTime();
		requestData["dateTime"] = dateTime.toString("yyyy-MM-dd hh:mm:ss");
		requestData["endDateTime"] = dateTime.toString("yyyy-MM-dd hh:mm:ss");
		requestData["ip"] = this->ipv4_str;

		auto iterator = requestData.constBegin();
		while (iterator!=requestData.constEnd())
		{
			QString  key  = "[" + iterator.key() + "]";
			QString value = iterator.value();
			query.replace(key, value);
			iterator++;
		}

		bool res = sqlQuery.exec(query);
		if (!res)//未知错误
		{
			data[responseTypeStr] = QString::number(TcpData::Enroll_error);
			response(TcpData::Enroll_Response, data);
			return;
		}
	}

	//注册成功
	data[responseTypeStr] = QString::number(TcpData::Enroll_Correct);
	response(TcpData::Enroll_Response, data);
}

/*************************************************
Description: 将响应数据序列化并发出通讯请求
	  Input: type=响应类型 data=响应数据
	 Return: 空
*************************************************/
void TcpSocket::response(TcpData::ResponseType type, QMap<QString, QString>& data)
{
	QByteArray byteArray = MessageJson::getResponseByteArray(type, data);
	write(byteArray);
}