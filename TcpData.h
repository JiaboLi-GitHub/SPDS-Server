#pragma once
#include<qstring.h>
class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	//��������
	enum RequestType
	{
		VerificationCode,		//��������֤��
		Enroll_Request,			//ע��
		LogIn_Request,			//��¼
		Detection_Save_Request,	//��������
		Detection_Read_Request	//��ȡ����
	};
	
	//��Ӧ����
	enum ResponseType
	{
		Enroll_Response,		//ע��
		LogIn_Response,			//��¼
		Detection_Read_Response	//��ȡ������Ӧ
	};
};

class EnrollData
{
public:
	EnrollData(){}
	~EnrollData(){}

public:
	//ע����Ӧ����
	enum Enroll_Response
	{
		Enroll_Correct,		//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Enroll_error		//δ֪����
	};
	QString userName;
	QString mailAddress;
	QString password;
	int code;
	int enroll_response;
};

class LoginData
{
public:
	LoginData() {};
	~LoginData() {};

public:
	//��¼��Ӧ����
	enum Login_Response
	{
		Login_Correct,		//��¼�ɹ�
		Account_Error,		//�˺Ŵ����˺Ų����ڣ�
		Password_Error,		//�������
		Login_Error			//δ֪����
	};
	QString userName;
	QString mailAddress;
	QString password;
	QString token;
	int login_response;
};
