#include "TcpSocket.h"
#include<qbytearray.h>
#include<qrandom.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qmutex.h>
#include<iostream>

#include"TcpData.h"
#include"MessageJson.h"
#include"Email.h"
#include"CommonData.h"
#include"MysqlConn.h"
#include<qdebug.h>
#include<qhostaddress.h>

TcpSocket::TcpSocket(qintptr id)
{
	setSocketDescriptor(id);
	socketDescriptor = id;
	mysqlConn = new MysqlConn(id);

	//��ȡ�û�IP��ַ
	this->ipv4_int32 = peerAddress().toIPv4Address();
	this->ipv4_str = QHostAddress(ipv4_int32).toString();

	connect(this, &QTcpSocket::readyRead, this, &TcpSocket::read);
	connect(this, &QTcpSocket::disconnected, this, &TcpSocket::disconnect);
}

TcpSocket::~TcpSocket()
{
	delete mysqlConn;
	qDebug() << u8"�ͷ�::�̣߳�" << QThread::currentThreadId();
}

void TcpSocket::read()
{
	QByteArray byteArray = readAll();
	TcpData::RequestType type = MessageJson::getRequestType(byteArray);

	switch (type)
	{
	case TcpData::RequestType::VerificationCode:
		verificationCode(byteArray);
		break;
	case TcpData::RequestType::Enroll_Request:
		enroll(byteArray);
		break;
	case TcpData::LogIn_Request:
		logIn(byteArray);
		break;
	default:
		break;
	}

	qDebug() << u8"TcpSocket::read�̣߳�" << QThread::currentThreadId();
}

void TcpSocket::disconnect()
{
	qDebug() << u8"TcpSocket disconnect�̣߳�" << QThread::currentThreadId();
	emit disconnected(socketDescriptor);
}

/*************************************************
Description: ����Ӧ�������л�������ͨѶ����
	  Input: type=��Ӧ���� data=��Ӧ����
	 Return: ��
*************************************************/
void TcpSocket::response(TcpData::ResponseType type, QMap<QString, QString>& data)
{
	QByteArray byteArray = MessageJson::getResponseByteArray(type, data);
	write(byteArray);
	qDebug() << u8"TcpSocket::response�̣߳�" << QThread::currentThreadId();
}

/*************************************************
Description: ����ͻ��˷���Ļ�ȡ��֤������
	  Input: byteArray=�ͻ��˷���������
	 Return: ��
*************************************************/
void TcpSocket::verificationCode(QByteArray &byteArray)
{
	//��ȡ�����
	QMap<QString,QString> requestData= MessageJson::getRequestData(byteArray);
	QString mailAddress_tmp = requestData["mailAddress"];

	Email email;
	if (email.connect())
	{
		code_DateTme = QDateTime::currentDateTime();
		int number = QRandomGenerator::global()->bounded(1000, 9999);
		QString code_tmp = QString::number(number);
		if (email.sendMail(mailAddress_tmp, code_tmp))
		{
			this->mailAddress = mailAddress_tmp;
			this->code = code_tmp;
		}
	}

	qDebug() << u8"TcpSocket::verificationCode�̣߳�" << QThread::currentThreadId();
}

/*************************************************
Description: ����ͻ��˷����ע������
	  Input: byteArray=�ͻ��˷���������
	 Return: ��
*************************************************/
void TcpSocket::enroll(QByteArray& byteArray)
{
	//��ȡ����ע������
	QMap<QString, QString> requestData = MessageJson::getRequestData(byteArray);
	QString    userName_tmp =  requestData["userName"];
	QString mailAddress_tmp =  requestData["mailAddress"];
	QString    password_tmp =  requestData["password"];
	QString        code_tmp =  requestData["code"];

	QMap<QString, QString> data;
	QString responseTypeStr = "Enroll_Response";
	data.insert(responseTypeStr, QString::number(TcpData::Enroll_Correct));

	//�Ա���֤��
	if (code_tmp != code && QDateTime::currentDateTime()<=code_DateTme.addSecs(300))
	{
		data[responseTypeStr] = QString::number(TcpData::Code_Error);
		response(TcpData::Enroll_Response, data);
		return;
	}
	code.clear();


	//��סSQL��user��
	QMutexLocker locker(&CommonData::sqlUser_Mutex);
	
	//��ѯ���ݿ�
	QString sqlStr = "select * from user where mailAddress = '[mailAddress]'";
	sqlStr.replace("[mailAddress]", mailAddress_tmp);
	auto sqlQuery = mysqlConn->run(sqlStr);

	//�˺��Ƿ��Ѿ�ע��
	if (sqlQuery.next())
	{
		data[responseTypeStr] = QString::number(TcpData::Exist_Error);
		response(TcpData::Enroll_Response,data);	
		return;
	}

	//ע��ɹ�
	if (mailAddress == mailAddress_tmp)
	{
		//�������ݿ�user��
		sqlStr = "insert into user values ('[mailAddress]',\
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
			sqlStr.replace(key, value);
			iterator++;
		}
		sqlQuery= mysqlConn->run(sqlStr);
	}
	data[responseTypeStr] = QString::number(TcpData::Enroll_Correct);
	response(TcpData::Enroll_Response, data);
}//���½��������ݿ�ı�

/*************************************************
Description: ����ͻ��˷���ĵ�¼����
	  Input: byteArray=�ͻ��˷���������
	 Return: ��
*************************************************/
void TcpSocket::logIn(QByteArray& byteArray)
{
	//��ȡ�����¼����
	QMap<QString, QString> requestData = MessageJson::getRequestData(byteArray);
	QString mailAddress_tmp = requestData["mailAddress"];
	QString    password_tmp = requestData["password"];
	QMap<QString, QString> data;
	QString responseTypeStr = "LogIn_Response";
	data.insert(responseTypeStr, QString::number(TcpData::Login_Correct));

	//��סSQL��user��
	QMutexLocker sqlUser_Locker(&CommonData::sqlUser_Mutex);

	//�˺Ų�����
	QString query = "select * from user where mailAddress = '[mailAddress]'";
	query.replace("[mailAddress]", mailAddress_tmp);
	QSqlQuery sqlQuery = mysqlConn->run(query);

	bool res = sqlQuery.next();
	if (!res)
	{
		data[responseTypeStr] = TcpData::Account_Error;
		response(TcpData::LogIn_Response,data);
		return;
	}

	QString    userName_sql = sqlQuery.value("userName").toString();
	QString mailAddress_sql = sqlQuery.value("mailAddress").toString();
	QString    password_sql = sqlQuery.value("password").toString();
	//�������
	if (password_sql != password_tmp)
	{
		data[responseTypeStr] = TcpData::Password_Error;
		response(TcpData::LogIn_Response, data);
		return;
	}

	//������
	QMutexLocker existMap_Locker(&CommonData::existMap_Mutex);
	if (CommonData::existMap[mailAddress_tmp])
	{
		data[responseTypeStr] = TcpData::Login_Error;
		response(TcpData::LogIn_Response, data);
		return;
	}
	CommonData::existMap[mailAddress_tmp] = true;

	//��¼�ɹ�
	userName = userName_sql;
	mailAddress = mailAddress_sql;
	password = password_sql;
	data[responseTypeStr] = TcpData::Login_Correct;
	data.insert("userName", userName);
	data.insert("mailAddress", mailAddress);
	response(TcpData::LogIn_Response, data);
	return;

	qDebug() << u8"TcpSocket::logIn�̣߳�" << QThread::currentThreadId();
}
