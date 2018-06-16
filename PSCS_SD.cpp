/**
* @file PSCS_SD.cpp
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*
*/
#include "PSCS_SD.h"
#include "Arduino.h"

tsk_SD::tsk_SD()
{

}


void tsk_SD::GPS(float GPSinfo)
{
  Serial.println("GPS");
  Serial.println("Location");
  Serial.println(GPSinfo);

}

void tsk_SD::SaveDataOrder()
{



}


mng_SD::mng_SD()
{}


void mng_SD::manager(int order)
{}


void mng_SD::Savedata()
{}
