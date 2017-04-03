// carServiceTracker.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "requestHandler.h"
#include <thread>
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

void basicProjectSetUp(serviceTracker& tracker, vehicleFactory& vFactory, taskGenerator& tGenerator);


int _tmain(int argc, _TCHAR* argv[])
{
	
	http_listener listener(L"http://localhost/demo");
	serviceTracker tracker;
	vehicleFactory vFactory;
	taskGenerator tGenerator;
	tracker.initialize(vFactory, tGenerator);
	requestHandler res(tracker,vFactory,tGenerator);

	basicProjectSetUp(tracker, vFactory, tGenerator);
	listener.support(methods::GET, [&](http_request req){
		res.hanle_get(req);

	});
	listener.support(methods::PUT, [&](http_request req){
		res.handle_put(req);
	
	});
	listener.support(methods::POST, [&](http_request req){
		res.handle_post(req);

	});
	
	listener.support(methods::DEL, [&](http_request req){
		res.handle_delete(req);

	});

	listener.support(methods::OPTIONS, [&](http_request req){
		res.handle_options(req);
	});

	while (true) {
		listener.open().wait();
	}

	std::cout << U("Bye Bye.") << endl;
	listener.close();

	return 0;
}

/*
   Just quickly create some general records to start with. For tasks, the system should start with some tasks to take a look at.
*/

void basicProjectSetUp(serviceTracker& tracker, vehicleFactory& vFactory, taskGenerator& tGenerator){
	string vin = "1234";
	string model = "ES350";
	string make = "Lexus";
	int odometer = 1000;
	int year = 19;
	string errMsg;

	string name = "oil-change";
	string description = "oil change for gas and disel vehicle.";
	double price = 55.55;
	string recordNum = "999";
	auto v = vFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
	auto t = tGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
	serviceRecord firstRecord(v, recordNum);
	firstRecord.setTask(t);
	tracker.addServiceRecord(firstRecord);

	vin = "4321";
	model = "M3";
	make = "BMW";
	//shared_ptr<vehicle> v1;
	auto v1 = vFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
	serviceRecord secondRecord(v1, vin);
	secondRecord.setTask(t);
	tracker.addServiceRecord(secondRecord);

	vin = "456";
	model = "P90D";
	make = "Tesla";
	auto v3 = vFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
	name = "battery-charge";
	description = "charge battery quickly";
	price = 20;
	auto battery = tGenerator.generateTask(taskList::chargeBattery, name, description, price, errMsg);
	
	name = "tire-rotation";
	description = "change all tires";
	price = 40;
	auto tire = tGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);

	recordNum = "8888";
	serviceRecord thirdRecord(v3, recordNum);
	thirdRecord.setTask(battery);
	tracker.addServiceRecord(thirdRecord);
}