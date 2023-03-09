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


	static CodeData toCodeData(QByteArray& byteArray);
	static EnrollData toEnrollData(QByteArray& byteArray);
	static LoginData toLogInData(QByteArray& byteArray);
	static SPDOnceData toSPDOnceData(QByteArray& byteArray);
	
	static QByteArray toQByteArray(EnrollData data);
	static QByteArray toQByteArray(LoginData data);
	static QByteArray toQByteArray(QVector<SPDData> dataList);
	static QByteArray toQByteArray(QuitData data);
};

