#include "CommonData.h"

QMutex CommonData::sqlUser_Mutex;
QMap<QString, bool> CommonData::existMap;		//��¼�˺��Ƿ�����
QMutex CommonData::existMap_Mutex;			//��existMap

CommonData::CommonData()
{

}
CommonData::~CommonData()
{

}