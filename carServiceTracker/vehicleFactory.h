#pragma once

#include "vehicle.h"
#include <memory>

using namespace std;

/*
Allowing override  for changing way of retriving or generating task(from database or other data source)
*/

class vehicleFactory
{
public:

	vehicleFactory();
	virtual ~vehicleFactory();
	//long parameters list but it is important to have all necessary information to create a proper vehicle
	virtual shared_ptr<vehicle> createNewVehicle(string& vin, string& model, string& make, int odometer, int year, vehicleType type, string& errorMsg);
	virtual shared_ptr<vehicle> getVehicleByVin(string& vin);
	virtual void removeVehicle(string& vin);

private:
	//return null if all information is valid
	class privateData;
	privateData *pData;

};

