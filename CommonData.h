#pragma once
#include<qmutex.h>

class CommonData
{
public:
	CommonData();
	~CommonData();

public:
	static QMutex sqlUser_mutex;	//�����ݿ�user��

};

