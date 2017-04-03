#include "stdafx.h"
#include <string>

using namespace std;

/*
   This file is to define the key of our json output and json input client send to us.
   Change value here will change our json output, which in my opinion is easier to maintain.
*/

//task json return key
const wstring jsonTasks = L"tasks";
const wstring jsonName = L"task_name";
const wstring jsonDescription = L"description";
const wstring jsonPrice = L"price";

//only for adding new task
const wstring jsonTaskType = L"type";
const wstring jsonOilChange = L"oil_change";
const wstring jsonChargeBattery = L"battery_change";
const wstring jsonTireRotation = L"tire_rotation";

//vehicle

const wstring jsonVin = L"vin_number";
const wstring jsonModel = L"model";
const wstring jsonMake = L"make";
const wstring jsonOdometer = L"odometer";
const wstring jsonYear = L"year_made";

const wstring jsonType = L"type";
const wstring jsonElectric = L"electric";
const wstring jsonGas = L"gas";
const wstring jsonDisel = L"disel";

//service record
const wstring jsonRecords = L"service_records";
const wstring jsonServiceVehicle = L"vehicle";
const wstring jsonDiscount = L"discount";
const wstring jsonChargePrice = L"charge_price";
const wstring jsonServiceRecordNum = L"record_number";
//get request task json key
const wstring jsonTaskKey = L"task";
 //for put request switching to another task
const wstring jsonServiceTask = L"task_name";

