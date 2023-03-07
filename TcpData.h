#pragma once
#include <qobject.h>
#include <qdatetime.h>

class TcpData : QObject
{
	Q_OBJECT
public:
	TcpData();
	~TcpData();

public:
	//请求类型
	enum RequestType
	{
		VerificationCode,	 //请求发送验证码
		Enroll_Request,		 //注册
		LogIn_Request,		 //登录
		Detection_Save_Request, //保存样本
		Detection_Read_Request, //读取样本
		Auto_Login_Request, //自动登录请求
		Quit_Request //注销登录
	};

	//响应类型
	enum ResponseType
	{
		Enroll_Response,	//注册
		LogIn_Response,		//登录
		Detection_Read_Response, //读取样本响应
		Auto_Login_Response //自动登录响应
	};

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //自动登录成功
		Auto_Login_Error       //自动登录失败
	};
};

class SPDDataPerDay : public QObject
{
	Q_OBJECT

public:
	SPDDataPerDay(QDate date, double accuracy, qint32 totalCount, qint32 normal,
		qint32 head, qint32 front, qint32 back, qint32 left, qint32 right) :
		date(date), accuracy(accuracy), totalCount(totalCount), normal(normal), head(head),
		front(front), back(back), left(left), right(right) {}
	~SPDDataPerDay() = default;

private:
	QDate date;
	double accuracy;
	qint32 totalCount;
	qint32 normal, head, front, back, left, right;
};

class SPDData :QObject
{
	Q_OBJECT
public:
	//检测结果
	enum Detection_Result
	{
		Normal, //正常
		Head,   //托头
		Front,  //前倾
		Back,   //后倾
		Left,   //左倾
		Right   //右倾
	};

public:
	SPDData(QDate date, Detection_Result result) :
		date(date), result(result) {}
	~SPDData() = default;

public:
	QDate date;		//日期
	int result;//监测结果
};

class EnrollData :QObject
{
	Q_OBJECT
public:
	EnrollData(QString userName, QString mailAddress, QString password,
		int code, int enroll_response) :
		userName(userName), mailAddress(mailAddress), password(password),
		code(code), enroll_response(enroll_response) {}
	~EnrollData() {}

public:
	//注册响应类型
	enum Enroll_Response
	{
		Enroll_Correct,		//注册成功
		Code_Error,			//验证码错误
		Exist_Error,		//账号已注册
		Enroll_Error		//未知错误
	};
	QString userName;
	QString mailAddress;
	QString password;
	int code;
	int enroll_response;
};

class LoginData :QObject
{
	Q_OBJECT
public:
	LoginData(QString userName, QString mailAddress, QString password,
		QString token, int login_response) :
		userName(userName), mailAddress(mailAddress), password(password),
		token(token), login_response(login_response) {}
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
