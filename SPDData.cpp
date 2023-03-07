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



void SPDData::addSample(Detection_Result type)
{
	switch (type)
	{
	case SPDData::Normal:
		normal++;
		break;
	case SPDData::Head:
		head++;
		break;
	case SPDData::Front:
		front++;
		break;
	case SPDData::Back:
		back++;
		break;
	case SPDData::Left:
		left++;
		break;
	case SPDData::Right:
		right++;
		break;
	default:
		break;
	}
	number++;
	accuracy = normal*1.0 / number;
}
