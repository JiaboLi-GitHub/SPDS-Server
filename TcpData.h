#pragma once
#include<qstring.h>
class TcpData
{
public:
	TcpData();
	~TcpData();

public:
	//请求类型
	enum RequestType
	{
		VerificationCode,		//请求发送验证码
		Enroll_Request,			//注册
		LogIn_Request,			//登录
		Detection_Save_Request,	//保存样本
		Detection_Read_Request	//读取样本
	};
	
	//响应类型
	enum ResponseType
	{
		Enroll_Response,		//注册
		LogIn_Response,			//登录
		Detection_Read_Response	//读取样本响应
	};
};

class EnrollData
{
public:
	EnrollData(){}
	~EnrollData(){}

public:
	//注册响应类型
	enum Enroll_Response
	{
		Enroll_Correct,		//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Enroll_error		//未知错误
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
	//登录响应类型
	enum Login_Response
	{
		Login_Correct,		//登录成功
		Account_Error,		//账号错误（账号不存在）
		Password_Error,		//密码错误
		Login_Error			//未知错误
	};
	QString userName;
	QString mailAddress;
	QString password;
	QString token;
	int login_response;
};
