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
