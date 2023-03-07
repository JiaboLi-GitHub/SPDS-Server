#include "TcpSocket.h"
#include<qbytearray.h>
#include<qrandom.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qmutex.h>
#include<iostream>

#include"TcpData.h"
#include"JsonServer.h"
#include"Email.h"
#include"CommonData.h"
#include"MysqlServer.h"
#include"SPDData.h"
#include<qdebug.h>
#include<qhostaddress.h>

TcpSocket::TcpSocket(qintptr id)
{
	setSocketDescriptor(id);
	socketDescriptor = id;
	mysqlServer = new MysqlServer(id);

	//获取用户IP地址
	this->ipv4_int32 = peerAddress().toIPv4Address();
	this->ipv4_str = QHostAddress(ipv4_int32).toString();

	connect(this, &QTcpSocket::readyRead, this, &TcpSocket::read);
	connect(this, &QTcpSocket::disconnected, this, &TcpSocket::disconnect);
}

TcpSocket::~TcpSocket()
{
	delete mysqlServer;
	qDebug() << u8"释放::线程：" << QThread::currentThreadId();
}

void TcpSocket::read()
{
	QByteArray byteArray = readAll();
	TcpData::RequestType type = JsonServer::getRequestType(byteArray);

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
	emit disconnected(socketDescriptor);
}


