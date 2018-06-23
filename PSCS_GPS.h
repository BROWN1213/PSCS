/**
* @file PSCS_GPS.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#ifndef PSCS_GPS_h
#define PSCS_GPS_h

#include "Arduino.h"



struct LocationInfo {
  float alt;                   //Altitude in meters
  float lat;                   //Latitude in hddd.ddddddd
  float lng;                   //Longitude in hddd.ddddddd
  float speed;         ///< ground speed in m/sec
  float course;        //< ground course in degrees
};
class MngGps
{
  public:
    void readGpsData(); //Measure GPS value
    bool isGpsDataReady();
    void gpsBegin();

    float time;
    int32_t date;
    float ground_speed;         ///< ground speed in m/sec
    float ground_course;        //< ground course in degrees
    uint8_t num_satellites;           //< Number of visible satellites
    float alt;                   //Altitude in meters
    float lat;                   //Latitude in hddd.ddddddd
    float lng;                   //Longitude in hddd.ddddddd

  private:

};
class TskGps
{
  public:
    TskGPS();
    void begin();
    void update();
    int32_t getTime();
    int32_t getDate();
    uint8_t getNumOfSatellites();
    LocationInfo getLocation();
    bool isGpsDataNew();
    void printGpsInfo();

  private:
    int32_t _last_gps_time_ms;
    MngGps _manager_gps;
    bool _gps_data_flag;

};


#endif
