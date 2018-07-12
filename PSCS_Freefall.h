/**
* @file PSCS_Freefall.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*
*/

#ifndef TSK_Freefall_h
#define TSK_Freefall_h

#include "Arduino.h"
#include "Servo.h"

class MngFreeFall

{
  public:
     MngFreeFall();
     void attach(int pin);
     void servoReady();
     void servoHooking();
     void servoRelease();

  private:
    int _pin;
    Servo _myservo;


};


class TskFreeFall
{
  public:
    TskFreeFall();
    /*  to move servo */
    void begin(int pin);
    void readyPosition();
    void hookingPosition();
    void releasePosition();


  private:
    MngFreeFall _manager_servo;



};


#endif
