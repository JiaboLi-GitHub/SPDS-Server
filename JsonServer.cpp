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
Description: 解析请求类型
	  Input: 来自客户端的请求字节数组
	 Return: 请求类型
*************************************************/
TcpData::RequestType JsonServer::getRequestType(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	int typeId = request_json["RequestType"].toInt();
	TcpData::RequestType type = TcpData::RequestType(typeId);
	return type;
}

CodeData JsonServer::toCodeData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	CodeData codeData;
	codeData.mailAddress = data_json["mailAddress"].toString();
	return codeData;
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
	enrollData.code = data_json["code"].toInt();
	return enrollData;
}

LoginData JsonServer::toLogInData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	LoginData loginData;
	loginData.mailAddress = data_json["mailAddress"].toString();
	loginData.password = data_json["password"].toString();
	return loginData;
}

SPDOnceData JsonServer::toSPDOnceData(QByteArray& byteArray)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(byteArray);
	QJsonObject request_json = document.object();
	QJsonObject data_json = request_json["data"].toObject();

	SPDOnceData spdOnceData;
	spdOnceData.date = QDate::fromString(data_json["mailAddress"].toString(),"yyyy-MM-dd");
	spdOnceData.result = (SPDOnceData::Detection_Result)data_json["Detection_Result"].toInt();
	return spdOnceData;
}

QByteArray JsonServer::toQByteArray(EnrollData data)
{
	QJsonObject reqData_json,data_json;
	data_json.insert("Enroll_Response", data.enroll_response);
	reqData_json.insert("ResponseType", TcpData::Enroll_Response);
	reqData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(reqData_json);
	return document.toBinaryData();
}

QByteArray JsonServer::toQByteArray(LoginData data)
{
	QJsonObject reqData_json, data_json;
	data_json.insert("Login_Response", data.login_response);
	data_json.insert("userName", data.userName);
	data_json.insert("mailAddress", data.mailAddress);
	data_json.insert("token", data.token);
	reqData_json.insert("ResponseType", TcpData::LogIn_Request);
	reqData_json.insert("data", data_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(reqData_json);
	return document.toBinaryData();
}

QByteArray JsonServer::toQByteArray(QVector<SPDData> dataList)
{
	QJsonObject reqData_json;
	QJsonArray dataList_json;
	for (auto& data : dataList)
	{
		QJsonObject data_json;
		data_json.insert("date",data.date.toString());
		data_json.insert("accuracy", data.accuracy);
		data_json.insert("number", data.number);
		data_json.insert("normal", data.normal);
		data_json.insert("head", data.head);
		data_json.insert("front", data.front);
		data_json.insert("back", data.back);
		data_json.insert("left", data.left);
		data_json.insert("right", data.right);
		dataList_json.append(data_json);
	}
	reqData_json.insert("ResponseType", TcpData::Detection_Read_Response);
	reqData_json.insert("data", dataList_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(reqData_json);
	return document.toBinaryData();
}

QByteArray JsonServer::toQByteArray(QuitData data)
{
	QJsonObject reqData_json;
	QJsonArray dataList_json;

	reqData_json.insert("ResponseType", TcpData::Quit_Request);
	reqData_json.insert("data", dataList_json);
	QJsonDocument document = QJsonDocument::QJsonDocument(reqData_json);
	return document.toBinaryData();
}

