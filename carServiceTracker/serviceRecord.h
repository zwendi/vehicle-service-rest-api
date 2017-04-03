#pragma once
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include "vehicle.h"
#include "task.h"
#include <memory>
#include <time.h>
#include <cpprest/json.h>

using namespace web;
class serviceRecord
{
public:
	serviceRecord(){}
	serviceRecord(shared_ptr<vehicle>& v, string& recordId, double discount = 1) : vehiclePtr(v), recordNum(recordId), discount(discount){
	}

	~serviceRecord(){};
	

	/*****
	  For this design I particularly forbidden change vehicle number through serviceRecord class
	  The record should be a new record if the vehicle vin number is changed.
	  However, a record task, discount can be changed through serviceRecord interface.
	*****/
	bool setDiscount(double percentage);
	//must set a task before the record is valid for task related information
	string setTask(shared_ptr<task> task);

	//return empty string if this is a valid serviceRecord
	string validateRecord();

	string getRecordNum();
	//vehicle information, is not subject to changed for an exisitng record.
	string getVinNum();
	string getMake();
	string getModel();

	//task information
	string getServiceName();
	string getServiceDescription();
	double getDiscount();
	double getPrice();
	
	double getChargePrice();


	json::value toJson();
private:
	/******
	Using private implementation here will allow records changes updated in the tracker
	without using the update method provided. So not using private implementation.  
	*******/
	string recordNum;
	double discount;
    shared_ptr<vehicle> vehiclePtr;
	shared_ptr<task> taskPtr;
};

