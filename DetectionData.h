#pragma once
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qdatetime.h>
#include<qjsonobject.h>
#include"CommonData.h"



class DetectionData
{
public:
	//检测结果
	enum Detection_Result
	{
		Normal,     //正常
		Head,       //托头
		Front,      //前倾
		Back,       //后倾
		Left,       //左倾
		Right      //右倾
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
    QDate   date;       //时间
    double  accuracy;   //正确率
    int     number;     //检测次数
    int     normal;     //正常
    int     head;       //托头
    int     front;      //前倾
    int     back;       //后倾
    int     left;       //左倾
    int     right;      //右倾

};

