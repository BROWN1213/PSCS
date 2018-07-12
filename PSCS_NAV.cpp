/*
* @file PSCS_NAV.cpp
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#include "PSCS_NAV.h"

#define MILLPER1T 1560.*1.5 //1.56sec per 1 turn. 1.5 is tuning value
#define MAXTURN 3.
#define CONTROLL_MAX 0.75 // 180deg to 0.75 turn
#define WINCHMAX_CALLBACKTIME MILLPER1T*CONTROLL_MAX

#define WINCH_MIN (uint16_t)(1500.- (600./MAXTURN*CONTROLL_MAX) ) //0.75 turn to 1650
#define WINCH_MAX (uint16_t)(1500.+ (600./MAXTURN*CONTROLL_MAX) )  //-0.75 turn to 1350
TskNavigation::TskNavigation()
{

}
void TskNavigation::begin(int pin,void (*winchCallback)())
{
  _mode=true; //set to manual mode
  _manager_winch.attach(pin);
  _control_angle=0; //
  winchControl(0);
  _pCallback=winchCallback;
  _timerId=0;

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

void TskNavigation::timerRun(){
  _winchTimer.run();
}

void TskNavigation::winchControl(float angle)
{
  uint16_t winch_neutal_time;
  if(!_mode){  // auto mode
    _manager_winch.turnWinch( _manager_winch.angleToMicrosec(_control_angle) );
    if(fabs(angle)>20){
      winch_neutal_time=(uint16_t)(WINCHMAX_CALLBACKTIME*fabs(angle)/180.);
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
    Serial.print(F("Turn winch(auto):"));
    Serial.println(_control_angle);
  }else{ //manual mode
    _manager_winch.turnWinch( _manager_winch.angleToMicrosec(angle) );
    if(fabs(angle)>20){
      winch_neutal_time=(uint16_t)(WINCHMAX_CALLBACKTIME*fabs(angle)/180.);
      //Serial.print("callback time=");
      //Serial.println(winch_neutal_time);
      if(_winchTimer.getNumTimers()>0)_winchTimer.deleteTimer(_timerId);
      _timerId=_winchTimer.setTimeout(winch_neutal_time, _pCallback);
    }
    Serial.print(F("Turn winch(manu):"));
    Serial.println(angle);
  }
}
void TskNavigation::winchNeutral(){
  _manager_winch.turnWinch( _manager_winch.angleToMicrosec(0) );

}
float TskNavigation::getControlAngle()
{
  return _control_angle;
}

void TskNavigation::printNavigationInfo()
{

  Serial.println(F("** Navi br:cr:dst:alt  **"));
  Serial.print( _bearing_angle_wrap180);Serial.print(F(":"));
  Serial.print( _course_angle_wrap180);Serial.print(F(":"));
  Serial.print( _distance_from_destination);Serial.print(F(":"));
  Serial.println( _diff_altitude);

  Serial.print(F("** Turn = "));
  Serial.println( _control_angle);
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
    Serial.print(F("Wrong winch angle"));
    Serial.println(angle);
    return;
  }
  //angle_micro=(uint16_t)map(angle,-179,180,900,2100); //max 3 turn too much turn
  //angle_micro=(uint16_t)map(angle,-179,180,1100,1900);  // max 2 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1200,1800);  // max 1.5 turn
  //angle_micro=(uint16_t)map(angle,-179,180,1350,1650);  // max 0.75 turn
  angle_micro=(uint16_t)map(angle,-179,180,WINCH_MIN,WINCH_MAX);
  //Serial.print(WINCH_MIN);Serial.println(WINCH_MAX);
  return angle_micro;
}
