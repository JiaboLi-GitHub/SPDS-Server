#include "SPDData.h"
#include<qvariant.h>

SPDData::SPDData()
{
}

SPDData::SPDData(QDate date)
{
    date = date;
    accuracy = 1;
    number = normal = head = front = back = left = right = 0;
}

SPDData::~SPDData()
{
}



void SPDData::addSample(SPDOnceData::Detection_Result type)
{
	switch (type)
	{
	case SPDOnceData::Normal:
		normal++;
		break;
	case SPDOnceData::Head:
		head++;
		break;
	case SPDOnceData::Front:
		front++;
		break;
	case SPDOnceData::Back:
		back++;
		break;
	case SPDOnceData::Left:
		left++;
		break;
	case SPDOnceData::Right:
		right++;
		break;
	default:
		break;
	}
	number++;
	accuracy = normal*1.0 / number;
}
