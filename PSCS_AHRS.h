/**
* @file PSCS_AHRS.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#ifndef PSCS_AHRS_h
#define PSCS_AHRS_h

#include "Arduino.h"



struct AhrsInfo {
  float roll;
  float pitch;
  float yaw;
  float ax;
  float ay;
  float az;
};
struct GyroInfo {
  float roll;
  float pitch;
  float yaw;
};
struct AcceleroInfo {
  float ax;
  float ay;
  float az;
};
class MngAhrs
{
  public:
    void readAhrsData(); //Measure GPS value
    bool isAhrsDataReady();
    void ahrsBegin();
    void rx_empty();

    float roll;
    float pitch;
    float yaw;
    float ax;
    float ay;
    float az;

  private:
    float _sbuf[6];
};
class TskAhrs
{
  public:
    void begin();
    void update();
    GyroInfo getGyro();
    AcceleroInfo getAccelero();
    float getMotionAcceleration();
    void caculrateMotionAcceleration();

    bool isAhrsDataNew();
    void printAhrsInfo();
    void rx_empty();

  private:

    MngAhrs _manager_ahrs;
    bool _ahrs_data_flag;
    float _motion_acceleration;
    int _print_count;

};


#endif
