#include "JsonServer.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
#include<qmap.h>
#include<qstringlist.h>
#include"TcpData.h"
#include<qdebug.h>
#include"SPDData.h"
#include"MysqlServer.h"



/*************************************************
Description: ������������
	  Input: ���Կͻ��˵������ֽ�����
	 Return: ��������
*************************************************/
TcpData::RequestType JsonServer::getRequestType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	int typeId = request_json["RequestType"].toInt();
	TcpData::RequestType type = TcpData::RequestType(typeId);
	return type;
}


EnrollData JsonServer::toEnrollData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();
	
	EnrollData enrollData;
	enrollData.userName = data_json["userName"].toString();
	enrollData.mailAddress = data_json["mailAddress"].toString();
	enrollData.password = data_json["password"].toString();
	enrollData.code = data_json["userName"].toInt();
	return enrollData;
}

QByteArray JsonServer::toQByteArray(EnrollData data)
{
	QJsonObject ResponseData_json,data_json;
	data_json.insert("Enroll_Response", data.enroll_response);
	ResponseData_json.insert("ResponseType", TcpData::Enroll_Response);
	ResponseData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(ResponseData_json);
	return document.toBinaryData();
}










const QStringList JsonServer::VerificationCode_Request_Str = {"mailAddress"};
const QStringList JsonServer::Enroll_Request_Str = { "userName","mailAddress","password","code" };
const QStringList JsonServer::LogIn_Request_Str = {"mailAddress","password"};
/*-------------------------------------------����---------------------------------------------------*/



/*************************************************
Description: ������������
	  Input: ���Կͻ��˵������ֽ�����
	 Return: �Ƿ����ӳɹ�
*************************************************/
QMap<QString, QString> JsonServer::getRequestData(QByteArray& byteArray)
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
QStringList JsonServer::getRequestStr(TcpData::RequestType type)
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
Description: �������л�
	  Input: type=��Ӧ���� data=����
	 Return: �ַ���
*************************************************/
QByteArray JsonServer::getResponseByteArray(TcpData::ResponseType type, QMap<QString, QString> data)
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
	return document.toBinaryData();
}

/*************************************************
Description: �������л�
	  Input: ��������
	 Return: ���ݸ�ʽ
*************************************************/
QByteArray JsonServer::getDetectionDataByteArray(TcpData::ResponseType type,QSqlQuery &sqlQuery)
{
	int n = 0;
	QJsonObject responseData_json;
	QJsonArray jsonArray;
	responseData_json.insert("ResponseType", type);

	while (sqlQuery.next())
	{
		n++;
		SPDData spdData = MysqlServer::getDetectionData(sqlQuery);
		QJsonObject jsonObject = JsonServer::toQJsonObject(spdData);
		jsonArray.append(jsonObject);
	}
	responseData_json.insert("data", jsonArray);
	QJsonDocument document = QJsonDocument::QJsonDocument(responseData_json);
	return document.toBinaryData();;
}

QJsonObject JsonServer::toQJsonObject(SPDData &detectionData)
{
	QJsonObject jsonObject;
	jsonObject.insert("date", detectionData.date.toString());
	jsonObject.insert("totalcount", detectionData.accuracy);
	jsonObject.insert("pos0", detectionData.number);
	jsonObject.insert("pos1", detectionData.normal);
	jsonObject.insert("pos2", detectionData.head);
	jsonObject.insert("pos3", detectionData.front);
	jsonObject.insert("pos4", detectionData.back);
	jsonObject.insert("pos5", detectionData.left);
	jsonObject.insert("pos6", detectionData.right);
	return jsonObject;
}
