/*
* @file PSCS_NAV.cpp
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#include "PSCS_NAV.h"



TskNavigation::TskNavigation()
{

}
void TskNavigation::begin(int pin)
{
  _mode=false; //set to auto mode
  _manager_winch.attach(pin);
  _control_angle=0; //
  winchControl(0);
}
void TskNavigation::setNavigationMode(bool mode)
{
  _mode=mode;
}
bool TskNavigation::getNavigationMode()
{
  return _mode;
}

void TskNavigation::updateNavigationParamers(float dist,float bearing, float course,float d_alt)
{
  _distance_from_destination=dist;
  _diff_altitude=d_alt;

  //convert 0~359 to -179~+180
  _bearing_angle_wrap180=bearing;
  if(_bearing_angle_wrap180>180)_bearing_angle_wrap180 -= 360;
  _course_angle_wrap180=course;
  if(_course_angle_wrap180>180)_course_angle_wrap180 -= 360;
}
void TskNavigation::updateControlAngle()
{

  _control_angle=_bearing_angle_wrap180-_course_angle_wrap180;
  //To keep _control_angle into -179~+180
  if(_control_angle< -180){
     _control_angle+=360.f;
  }
  if(_control_angle> 180){
    _control_angle-=360.f;
  }
}

void TskNavigation::winchControl(float angle)
{
  if(!_mode){  // auto mode
    _manager_winch.turnWinch( _manager_winch.angleToMicrosec(_control_angle) );
    Serial.print("Turn winch(auto):");
    Serial.println(_control_angle);
  }else{ //manual mode
    _manager_winch.turnWinch( _manager_winch.angleToMicrosec(angle) );
    Serial.print("Turn winch(manu):");
    Serial.println(angle);
  }
}

float TskNavigation::getControlAngle()
{
  return _control_angle;
}

void TskNavigation::printNavigationInfo()
{

  Serial.println("******* Navigation Info *********");
  Serial.println("* bearin:course:dist:altitude =  ");
  Serial.print( _bearing_angle_wrap180);Serial.print(":");
  Serial.print( _course_angle_wrap180);Serial.print(":");
  Serial.print( _distance_from_destination);Serial.print(":");
  Serial.println( _diff_altitude);

  Serial.print("**** turn  ");
  Serial.print( _control_angle);
  Serial.println("  ******");
  Serial.println("********************************* ");
}

MngNavigation::MngNavigation()
{

}
void MngNavigation::attach(int pin)
{
  _winch_servo_pin=pin;
  _winch_servo.attach(pin);
}
void MngNavigation::turnWinch(uint16_t microsec)
{
  // 1500 neutral position
  // 900 fully counter-clockwise, 2100 fully clockwise
  if(microsec<900 || microsec>2100){
    return;
  }
  _winch_servo.writeMicroseconds(microsec);

}

uint16_t MngNavigation::angleToMicrosec(float angle)
{
  uint16_t angle_micro;  //900~2100
  //the angle is -179~180
  angle=0-angle;
  if(angle< -179 || angle>180){
    Serial.print("Wrong winch angle");
    Serial.println(angle);
    return;
  }
  //angle_micro=(uint16_t)map(angle,-179,180,900,2100); //max 3 turn too much turn
  //angle_micro=(uint16_t)map(angle,-179,180,1100,1900);  // max 2 turn
  angle_micro=(uint16_t)map(angle,-179,180,1200,1800);  // max 1.5 turn

  return angle_micro;
}
