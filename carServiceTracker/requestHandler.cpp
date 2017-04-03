#include "stdafx.h"
#include "requestHandler.h"
#include <iostream>

//used for tokenize string by /
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "restColumn.h"

//json::value emptyJson(U(""));
const wstring error = L"Error";

//const wstring rest
using namespace utility;

class requestHandler::privateData{

public:

	json::value convertVehiclesToJson(vector<shared_ptr<vehicle>>& vecs);
	json::value convertTasksToJson(vector<shared_ptr<task>>& task);
	json::value convertServiceRecordstoJson(vector<serviceRecord>& serRecord);

	void handleJsonResponse(wstring errMsg, json::value& jsonObject, http_request& request);

	wstring checkRequest(http_request& requst, int numResourceReq);

	shared_ptr<vehicle> createVehicleFromJson(json::value& vehicleJson, vehicleFactory* factory);
	shared_ptr<task> createTaskFromJson(json::value& taskJson, taskGenerator* generator, string& errMsg);
	shared_ptr<task> createRecordFromJson(json::value& recordJson, taskGenerator* generator, vehicleFactory*, string& errMsg);


	//following support method is from stack overflow.
	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}


	//to process uri to a vector<string> so we can use in all types requests
	vector<string> processUri(uri& uri){
		auto resources = uri.resource().to_string();
		removeAdditionalCharacter(resources);
		string resoucesStr = utility::conversions::utf16_to_utf8(resources);
		std::transform(resoucesStr.begin(), resoucesStr.end(), resoucesStr.begin(), ::tolower);
		return split(resoucesStr, '/');
	}

	/*rest c++ parse json with some additional characters during testing using postman
	* need to clean the string before use it.
	*/
	void removeAdditionalCharacter(wstring& str)
	{
		size_t s = str.size();
		wchar_t chars[] = L"\"";

		for (unsigned int i = 0; i < s; ++i)
		{
			// you need include <algorithm> to use general algorithms like std::remove()
			str.erase(std::remove(str.begin(), str.end(), chars[i]), str.end());
		}
		
	}

};


requestHandler::requestHandler()
{
	pData = new privateData;
}


requestHandler::~requestHandler()
{
	vFactory = 0;
	tGenerator = 0;
	sTracker = 0;
	delete pData;
}

requestHandler::requestHandler(serviceTracker& tracker, vehicleFactory& factory, taskGenerator& task)
{
	vFactory = &factory;
	tGenerator = &task;
	sTracker = &tracker;
	pData = new privateData;

}



void requestHandler::hanle_get(http_request& request)
{
	auto uri = request.relative_uri();
	json::value resVal;

	if (uri.is_empty())
	{
		pData->handleJsonResponse(L"No uri received.", resVal, request);
		return; 
	}
	auto resources = pData->processUri(uri);
	string resource = resources[1];
	if (resource.compare(getAllVehicles)==0)
	{
		auto vehicles = sTracker->getAllVehicles();
		resVal = pData->convertVehiclesToJson(vehicles);
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getAllTasks) == 0)
	{
		auto taskArray = tGenerator->getAllTasks();
		resVal = pData->convertTasksToJson(taskArray);
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getAllRecords) == 0)
	{
		auto recordsArray = sTracker->getAllServiceRecords();
		resVal = pData->convertServiceRecordstoJson(recordsArray);
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getVehicle) == 0){
		if (resources.size() < 3){
			pData->handleJsonResponse(L"The request is missing vehicle vin number.", resVal, request);
			return;
		}
		string vinNum = resources[2];
		auto vPtr = vFactory->getVehicleByVin(vinNum);
		if (vPtr.get() == NULL)
		{
			wstring errMsg = L"There is no vehicle with vin number " + utility::conversions::to_utf16string(vinNum);
			pData->handleJsonResponse(errMsg, resVal, request);
			return;
		}
		resVal = vPtr->toJson();
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getServiceRecord) == 0)
	{
		if (resources.size() < 3){	
			pData->handleJsonResponse(L"The request is missing record's record number.", resVal, request);
			return;
		}
		string recordNum = resources[2];
		serviceRecord record;
		string  errMsg = sTracker->getServiceRecord(recordNum,record);
		if (errMsg.size() > 1)
		{
			pData->handleJsonResponse(utility::conversions::to_utf16string(errMsg), resVal, request);
			return;
		}
		resVal = record.toJson();
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getVehicleRecords) == 0)
	{
		if (resources.size() < 3){	
			pData->handleJsonResponse(L"The request is missing vin number.", resVal, request);
			return;
		}
		string vinNum = resources[2];
		auto records = sTracker->getAllServiceRecords(vinNum);
		if (records.size() < 1)
		{
			pData->handleJsonResponse(L"There is no service record associated with vin number " + utility::conversions::to_utf16string(vinNum), resVal, request);
			return;
		}
		resVal = pData->convertServiceRecordstoJson(records);
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(getTask) == 0)
	{
		if (resources.size() < 3){
			pData->handleJsonResponse(L"The request is missing task name.", resVal, request);
			return;
		}
		string taskName = resources[2];
		auto taskPtr = tGenerator->getTask(taskName);
		if (taskPtr.get() == NULL)
		{
			pData->handleJsonResponse(L"No such task found.", resVal, request);
			return;
		}
		resVal = taskPtr->toJson();
		pData->handleJsonResponse(L"", resVal, request);
	}
	else
	{
		pData->handleJsonResponse(L"Unrecognized resource requested. Resource name is:" + utility::conversions::to_utf16string(resource), resVal, request);
	}
	return;
}

