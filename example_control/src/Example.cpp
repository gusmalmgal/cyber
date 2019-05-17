/*
 * Copyright (C) 2019 Yue Kang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon-complete.hpp"
#include "messages.hpp"


int32_t main(int32_t argc, char **argv) {


    bool turn = false;
    bool follow = false;
    bool followCar = true;
    std::string direction = ""; 
    std::string stopping = ""; 

    

    std::cout << "this is testing the commandline";
    // Parse the arguments from the command line
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);



    if ( (0 == commandlineArguments.count("cid")) || (0 != commandlineArguments.count("help")) )
    {
        std::cerr << argv[0] << " is an example application for miniature vehicles (Kiwis) of DIT638 course." << std::endl;
        std::cerr << "Usage:  " << argv[0] << " --cid=<CID of your OD4Session> [--freq=<Frequency>] [--verbose] [--help]" << std::endl;
        std::cerr << "example:  " << argv[0] << " --cid=112 --freq=30 --verbose" << std::endl;
        return -1;
    }
    else
    {
        cluon::OD4Session od4Drive{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
        cluon::OD4Session od4Turn{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
        cluon::OD4Session od4Command{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};



        if (0 == od4Drive.isRunning() && 0 == od4Turn.isRunning())
        {
            std::cerr << "ERROR: No OD4Session running!!!" << std::endl;
            return -2;
        }

        const bool VERBOSE{commandlineArguments.count("verbose") != 0};
        //const float FREQ{(commandlineArguments["freq"].size() != 0) ? static_cast<float>(std::stof(commandlineArguments["freq"])) : static_cast<float>(-1.0)};


//------------_______----___---__---_--__---_______----_______----_-------___--------____________-----_----___-----_






//_________---------______----------___-----_-------______---________-----_--_----___---___--__________----______--_-------__-_______----__------_-

              


//-------_______-------___-------___---_________----____----_--_____________----_----___---_----_----________---_______-------___--------__---__----

  auto commandHandler{[&od4Command, &od4Drive, &od4Turn, VERBOSE, &turn, &follow, &direction, &followCar](cluon::data::Envelope &&envelope)

            {

                           


            
            //declear msg to sabe whats recieved from custom message
            auto msg = cluon::extractMessage<opendlv::proxy::instructions>(std::move(envelope));
            //trash shit to avoid errors

            const int16_t delay{500};

            opendlv::proxy::PedalPositionRequest pedalReq;
            opendlv::proxy::GroundSteeringRequest steerReq;

            //save content from message into tempSize
            turn = msg.turn(); // Corresponds to odvd message set
            follow = msg.follow();
            direction = msg.direction();



                if(VERBOSE){

                       if(turn == true && direction == "left"){

                        steerReq.groundSteering(0.13);
                        od4Turn.send(steerReq);

               
                //if(VERBOSE){

                    //set speed
                        pedalReq.position(0.14);
                        od4Drive.send(pedalReq);

                    //for how long we gasar
                        std::this_thread::sleep_for(std::chrono::milliseconds(6 * delay));

                        pedalReq.position(0.0);
                        od4Drive.send(pedalReq);

                    //reset wheels
                        steerReq.groundSteering(0.0);
                        od4Turn.send(steerReq);

//----___________--------__---_--______-----_--__---_-------___---_----_-------___---_______----_----_-------_-------_------
                        
                       }else if(turn == true && direction == "right"){

                        steerReq.groundSteering(-0.37);
                        od4Turn.send(steerReq);

               
                //if(VERBOSE){

                    //set speed
                        pedalReq.position(0.14);
                        od4Drive.send(pedalReq);

                    //for how long we gasar
                        std::this_thread::sleep_for(std::chrono::milliseconds(5 * delay));

                        pedalReq.position(0.0);
                        od4Drive.send(pedalReq);

                    //reset wheels
                        steerReq.groundSteering(0.0);
                        od4Turn.send(steerReq);

//------------_________-----_--___--__---______-----___---______-----_-__----_-------___-----___-----_----__----_----_--__---__------                        

                       }else if(turn == true && direction == "straight"){

                            steerReq.groundSteering(0.0);
                            od4Turn.send(steerReq);


                            pedalReq.position(0.15);
                            od4Drive.send(pedalReq);

                    //for how long we gasar
                            std::this_thread::sleep_for(std::chrono::milliseconds(6 * delay));

                            pedalReq.position(0.0);
                            od4Drive.send(pedalReq);

                            steerReq.groundSteering(0.0);
                            od4Turn.send(steerReq);
                        
                       }else if(follow == true){
                        followCar = true;
                       }
                }

            }

        };

                    od4Command.dataTrigger(opendlv::proxy::instructions::ID(), commandHandler);               


//----------____________----__---__----________-----________--------------_-----__------____-------__---__------_---_---_--------______----------__
     
     float tempTurn{0.0};
        
        //declearing function and lambda function
        auto onTurn{[&od4Turn, VERBOSE, &tempTurn](cluon::data::Envelope &&envelope)
            // &<variables> will be captured by reference (instead of value only)
            {
            

                std::cout << "recieved size from camera-microcontroler"  << std::endl;

            //declear msg to sabe whats recieved from custom message
            auto msg = cluon::extractMessage<opendlv::proxy::correctTurn>(std::move(envelope));
            //trash shit to avoid errors
            
           
            //save content from message into tempSize
            tempTurn = msg.offset(); // Corresponds to odvd message set
            
            opendlv::proxy::GroundSteeringRequest steerReq;



           
            //declear message in order to send pedal request
            opendlv::proxy::PedalPositionRequest pedalReq;
            if(VERBOSE){
            
                if(tempTurn > -320 && tempTurn < -192){

                steerReq.groundSteering(0.2);
                od4Turn.send(steerReq);


                }else if(tempTurn > -192 && tempTurn < -64){

                steerReq.groundSteering(0.1);
                od4Turn.send(steerReq);

                }else if(tempTurn > 64 && tempTurn < 192){

                steerReq.groundSteering(-0.1);
                od4Turn.send(steerReq);

                }else if(tempTurn > 192 && tempTurn < 320){

                steerReq.groundSteering(-0.2);
                od4Turn.send(steerReq);

                }else{

                steerReq.groundSteering(0.0);
                od4Turn.send(steerReq);

                }
            
                std::cout <<"sending steering to control " << std::endl;

            }

            }   

        };


    

            

      

//---------------_____-------------------------------__---------_____-----____-------------------________________---___-----_-------_-------_--
            
        
        int counter = 0;
        //declearing function and lambda function
        auto onFollow{[&od4Drive, VERBOSE, &stopping, &counter](cluon::data::Envelope &&envelope)
            // &<variables> will be captured by reference (instead of value only)
            {

                const int16_t delay{500};

            
                float tempSize{0.0};
                std::cout << "recieved size from camera-microcontroler"  << std::endl;

            //declear msg to sabe whats recieved from custom message
            auto msg1 = cluon::extractMessage<opendlv::proxy::sizeReading>(std::move(envelope));
            //trash shit to avoid errors
            auto msg2 = cluon::extractMessage<opendlv::proxy::stopRequest>(std::move(envelope));

            
           
            //save content from message into tempSize
            tempSize = msg1.size(); // Corresponds to odvd message set
            stopping = msg2.stopping();
            

            //declear message in order to send pedal request
            opendlv::proxy::PedalPositionRequest pedalReq;
            if(VERBOSE){



            if(stopping == "stop"){

                std::cout << "recieved: (" << stopping << ") prepaing to stop" << std::endl;

                tempSize = 0.0;            

                pedalReq.position(-0.18);
                od4Drive.send(pedalReq);

                std::this_thread::sleep_for(std::chrono::milliseconds(delay));

                pedalReq.position(0.0);
                od4Drive.send(pedalReq);

                std::cout << "no square, stopping car" << std::endl;

               
            }else{

            }


            if(tempSize > 1 && tempSize < 10000){
         
                

                std::cout << "reacted on square, size is: (" << tempSize << ") " <<" sending: 0.3 to adjust speed" << std::endl;

                    pedalReq.position(0.145);
                    od4Drive.send(pedalReq);

                                std::cout << "sending speed to car " << std::endl;


            }else if(tempSize > 10001 && tempSize < 18000){

                

                std::cout << "reacted on square, size is: (" << tempSize << ") " <<" sending: 0.26 to adjust speed" << std::endl;

                    pedalReq.position(0.13);
                    od4Drive.send(pedalReq);

                                std::cout << "sending speed to car " << std::endl;


            }else if(tempSize > 18001 && tempSize < 20000){

              

                std::cout << "reacted on square, size is: (" << tempSize << ") " << " sending: 0.21 to adjust speed" << std::endl;

                pedalReq.position(0.125);
                    od4Drive.send(pedalReq);

                                std::cout << "sending speed to car " << std::endl;


            }else if(tempSize > 20001 && tempSize < 25000){

                

                std::cout << "reacted on square, size is: (" << tempSize << ") " <<" sending: 0.18 to adjust speed" << std::endl;

                pedalReq.position(0.14);
                    od4Drive.send(pedalReq);

                                std::cout << "sending speed to car " << std::endl;


            }else{

                pedalReq.position(-0.18);
                od4Drive.send(pedalReq);

                std::this_thread::sleep_for(std::chrono::milliseconds(delay));


                pedalReq.position(0.0);
                    od4Drive.send(pedalReq);

                                std::cout << "sending speed to car " << std::endl;

                 tempSize = 0.0;            

                std::cout << "this is the else" << std::endl;

            }

            
            std::cout << "sending speed to car " << std::endl;

           

            }

               

            }   

        };


        while(followCar == true){

            od4Drive.dataTrigger(opendlv::proxy::correctTurn::ID(), onTurn);
            od4Drive.dataTrigger(opendlv::proxy::sizeReading::ID(), onFollow);
            od4Drive.dataTrigger(opendlv::proxy::stopRequest::ID(), onFollow);

        }
        
//------------____________-------___---_----_-----_____---____----______--___---__---_----------_----_____---___---__---_-------___---__---_-------__--


        
        //declearing function and lambda function
        auto onStop{[&od4Turn, VERBOSE, &tempTurn](cluon::data::Envelope &&envelope)
            // &<variables> will be captured by reference (instead of value only)
            {
            
                

            //declear msg to sabe whats recieved from custom message
            auto msg = cluon::extractMessage<opendlv::proxy::correctTurn>(std::move(envelope));
            //trash shit to avoid errors
            
           
            //save content from message into tempSize
            tempTurn = msg.offset(); // Corresponds to odvd message set
            
            opendlv::proxy::GroundSteeringRequest steerReq;



           
            //declear message in order to send pedal request
            opendlv::proxy::PedalPositionRequest pedalReq;
            if(VERBOSE){
            
                if(tempTurn > -320 && tempTurn < -192){

                steerReq.groundSteering(0.2);
                od4Turn.send(steerReq);


                }else if(tempTurn > -192 && tempTurn < -64){

                steerReq.groundSteering(0.1);
                od4Turn.send(steerReq);

                }else if(tempTurn > 64 && tempTurn < 192){

                steerReq.groundSteering(-0.1);
                od4Turn.send(steerReq);

                }else if(tempTurn > 192 && tempTurn < 320){

                steerReq.groundSteering(-0.2);
                od4Turn.send(steerReq);

                }else{

                steerReq.groundSteering(0.0);
                od4Turn.send(steerReq);

                }
            
                std::cout <<"sending steering to control " << std::endl;

            }

            }   

        };





//---------------______-----------____----_----__----____----_____----__----____________----__----_______----_----_______----_----------_--_______________---_-

        while(od4Drive.isRunning()){

            const int16_t delay{1000};

            std::cout << "od4 is running" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(3 * delay));

        }

        return 0;
    }
}



