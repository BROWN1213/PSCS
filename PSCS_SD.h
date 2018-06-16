
/**
* @file PSCS_SD.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*
*/
#ifndef PSCS_SD_h
#define PSCS_SD_h


#include "Arduino.h"

class tsk_SD
{
  public:
  tsk_SD();
    void GPS(float GPSinfo);
    void SaveDataOrder();
  private:
    float _GPSinfo;
//to be going to add:ACC, falling point, parachute opening point
};

class mng_SD
{
  public:
    mng_SD();
    void manager(int order);
    void Savedata();
  private:
    int _order;
};
#endif