void requestHandler::handle_put(http_request& request)
{
	auto uri = request.relative_uri();
	json::value resVal;

	if (uri.is_empty())
	{
		pData->handleJsonResponse(L"No uri received.", resVal, request);
		return;
	}
	
    auto jsonObject = request.extract_json().get();
	auto resources = pData->processUri(uri);
	if (resources.size() != 3)
	{
		pData->handleJsonResponse(L"Too many or too few resources are supply.", resVal, request);
		return;
	}
	if (jsonObject.size() < 1)
	{
		pData->handleJsonResponse(L"No json object is supplied with the request.", resVal, request);
		return;
	}
	string resource = resources[1];
	string errMsg;

	if (resource.compare(updateRecord) == 0)
	{
		string recordId = resources[2];
		serviceRecord newRecord;
		errMsg = sTracker->getServiceRecord(recordId, newRecord);
		if (errMsg.size() > 0)
		{
			pData->handleJsonResponse(utility::conversions::to_utf16string(errMsg), resVal, request);
			return;
		}
		if (!jsonObject[jsonDiscount].is_null()){
			wstring discountStr = jsonObject[jsonDiscount].to_string();
			pData->removeAdditionalCharacter(discountStr);
			double dis = newRecord.getDiscount();
			try{
				dis = std::stod(discountStr);
			}
			catch (exception e)
			{
				errMsg = "Invalid discount double submiteed.";
				pData->handleJsonResponse(conversions::utf8_to_utf16(errMsg), resVal, request);
				return;
			}
			if (!newRecord.setDiscount(dis))
			{
				pData->handleJsonResponse(L"Can't update the discount value.", resVal, request);
				return;
			}
		}
		if (!jsonObject[jsonServiceTask].is_null())
		{
			auto tName = jsonObject[jsonServiceTask].to_string();
			pData->removeAdditionalCharacter(tName);
			auto newTask = tGenerator->getTask(utility::conversions::to_utf8string(tName));
			if (newTask.get() == NULL)
			{
				pData->handleJsonResponse(L"Can't find task with task name:" + tName, resVal, request);
				return;
			}	
			errMsg = newRecord.setTask(newTask);
			if (errMsg.size() > 0)
			{
				pData->handleJsonResponse(utility::conversions::to_utf16string(errMsg), resVal, request);
				return;
			}
		}

		//we are only allowing update discount and switch task for now
		errMsg = sTracker->updateServiceRecord(newRecord);
		if (errMsg.size() > 0)
		{
			pData->handleJsonResponse(utility::conversions::to_utf16string(errMsg), resVal, request);
			return;
		}
		pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(updateVehicle) == 0)
	{
		//currently just implement updating year and odometer, but more update can be  implement.
		string viNum = resources[2];
		auto v = vFactory->getVehicleByVin(viNum);
		if (v.get() == NULL)
		{
			pData->handleJsonResponse(L"No such vehicle with found.", resVal, request);
			return;
		}
		if (!jsonObject[jsonOdometer].is_null())
		{
			wstring odoStr = jsonObject[jsonOdometer].to_string();
			pData->removeAdditionalCharacter(odoStr);
			int odo = v->getOdometer();
			try{
				odo = std::stoi(odoStr);
			}
			catch (exception e)
			{
				errMsg = "Invalid integer submiteed.";
				pData->handleJsonResponse(conversions::utf8_to_utf16(errMsg), resVal, request);
				return;
			}
			
			errMsg = v->setOdometer(odo);
			if (errMsg.size()>0)
			{
				pData->handleJsonResponse(conversions::utf8_to_utf16(errMsg), resVal, request);
				return;
			}
		}
		if (!jsonObject[jsonYear].is_null())
		{
			wstring yearStr = jsonObject[jsonYear].to_string();
			pData->removeAdditionalCharacter(yearStr);
			int yearInfo = v->getYear();
			try{
				yearInfo = std::stoi(yearStr);
			}
			catch (exception e)
			{
				errMsg = "Invalid integer submiteed.";
				pData->handleJsonResponse(conversions::utf8_to_utf16(errMsg), resVal, request);
				return;
			}
			errMsg = v->setYear(yearInfo);
			if (errMsg.size()>0)
			{
				pData->handleJsonResponse(conversions::utf8_to_utf16(errMsg), resVal, request);
				return;
			}
		}
		pData->handleJsonResponse(L"", resVal, request);
	}
	else
	{
		pData->handleJsonResponse(L"Unrecognized resource requested.", resVal, request);
		return;
	}
}


