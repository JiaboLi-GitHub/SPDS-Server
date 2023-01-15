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

	enum Response
	{
	//	Enroll,
	//	LogIn
	};

	enum Enroll_Response
	{
		Correct,			//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Unknown_error		//未知错误
	};
};