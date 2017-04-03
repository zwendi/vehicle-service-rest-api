#pragma once
#include "serviceTracker.h"
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>
//this class is developed for quickly handling request

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

//resource definition 
//get request resources supported
const string getAllVehicles  = "vehicles";
const string getAllTasks = "tasks";
const string getAllRecords = "records";

//all those should be with thier unique id next to it
const string getVehicle = "vehicle";
const string getServiceRecord = "record";
const string getVehicleRecords = "v-record";
const string getTask = "task";


//post request resource supported
const string createRecord = "record";
const string createTask = "task";

//put request resource supported
const string updateRecord = "record";
const string updateVehicle = "vehicle";

//delete request resource supported
const string deleteRecord = "record";







//vehicle json return key
//casablanca function requires use of wstring
const wstring vehicleStr = L"vehicles";
const wstring tasksStr = L"tasks";
const wstring recordStr = L"records";




class requestHandler
{
public:
	requestHandler();
	~requestHandler();
	requestHandler(serviceTracker& tracker, vehicleFactory& factory, taskGenerator& task);
	
	void hanle_get(http_request& request);
	//storing serviceRecords
	void handle_put(http_request& request);

	void handle_post(http_request& request);
	
	void handle_delete(http_request& request);
	void handle_options(http_request& request);

private:
	//dont want to make the header too messy here, use private implementation
	class privateData;
	privateData* pData;

	serviceTracker* sTracker;
	vehicleFactory* vFactory;
	taskGenerator* tGenerator;

};

