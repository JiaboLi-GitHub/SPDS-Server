#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qmap.h>
#include<qstringlist.h>

//返回请求类型
RequestType MessageJson::getRequestType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	int typeId = request_json["RequestType"].toInt();
	RequestType type = RequestType(typeId);
	return type;
}

QMap<QString, QString> MessageJson::getRequestData(QByteArray& byteArray, QStringList &keyList)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	QMap<QString, QString> enrollData;
	for (auto& key : keyList)
	{
		enrollData[key] = data_json[key].toString();
	}
	return enrollData;
}
