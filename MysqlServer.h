#pragma once
#include<qstring.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qsqldatabase.h>
#include"SPDData.h"

class MysqlServer
{
public: 
	MysqlServer(qintptr id);
	~MysqlServer();

public:
	QSqlQuery run(QString sqlStr);

public:
	static void readConfiguration();
	static SPDData getSPDData(QSqlQuery& sqlQuery);

private:
	/*数据库配置信息*/
	static const QString ConfigurationFile;
	static QString DatabaseName;			//数据库名称
	static QString HostName;				//主机名
	static QString Port;					//端口
	static QString UserName;				//用户名
	static QString Password;				//密码
	QSqlDatabase SQL;
};

