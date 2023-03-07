#include "MysqlServer.h"
#include<qfile.h>
#include<qjsondocument.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<iostream>
#include<qdebug.h>

const QString MysqlServer::ConfigurationFile = ":/resources/configuration/mysql.json";
QString MysqlServer::DatabaseName;
QString MysqlServer::HostName;
QString MysqlServer::Port;
QString MysqlServer::UserName;
QString MysqlServer::Password;


MysqlServer::MysqlServer(qintptr id)
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

MysqlServer::~MysqlServer()
{
	SQL.close();
	qDebug() << u8"数据库连接断开";
}

/*************************************************
Description: 执行sql语句
	  Input: sqlStr=sql语句
	 Return: 查询结果
*************************************************/
QSqlQuery MysqlServer::run(QString sqlStr)
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
void MysqlServer::readConfiguration()
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

/*************************************************
Description: 将数据库查询到的数据封装成DetectionData类型
	  Input: 数据库查询结果
	 Return: DetectionData实例
*************************************************/
SPDData MysqlServer::getSPDData(QSqlQuery& sqlQuery)
{
	SPDData data;
	data.date = QDate::fromString(sqlQuery.value("accuracy").toString());
	data.accuracy = sqlQuery.value("accuracy").Double;
	data.number = sqlQuery.value("number").Int;
	data.normal = sqlQuery.value("normal").Int;
	data.head = sqlQuery.value("head").Int;
	data.front = sqlQuery.value("front").Int;
	data.back = sqlQuery.value("back").Int;
	data.left = sqlQuery.value("left").Int;
	data.right = sqlQuery.value("right").Int;
	return data;
}
