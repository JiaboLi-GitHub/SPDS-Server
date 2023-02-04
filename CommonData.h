#pragma once
#include<qmutex.h>
#include<qmap.h>

class CommonData
{
public:
	CommonData();
	~CommonData();

public:
	static QMutex sqlUser_Mutex;			//锁数据库user表
	static QMap<QString,bool> existMap;		//记录账号是否在线
	static QMutex existMap_Mutex;			//锁existMap
};
