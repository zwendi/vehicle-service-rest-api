#include "stdafx.h"
#include "CppUnitTest.h"
#include "taskGenerator.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace taskTest
{
	TEST_CLASS(taskGeneratorTest)
	{
	public:

		string description = "Charging electric vehicle fully.";
		string name = "Super Battery Charge";
		double price = 20.0;
		string errorMsg;
		taskGenerator generator;
		TEST_METHOD(getAllTasks)
		{
			// TODO: Your test code here
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v1";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v3";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);

			vector<shared_ptr<task>> tasks = generator.getAllTasks();
			size_t resultSize = 3;
			Assert::AreEqual(resultSize, tasks.size(), L"We have generated 3 tasks.");
		}

		TEST_METHOD(removeTask)
		{
			// TODO: Your test code here
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v1";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v3";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v1";

			string result = generator.removeTask(name);
			size_t resultSize = 2;
			string expectedResult = "";
			Assert::AreEqual(expectedResult, result, L"Should return empty error message.");

			auto taskArray = generator.getAllTasks();

			for (auto& task : taskArray)
			{
				Assert::AreNotEqual(name, task->getName(), L"Should not consist delete task name.");
			}
		}



		TEST_METHOD(removeNonExistTask)
		{
			// TODO: Your test code here
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v1";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v3";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v4";

			string result = generator.removeTask(name);
			size_t resultSize = 3;
			string expectedResult = "Can not find the task with task name:"+ name;
			Assert::AreEqual(expectedResult, result, L"Should return empty error message.");

			name = "";
			result = generator.removeTask(name);
			resultSize = 3;
			expectedResult = "Can not find the task with task name:" + name;
			Assert::AreEqual(expectedResult, result, L"Should return empty error message.");

		}


		TEST_METHOD(getTask)
		{
			// TODO: Your test code here
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v1";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);
			name = "Super Battery Charge v3";
			generator.generateTask(taskList::chargeBattery, name, description, price, errorMsg);

			auto returnTask = generator.getTask(name);
			Assert::AreEqual(name, returnTask->getName(), L"Should return task with same name.");
			Assert::AreEqual(description, returnTask->getDescription(), L"Should return task with same description.");
			Assert::AreEqual(price, returnTask->getPrice(), L"Should return task with same price.");

			name = "";
			returnTask = generator.getTask(name);
			Assert::IsNull(returnTask.get());
		}

	};
}