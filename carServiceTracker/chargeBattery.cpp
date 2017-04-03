#include "stdafx.h"
#include "chargeBattery.h"



bool chargeBattery::isAbleToService(shared_ptr<vehicle>& veh)
{
	if (veh.get())
	{
		if (veh->getVehicleType() == vehicleType::electric)
			return true;
	}
	return false;
}