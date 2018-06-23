
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
#include <SPI.h>
#include <SD.h>


class MngSdWrite
{
  public:
    bool begin(int cs_pin);
    File fileOpen(String sfilename);
    void fileClose(File fp);
  private:
    File _file_handle;
    String _file_name;
};
class TskSd
{
  public:
    bool beginSdcard(int cs_pin);
    void setFileName(String sfilename);
    bool saveData(String sdata_string);
  private:
    String _slog_file_name;
    MngSdWrite _manager_sd_write;
//to be going to add:ACC, falling point, parachute opening point
};


#endif
