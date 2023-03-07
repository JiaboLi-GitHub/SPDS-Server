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
	//��������
	enum RequestType
	{
		VerificationCode,	 //��������֤��
		Enroll_Request,		 //ע��
		LogIn_Request,		 //��¼
		Detection_Save_Request, //��������
		Detection_Read_Request, //��ȡ����
		Auto_Login_Request, //�Զ���¼����
		Quit_Request //ע����¼
	};

	//��Ӧ����
	enum ResponseType
	{
		Enroll_Response,	//ע��
		LogIn_Response,		//��¼
		Detection_Read_Response, //��ȡ������Ӧ
		Auto_Login_Response //�Զ���¼��Ӧ
	};

	enum Auto_Login_Response
	{
		Auto_Login_Correct,    //�Զ���¼�ɹ�
		Auto_Login_Error       //�Զ���¼ʧ��
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
	//�����
	enum Detection_Result
	{
		Normal, //����
		Head,   //��ͷ
		Front,  //ǰ��
		Back,   //����
		Left,   //����
		Right   //����
	};

public:
	SPDData(QDate date, Detection_Result result) :
		date(date), result(result) {}
	~SPDData() = default;

public:
	QDate date;		//����
	int result;//�����
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
	//ע����Ӧ����
	enum Enroll_Response
	{
		Enroll_Correct,		//ע��ɹ�
		Code_Error,			//��֤�����
		Exist_Error,		//�˺���ע��
		Enroll_Error		//δ֪����
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
