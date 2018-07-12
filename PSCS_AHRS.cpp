/**
* @file PSCS_AHRS.cpp
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#include "PSCS_AHRS.h"
#include "CansatIMU.h"


void TskAhrs::begin()
{
  _manager_ahrs.ahrsBegin();
  _ahrs_data_flag=false;
}
void TskAhrs::update()
{
  if(_manager_ahrs.isAhrsDataReady()){
    _manager_ahrs.readAhrsData();
    caculrateMotionAcceleration();
    _ahrs_data_flag=true;
  }
}
GyroInfo TskAhrs::getGyro()
{
  GyroInfo gyro;
  gyro.roll=_manager_ahrs.roll;
  gyro.pitch=_manager_ahrs.pitch;
  gyro.yaw=_manager_ahrs.yaw;
  return gyro;
}
AcceleroInfo TskAhrs::getAccelero()
{
  AcceleroInfo accelero;
  accelero.ax=_manager_ahrs.ax;
  accelero.ay=_manager_ahrs.ay;
  accelero.az=_manager_ahrs.az;
  return accelero;
}
float TskAhrs::getMotionAcceleration()
{
  return _motion_acceleration;
}

void TskAhrs::caculrateMotionAcceleration()
{
  float x,y,z;
  x=_manager_ahrs.ax;
  y=_manager_ahrs.ay;
  z=_manager_ahrs.az;

  _motion_acceleration=pow( (x*x+y*y+z*z),0.5);
}

bool TskAhrs::isAhrsDataNew()
{
  if(_ahrs_data_flag){
    _ahrs_data_flag=false;
    return(true);
  }else{
    return(false);
  }

}
void TskAhrs::rx_empty(){
  _manager_ahrs.rx_empty();
}
void TskAhrs::printAhrsInfo()
{
  if(_print_count>=10){
    Serial.println(F("++++  AHRS Info   +++++++++++++++"));
    Serial.println(F("r/p/y/x/y/z= "));
    Serial.print(_manager_ahrs.roll,1);Serial.print(F(","));
    Serial.print(_manager_ahrs.pitch,1);Serial.print(F(","));
    Serial.print(_manager_ahrs.yaw,1);Serial.print(F(","));
    Serial.print(_manager_ahrs.ax,1);Serial.print(F(","));
    Serial.print(_manager_ahrs.ay,1);Serial.print(F(","));
    Serial.println(_manager_ahrs.az,1);

    Serial.print(F("motionAcc= "));
    Serial.println(_motion_acceleration,3);
    //Serial.println(F("++++++++++++++++++++++++++++++++++"));
    _print_count=0;
 }
 _print_count++;

}



void MngAhrs::readAhrsData()
{
  roll=_sbuf[0];
  pitch=_sbuf[1];
  yaw=_sbuf[2];
  ax=_sbuf[3];
  ay=_sbuf[4];
  az=_sbuf[5];

}
bool MngAhrs::isAhrsDataReady()
{
  bool ready;
  ready;

  if(cansatIMU.read(_sbuf,6)){

    ready=true;
  }else{
    ready=false;
  }

  return ready;
}
void MngAhrs::ahrsBegin()
{
  cansatIMU.begin(115200);
}
void MngAhrs::rx_empty()
{
  cansatIMU.rx_empty();
}
