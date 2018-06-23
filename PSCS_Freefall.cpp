/**
* @file BlynkConfig.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*
*/

#include "PSCS_Freefall.h"
/************************/
/* TskFreeFall class    */
/************************/
TskFreeFall::TskFreeFall(){}

void TskFreeFall::begin(int pin)
{
  _manager_servo.attach(pin);
}

void TskFreeFall::readyPosition(){

  Serial.println("readyPosition");
  _manager_servo.servoReady();
}
void TskFreeFall::hookingPosition(){
  _manager_servo.servoHooking();
}
void TskFreeFall::releasePosition(){
  _manager_servo.servoRelease();
}


/************************/
/* MngFreeFall class    */
/************************/

MngFreeFall::MngFreeFall(){}

void MngFreeFall::attach(int pin)
{
  _pin=pin; //keep pin number
  _myservo.attach(pin);  //attach data pin to my servo
}

void MngFreeFall::servoReady()
{
  _myservo.attach(_pin);
  _myservo.write(90);
  delay(200);
  _myservo.detach();
}
void MngFreeFall::servoHooking()
{
  _myservo.attach(_pin);
  _myservo.write(180);
  delay(200);
  _myservo.detach();
}
void MngFreeFall::servoRelease()
{
  _myservo.attach(_pin);
  _myservo.write(20);
  delay(200);
  _myservo.detach();

}
