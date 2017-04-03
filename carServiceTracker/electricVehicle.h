#pragma once
#include "vehicle.h"
class electricVehicle : public vehicle
{
public:
	electricVehicle(string& vin, string& model, string& make, int odometer, int year)
		: vehicle(vin, model, make, odometer, year){};
	~electricVehicle(){};

	//allowing furthur extension of subType in case some tasks are only for specific sub type
	virtual vehicleType getVehicleType() override;

private:
	electricVehicle(){}
};

