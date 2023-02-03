#pragma once
class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	enum RequestType
	{
		VerificationCode,	//请求发送验证码
		Enroll,				//注册
		LogIn				//登录	
	};

	enum ResponseType
	{
		//	Enroll,
		//	LogIn
	};

	enum Enroll_Response
	{
		Enroll_Correct,		//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Enroll_error		//未知错误
	};

	enum Login_Response
	{
		Login_Correct,		//登录成功
		Account_Error,		//账号错误
		Login_error			//未知错误
	};
};

