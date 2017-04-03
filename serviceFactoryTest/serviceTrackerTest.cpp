#include "stdafx.h"
#include "CppUnitTest.h"
#include "serviceTracker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace serviceFactoryTest
{		
	TEST_CLASS(serviceTrackerTest)
	{
	public:
		
		vehicleFactory  ourFactory;
		taskGenerator ourGenerator;
		string vin = "1234";
		string model = "Es350";
		string make = "Lexus";
		int odometer = 1000;
		int year = 2017;
		string errMsg;

		//taskGenerator
		string description = "Change engine oil completely.";
		string name = "Oil change";
		double price = 20.0;

		TEST_METHOD(getAllRecords)
		{
			// TODO: Your test code here
			serviceTracker ourTracker;
			string recordId = "123456";
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			
			//test getAllTask right after initialization
			ourTracker.initialize(ourFactory, ourGenerator);
			auto emptyRecord = ourTracker.getAllServiceRecords();
			size_t resultSize = 0;
			Assert::AreEqual(resultSize, emptyRecord.size(), L"Should return size 0 right after initialization.");

			//adding two records with same vehicle
			serviceRecord record(vehicle, recordId);
			record.setTask(task);
			recordId = "1234";
			serviceRecord secondRecord(vehicle, recordId);
			secondRecord.setTask(task);		
			string addRecord = ourTracker.addServiceRecord(record);
			addRecord = ourTracker.addServiceRecord(secondRecord);
			auto records = ourTracker.getAllServiceRecords();		
			resultSize = 2; 
			Assert::AreEqual(resultSize, records.size(), L"Should return size 2 after we added two records");

			//add another record with different vehicle
			vin = "v123";
			recordId = "3431";
			auto anotherVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			serviceRecord thirdRecord(anotherVehicle, recordId);
			thirdRecord.setTask(task);
			ourTracker.addServiceRecord(thirdRecord);
			records = ourTracker.getAllServiceRecords();
			resultSize = 3;
			Assert::AreEqual(resultSize, records.size(), L"Should return size 3 after we added three records");

		}

		TEST_METHOD(addRecords){
			// TODO: Your test code here
			serviceTracker ourTracker;
			string recordId = "123456";
		
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			
			//adding record with no recordId
			string nullRecordId;
			serviceRecord record(vehicle, nullRecordId);

			ourTracker.initialize(ourFactory, ourGenerator);
			string expectedStr = "Record number can not be empty.";
			errMsg = ourTracker.addServiceRecord(record);
			Assert::AreEqual(expectedStr, errMsg, L"Not allowing adding record without record number.");

			//adding record without assigned task
			serviceRecord record1(vehicle, recordId);
			errMsg = ourTracker.addServiceRecord(record1);
			expectedStr = "Record should contain an assigned task with a valid name.";
			Assert::AreEqual(expectedStr, errMsg, L"Not allowing adding record without assigned valid task.");

			//adding a normal valid record
			record1.setTask(task);
			errMsg = ourTracker.addServiceRecord(record1);
			expectedStr = "";
			Assert::AreEqual(expectedStr, errMsg, L"Should allow adding a valid record.");
			size_t s = 1;
			Assert::AreEqual(s, ourTracker.getAllServiceRecords().size(), L"Should contain only one record.");


			//adding a record with same record id, apply to the different vehicle;
			string vin2 = "999";
			auto vehicle2 = ourFactory.createNewVehicle(vin2, model, make, odometer, year, vehicleType::gas, errMsg);
			serviceRecord record2(vehicle2, recordId);
			record2.setTask(task);
			errMsg = ourTracker.addServiceRecord(record2);
			expectedStr = "Record number has been used.Please change the record number.";
			Assert::AreEqual(expectedStr, errMsg, L"Should not allow adding a record with same record number.");

			//adding a record with valid record id, apply to different vehicle
			recordId = "new record id";
			serviceRecord record3(vehicle2, recordId);
			record3.setTask(task);
			errMsg = ourTracker.addServiceRecord(record3);
			expectedStr = "";
			Assert::AreEqual(expectedStr, errMsg, L"Should be able to add a valid service record.");
			s=2;
			Assert::AreEqual(s, ourTracker.getAllServiceRecords().size(), L"Should contain two records. One for each vehicle.");
		}

		TEST_METHOD(removeServiceRecord){
			serviceTracker ourTracker;
			ourTracker.initialize(ourFactory, ourGenerator);
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
		
			string toDelRecordId = "";
		    //try to delete an empty record id;
			errMsg = ourTracker.removeRecord(toDelRecordId);
			string expectedMsg = "Record number can not be empty.";
			Assert::AreEqual(expectedMsg, errMsg, L"Should not be able to delete a record without record number.");

			//try to delete non-existing record number 
			toDelRecordId = "1234";
			errMsg = ourTracker.removeRecord(toDelRecordId);
			expectedMsg = "No record with record id: "+toDelRecordId;
			Assert::AreEqual(expectedMsg, errMsg, L"Should not be able to delete a record without record number.");
			
			//setting up two records with different record number and different tasks
			vin = "456";
			auto vehicle2 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			auto task1 = ourGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);
			string recordId = "617";
			serviceRecord record1(vehicle, recordId);
			recordId = "999";
			serviceRecord record2(vehicle2, recordId);
			record1.setTask(task);
			record2.setTask(task1); 
			ourTracker.addServiceRecord(record2);
			ourTracker.addServiceRecord(record1);

			//Try to remove non-existing record number when there's record in tracker
			errMsg = ourTracker.removeRecord(toDelRecordId);
			expectedMsg = "No record with record id: " + toDelRecordId;
			Assert::AreEqual(expectedMsg, errMsg, L"Should not be able to delete a record without record number.");

			//try to remove existing record number
			errMsg = ourTracker.removeRecord(recordId);
			expectedMsg = "";
			Assert::AreEqual(expectedMsg, errMsg, L"Should not be able to delete a record with valid record number.");

			auto allRecords = ourTracker.getAllServiceRecords();
			size_t s = 1;
			Assert::AreEqual(s, allRecords.size(), L"Should contain only one record after delete.");

			auto theRecord = allRecords[0];
			expectedMsg = "617";
			Assert::AreEqual(expectedMsg, theRecord.getRecordNum(), L"Should contain one non-deleted record.");
		}

		TEST_METHOD(getServiceRecordByVin)
		{
			serviceTracker ourTracker;
			ourTracker.initialize(ourFactory, ourGenerator);
			string testVin = "";

			//getting all service  records for a new tracker should return a 
			auto records = ourTracker.getAllServiceRecords(testVin);
			size_t s = 0;
			Assert::AreEqual(s, records.size(), L"The records set should be empty for new tracker.");

			//adding few records with different vehicle
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto task1 = ourGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);
			vin = "123";
			auto vehicle123 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			vin = "999";
			auto vehicle999 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			
			//vehicle vin =123 will be associated with 3 records
			string recordNum = "1";
			serviceRecord record1(vehicle123, recordNum);
			record1.setTask(task);
			recordNum = "2";
			serviceRecord record2(vehicle123, recordNum);
			record2.setTask(task1);
			recordNum = "3";
			serviceRecord record3(vehicle123, recordNum);
			record3.setTask(task);

			//vehicle vin =999 will be associated with 2 records
			recordNum = "4";
			serviceRecord record4(vehicle999, recordNum);
			record4.setTask(task1);

			recordNum = "5";
			serviceRecord record5(vehicle999, recordNum);
			record5.setTask(task);

			ourTracker.addServiceRecord(record1);
			ourTracker.addServiceRecord(record2);
			ourTracker.addServiceRecord(record3);
			ourTracker.addServiceRecord(record4);
			ourTracker.addServiceRecord(record5);

			//try get records with non-existing vin
			vin = "non-existing vin.";
			records = ourTracker.getAllServiceRecords(vin);
			s = 0;
			Assert::AreEqual(s, records.size(), L"The records set should be empty for new tracker.");

			//get records for vehicle vin = 999
			vin = "999";
			records = ourTracker.getAllServiceRecords(vin);
			s = 2;
			Assert::AreEqual(s, records.size(), L"There are two records with vehicle 999");

			for (auto& rec : records)
			{
				Assert::AreEqual(vin, rec.getVinNum(), L"All records should be associated with vehicle 999");
			}

			//get records for vehicle vin = 123
			vin = "123";
			records = ourTracker.getAllServiceRecords(vin);
			s = 3;
			Assert::AreEqual(s, records.size(), L"There are three records with vehicle 123");

			for (auto& rec : records)
			{
				Assert::AreEqual(vin, rec.getVinNum(), L"All records should be associated with vehicle 123");
			}

		}

	
		TEST_METHOD(getAllVehicles)
		{
			serviceTracker ourTracker;
			ourTracker.initialize(ourFactory, ourGenerator);
			//getting all vehicles that's in our system. 
			auto allVehicles = ourTracker.getAllVehicles();		  
			size_t s = 0;
			Assert::AreEqual(s, allVehicles.size(), L"Should return an empty vector since no records are inserted.");


			//adding few records with different vehicles
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto task1 = ourGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);
			vin = "123";
			auto vehicle123 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			vin = "999";
			auto vehicle999 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			//vehicle vin =123 will be associated with 3 records
			string recordNum = "1";
			serviceRecord record1(vehicle123, recordNum);
			record1.setTask(task);
			recordNum = "2";
			serviceRecord record2(vehicle123, recordNum);
			record2.setTask(task1);
			recordNum = "3";
			serviceRecord record3(vehicle123, recordNum);
			record3.setTask(task);
			ourTracker.addServiceRecord(record1);
			ourTracker.addServiceRecord(record2);
			ourTracker.addServiceRecord(record3);

			allVehicles = ourTracker.getAllVehicles();
			s = 1;			
			Assert::AreEqual(s, allVehicles.size(), L"Should only contain one vehicle information.");
			Assert::AreEqual(vehicle123->getVinNum(), allVehicles[0]->getVinNum(), L"Vin number should match.");


			//vehicle vin =999 will be associated with 2 records
			recordNum = "4";
			serviceRecord record4(vehicle999, recordNum);
			record4.setTask(task1);
			recordNum = "5";
			serviceRecord record5(vehicle999, recordNum);
			record5.setTask(task);
			ourTracker.addServiceRecord(record4);
			ourTracker.addServiceRecord(record5);
			allVehicles = ourTracker.getAllVehicles();
			s = 2;
			Assert::AreEqual(s, allVehicles.size(), L"Now should contain two vehicle information.");		
		}


		TEST_METHOD(getServiceRecord)
		{
			serviceTracker ourTracker;
			string recordId = "123456";

			ourTracker.initialize(ourFactory, ourGenerator);
			auto oilChange = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto tireRotation = ourGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			vin = "3123";
			auto vehicle2 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			//both records are related to oil change;
			serviceRecord record1(vehicle, recordId);
			record1.setTask(oilChange);

			recordId = "9999";
			serviceRecord record2(vehicle2, recordId);
			record2.setTask(tireRotation);

			ourTracker.addServiceRecord(record1);
			ourTracker.addServiceRecord(record2);

			serviceRecord res;
			//get service record for non exisint record num
			string recordNum = "non-exist";
			errMsg = ourTracker.getServiceRecord(recordNum, res);
			string expectedStr = "Record id is not found.";
			Assert::AreEqual(expectedStr, errMsg, L"Should return error for non-existing record num.");
			
			//get record from valid recordNum;
			recordNum = "9999";
			expectedStr ="";
			errMsg = ourTracker.getServiceRecord(recordNum, res);
			Assert::AreEqual(expectedStr, errMsg, L"Should produce no error with a valid record number.");
			Assert::AreEqual(recordNum, res.getRecordNum(), L"Should copy the same valid for designated record number.");

		
		}

		TEST_METHOD(updateServiceRecords)
		{
			serviceTracker ourTracker;
			string recordId = "123456";
			ourTracker.initialize(ourFactory, ourGenerator);

			auto oilChange = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			name = "Four tires rotation";
			auto tireRotation = ourGenerator.generateTask(taskList::tireRotation, name, description, price, errMsg);

			//both records are related to oil change;
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			vin = "321";
			auto vehicle2 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			//first set both records to oil change
			serviceRecord record1(vehicle, recordId);
			record1.setTask(oilChange);		
			recordId = "9999";
			serviceRecord record2(vehicle2, recordId);
			record2.setTask(oilChange);
			ourTracker.addServiceRecord(record1);
			ourTracker.addServiceRecord(record2);

			//then we change record2 to tireRotation;
			record2.setTask(tireRotation);

			serviceRecord res;
			ourTracker.getServiceRecord(recordId, res);
			string expectedRes = "Oil change";
			Assert::AreEqual(expectedRes, res.getServiceName(), L"before update it should contain the same name");

			//now we have change record2 to tireRotation
			ourTracker.updateServiceRecord(record2);
			ourTracker.getServiceRecord(recordId, res);
			expectedRes = "Four tires rotation";
			Assert::AreEqual(expectedRes, res.getServiceName(), L"Should update the record's service name to tire rotation");

			//update an non existing vehicle record
			vin = "new3";
			auto vehicle3 = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			recordId = "3214";
			serviceRecord record3(vehicle3, recordId);
			record3.setTask(tireRotation);
			errMsg = ourTracker.updateServiceRecord(record3);
			expectedRes = "For new vehicle record use addServiceRecord method instead.";
			Assert::AreEqual(expectedRes, errMsg, L"For new vehicle record use addServiceRecord method instead.");
		}

		TEST_METHOD(trackerMustInitialized)
		{
			// TODO: Your test code here
			serviceTracker ourTracker;
			string recordId = "123456";
			auto task = ourGenerator.generateTask(taskList::oilChange, name, description, price, errMsg);
			auto vehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			serviceRecord record(vehicle, recordId);
			record.setTask(task);
			string addRecord = ourTracker.addServiceRecord(record);
			string expectedResult = "Service tracker is not properly initialized.";
			Assert::AreEqual(expectedResult, addRecord, L"Non initialized tracker should not be able to use.");

			addRecord = ourTracker.removeRecord(recordId);
			Assert::AreEqual(expectedResult, addRecord, L"Non initialized tracker should not be able to use.");

			//able to add after initialization
			ourTracker.initialize(ourFactory, ourGenerator);
			addRecord = ourTracker.addServiceRecord(record);
			expectedResult = "";
			Assert::AreEqual(expectedResult, addRecord, L"Initialized tracker should be able to add record.");

			//able to remove after initialization
			addRecord = ourTracker.removeRecord(recordId);
			Assert::AreEqual(expectedResult, addRecord, L"Initialized tracker should be able to remove record.");

		}
	};
}