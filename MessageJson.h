#pragma once
#include<qbytearray.h>
#include<qjsonobject.h>
#include"TcpData.h"

class MessageJson
{
public:
	static TcpData::RequestType getRequestType(QByteArray &byteArray);	
	static QMap<QString, QString> getRequestData(QByteArray& byteArray,QStringList &keyList);
};

