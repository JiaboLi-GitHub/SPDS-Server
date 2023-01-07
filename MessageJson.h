#pragma once
#include<qbytearray.h>
#include<qjsonobject.h>
#include"TcpData.h"

class MessageJson
{
public:
	static TcpData::RequestType requestType(QByteArray byteArray);

	static QString getMailAddress(QByteArray byteArray);
};

