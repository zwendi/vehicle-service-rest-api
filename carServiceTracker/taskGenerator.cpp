#include "stdafx.h"
#include "taskGenerator.h"
#include "oilChange.h"
#include "tireRotation.h"
#include "chargeBattery.h"
#include <vector>


class taskGenerator::privateData{
public:
	vector<shared_ptr<task>> allTasks;
	//return -1 if not found
	int taskIndexWithName(string& name);
	
	//return null if no error occur, otherwise return the error message
	string infoCheck(string& name, string& description, double price);
};


int taskGenerator::privateData::taskIndexWithName(string& name){
	//using range loop and remove things in vector will create additional troubles
	size_t vecSize = allTasks.size();
	int index = -1;
	for (size_t i = 0; i < vecSize; i++)
	{
		if (allTasks[i]->getName() == name)
		{
			index = i;
			break;
		}
	}
	return index;
}



taskGenerator::taskGenerator()
{

	pData = new privateData;
}

taskGenerator::~taskGenerator()
{
	delete pData;
}
shared_ptr<task> taskGenerator::generateTask(taskList type, string& name, string& description, double price, string& errMsg)
{
	errMsg = pData->infoCheck(name, description, price);
	if (errMsg.size() > 0)
		return NULL; 
	shared_ptr<task> respondTask;

	switch (type)
	{

	case taskList::chargeBattery:
		respondTask = shared_ptr<task>(new chargeBattery(name, description, price));
		break;
	case taskList::oilChange:
		respondTask = shared_ptr<task>(new oilChange(name, description, price));
		break;
	case taskList::tireRotation:
		respondTask = shared_ptr<task>(new tireRotation(name, description, price));
		break;
	case taskList::tasksMax:
	default:
		errMsg = "Invalid task type.";
		respondTask = NULL;
		break;
	}
	if (respondTask.get())
		pData->allTasks.push_back(respondTask);

	return respondTask;
}


string taskGenerator::privateData::infoCheck(string& name, string& description, double price)
{
	if (name.size() < 1)
		return "Name is required for a task.";
	else if (description.size() < 1)
		return "Description is required for a task";
	else if (price < 0)
		return "Negative price is not allow.";
	else
		return "";
}


vector<shared_ptr<task>> taskGenerator::getAllTasks()
{
	return pData->allTasks;
}


string taskGenerator::removeTask(string& name)
{
	int index = pData->taskIndexWithName(name);

	if (index < 0)
		return "Can not find the task with task name:"+name;

    pData->allTasks.erase(pData->allTasks.begin() + index);
	return "";
}


shared_ptr<task>  taskGenerator::getTask(string& name)
{
	int index = pData->taskIndexWithName(name);
	if (index < 0)
		return NULL;

	return pData->allTasks[index];

}