void requestHandler::handle_post(http_request& request)
{
	auto uri = request.relative_uri();
	json::value resVal;
	if (uri.is_empty())
	{
		pData->handleJsonResponse(L"No uri received.", resVal, request);
		return;
	}

	auto jsonObject = request.extract_json().get();
	auto resources = pData->processUri(uri);
	if (resources.size() != 2)
	{
		pData->handleJsonResponse(L"Too many or too few resources are supply.", resVal, request);
		return;
	}
	if (jsonObject.size() < 1)
	{
		pData->handleJsonResponse(L"No json object is supplied with the request.", resVal, request);
		return;
	}

	string resource = resources[1];
	string errMsg;
	if (resource.compare(createRecord) == 0)
	{
		
		if (jsonObject[jsonTaskKey].is_null())
		{
			pData->handleJsonResponse(L"No task json information is supplied with the request.", resVal, request);
			return;
		}
		if (jsonObject[jsonTaskKey][jsonName].is_null())
		{
			pData->handleJsonResponse(L"No task name json information is supplied with the request.", resVal, request);
			return;
		}

		wstring taskName = jsonObject[jsonTaskKey][jsonName].to_string();
		pData->removeAdditionalCharacter(taskName);
		auto taskPtr = tGenerator->getTask(utility::conversions::to_utf8string(taskName));

		if (taskPtr.get() == NULL)
		{
			pData->handleJsonResponse(L"No such task exists.", resVal, request);
			return;
		}

		if (jsonObject[jsonServiceVehicle].is_null())
		{
			pData->handleJsonResponse(L"No vehicle json information is supplied with the request.", resVal, request);
			return;
		}
		if (jsonObject[jsonServiceVehicle][jsonVin].is_null())
		{
			pData->handleJsonResponse(L"No vehicle json information is supplied with the request.", resVal, request);
			return;
		}
		auto veh = pData->createVehicleFromJson(jsonObject[jsonServiceVehicle], vFactory);
		if (veh.get() == NULL)
		{
			pData->handleJsonResponse(L"Can not create the vehicle." + jsonObject[jsonServiceVehicle].to_string(), resVal, request);
			return;
		}

		if (jsonObject[jsonServiceRecordNum].is_null())
		{
			pData->handleJsonResponse(L"No service number is detected in JSON.", resVal, request);
			return;
		}
		wstring recordNum = jsonObject[jsonServiceRecordNum].to_string();
		pData->removeAdditionalCharacter(recordNum);

		double discount=1;
		if (!jsonObject[jsonDiscount].is_null()){
			auto discountStr = jsonObject[jsonDiscount].to_string();
			pData->removeAdditionalCharacter(discountStr);
			try{
				discount = std::stod(discountStr);
			}
			catch (exception e)
			{
				pData->handleJsonResponse(L"Invalid discount inputted.", resVal, request);
				return;
			}
		}
		serviceRecord newServiceRecord(veh, conversions::to_utf8string(recordNum), discount);
		errMsg = newServiceRecord.setTask(taskPtr);
		
		if (errMsg.size()>0)
			pData->handleJsonResponse(conversions::to_utf16string(errMsg), resVal, request);
		errMsg = sTracker->addServiceRecord(newServiceRecord);
		
		//add the record to tracker
		if (errMsg.size()>0)
			pData->handleJsonResponse(conversions::to_utf16string(errMsg), resVal, request);
		else
			pData->handleJsonResponse(L"", resVal, request);
	}
	else if (resource.compare(createTask)==0)
	{
		pData->createTaskFromJson(jsonObject, tGenerator, errMsg);
		pData->handleJsonResponse(conversions::to_utf16string(errMsg), resVal, request);
	}
	else
	{
		pData->handleJsonResponse(L"No such resource for post request.", resVal, request);
	}
}

