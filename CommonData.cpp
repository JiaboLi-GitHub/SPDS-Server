#include "CommonData.h"


QMutex CommonData::sqlUser_Mutex;
QMap<QString, bool> CommonData::existMap;
QMutex CommonData::existMap_Mutex;

CommonData::CommonData()
{

}
CommonData::~CommonData()
{

}

