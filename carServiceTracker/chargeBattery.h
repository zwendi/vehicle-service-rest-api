#pragma once
#include "task.h"
class chargeBattery :
	public task
{
public:
	chargeBattery(string& name, string& description, double price) : task(name, description, price){}
	virtual ~chargeBattery(){};

	//to allow subclass to implement their own criteria
	virtual bool isAbleToService(shared_ptr<vehicle>&  veh) override;
protected:
	chargeBattery(){}

};

