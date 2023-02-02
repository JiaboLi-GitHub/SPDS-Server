#include <QtCore/QCoreApplication>
#include<qtcpserver.h>
#include<qthread.h>
#include<qsqldatabase.h>
#include<qdebug.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qfile.h>
#include <iostream>

#include"TcpServer.h"
#include"Email.h"

/*************************************************
Description: ��ȡMYSQL�����ļ�
      Input: 
     Return: �����ļ��ֵ�
*************************************************/
QMap<QString, QString> readMysqlConfiguration()
{
	QMap<QString, QString> mysqlConfiguration;

    QFile file(":/resources/configuration/mysql.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return mysqlConfiguration;

	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	while (!stream.atEnd()) {
		QString line = stream.readLine();
		QStringList strList = line.split("=");
        if (strList.size() == 2)
        {
            QString key = strList[0].simplified();
            QString value = strList[1].simplified();
            mysqlConfiguration[key] = value;
        }
	}
	file.close();

    return mysqlConfiguration;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //�������ݿ�
    QMap<QString, QString> mysqlConfiguration = readMysqlConfiguration();
    QSqlDatabase SQL = QSqlDatabase::addDatabase("QODBC");
    SQL.setDatabaseName(mysqlConfiguration["DatabaseName"]);
    SQL.setHostName(mysqlConfiguration["Host"]);
    SQL.setPort(mysqlConfiguration["Port"].toInt());
    SQL.setUserName(mysqlConfiguration["UserName"]);
    SQL.setPassword(mysqlConfiguration["Password"]);
    if (SQL.open())
    {
        std::cout << "���ݿ����ӳɹ�!"<<std::endl;
    }
    else
    {
        std::cout << "���ݿ�����ʧ��!" << std::endl;;
    }

    QString query = "SELECT * from user where mailAddress = [mailAddress]";
    query.replace("[mailAddress]", "'1955474265@qq.com'");
    qDebug() << query;
    QSqlQuery sqlQuery;
    sqlQuery.exec(query);
    qDebug() << sqlQuery.next();
    qDebug() << sqlQuery.value("userName").toString();
    
    TcpServer *tcpServer=new TcpServer(new QObject(),8888);
    MainStart* mainStart = new MainStart();
    return a.exec();
}
