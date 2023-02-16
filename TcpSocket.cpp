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
#include"DetectionData.h"
#include<qdebug.h>
#include<qhostaddress.h>

TcpSocket::TcpSocket(qintptr id)
{
	setSocketDescriptor(id);
	socketDescriptor = id;
	mysqlConn = new MysqlConn(id);

	//获取用户IP地址
	this->ipv4_int32 = peerAddress().toIPv4Address();
	this->ipv4_str = QHostAddress(ipv4_int32).toString();

	connect(this, &QTcpSocket::readyRead, this, &TcpSocket::read);
	connect(this, &QTcpSocket::disconnected, this, &TcpSocket::disconnect);
}

TcpSocket::~TcpSocket()
{
	delete mysqlConn;
	qDebug() << u8"释放::线程：" << QThread::currentThreadId();
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
	case TcpData::Detection_Save_Request:
		detectionSave(byteArray);
		break;
	default:
		break;
	}

	qDebug() << u8"TcpSocket::read线程：" << QThread::currentThreadId();
}

void TcpSocket::disconnect()
{
	qDebug() << u8"TcpSocket disconnect线程：" << QThread::currentThreadId();
	emit disconnected(socketDescriptor);
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
	qDebug() << u8"TcpSocket::response线程：" << QThread::currentThreadId();
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
		code_DateTme = QDateTime::currentDateTime();
		int number = QRandomGenerator::global()->bounded(1000, 9999);
		QString code_tmp = QString::number(number);
		if (email.sendMail(mailAddress_tmp, code_tmp))
		{
			this->mailAddress = mailAddress_tmp;
			this->code = code_tmp;
		}
	}

	qDebug() << u8"TcpSocket::verificationCode线程：" << QThread::currentThreadId();
}

/*************************************************
Description: 处理客户端发起的注册请求
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
	QString responseTypeStr = "Enroll_Response";
	data.insert(responseTypeStr, QString::number(TcpData::Enroll_Correct));

	//对比验证码
	if (code_tmp != code && QDateTime::currentDateTime()<=code_DateTme.addSecs(300))
	{
		data[responseTypeStr] = QString::number(TcpData::Code_Error);
		response(TcpData::Enroll_Response, data);
		return;
	}
	code.clear();


	//锁住SQL中user表
	QMutexLocker locker(&CommonData::sqlUser_Mutex);
	
	//查询数据库
	QString sqlStr = "select * from user where mailAddress = '[mailAddress]'";
	sqlStr.replace("[mailAddress]", mailAddress_tmp);
	auto sqlQuery = mysqlConn->run(sqlStr);

	//账号是否已经注册
	if (sqlQuery.next())
	{
		data[responseTypeStr] = QString::number(TcpData::Exist_Error);
		response(TcpData::Enroll_Response,data);	
		return;
	}

	//注册成功
	if (mailAddress == mailAddress_tmp)
	{
		//插入数据库user表
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
}//差新建几张数据库的表

/*************************************************
Description: 处理客户端发起的登录请求
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::logIn(QByteArray& byteArray)
{
	//获取请求登录数据
	QMap<QString, QString> requestData = MessageJson::getRequestData(byteArray);
	QString mailAddress_tmp = requestData["mailAddress"];
	QString    password_tmp = requestData["password"];
	QMap<QString, QString> data;
	QString responseTypeStr = "LogIn_Response";
	data.insert(responseTypeStr, QString::number(TcpData::Login_Correct));

	//锁住SQL中user表
	QMutexLocker sqlUser_Locker(&CommonData::sqlUser_Mutex);

	//账号不存在
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
	//密码错误
	if (password_sql != password_tmp)
	{
		data[responseTypeStr] = TcpData::Password_Error;
		response(TcpData::LogIn_Response, data);
		return;
	}

	//已在线
	QMutexLocker existMap_Locker(&CommonData::existMap_Mutex);
	if (CommonData::existMap[mailAddress_tmp])
	{
		data[responseTypeStr] = TcpData::Login_Error;
		response(TcpData::LogIn_Response, data);
		return;
	}
	CommonData::existMap[mailAddress_tmp] = true;

	//登录成功
	userName = userName_sql;
	mailAddress = mailAddress_sql;
	password = password_sql;
	data[responseTypeStr] = TcpData::Login_Correct;
	data.insert("userName", userName);
	data.insert("mailAddress", mailAddress);
	response(TcpData::LogIn_Response, data);
	return;

	qDebug() << u8"TcpSocket::logIn线程：" << QThread::currentThreadId();
}

//改成读写锁

/*************************************************
Description: 保存单次坐姿检测数据
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::detectionSave(QByteArray& byteArray)
{
	QMap<QString, QString> requestData = MessageJson::getRequestData(byteArray);
	QDate date_tmp = QDate::fromString(requestData["date"]);
	DetectionData::Detection_Result type_tmp = (DetectionData::Detection_Result)requestData["type"].toInt();

	//查询数据库
	QString sqlStr = "select * from [formname] where date= '[date]';";
	QString dateStr = date_tmp.toString();
	QString formname = "detectionData" + mailAddress;
	sqlStr.replace("[formname]", formname);
	sqlStr.replace("[date]", dateStr);
	QSqlQuery sqlQuery = mysqlConn->run(sqlStr);

	auto detectionData = new DetectionData(date_tmp);
	if (!sqlQuery.next())
	{
		//向数据库添加代表当日的记录行
		sqlStr = "INSERT INTO [formname] ( date, accuracy, number, normal, head, front, back, left, right )\
				  VALUES\
				  ('[date]', 1, 0, 0, 0, 0, 0, 0, 0 ); ";
		sqlStr.replace("[formname]", formname);
		sqlStr.replace("[date]", dateStr);
		mysqlConn->run(sqlStr);
	}
	else
	{
		detectionData->setDetectionData(sqlQuery);
	}
	detectionData->addSample(type_tmp);

	sqlStr = "UPDATE   [formname]	\
			SET date = [date],		\
			accuracy = [accuracy],	\
			  number = [number],	\
			  normal = [normal],	\
			    head = [head],		\
			   front = [front],		\
			    back = [back],		\
			  `left` = [left],		\
			 `right` = [right]; ";
	sqlStr.replace("[formname]", formname);
	sqlStr.replace("[date]", detectionData->date.toString());
	sqlStr.replace("[accuracy]", QString::number(detectionData->accuracy));
	sqlStr.replace("[number]", QString::number(detectionData->number));
	sqlStr.replace("[normal]", QString::number(detectionData->normal));
	sqlStr.replace("[head]", QString::number(detectionData->head));
	sqlStr.replace("[front]", QString::number(detectionData->front));
	sqlStr.replace("[back]", QString::number(detectionData->back));
	sqlStr.replace("[left]", QString::number(detectionData->left));
	sqlStr.replace("[right]", QString::number(detectionData->right));
	mysqlConn->run(sqlStr);
}    

/*************************************************
Description: 获取坐姿数据
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::detectionRead()
{
	QString sqlStr = "select * from [formname];";
	QString formname = "detectionData" + mailAddress;
	sqlStr.replace("[formname]", formname);
	QSqlQuery sqlQuery = mysqlConn->run(sqlStr);
	QByteArray byteArray = MessageJson::getDetectionDataByteArray(TcpData::Detection_Read_Response, sqlQuery);
	write(byteArray);
}
