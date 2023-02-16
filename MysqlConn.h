#pragma once
#include<qstring.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qsqldatabase.h>

class MysqlConn
{
public: 
	MysqlConn(qintptr id);
	~MysqlConn();

public:
	QSqlQuery run(QString sqlStr);

public:
	static void readMysqlConfiguration();

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

