#pragma once
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qdatetime.h>
#include<qjsonobject.h>
#include"CommonData.h"

class SPDOnceData
{
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
	SPDOnceData(){};
	SPDOnceData(QDate date, Detection_Result result) :
		date(date), result(result) {}
	~SPDOnceData() = default;

public:
	QDate date;		//����
	Detection_Result result;//�����
};


class SPDData
{
public:
	SPDData();
	SPDData(QDate date);
	~SPDData();

public:
	void addSample(SPDOnceData::Detection_Result type);

public:
    QDate   date;       //ʱ��
    double  accuracy;   //��ȷ��
    int     number;     //������
    int     normal;     //����
    int     head;       //��ͷ
    int     front;      //ǰ��
    int     back;       //����
    int     left;       //����
    int     right;      //����
};

