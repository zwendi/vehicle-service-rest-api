# vehicle-service-rest-api
simple c++ rest api that simulates a service record system
This is a visual studio 2013 c++ project with package cpprestsdk.v120.windesktop.msvcst.

If you debug the program, it will start listen on: "localhost/demo/"

Rest api supported:

Get requests:
localhost/demo/tasks                                    
localhost/demo/vehicles                               
localhost/demo/records                             

localhost/demo/task/{task_name}                               
localhost/demo/vehicle/{vin_number}                               
localhost/demo/record/{record_number}                        
localhost/demo/record/v-record/{vin_number}      //getting all records associated with particular vehicles


Put requests:
localhost/vehicle/{vin_number}        //vin_number should be within the system already  
localhost/record/{record_number}      //record_number should be within the system already


Post requests:
localhost/record        //json should contain a new record_number. A brand new client visit for a service, a new vehicle will be generated
localhost/task          //json should contain a new task name, a valid task type.

Delete request:
localhost/record        //since I am trying to put data in the memory, i am only allowing deleting the record.
                        //Vehicle will be deleted once there is no record associated with it. 
                        //Task should be always in the system for historical record. Create new record if needed.



