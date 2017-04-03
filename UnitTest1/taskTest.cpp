#include "stdafx.h"
#include "CppUnitTest.h"
#include "vehicleFactory.h"
#include "chargeBattery.h"
#include "oilChange.h"
#include "tireRotation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace taskTest
{		
	TEST_CLASS(chargeBatteryTest)
	{
	public:
		string vin;
		string model;
		string make;
	
		string errMsg;
		vehicleFactory ourFactory;
		int odometer=100;
		int year=2016;
		string description = "Charging electric vehicle fully.";
		string name = "Super Battery Charge.";
		double price = 20.0;

		TEST_METHOD(chargeBatteryIsAbleToServeToElectricVehicle)
		{
			// TODO: Your test code here
			vin = "123";
			model = "tesla";
			make = "P90D";
			shared_ptr<vehicle> electricCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
		    chargeBattery batteryTask(name, description, price);
			bool okToService = batteryTask.isAbleToService(electricCar);
			Assert::AreEqual(true, okToService, L"Battery charge should be able to applied to tesla.");
		}

		TEST_METHOD(chargeBatteryIsNotAbleToServeNonElectricVehicle)
		{
			// TODO: Your test code here
			vin = "123";
			model = "es350";
			make = "lexus";	
			shared_ptr<vehicle> gasCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			chargeBattery batteryTask(name, description, price);
			bool okToService = batteryTask.isAbleToService(gasCar);
			Assert::AreEqual(false, okToService, L"Battery charge should not be able to applied to gas car.");
			vin = "1234";
			make = "ford";
			model = "3000";
			shared_ptr<vehicle> diselCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			okToService = batteryTask.isAbleToService(diselCar);
			Assert::AreEqual(false, okToService, L"Battery charge should not be able to applied to disel truck.");
		}

		TEST_METHOD(chargeBatteryIsNotAbleToServeNonExistVehicle)
		{
			
			shared_ptr<vehicle> ghostCar;
			chargeBattery batteryTask(name, description, price);
			bool okToService = batteryTask.isAbleToService(ghostCar);
			Assert::AreEqual(false, okToService, L"Battery charge should not be able to applied to non exist car.");
		}

	};

	TEST_CLASS(oilChangeTest)
	{
	public:
		string vin = "123";
		string model;
		string make;
		int odometer = 1000;
		int year=2017;
		string errMsg;

		string description = "Changing engine oil to appropriate level.";
		
		string name = "Oil Change";
		double price  = 80.99;
		TEST_METHOD(oilChangeIsNotAbleToServeElectricVehicle)
		{
			// TODO: Your test code here
			vehicleFactory ourFactory;

			model = "tesla";
			make = "P90D";
			shared_ptr<vehicle> electricCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			oilChange oilChange(name, description, price);
			bool okToService = oilChange.isAbleToService(electricCar);
			Assert::AreEqual(false, okToService, L"Oil change should not be able to applied to electric vehicle.");
		}

		TEST_METHOD(oilChangeIsAbleToServeNonElectricVehicle)
		{
			// TODO: Your test code here
			vehicleFactory ourFactory;

			model = "es350";
			make = "lexus";
			shared_ptr<vehicle> gasCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			oilChange oilChange(name, description, price);
			bool okToService = oilChange.isAbleToService(gasCar);
			Assert::AreEqual(true, okToService, L"Oil change should be able to applied to gas car.");
			make = "ford";
			model = "3000";
			vin = "12345";

			shared_ptr<vehicle> diselCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			okToService = oilChange.isAbleToService(diselCar);
			Assert::AreEqual(true, okToService, L"Oil change should not be able to applied to disel truck.");
		}

		TEST_METHOD(oilChangeIsNotAbleToServeNonExistVehicle)
		{

			shared_ptr<vehicle> ghostCar;
			oilChange oilChange(name, description, price);
			bool okToService = oilChange.isAbleToService(ghostCar);
			Assert::AreEqual(false, okToService, L"Oil change should not be able to applied to non exist car.");
		}

	};

	TEST_CLASS(tireRotationTest)
	{
	public:
		string vin = "123";
		string model;
		string make;
		int odometer = 100000;
		int year  = 2000;
		string errMsg;
		vehicleFactory ourFactory;

		string description = "Changing all tire and check all tires pressure and balance.";
		string name = "Speed tire change";
		double price = 45.5;
		TEST_METHOD(tireRotationIsAbleToServeToElectricVehicle)
		{
			// TODO: Your test code here
			model = "tesla";
			make = "P90D";
			shared_ptr<vehicle> electricCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			tireRotation tireRotation(name, description, price);
			bool okToService = tireRotation.isAbleToService(electricCar);
			Assert::AreEqual(true, okToService, L"Tire rotation should be able to applied to tesla.");
		}

		TEST_METHOD(tireRotationIsAbleToServeNonElectricVehicle)
		{
			// TODO: Your test code here
			vehicleFactory ourFactory;

			model = "es350";
			make = "lexus";

			shared_ptr<vehicle> gasCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			tireRotation tireRotation(name, description, price);
			bool okToService = tireRotation.isAbleToService(gasCar);
			Assert::AreEqual(true, okToService, L"Tire rotation should be able to applied to gas car.");
			vin = "1234";
			make = "ford";
			model = "3000";
			shared_ptr<vehicle> diselCar = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			okToService = tireRotation.isAbleToService(diselCar);
			Assert::AreEqual(true, okToService, L"Tire rotation should be able to applied to disel truck.");
		}

		TEST_METHOD(chargeBatteryIsNotAbleToServeNonExistVehicle)
		{

			shared_ptr<vehicle> ghostCar;
			tireRotation tireRotation(name, description, price);
			bool okToService = tireRotation.isAbleToService(ghostCar);
			Assert::AreEqual(false, okToService, L"Tire rotation should not be able to applied to non exist car.");
		}

	};
	
}