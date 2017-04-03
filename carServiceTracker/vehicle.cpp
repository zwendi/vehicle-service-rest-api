#include "stdafx.h"
#include "vehicle.h"

#include "restColumn.h"

json::value vehicle::toJson()
{
	json::value object;

	if (vin.size() < 1 || year<0 || odometer<0)
		return object;
	object[jsonVin] = web::json::value(utility::conversions::to_utf16string(vin));
	object[jsonMake] = web::json::value(utility::conversions::to_utf16string(make));
	object[jsonModel] = web::json::value(utility::conversions::to_utf16string(model));
	object[jsonYear] = web::json::value(year);
	object[jsonOdometer] = web::json::value(odometer);
	auto vType = this->getVehicleType();
	string typeString = "";
	switch (vType)
	{
	case  vehicleType::disel:
		typeString = utility::conversions::to_utf8string(jsonDisel);
		break;
	case  vehicleType::electric:
		typeString = utility::conversions::to_utf8string(jsonElectric);
		break;
	case  vehicleType::gas:
		typeString = utility::conversions::to_utf8string(jsonGas);
		break;
	}
	object[jsonType] = web::json::value(utility::conversions::to_utf16string(typeString));
	return object;

}

string vehicle::setOdometer(int odo)
{
	if (odo < 0)
		return "Try to update vehicle with invalid odometer.";
	odometer = odo;
	return "";

}
string vehicle::setYear(int yearNum)
{
	if (yearNum<0)
		return "Try to update vehicle with invalid year.";
	year = yearNum;
	return "";
}


string vehicle::setModel(string& model)
{
	if (model.size()<1)
		return "Try to update vehicle with invalid model.";
	model = model;
	return "";
}

string vehicle::setMake(string& make)
{
	if (make.size()<1)
		return "Try to update vehicle with invalid make.";
	make = make;
	return "";
}

