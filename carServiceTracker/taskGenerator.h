#pragma once
#include "task.h"
#include <vector>
#include <memory>

enum class taskList{
	chargeBattery =0,
	oilChange,
	tireRotation,
	tasksMax
};

/*
Allowing override  for changing way of retriving or generating task(from database or other data source)
*/
class taskGenerator
{
public:
	taskGenerator();
	virtual~taskGenerator();
	virtual shared_ptr<task> generateTask(taskList type, string& name, string& description, double price, string& errMsg);
	virtual vector<shared_ptr<task>> getAllTasks();
	virtual string removeTask(string& name);

	virtual shared_ptr<task> getTask(string& name);

private:
	class privateData;
	privateData* pData;

};

