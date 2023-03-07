#pragma once
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qdatetime.h>
#include<qjsonobject.h>
#include"CommonData.h"

class SPDData
{
public:
	//�����
	enum Detection_Result
	{
		Normal,     //����
		Head,       //��ͷ
		Front,      //ǰ��
		Back,       //����
		Left,       //����
		Right      //����
	};

public:
	SPDData();
	SPDData(QDate date);
	~SPDData();

public:
	void addSample(Detection_Result type);

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

