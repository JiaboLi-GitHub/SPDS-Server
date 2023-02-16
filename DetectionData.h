#pragma once
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qdatetime.h>
#include<qjsonobject.h>
#include"CommonData.h"



class DetectionData
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
	DetectionData();
	DetectionData(QDate date);
	~DetectionData();

public:
    void setDetectionData(QSqlQuery& sqlQuery);
	void addSample(Detection_Result type);
	QJsonObject getQJsonObject();


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

