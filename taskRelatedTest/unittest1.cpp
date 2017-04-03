/*
#include "stdafx.h"
#include "CppUnitTest.h"
#include "vehicleFactory.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace taskRelatedTest
{		
	TEST_CLASS(chargeBatteryTry)
	{
	public:
		string vin;
		string model;
		string make;
		int odometer;
		int year;
		string errMsg;
		vehicleFactory ourFactory;

		TEST_METHOD(createNewGasVehicleVersionTested2)
		{
			// TODO: Your test code here
			vin = "123";
			model = "ES350";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			Assert::AreEqual(vin, gasVehicle.get()->getVinNum(), L"Vin number is matching.");
			Assert::AreEqual(model, gasVehicle.get()->getModel(), L"Model is matching.");
			Assert::AreEqual(make, gasVehicle.get()->getMake(), L"Make is matching.");
			Assert::AreEqual(year, gasVehicle.get()->getYear(), L"Year built is matching.");
			Assert::AreEqual(odometer, gasVehicle.get()->getOdometer(), L"Odometer is matching.");
			Assert::AreEqual((int)vehicleType::gas, (int)gasVehicle.get()->getVehicleType(), L"Vehicle type is matching.");
		}
	};
}
*/
#include "stdafx.h"
#include "CppUnitTest.h"
#include "vehicleFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vehicleFactoryUnitTest1
{
	TEST_CLASS(createNewVehicle1)
	{
	public:
		string vin;
		string model;
		string make;
		int odometer;
		int year;
		string errMsg;
		vehicleFactory ourFactory;

		TEST_METHOD(createNewGasVehicle3)
		{
			// TODO: Your test code here
			vin = "123";
			model = "ES350";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			Assert::AreEqual(vin, gasVehicle.get()->getVinNum(), L"Vin number is matching.");
			Assert::AreEqual(model, gasVehicle.get()->getModel(), L"Model is matching.");
			Assert::AreEqual(make, gasVehicle.get()->getMake(), L"Make is matching.");
			Assert::AreEqual(year, gasVehicle.get()->getYear(), L"Year built is matching.");
			Assert::AreEqual(odometer, gasVehicle.get()->getOdometer(), L"Odometer is matching.");
			Assert::AreEqual((int)vehicleType::gas, (int)gasVehicle.get()->getVehicleType(), L"Vehicle type is matching.");
		}
	};
}