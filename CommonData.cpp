#include "CommonData.h"

QMutex CommonData::sqlUser_Mutex;
QMap<QString, bool> CommonData::existMap;		//记录账号是否在线
QMutex CommonData::existMap_Mutex;			//锁existMap

CommonData::CommonData()
{

}
CommonData::~CommonData()
{

}