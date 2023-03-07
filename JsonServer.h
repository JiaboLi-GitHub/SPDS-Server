#pragma once
#include<qbytearray.h>
#include<qjsonobject.h>
#include<qsqlquery.h>
#include"TcpData.h"
#include"SPDData.h"

class JsonServer
{
public:
	static TcpData::RequestType getRequestType(QByteArray &byteArray);
	static EnrollData toEnrollData(QByteArray& byteArray);
	static QByteArray toQByteArray(EnrollData data);

	static QMap<QString, QString> getRequestData(QByteArray& byteArray);
	static QByteArray getResponseByteArray(TcpData::ResponseType type,QMap<QString,QString> data);
	static QByteArray JsonServer::getDetectionDataByteArray(TcpData::ResponseType type,QSqlQuery &sqlQuery);

public:
	static QJsonObject toQJsonObject(SPDData &detectionData);
		
private:
	static QStringList getRequestStr(TcpData::RequestType type);

private:
	static const QStringList VerificationCode_Request_Str;
	static const QStringList Enroll_Request_Str;
	static const QStringList LogIn_Request_Str;
};

