#pragma once
#include "vehicle.h"
class dieselVehicle : public  vehicle{

public:
	dieselVehicle(string& vin, string& model, string& make, int odometer, int year)
		: vehicle(vin, model, make, odometer, year){}
	~dieselVehicle(){}

	//allowing furthur extension of subType in case some tasks are only for specific sub type
	virtual vehicleType getVehicleType() override;

private:
	dieselVehicle(){}

};