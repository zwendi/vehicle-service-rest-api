#pragma once
#include "task.h"
class oilChange :
	public task
{
public:
	oilChange(string& name, string& description, double price) : task(name, description, price){}
	virtual ~oilChange(){};

	//to allow subclass to implement their own criteria

	virtual bool isAbleToService(shared_ptr<vehicle>&  veh) override;
protected:
	oilChange(){}


};

