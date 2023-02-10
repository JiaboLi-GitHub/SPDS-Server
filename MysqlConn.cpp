#include "MysqlConn.h"
#include<qfile.h>
#include<qjsondocument.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<iostream>
#include<qdebug.h>

const QString MysqlConn::ConfigurationFile = ":/resources/configuration/mysql.json";
QString MysqlConn::DatabaseName;
QString MysqlConn::HostName;
QString MysqlConn::Port;
QString MysqlConn::UserName;
QString MysqlConn::Password;


MysqlConn::MysqlConn(qintptr id)
{
	//连接数据库
	QString name = QString::number(id);
	SQL = QSqlDatabase::addDatabase("QODBC",name);
	SQL.setDatabaseName(DatabaseName);
	SQL.setHostName(HostName);
	SQL.setPort(Port.toInt());
	SQL.setUserName(UserName);
	SQL.setPassword(Password);
	if (SQL.open())
	{
		qDebug() << u8"数据库连接成功";
	}
	else
	{
		qDebug() << u8"数据库连接失败";
	}
}

MysqlConn::~MysqlConn()
{
	SQL.close();
	qDebug() << u8"数据库连接断开";
}

/*************************************************
Description: 执行sql语句
	  Input: sqlStr=sql语句
	 Return: 查询结果
*************************************************/
QSqlQuery MysqlConn::run(QString sqlStr)
{
	QSqlQuery sqlQuery= QSqlQuery(SQL);
	sqlQuery.exec(sqlStr);
	return sqlQuery;
}

/*************************************************
Description: 读取数据库配置文件
	  Input: 
	 Return: 
*************************************************/
void MysqlConn::readMysqlConfiguration()
{
	QFile file(ConfigurationFile);
	file.open(QFile::ReadOnly);
	QByteArray jsonByteArray = file.readAll();
	file.close();

	QJsonDocument document = QJsonDocument::fromJson(jsonByteArray);
	QJsonObject json = document.object();
	DatabaseName = json["DatabaseName"].toString();
	HostName = json["HostName"].toString();
	Port = json["Port"].toString();
	UserName = json["UserName"].toString();
	Password = json["Password"].toString();
}
