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
	/*���ݿ�������Ϣ*/
	static const QString ConfigurationFile;
	static QString DatabaseName;			//���ݿ�����
	static QString HostName;				//������
	static QString Port;					//�˿�
	static QString UserName;				//�û���
	static QString Password;				//����
	QSqlDatabase SQL;
};

