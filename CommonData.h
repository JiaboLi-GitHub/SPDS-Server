#pragma once
#include<qmutex.h>
#include<qmap.h>

class CommonData
{
public:
	CommonData();
	~CommonData();

public:
	static QMutex sqlUser_Mutex;			//�����ݿ�user��
	static QMap<QString,bool> existMap;		//��¼�˺��Ƿ�����
	static QMutex existMap_Mutex;			//��existMap
};
