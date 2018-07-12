/*
* @file PSCS_NAV.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#ifndef PSCS_NAV_h
#define PSCS_NAV_h

#include "Arduino.h"
#include<Servo.h>
#include<SimpleTimer.h>

class MngNavigation
{
  public:
    MngNavigation();
    void attach(int pin);
    void turnWinch(uint16_t microsec);
    uint16_t angleToMicrosec(float angle);
  private:
    int _winch_servo_pin;
    Servo _winch_servo;


};

class TskNavigation
{
  public:
    TskNavigation();
    void begin(int pin,void (*winchCallback)());
    void updateNavigationParamers(float dist,float bearing, float course,float d_alt);
    void updateControlAngle();
    void printNavigationInfo();
    void setNavigationMode(bool mode);
    bool getNavigationMode();
    void winchControl(float angle);
    void winchNeutral();
    float getControlAngle();
    void timerRun();
    MngNavigation _manager_winch;

  private:
    float _distance_from_destination;
    float _bearing_angle_wrap180;
    float _course_angle_wrap180;
    float _control_angle;
    float _diff_altitude;
    uint32_t _winch_angle;
    bool _mode;  // false: automode, true: manual mode

    SimpleTimer _winchTimer;
    int _timerId;
    void (*_pCallback)(void);
};




#endif
