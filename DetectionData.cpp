#include "DetectionData.h"
#include<qvariant.h>

DetectionData::DetectionData()
{
}

DetectionData::DetectionData(QDate date)
{
    date = date;
    accuracy = 1;
    number = normal = head = front = back = left = right = 0;
}

DetectionData::~DetectionData()
{
}

void DetectionData::setDetectionData(QSqlQuery& sqlQuery)
{
	date = QDate::fromString(sqlQuery.value("accuracy").toString());
	accuracy = sqlQuery.value("accuracy").Double;
	number = sqlQuery.value("number").Int;
	normal = sqlQuery.value("normal").Int;
	head = sqlQuery.value("head").Int;
	front = sqlQuery.value("front").Int;
	back = sqlQuery.value("back").Int;
	left = sqlQuery.value("left").Int;
	right = sqlQuery.value("right").Int;
}

void DetectionData::addSample(Detection_Result type)
{
	switch (type)
	{
	case DetectionData::Normal:
		normal++;
		break;
	case DetectionData::Head:
		head++;
		break;
	case DetectionData::Front:
		front++;
		break;
	case DetectionData::Back:
		back++;
		break;
	case DetectionData::Left:
		left++;
		break;
	case DetectionData::Right:
		right++;
		break;
	default:
		break;
	}
	number++;
	accuracy = normal*1.0 / number;
}

QJsonObject DetectionData::getQJsonObject()
{
	QJsonObject jsonObject;
	jsonObject.insert("date", date.toString());
	jsonObject.insert("totalcount", accuracy);
	jsonObject.insert("pos0", number);
	jsonObject.insert("pos1", normal);
	jsonObject.insert("pos2", head);
	jsonObject.insert("pos3", front);
	jsonObject.insert("pos4", back);
	jsonObject.insert("pos5", left);
	jsonObject.insert("pos6", right);
	return jsonObject;
}
