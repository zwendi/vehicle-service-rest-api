#pragma once
#include "task.h"
class tireRotation :
	public task
{
public:
	tireRotation(string& name, string& description, double price) : task(name, description, price){}
	virtual ~tireRotation(){};

	//to allow subclass to implement their own criteria
	virtual bool isAbleToService(shared_ptr<vehicle>& veh) override;
protected:
	tireRotation(){}
};

