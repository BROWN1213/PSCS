/**
* @file PSCS_GPS.cpp
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#include "PSCS_GPS.h"
#include "CansatGPS.h"

TskGps::TskGPS()
{

}
void TskGps::begin()
{
  _manager_gps.gpsBegin();
  _gps_data_flag=false;
}
void TskGps::update()
{
  if(_manager_gps.isGpsDataReady()){
    _manager_gps.readGpsData();
    _last_gps_time_ms=_manager_gps.time;
     _gps_data_flag=true;
  }
}
int32_t TskGps::getTime()
{
  return((int32_t)_manager_gps.time);
}
int32_t TskGps::getDate()
{
  return(_manager_gps.date);
}
uint8_t TskGps::getNumOfSatellites()
{
  return (_manager_gps.num_satellites);
}
LocationInfo TskGps::getLocation()
{
  LocationInfo location;
  location.alt=_manager_gps.alt;
  location.lat=_manager_gps.lat;
  location.lng=_manager_gps.lng;
  location.speed=_manager_gps.ground_speed;
  location.course=_manager_gps.ground_course;
  return(location);
}

bool TskGps::isGpsDataNew()
{
  if(_gps_data_flag){
    _gps_data_flag=false;
    return(true);
  }else{
    return(false);
  }

}
void TskGps::rx_empty(){
  _manager_gps.rx_empty();
}
void TskGps::printGpsInfo()
{
  Serial.println(F("** GPS Info **"));
  Serial.print(F("date="));
  Serial.println(_manager_gps.date);
  Serial.print(F("time="));
  Serial.println(_manager_gps.time);
  Serial.print(F("lat="));
  Serial.println(_manager_gps.lat,7);
  Serial.print(F("lng="));
  Serial.println(_manager_gps.lng,7);
  Serial.print(F("alt="));
  Serial.println(_manager_gps.alt);
  Serial.print(F("nsat="));
  Serial.println(_manager_gps.num_satellites);
  Serial.print(F("spd="));
  Serial.println(_manager_gps.ground_speed);
  Serial.print(F("crs="));
  Serial.println(_manager_gps.ground_course);

}



void MngGps::readGpsData()
{
  time=cansatGPS.time()/100.;
  date=cansatGPS.date();
  ground_speed=cansatGPS.ground_speed_ms();         ///< ground speed in m/sec
  ground_course=cansatGPS.ground_course();        //< ground course in degrees
  num_satellites=cansatGPS.num_sats();           //< Number of visible satellites
  alt=cansatGPS.location().alt/100.;                   //Altitude in meters
  lat=(cansatGPS.location().lat* 1.0e-7f);                   //Latitude in hddd.ddddddd
  lng=(cansatGPS.location().lng* 1.0e-7f);                   //Longitude in hddd.ddddddd
}
bool MngGps::isGpsDataReady()
{
  bool ready;

  if(cansatGPS.read()){
    ready=true;
  }else {
    ready=false;
  }

  return ready;
}
void MngGps::gpsBegin()
{
  cansatGPS.begin(9600);
}

void MngGps::rx_empty()
{
  cansatGPS.rx_empty();
}
