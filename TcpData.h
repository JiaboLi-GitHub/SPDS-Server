#pragma once
class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	//��������
	enum RequestType
	{
		VerificationCode,	//��������֤��
		Enroll_Request,		//ע��
		LogIn_Request		//��¼
	};
	
	//��Ӧ����
	enum ResponseType
	{
		Enroll_Response,	//ע��
		LogIn_Response		//��¼
	};

	//ע����Ӧ����
	enum Enroll_Response
	{
		Enroll_Correct,		//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Enroll_error		//δ֪����
	};

	//��¼��Ӧ����
	enum Login_Response
	{
		Login_Correct,		//��¼�ɹ�
		Account_Error,		//�˺Ŵ���
		Login_error			//δ֪����
	};
};

