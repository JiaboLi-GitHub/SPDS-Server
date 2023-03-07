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

MysqlServer::~MysqlServer()
{
	SQL.close();
	qDebug() << u8"���ݿ����ӶϿ�";
}

/*************************************************
Description: ִ��sql���
	  Input: sqlStr=sql���
	 Return: ��ѯ���
*************************************************/
QSqlQuery MysqlServer::run(QString sqlStr)
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
Description: �����ݿ��ѯ�������ݷ�װ��DetectionData����
	  Input: ���ݿ��ѯ���
	 Return: DetectionDataʵ��
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