void  requestHandler::handle_delete(http_request& request){
	auto uri = request.relative_uri();
	json::value resVal;

	if (uri.is_empty())
	{
		pData->handleJsonResponse(L"No uri received.", resVal, request);
		return;
	}
	auto resources = pData->processUri(uri);
	if (resources.size() != 3)
	{
		pData->handleJsonResponse(L"Too many or too few resources are supply.", resVal, request);
		return;
	}
	string resource = resources[1];
	string errMsg;


	if (resource.compare(deleteRecord) == 0)
	{
		auto recordNum = resources[2];
		errMsg = sTracker->removeRecord(recordNum);
		pData->handleJsonResponse(utility::conversions::to_utf16string(errMsg), resVal, request);
		return;
	}
	else{
		pData->handleJsonResponse(L"Unrecognized resource requested.", resVal, request);
		return;
	}
}





/*
all nasty json conversion work for casablanca rest c++ 
Originally i put the single object json conversion in this class
which make this class fairly complex and can't be reused anywhere else. So I decide to refactor those methods to each single 
object class. However, that makes vehicle, serviceRecord, task class need to compile with additional codes from the JSON library.
One potential solution is to make those toJson() return a standard string, which improves overall reusability and compilation size.
However due to time constraint i decide to leave it like that for now.
*/

//possible for template method 
json::value requestHandler::privateData::convertTasksToJson(vector<shared_ptr<task>>& tasks )
{
	json::value value;
	std::vector<json::value> arrayTasks;
	size_t s = tasks.size();
	if (s < 1)
		return value;
	else if (s == 1)
		return tasks[0]->toJson();

	for (auto& a : tasks)
	{
		arrayTasks.push_back(a->toJson());
	}
	if (arrayTasks.size() < 1)
		return value;
	value[tasksStr] = json::value::array(arrayTasks);
	return value;

}


json::value requestHandler::privateData::convertVehiclesToJson(vector<shared_ptr<vehicle>>& vecs)
{
	json::value value;
	size_t s = vecs.size();
	if (s < 1)
		return value;
	else if (s == 1)
		return vecs[0]->toJson();

	std::vector<json::value> arrayVehicles;
	for (auto& a : vecs)
	{
		arrayVehicles.push_back(a->toJson());
	}

	value[vehicleStr] = json::value::array(arrayVehicles);
	return value;
}

json::value requestHandler::privateData::convertServiceRecordstoJson(vector<serviceRecord>& allRecords)
{
	json::value value;
	std::vector<json::value> arrayServices;
	size_t s = allRecords.size();
	if (s < 1)
		return value;
	
	for (auto& a : allRecords)
	{
		arrayServices.push_back(a.toJson());
	}
	if (arrayServices.size() < 1)
		return value;
	value[recordStr] = json::value::array(arrayServices);
	return value;

}

void requestHandler::privateData::handleJsonResponse(wstring errMsg, json::value& jsonObject,http_request& request)
{
	json::value resVal;
	http_response resp;
	resp.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	if (errMsg.size()>0)
	{
		resVal[error] = json::value(errMsg);
		resp.set_status_code(web::http::status_codes::BadRequest);
		resp.set_body(resVal); 
	}
	else
	{
		resp.set_status_code(web::http::status_codes::OK);
		resp.set_body(jsonObject);
	}
	request.reply(resp);
}


