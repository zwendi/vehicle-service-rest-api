#include "stdafx.h"
#include "CppUnitTest.h"
#include "serviceRecord.h"
#include "vehicleFactory.h"
#include "chargeBattery.h"
#include "oilChange.h"
#include "tireRotation.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace serviceRecordTest
{		
	TEST_CLASS(serviceRecordTest)
	{
	public:
		string vin;
		string model;
		string make;
		int odometer = 0;
		int year = 2016;
		string errMsg;
		vehicleFactory ourFactory;
		//TASK
		string taskName;
		string description;
		double price;
		//service record
		double discount;
		string recordId;
		TEST_METHOD(serviceRecordGetterTest)
		{
			// TODO: Your test code here
			vin = "123";
			model = "P90D";
			make = "tesla";
			taskName = "tire rotation";
			description = "to change tire and rebalance tires.";
			price = 45;
			tireRotation* r = new tireRotation(taskName, description, price);
			shared_ptr<task> tirePtr(r);
			shared_ptr<vehicle> tesla = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			recordId = "123";
			
			serviceRecord firstRecord(tesla,recordId);
			firstRecord.setTask(tirePtr);
			Assert::AreEqual(price, firstRecord.getPrice(), L"Price should match list price.");
			Assert::AreEqual(price, firstRecord.getChargePrice(), L"Charge Price should match task price when no discount is set.");

			Assert::AreEqual(description, firstRecord.getServiceDescription(), L"Description should match task description.");
			Assert::AreEqual(taskName, firstRecord.getServiceName(), L"taskName should match task description.");
			Assert::AreEqual(recordId, firstRecord.getRecordNum(), L"RecordId should be matching.");
						
			Assert::AreEqual(vin, firstRecord.getVinNum(), L"Vin number should be matching.");
			Assert::AreEqual(model, firstRecord.getModel(), L"Model should be matching.");
			Assert::AreEqual(make, firstRecord.getMake(), L"Make should be matching.");
			double defaultDiscount = 1;
			Assert::AreEqual(defaultDiscount, firstRecord.getDiscount(), L"Discount should be matching.");
		}


		TEST_METHOD(setDiscount)
		{
			vin = "123";
			model = "P90D";
			make = "tesla";
			taskName = "tire rotation";
			description = "to change tire and rebalance tires.";
			price = 45;
			tireRotation* r = new tireRotation(taskName, description, price);
			shared_ptr<task> tirePtr(r);
			shared_ptr<vehicle> tesla = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			recordId = "123";

			serviceRecord firstRecord(tesla, recordId);
			firstRecord.setTask(tirePtr);
			double defaultDiscount = 1;
			Assert::AreEqual(defaultDiscount, firstRecord.getDiscount(), L"Discount should be matching.");
			double halfDiscount = 0.5;
			firstRecord.setDiscount(halfDiscount);
			Assert::AreEqual(halfDiscount, firstRecord.getDiscount(), L"Discount should be matching.");

			double invalidDiscount = -1.0;
			bool ok = firstRecord.setDiscount(invalidDiscount);
			Assert::AreEqual(false, ok, L"Negative discount is not allowed.");

			invalidDiscount = 1.25;
			ok = firstRecord.setDiscount(invalidDiscount);
			Assert::AreEqual(false, ok, L"Discount greater than 1 is not allowed.");

			invalidDiscount = 0;
			ok = firstRecord.setDiscount(invalidDiscount);
			Assert::AreEqual(false, ok, L"Discount equal to zero is not allowed.");
		}


		TEST_METHOD(getChargePriceAndGetPrice)
		{
			vin = "123";
			model = "P90D";
			make = "tesla";
			taskName = "tire rotation";
			description = "to change tire and rebalance tires.";
			price = 45;
			tireRotation* r = new tireRotation(taskName, description, price);
			shared_ptr<task> tirePtr(r);
			shared_ptr<vehicle> tesla = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			recordId = "123";

			serviceRecord firstRecord(tesla, recordId);
			firstRecord.setTask(tirePtr);
			double halfDiscount = 0.5;
			firstRecord.setDiscount(halfDiscount);
			
			//getPrice() should match the task price
			Assert::AreEqual(price, firstRecord.getPrice(), L"getPrice() should match the task price");
			double discountPrice = price* halfDiscount;
			Assert::AreEqual(discountPrice, firstRecord.getChargePrice(), L"Charge price should match the discount");
		}
			
		TEST_METHOD(isValidRecord)
		{
		
		
		}
	};
}