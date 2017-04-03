#include "stdafx.h"
#include "CppUnitTest.h"
#include "vehicleFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vehicleFactoryUnitTest
{		
	TEST_CLASS(createNewVehicle)
	{
	public:
		string vin;
		string model;
		string make;
		int odometer;
		int year;
		string errMsg;
		vehicleFactory ourFactory;

		TEST_METHOD(createNewGasVehicle)
		{
			// TODO: Your test code here
			vin = "123";
			model = "ES350";
			make = "lexus"; 
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			Assert::AreEqual(vin, gasVehicle.get()->getVinNum(),L"Vin number is matching.");
			Assert::AreEqual(model, gasVehicle.get()->getModel(), L"Model is matching.");
			Assert::AreEqual(make, gasVehicle.get()->getMake(), L"Make is matching.");
			Assert::AreEqual(year, gasVehicle.get()->getYear(), L"Year built is matching.");
			Assert::AreEqual(odometer, gasVehicle.get()->getOdometer(), L"Odometer is matching.");
			Assert::AreEqual((int)vehicleType::gas, (int)gasVehicle.get()->getVehicleType(), L"Vehicle type is matching.");
		}

		TEST_METHOD(createNewDieselVehicle)
		{
			// TODO: Your test code here
			vin = "123";
			model = "ES350";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> dieselVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);

			Assert::AreEqual(vin, dieselVehicle.get()->getVinNum(), L"Vin number is matching.");
			Assert::AreEqual(model, dieselVehicle.get()->getModel(), L"Model is matching.");
			Assert::AreEqual(make, dieselVehicle.get()->getMake(), L"Make is matching.");
			Assert::AreEqual(year, dieselVehicle.get()->getYear(), L"Year built is matching.");
			Assert::AreEqual(odometer, dieselVehicle.get()->getOdometer(), L"Odometer is matching.");
			Assert::AreEqual((int)vehicleType::disel, (int)dieselVehicle.get()->getVehicleType(), L"Vehicle type is matching.");
		}
		TEST_METHOD(createNewElectricVehicle)
		{
			// TODO: Your test code here
			vin = "123";
			model = "ES350";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> electricVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);

			Assert::AreEqual(vin, electricVehicle.get()->getVinNum(), L"Vin number is matching.");
			Assert::AreEqual(model, electricVehicle.get()->getModel(), L"Model is matching.");
			Assert::AreEqual(make, electricVehicle.get()->getMake(), L"Make is matching.");
			Assert::AreEqual(year, electricVehicle.get()->getYear(), L"Year built is matching.");
			Assert::AreEqual(odometer, electricVehicle.get()->getOdometer(), L"Odometer is matching.");
			Assert::AreEqual((int)vehicleType::electric, (int)electricVehicle.get()->getVehicleType(), L"Vehicle type is matching.");
		}

		TEST_METHOD(createNewVehicleWithEmptyVin)
		{
			// TODO: Your test code here
			vin = "";
			model = "ES350";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> electricVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			Assert::IsNull(electricVehicle.get());
			string expectedErrMsg = "Vin number should not be empty.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Empty vin number will cause creation failure.");

		}

		TEST_METHOD(createNewVehicleWithEmptyModel)
		{
			// TODO: Your test code here
			vin = "130";
			model = "";
			make = "lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			Assert::IsNull(gasVehicle.get());
			string expectedErrMsg = "Model number should not be empty.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Empty model number will cause creation failure.");
		}

		TEST_METHOD(createNewVehicleWithEmptyMake)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> diselVehicle = ourFactory.createNewVehicle(vin, model, make, 1000, 2010, vehicleType::disel, errMsg);
			Assert::IsNull(diselVehicle.get());
			string expectedErrMsg = "Make information should not be empty.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Empty make information will cause creation failure.");
		}

		TEST_METHOD(createNewVehicleWithNegativeYear)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "Lexus";
			year = -2010;
			odometer = 1000;
			shared_ptr<vehicle> electricVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::electric, errMsg);
			Assert::IsNull(electricVehicle.get());
			string expectedErrMsg = "Year made information is invalid.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Negative year information should casue creation failure.");
		}

		TEST_METHOD(createNewVehicleWithNegativeOdometer)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "Lexus";
			year = 2010;
			odometer = -1000;
			shared_ptr<vehicle> diselVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			Assert::IsNull(diselVehicle.get());
			string expectedErrMsg = "Odometer information is invalid.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Negative odometer information should casue creation failure.");
		}

	};

	TEST_CLASS(vehicleFacotryWithMemory){
	 public:
		string vin;
		string model;
		string make;
		int odometer;
		int year;
		string errMsg;
		vehicleFactory ourFactory;
	    
		TEST_METHOD(createVehicleWithExistingVin)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "Lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> diselVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			shared_ptr<vehicle> gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			Assert::IsNull(gasVehicle.get());
			string expectedErrMsg = "Vin number is taken.";
			Assert::AreEqual(expectedErrMsg, errMsg, L"Creating second vehicle with same vin is not allowed");
			vin = "1234";


			//to ensure if the vin is different, a gasVehicle is able to be find
			gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			Assert::AreEqual(vin, gasVehicle->getVinNum(), L"Create vihecle with vin = 1234");
			expectedErrMsg = "";
			Assert::AreEqual(expectedErrMsg, errMsg, L"No error message is expected to create vehicle with vin = 1234");
		}


		TEST_METHOD(retriveVehicleByVin)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "Lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> diselVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			vin = "1234";
			make = "BMW";
			model = "M3";
			shared_ptr<vehicle>  gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);
			
			shared_ptr<vehicle> resultVehicle = ourFactory.getVehicleByVin(vin);
			Assert::AreEqual(gasVehicle->getVinNum(), resultVehicle.get()->getVinNum(), L"Vin 1234 should get a vehicle with vin 1234");
			Assert::AreEqual(gasVehicle->getModel(), resultVehicle.get()->getModel(), L"Vin 1234 is associated with model M3");
			Assert::AreEqual(gasVehicle->getMake(), resultVehicle.get()->getMake(), L"Vin 1234 is associated with a BMW");
			vin = "130";
			resultVehicle = ourFactory.getVehicleByVin(vin);
			Assert::AreEqual(diselVehicle->getVinNum(), resultVehicle.get()->getVinNum(), L"Vin 130 should get a vehicle with vin 130");
			Assert::AreEqual(diselVehicle->getModel(), resultVehicle.get()->getModel(), L"Vin 130 is associated with model ES350");
			Assert::AreEqual(diselVehicle->getMake(), resultVehicle.get()->getMake(), L"Vin 130 is associated with a Lexus");

		}
	
		TEST_METHOD(retriveVehicleByInvalidVin)
		{
			// TODO: Your test code here
			vin = "130";
			model = "ES350";
			make = "Lexus";
			year = 2010;
			odometer = 1000;
			shared_ptr<vehicle> diselVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::disel, errMsg);
			vin = "1234";
			make = "BMW";
			model = "M3";
			shared_ptr<vehicle>  gasVehicle = ourFactory.createNewVehicle(vin, model, make, odometer, year, vehicleType::gas, errMsg);

			vin = "1243245253";
			shared_ptr<vehicle> resultVehicle = ourFactory.getVehicleByVin(vin);

			Assert::IsNull(resultVehicle.get());

		}
	};
}