#pragma once
#include <string>
#include <cpprest/json.h>
using namespace std;
using namespace web;

enum class vehicleType{
	electric =0,
	gas,
	disel,
	vehicleMax
};

class vehicle
{
public:

	vehicle(string& vin, string& model, string& make, int odometer, int year) :vin(vin), model(model), make(make), odometer(odometer), year(year){}
	virtual ~vehicle(){}
	inline string getVinNum(){ return vin; }
	inline string getMake(){ return make; }
	inline string getModel(){ return model;}
	inline int getYear(){ return year; }
	inline int getOdometer(){ return odometer;  }
	virtual vehicleType getVehicleType() = 0;
	virtual json::value toJson();

	//setter, vin number can't be changed. Different vin is different vehicle.
	virtual string setOdometer(int odo);
	virtual string setYear(int yearNum);
	virtual string setModel(string& model);
	virtual string setMake(string& make);


protected:
	vehicle(){}
    
private:
	string vin;
	string model;
	string make;
	int year;
	int odometer;



};

