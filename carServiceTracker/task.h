#pragma once
#include "vehicle.h"
#include <memory>
#include <cpprest/json.h>

using namespace web;
class task
{
public:
	task(string& name, string& description, double price) : taskName(name), taskDescription(description), taskPrice(price){};
	virtual ~task(){};
	
	bool setName(string& name);
	bool setDescription(string& description);
	bool setPrice(double price);

	inline string getName(){ return taskName; };
	inline string getDescription(){ return taskDescription; };
	inline double getPrice(){ return taskPrice; };

	//subclass to implement necessary constraint
	virtual bool isAbleToService(shared_ptr<vehicle>& veh) = 0;

	//subclass is able to override it
	virtual json::value toJson();

protected:
	/***
	preventing task exist without necessary information
	but subclass can decide if they want to expose it
	***/
	task(){}

private:
	string taskName;
	string taskDescription;
	double taskPrice;
};

