#pragma once

class TcpData
{
public:
	TcpData();
	~TcpData();



public:
	enum RequestType
	{
		VerificationCode,	//��������֤��
		Enroll,				//ע��
		LogIn				//��¼	
	};

	enum Response
	{
	//	Enroll,
	//	LogIn
	};

	enum Enroll_Response
	{
		Correct,			//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Unknown_error		//δ֪����
	};
};