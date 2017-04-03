#include "stdafx.h"
#include "oilChange.h"


bool oilChange::isAbleToService(shared_ptr<vehicle>& veh)
{
	if (veh.get() == NULL || veh->getVehicleType() == vehicleType::electric)
		return false;
	return true;
}