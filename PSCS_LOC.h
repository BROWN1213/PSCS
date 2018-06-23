/*
* @file PSCS_LOC.h
* @author SeungMin Shin, Haneulbit Kim, Chan Lee
* @license This project is released under the MIT License (MIT)
* @copyright Copyright (c) 2018 Asgardia
* @date June 2018
* @brief ...
*/

#ifndef PSCS_LOC_h
#define PSCS_LOC_h

#include "Arduino.h"





struct GpsCoordinates {
float alt;                   //Altitude in meters
float lat;                   //Latitude in hddd.ddddddd
float lng;                   //Longitude in hddd.ddddddd
};

class AlgLocation
{
  public:
    bool getDistanceBearing(float point1[],float point2[],float results[]); //Calculate Locaion distance
    void getDestination(float point1[],float dest[],float bearing, float dist);
  private:

};

class TskLocation
{
  public:
    TskLocation();
    void begin();
    void setDestination(GpsCoordinates coord);
    GpsCoordinates getDestination();
    void updateCurrentCoord(GpsCoordinates coord);
    void calculateDistanceBearing();
    float getDistance();
    float getBearingAngle();
    bool testVincenty();

  private:
    GpsCoordinates _destination_coord;
    GpsCoordinates _current_coord;
    float _distance;
    float _bearing_angle;
    AlgLocation _alorithm_location;

};




#endif
