#include "stdafx.h"
#include "vehicleFactory.h"
#include "dieselVehicle.h"
#include "electricVehicle.h"
#include "gasVehicle.h"
#include <unordered_map>
const string nullError = "should not be empty."; 
const string invalidNumError = "is invalid.";
const string vinNumberTaken = "Vin number is taken.";

class vehicleFactory::privateData{
public:

	// using private implementation to hide implementation detail, we may use database connection for this but not affecting rest of code
	unordered_map<string,shared_ptr<vehicle>> vinMap;  
	string infoCheck(string& vin, string& model, string& make, int odometer, int year);

};

vehicleFactory::vehicleFactory()
{
	pData = new privateData;
}
vehicleFactory::~vehicleFactory(){
	delete pData;
};

string vehicleFactory::privateData::infoCheck(string& vin, string& model, string& make, int odometer, int year)
{
	if (vin.size() < 1)
		return "Vin number " + nullError;	
	else if (vinMap.count(vin)==1)          
		return vinNumberTaken;	
	else if (model.size() < 1)
		return "Model number " + nullError;
	else if (make.size() < 1)
		return "Make information " + nullError;
	else if (odometer < 0)
		return "Odometer information " + invalidNumError;
	else if (year < 0)
		return "Year made information " + invalidNumError;
	else
		return "";
}

shared_ptr<vehicle> vehicleFactory::createNewVehicle(string& vin, string& model, string& make, int odometer, int year, vehicleType type, string& errorMsg)
{
	errorMsg = pData->infoCheck(vin, model, make, odometer, year);
	if (errorMsg.size() > 0)
		return NULL;

	shared_ptr<vehicle> resPtr;
	switch (type)
	{
	case vehicleType::electric:
		resPtr = shared_ptr<vehicle>(new electricVehicle(vin, model, make, odometer, year));
		break;
	case vehicleType::gas:
		resPtr = shared_ptr<vehicle>(new gasVehicle(vin, model, make, odometer, year));
		break;
	case vehicleType::disel:
		resPtr = shared_ptr<vehicle>(new dieselVehicle(vin, model, make, odometer, year));
		break;
	default:
		errorMsg = "Invalid vehicle type.";
		resPtr = NULL;
		break;
	}
	
	// registering vin number for unique vinNumber for each vehicle;
	if (resPtr)
		pData->vinMap.insert(std::make_pair(string(vin),resPtr));   
	
	return resPtr;
}


shared_ptr<vehicle> vehicleFactory::getVehicleByVin(string& vin)
{
	if (pData->vinMap.find(vin) != pData->vinMap.end())
		return pData->vinMap[vin];
	return NULL;
}

void vehicleFactory::removeVehicle(string& vin)
{
	if (pData->vinMap.find(vin) != pData->vinMap.end())
		pData->vinMap.erase(vin);

}