/*************************************************
Description: 处理客户端发起的获取验证码请求
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::verificationCode(QByteArray &byteArray)
{
	//获取邮箱号
	QString mailAddress_tmp = JsonServer::getMailAddress(byteArray);

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
	EnrollData enrollData = JsonServer::toEnrollData(byteArray);

	//对比验证码
	if (enrollData.code != code && QDateTime::currentDateTime()<=code_DateTme.addSecs(300))
	{
		enrollData.enroll_response = EnrollData::Code_Error;
		QByteArray resByteArray = JsonServer::toQByteArray(enrollData);
		write(byteArray);
		return;
	}
	code.clear();

	//锁住SQL中user表
	QMutexLocker locker(&CommonData::sqlUser_Mutex);
	
	//查询数据库
	QString sqlStr = "select * from user where mailAddress = '[mailAddress]'";
	sqlStr.replace("[mailAddress]", enrollData.mailAddress);
	auto sqlQuery = mysqlServer->run(sqlStr);

	//账号是否已经注册
	if (sqlQuery.next())
	{
		enrollData.enroll_response = EnrollData::Exist_Error;
		QByteArray resByteArray = JsonServer::toQByteArray(enrollData);
		write(byteArray);
		return;
	}

	//注册成功
	if (mailAddress == enrollData.mailAddress)
	{
		//插入数据库user表
		sqlStr = "insert into user values ('[mailAddress]',	\
							 			  '[userName]',		\
						  				  '[password]',		\
										  '[dateTime]',		\
										  '[endDateTime]',	\
										  '[ip]',			\
										  '[token]')";

		sqlStr.replace("[mailAddress]", enrollData.mailAddress);
		sqlStr.replace("[userName]",	enrollData.userName);
		sqlStr.replace("[password]",	enrollData.password);
		sqlStr.replace("[enrollDate]",	QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
		sqlStr.replace("[ip]",			this->ipv4_str);
		sqlStr.replace("[token]",		tokenBuilder(enrollData.mailAddress));
		sqlQuery= mysqlServer->run(sqlStr);
	}
	enrollData.enroll_response = EnrollData::Enroll_Correct;
	QByteArray resByteArray = JsonServer::toQByteArray(enrollData);
	write(byteArray);
	
	//创建该用户的坐姿记录表
	sqlStr="CREATE TABLE `[tableName]` (	\
		`id` INT NOT NULL,			\
		`date` VARCHAR(15) NOT NULL,\
		`accuracy` DOUBLE DEFAULT NULL,\
		`number` INT DEFAULT NULL,\
		`normal` INT DEFAULT NULL,\
		`head` INT DEFAULT NULL,\
		`front` INT DEFAULT NULL,\
		`back` INT DEFAULT NULL,\
		`left` INT DEFAULT NULL,\
		`right` INT DEFAULT NULL,\
		PRIMARY KEY(`date`)\
		)";
	sqlStr.replace("[tableName]", mailAddress);
	mysqlServer->run(sqlStr);
}

/*************************************************
Description: 处理客户端发起的登录请求
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::logIn(QByteArray& byteArray)
{
	LoginData logInData = JsonServer::toLogInData(byteArray);

	//锁住SQL中user表
	QMutexLocker sqlUser_Locker(&CommonData::sqlUser_Mutex);

	//账号不存在
	QString query = "select * from user where mailAddress = '[mailAddress]'";
	query.replace("[mailAddress]", logInData.mailAddress);
	QSqlQuery sqlQuery = mysqlServer->run(query);
	bool res = sqlQuery.next();
	if (!res)
	{
		logInData.login_response = LoginData::Account_Error;
		QByteArray resByteArray = JsonServer::toQByteArray(logInData);
		write(resByteArray);
		return;
	}

	//密码错误
	QString    userName_sql = sqlQuery.value("userName").toString();
	QString mailAddress_sql = sqlQuery.value("mailAddress").toString();
	QString    password_sql = sqlQuery.value("password").toString();
	if (password_sql != logInData.password)
	{
		logInData.login_response = LoginData::Password_Error;
		QByteArray resByteArray = JsonServer::toQByteArray(logInData);
		write(resByteArray);
		return;
	}

	//已在线
	QMutexLocker existMap_Locker(&CommonData::existMap_Mutex);
	if (CommonData::existMap[logInData.mailAddress])
	{
		logInData.login_response = LoginData::Login_Error;
		QByteArray resByteArray = JsonServer::toQByteArray(logInData);
		write(resByteArray);
		return;
	}
	CommonData::existMap[logInData.mailAddress] = true;

	//登录成功
	userName = userName_sql;
	mailAddress = mailAddress_sql;
	password = password_sql;
	QString token = tokenBuilder(mailAddress);
	logInData.token = token;
	logInData.login_response = LoginData::Login_Correct;
	QByteArray resByteArray = JsonServer::toQByteArray(logInData);
	write(resByteArray);

	//更新数据库中的token
	QString sqlStr = "update user set token=[token] where mailAddress=[mailAddress];";
	sqlStr.replace("[token]", token);
	sqlStr.replace("[mailAddress]", mailAddress);
	mysqlServer->run(sqlStr);
	return;
}

/*************************************************
Description: 保存单次坐姿检测数据
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::detectionSave(QByteArray& byteArray)
{
	SPDOnceData spdOnceData = JsonServer::toSPDOnceData(byteArray);

	//查询数据库
	QString sqlStr = "select * from [tableName] a order by id desc limit 1;";
	sqlStr.replace("[tableName]", mailAddress);
	QSqlQuery sqlQuery = mysqlServer->run(sqlStr);
	int st = sqlQuery.next();
	int id = 0;
	if (st)
	{
		QString date_sql = sqlQuery.value("date").toString();
		id = sqlQuery.value("id").toInt();
		if (date_sql != spdOnceData.date.toString())
		{
			st = false;
		}
	}

	SPDData detectionData(spdOnceData.date);
	//是否存在今日数据
	if (!st)
	{
		sqlStr = "INSERT INTO [tableName] ( id, date, accuracy, number, normal, head, front, back, left, right )\
				  VALUES\
				  ('[id]','[date]', 1, 0, 0, 0, 0, 0, 0, 0 ); ";
		sqlStr.replace("[tableName]", mailAddress);
		sqlStr.replace("[id]", QString::number(id + 1));
		sqlStr.replace("[date]", spdOnceData.date.toString());
		mysqlServer->run(sqlStr);
	}
	else
	{
		detectionData = MysqlServer::getSPDData(sqlQuery);
	}
	detectionData.addSample(spdOnceData.result);

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
	sqlStr.replace("[formname]", mailAddress);
	sqlStr.replace("[date]", detectionData.date.toString());
	sqlStr.replace("[accuracy]", QString::number(detectionData.accuracy));
	sqlStr.replace("[number]", QString::number(detectionData.number));
	sqlStr.replace("[normal]", QString::number(detectionData.normal));
	sqlStr.replace("[head]", QString::number(detectionData.head));
	sqlStr.replace("[front]", QString::number(detectionData.front));
	sqlStr.replace("[back]", QString::number(detectionData.back));
	sqlStr.replace("[left]", QString::number(detectionData.left));
	sqlStr.replace("[right]", QString::number(detectionData.right));
	mysqlServer->run(sqlStr);
}    

/*************************************************
Description: 获取坐姿数据
	  Input: byteArray=客户端发来的数据
	 Return: 空
*************************************************/
void TcpSocket::detectionRead()
{
	//查询行数
	QString sqlStr = "SELECT COUNT(*) FROM [tableName];";
	sqlStr.replace("[tableName]", mailAddress);
	auto sqlQuery = mysqlServer->run(sqlStr);
	sqlQuery.next();
	int n = sqlQuery.value("COUNT(*)").toInt();

	//查询倒数5行
	sqlStr = "select * from [tableName] order by id desc limit [begin],[SampleStack_Max]";
	sqlStr.replace("[tableName]", mailAddress);
	sqlStr.replace("[SampleStack_Max]", QString::number(SampleStack_Max));
	sqlStr.replace("[begin]", QString::number(n >= SampleStack_Max ? n - SampleStack_Max : 0));
	sqlQuery = mysqlServer->run(sqlStr);

	QVector<SPDData> spdDataList;
	while (sqlQuery.next())
	{
		SPDData spdData = MysqlServer::getSPDData(sqlQuery);
		spdDataList.append(spdData);
	}
	QByteArray resByteArray = JsonServer::toQByteArray(spdDataList);
	write(resByteArray);
}

/*************************************************
Description: 根据邮箱号生成token字符串
	  Input: mailAddress=邮箱号
	 Return: token
*************************************************/
QString TcpSocket::tokenBuilder(QString mailAddress)
{
	QString token=mailAddress;
	int n = TOKENLEN - token.size();
	for (int i = 1; i <= n; i++)
	{
		int num = QRandomGenerator::global()->bounded(0, 35);
		num < 10 ? num += '0' : num += 'A';
		token += (char)num;
	}
	return token;
}