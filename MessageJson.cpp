#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>



//返回请求类型
TcpData::RequestType MessageJson::requestType(QByteArray byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject resource_json = document.object();
	int typeId = resource_json["RequestType"].toInt();
	TcpData::RequestType type = (TcpData::RequestType)typeId;
	return type;
}

QString MessageJson::getMailAddress(QByteArray byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject resource_json = document.object();
	QJsonObject data_json = document["data"].toObject();
	QString mailAddress = data_json["mailAddress"].toString();
	return mailAddress;
}
