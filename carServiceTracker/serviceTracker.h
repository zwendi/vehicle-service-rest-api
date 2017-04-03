#pragma once
#include "serviceRecord.h"
#include "vehicleFactory.h"
#include "taskGenerator.h"
#include <vector>

class serviceTracker
{
public:
	serviceTracker();
	~serviceTracker();

	//must initialize tracker with non empty factory and generator pointer
	bool initialize(vehicleFactory& vFactory, taskGenerator& tGenerator);


	//adding vehicle to track information
	string addServiceRecord(serviceRecord record);

	//string addNewVehicle(shared_ptr<vehicle>& v);
	string removeRecord(string& recordId);
   
	//get service records for particular vin 
	vector<serviceRecord> getAllServiceRecords(string& vinNum);
	
	//get all service records in the system
	vector<serviceRecord> getAllServiceRecords();
	
	//get all vehicles that have service records in the tracker
	vector<shared_ptr<vehicle>> serviceTracker::getAllVehicles();

	//get particular record by recordId, will update the serviceRecord reference
	string getServiceRecord(string& recordId, serviceRecord& serviceRecord);

	
	//this method only allows updating serviceRecord with existing service number.
	string updateServiceRecord(serviceRecord& updatedServiceRecord);

private: 
	class privateData;
	privateData * pData;
	 
};

