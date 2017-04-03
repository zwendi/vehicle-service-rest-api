#pragma once
#include "vehicle.h"
class gasVehicle :
	public vehicle
{
public:
	gasVehicle(string& vin, string& model, string& make, int odometer, int year) : vehicle(vin, model, make, odometer, year){}
	~gasVehicle(){}

	//allowing furthur extension of subType in case some tasks are only for specific sub type
	virtual vehicleType getVehicleType() override;

private:
	gasVehicle(){}
};

