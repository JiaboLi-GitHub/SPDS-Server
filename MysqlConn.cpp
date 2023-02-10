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
	//�������ݿ�
	QString name = QString::number(id);
	SQL = QSqlDatabase::addDatabase("QODBC",name);
	SQL.setDatabaseName(DatabaseName);
	SQL.setHostName(HostName);
	SQL.setPort(Port.toInt());
	SQL.setUserName(UserName);
	SQL.setPassword(Password);
	if (SQL.open())
	{
		qDebug() << u8"���ݿ����ӳɹ�";
	}
	else
	{
		qDebug() << u8"���ݿ�����ʧ��";
	}
}

MysqlConn::~MysqlConn()
{
	SQL.close();
	qDebug() << u8"���ݿ����ӶϿ�";
}

/*************************************************
Description: ִ��sql���
	  Input: sqlStr=sql���
	 Return: ��ѯ���
*************************************************/
QSqlQuery MysqlConn::run(QString sqlStr)
{
	QSqlQuery sqlQuery= QSqlQuery(SQL);
	sqlQuery.exec(sqlStr);
	return sqlQuery;
}

/*************************************************
Description: ��ȡ���ݿ������ļ�
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
