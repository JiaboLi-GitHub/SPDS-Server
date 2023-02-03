#pragma once
#include<qbytearray.h>
#include<qjsonobject.h>
#include"TcpData.h"


class MessageJson
{
public:
	static TcpData::RequestType getRequestType(QByteArray &byteArray);
	static QMap<QString, QString> getRequestData(QByteArray& byteArray);

private:
	static QStringList getRequestStr(TcpData::RequestType type);

private:
	static const QStringList Enroll_Request_Str;
	static const QStringList Login_Request_Str;
};