shared_ptr<vehicle> requestHandler::privateData::createVehicleFromJson(json::value& vehicleInfo, vehicleFactory* factory)
{
	auto vin = vehicleInfo[jsonVin].to_string();
	removeAdditionalCharacter(vin);

	//check we have created this vehicle before
	auto existVehicle = factory->getVehicleByVin(utility::conversions::to_utf8string(vin));
	if (existVehicle.get() != NULL)
		return existVehicle;

	auto makeInfo = vehicleInfo[jsonMake].to_string();
	removeAdditionalCharacter(makeInfo);

	auto modelInfo = vehicleInfo[jsonModel].to_string();
	removeAdditionalCharacter(modelInfo);

	auto yearStr = vehicleInfo[jsonYear].to_string();
	removeAdditionalCharacter(yearStr);
	
	auto odometerStr = vehicleInfo[jsonOdometer].to_string();
	removeAdditionalCharacter(odometerStr);

	int yearInt, odometerInt;
	try{
		yearInt = std::stoi(yearStr);
		odometerInt = std::stoi(odometerStr);
	}
	catch (exception e)
	{
		return existVehicle;
	}

	vehicleType type;
	auto typeInfo = vehicleInfo[jsonType].as_string();
	removeAdditionalCharacter(typeInfo);

	if (typeInfo.compare(jsonDisel) == 0)
		type = vehicleType::disel;
	else if (typeInfo.compare(jsonGas) == 0)
		type = vehicleType::gas;
	else if (typeInfo.compare(jsonElectric) == 0)
		type = vehicleType::electric;
	else
		type = vehicleType::vehicleMax;

	string errMsg;
	existVehicle = factory->createNewVehicle(conversions::to_utf8string(vin), conversions::to_utf8string(modelInfo), conversions::to_utf8string(makeInfo), odometerInt, yearInt, type, errMsg);
	return existVehicle;
}

shared_ptr<task> requestHandler::privateData::createTaskFromJson(json::value& taskJson, taskGenerator* generator,string& errMsg)
{
	auto nameInfo = taskJson[jsonName].to_string();
	removeAdditionalCharacter(nameInfo);

	auto descriptionInfo = taskJson[jsonDescription].to_string();
	removeAdditionalCharacter(descriptionInfo);

	auto priceStr = taskJson[jsonPrice].as_string();
	removeAdditionalCharacter(priceStr);

	double priceInfo;
	try{
		priceInfo = std::stod(priceStr);
	}
	catch (exception e)
	{
		errMsg = "Invalid price information supplied.";
		return NULL;
	}

	auto taskType = taskJson[jsonTaskType].to_string();
	removeAdditionalCharacter(descriptionInfo);
	taskList type;
	if (taskType.compare(jsonOilChange))
		type = taskList::oilChange;
	else if (taskType.compare(jsonChargeBattery))
		type = taskList::chargeBattery;
	else if (taskType.compare(jsonTireRotation))
		type = taskList::tireRotation;
	else
		type = taskList::tasksMax;

	auto genTask = generator->generateTask(type, conversions::to_utf8string(nameInfo), conversions::to_utf8string(descriptionInfo), priceInfo, errMsg);
	return genTask;
}

wstring requestHandler::privateData::checkRequest(http_request& request, int numResourceReq)
{
	auto uri = request.relative_uri();
	json::value resVal;
	if (uri.is_empty())
	{
		return L"No uri received.";
	}

	auto jsonObject = request.extract_json().get();
	auto resources = processUri(uri);
	if (resources.size() != numResourceReq)
	{
		return L"Too many or too few resources are supply.";
	}
	if (jsonObject.size() < 1)
	{
		return L"No json object is supplied with the request.";
	}

	return L"";

}


void requestHandler::handle_options(http_request& request)
{
	http_response response(status_codes::OK);
	response.headers().add(U("Allow"), U("GET, POST, PUT, DELETE, OPTIONS"));
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, PUT, DELETE, OPTIONS"));
	response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
	request.reply(response);
}