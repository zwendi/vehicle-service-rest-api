#include "stdafx.h"
#include "serviceTracker.h"
#include <unordered_map>
//implementation abbreviation
#define recordMap  unordered_map<string, serviceRecord>

const string notEmptyError = " can not be empty.";
const string nonInitializedError = "Service tracker is not properly initialized.";
const string recordNumTakenError = "Record number has been used.Please change the record number.";

class serviceTracker::privateData{
public:
	~privateData()
	{
		vFactory = 0;
		tGenerator = 0;
	}
	bool checkInitialized();
	unordered_map<string, recordMap> vehicleStore;
	vehicleFactory* vFactory;
	taskGenerator* tGenerator;
	bool initialized = false;
};

bool serviceTracker::privateData::checkInitialized(){
	return initialized;
}

serviceTracker::serviceTracker()
{
	pData = new privateData;
}

serviceTracker::~serviceTracker()
{
	delete pData;
}

bool serviceTracker::initialize(vehicleFactory& vFactory, taskGenerator& tGenerator)
{
	if (&vFactory == NULL || &tGenerator == NULL)
		return false;
	pData->vFactory =  &vFactory;
	pData->tGenerator = &tGenerator;
	return pData->initialized = true;
	
}

string serviceTracker::addServiceRecord(serviceRecord record)
{
	if (!pData->checkInitialized())
		return nonInitializedError;

	string errMsg = record.validateRecord();
	if (errMsg.size() > 1)
		return errMsg;

    string recordNum = record.getRecordNum();
    string vinNum = record.getVinNum();

	serviceRecord tempRecord;
	string searchMsg = getServiceRecord(recordNum, tempRecord);
	if(searchMsg.size() == 0)
		return recordNumTakenError;

    //if vin is not registered before.
	if (pData->vehicleStore.count(vinNum) == 0)
		pData->vehicleStore[vinNum] =  unordered_map < string, serviceRecord >();

	bool ok = pData->vehicleStore[vinNum].insert(make_pair(recordNum, record)).second;
	if (!ok)
		return recordNumTakenError;
	return "";
}

/***********
this is not efficient not O(1) becuase the way I design 
to store the data is to optimize retrieving and adding record 
for particular vehicle.
this is an o(v) #number of vehicle operation
***********/
string serviceTracker::removeRecord(string& recordId)
{
	if (!pData->checkInitialized())
		return nonInitializedError;
	if (recordId.size() < 1)
		return "Record number" + notEmptyError;

	for (auto& key : pData->vehicleStore)
	{
		if (key.second.count(recordId) == 1)
		{
			key.second.erase(recordId);
			if (key.second.size() == 0)
			{
				auto vinNumver = key.first;
				pData->vehicleStore.erase(vinNumver);
				pData->vFactory->removeVehicle(vinNumver);
			}
			return "";
		}
	}
	return "No record with record id: " + recordId;
}

string serviceTracker::getServiceRecord(string& recordId, serviceRecord& resultRecord)
{		
		for (auto& key : pData->vehicleStore)
		{
			auto resultItr = key.second.find(recordId);
			if ( resultItr != key.second.end())
			{
				resultRecord = resultItr->second;
				return "";
			}
		}
		return "Record id is not found.";
}



vector<shared_ptr<vehicle>> serviceTracker::getAllVehicles()
{

	vector<shared_ptr<vehicle>>allVehicles;
	if (!pData->checkInitialized())
		return allVehicles;
	allVehicles.reserve(pData->vehicleStore.size());

	for (auto key : pData->vehicleStore){
		string vin = key.first;
		shared_ptr<vehicle> vPtr = pData->vFactory->getVehicleByVin(vin);
		if (vPtr.get() != NULL)
			allVehicles.push_back(vPtr);
	}
	return allVehicles;
}


vector<serviceRecord> serviceTracker::getAllServiceRecords()
{
	vector<serviceRecord> allRecords;

	for (auto& vRef : pData->vehicleStore)
	{
		for (auto& records : vRef.second)
		{
			allRecords.push_back(records.second);

		}
	}

	return allRecords;
}

vector<serviceRecord> serviceTracker::getAllServiceRecords(string& vin)
{
	
	vector<serviceRecord> records;
	if (!pData->checkInitialized())
		return records;

	if (vin.size() < 1)
		return records;

	auto itr = pData->vehicleStore.find(vin);
	auto endItr = pData->vehicleStore.end();
	if (itr != endItr)
	{
		for (auto& record : itr->second)
		{
			records.push_back(record.second);
		}
	}

	return records;

}



string serviceTracker::updateServiceRecord(serviceRecord& updatedServiceRecord)
{

	string errMsg = updatedServiceRecord.validateRecord();
	if (errMsg.size() > 1)
		return errMsg;

	string stringHolder = updatedServiceRecord.getVinNum();
	auto itr = pData->vehicleStore.find(stringHolder);
	if (itr == pData->vehicleStore.end())
		return "For new vehicle record use addServiceRecord method instead.";

	stringHolder = updatedServiceRecord.getRecordNum();
	auto recordItr = itr->second.find(stringHolder);
	if (recordItr == itr->second.end())
		return "For record with new record number use addServiceRecord method instead.";

	recordItr->second = updatedServiceRecord;
	return "";
}



