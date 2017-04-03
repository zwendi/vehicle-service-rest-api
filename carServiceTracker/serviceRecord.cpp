
#include "stdafx.h"
#include "serviceRecord.h"
#include "restColumn.h"
const string notEmptyError = " can not be empty.";
//service record json



bool serviceRecord::setDiscount(double percentage)
{
	if (percentage <= 0 || percentage >1)
		return false;
	discount = percentage;
	return true;
}


string serviceRecord::setTask(shared_ptr<task> targetTask)
{
	if (targetTask.get() == NULL)
		return "Empty task is not allowed.";
	if (!targetTask->isAbleToService(vehiclePtr))
		return "Can not perform " + targetTask->getName() + " to vehicle " + vehiclePtr->getVinNum();
	taskPtr = targetTask;
	return "";
}


string serviceRecord::getServiceName()
{
	if (taskPtr.get())
		return taskPtr->getName();
	return "";
}


string serviceRecord::getServiceDescription()
{
	if (taskPtr.get())
		return taskPtr->getDescription();
	return "";
}

double serviceRecord::getDiscount()
{
	return discount;
}

double serviceRecord::getPrice()
{
	if (taskPtr.get())
		return taskPtr->getPrice();
	return -1;
}

double serviceRecord::getChargePrice()
{
	if (taskPtr.get())
		return taskPtr->getPrice()*discount;
	return -1;
}




string serviceRecord::getRecordNum()
{
	return recordNum;
}

string serviceRecord::getVinNum()
{
	return vehiclePtr->getVinNum();
}
string serviceRecord::getMake()
{
	return vehiclePtr->getMake();
}
string serviceRecord::getModel()
{
	return vehiclePtr->getModel();
}

string serviceRecord::validateRecord()
{
	if (recordNum.size() < 1)
		return "Record number" + notEmptyError;

	if (getVinNum().size() < 1)
		return "Vehicle vin number" + notEmptyError;

	if (getServiceName().size() < 1)
		return "Record should contain an assigned task with a valid name.";
	
	return "";
}


json::value serviceRecord::toJson()
{
	json::value object;

	if (validateRecord().size() >1)
		return object;

	object[jsonServiceRecordNum] = web::json::value(utility::conversions::to_utf16string(recordNum));
	object[jsonDiscount] = web::json::value(discount);
	object[jsonChargePrice] = web::json::value(getChargePrice());
	object[jsonTaskKey] = taskPtr->toJson();
	object[jsonServiceVehicle] = vehiclePtr->toJson();

	return object;


}
