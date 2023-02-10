#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qmap.h>
#include<qstringlist.h>
#include"TcpData.h"
#include<qdebug.h>

const QStringList MessageJson::VerificationCode_Request_Str = {"mailAddress"};
const QStringList MessageJson::Enroll_Request_Str = { "userName","mailAddress","password","code" };
const QStringList MessageJson::LogIn_Request_Str = {"mailAddress","password"};
/*-------------------------------------------����---------------------------------------------------*/

/*************************************************
Description: ������������
	  Input: ���Կͻ��˵������ֽ�����
	 Return: ��������
*************************************************/
TcpData::RequestType MessageJson::getRequestType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	int typeId = request_json["RequestType"].toInt();
	TcpData::RequestType type = TcpData::RequestType(typeId);
	return type;
}


/*************************************************
Description: ������������
	  Input: ���Կͻ��˵������ֽ�����
	 Return: �Ƿ����ӳɹ�
*************************************************/
QMap<QString, QString> MessageJson::getRequestData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();
	int typeId = request_json["RequestType"].toInt();
	TcpData::RequestType type = TcpData::RequestType(typeId);
	QStringList keyList = getRequestStr(type);
	
	QMap<QString, QString> data;
	for (auto& key : keyList)
	{
		data[key] = data_json[key].toString();
	}
	return data;
}


/*************************************************
Description: �����������ͻ�ȡ���ݸ�ʽ
	  Input: ��������
	 Return: ���ݸ�ʽ
*************************************************/
QStringList MessageJson::getRequestStr(TcpData::RequestType type)
{
	QStringList requestStr;
	switch (type)
	{
	case TcpData::VerificationCode:	requestStr = VerificationCode_Request_Str;	break;
	case TcpData::Enroll_Request:	requestStr = Enroll_Request_Str;			break;
	case TcpData::LogIn_Request:	requestStr = LogIn_Request_Str;			break;
	default:
		break;
	}
	return requestStr;
}

/*-------------------------------------------��Ӧ---------------------------------------------------*/


/*************************************************
Description: �����������ͻ�ȡ���ݸ�ʽ
	  Input: ��������
	 Return: ���ݸ�ʽ
*************************************************/
QByteArray MessageJson::getResponseByteArray(TcpData::ResponseType type, QMap<QString, QString> data)
{
	QJsonObject ResponseData_json;
	ResponseData_json.insert("ResponseType", type);
	
	QJsonObject data_json;
	auto it = data.constBegin();
	while (it!=data.constEnd())
	{
		data_json.insert(it.key(), it.value());
		it++;
	}
	ResponseData_json.insert("data", data_json);
	
	QJsonDocument document = QJsonDocument::QJsonDocument(ResponseData_json);
	QByteArray requestCode_byteArray = document.toBinaryData();
	return requestCode_byteArray;
}