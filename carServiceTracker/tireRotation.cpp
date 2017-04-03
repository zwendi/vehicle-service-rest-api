#include "stdafx.h"
#include "tireRotation.h"

//for this example tire rotation is applied to all types of vehicle. Don't inline it to keep coding stype consistent
bool tireRotation::isAbleToService(shared_ptr<vehicle>& vehicle)
{
	if (vehicle.get())
		return true;
	return false;
}