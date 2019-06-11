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


bool TskSd::begin(int cs_pin)
{
  _manager_sd_write.beginSdcard(cs_pin);
  _slog_file_name="";
  _sImu_file_name="";
}
bool TskSd::setFileName(String sfilename)
{
    bool result;
    _slog_file_name=sfilename+".csv";
    result=_manager_sd_write.createFile(_slog_file_name);
    _sImu_file_name=sfilename+".imu";
    result&= _manager_sd_write.createFile(_sImu_file_name);
    return result;
}
bool TskSd::setFileName(int32_t date,int32_t times)
{
  bool result;
  String fr,be;

  fr=String(date/100); //ddmmyy  -> ddmm
  while(fr.length()<4)fr="0"+fr;
  be=String(times/100); //hhmmss -> hhmm
  while(be.length()<4)be="0"+be;

  _slog_file_name=fr+be+".csv"; //ddmmhhmm.csv
  result=_manager_sd_write.createFile(_slog_file_name);
  _sImu_file_name=fr+be+".imu";
  result&=_manager_sd_write.createFile(_sImu_file_name);
  return result;

}
bool TskSd::saveData(LogPacket *log_packet)
{
  File fp=_manager_sd_write.fileOpen(_slog_file_name);
  if(fp){
    fp.println(logPacketToString(log_packet));
    _manager_sd_write.fileClose(fp);
    return true;
  }else{
    Serial.println(F("Err WR LOG"));
    return false;
  }

}
bool TskSd::saveDataImu(ImuPacket *imu_packet)
{
  File fp=_manager_sd_write.fileOpen(_sImu_file_name);
  if(fp){
    fp.println(imuPacketToString(imu_packet));
    _manager_sd_write.fileClose(fp);
    return true;
  }else{
    Serial.println(F("Err WR IMUdata"));
    return false;
  }

}
bool TskSd::saveDataImu(ImuPacket *array_imu_packet,uint16_t size)
{
  File fp=_manager_sd_write.fileOpen(_sImu_file_name);
  if(fp){
    for(uint16_t i=0;i<size;i++){
      fp.println(imuPacketToString(array_imu_packet+i) );
    }
     _manager_sd_write.fileClose(fp);
    return true;
  }else{
    Serial.println(F("Err WR IMUdata"));
    return false;
  }

}

String TskSd::logPacketToString(LogPacket *lp){
  String saveData;
  String c=",";
  saveData=String(lp->date)+c+
           String(lp->time)+c+
           String(lp->time_index)+c+
           lp->mode+c+
           String(lp->longitude,7)+c+
           String(lp->latitude,7)+c+
           String(lp->altitude)+c+
           String(lp->numsat)+c+
           String(lp->speed)+c+
           String(lp->cource)+c+
           String(lp->bearing)+c+
           String(lp->distance)+c+
           String(lp->controlangle);
  return(saveData);
}
String TskSd::imuPacketToString(ImuPacket *ip){
  String saveData;
  String c=",";
  saveData=String(ip->gpsTime)+c+
           String(ip->systemTime)+c+
           String(ip->roll,6)+c+
           String(ip->pitch,6)+c+
           String(ip->yaw,6)+c+
           String(ip->ax,6)+c+
           String(ip->ay,6)+c+
           String(ip->az,6)+c+
           String(ip->motion_acceleration,6);
  return(saveData);
}

bool TskSd::saveLabel(String label){
  File fp=_manager_sd_write.fileOpen(_slog_file_name);
  if(fp){
    fp.println(label);
    _manager_sd_write.fileClose(fp);
    return true;
  }else{
    Serial.println(F("Err WR label"));
    return false;
  }

}

bool TskSd::saveLabelImu(String label){
  File fp=_manager_sd_write.fileOpen(_sImu_file_name);
  if(fp){
    fp.println(label);
    _manager_sd_write.fileClose(fp);
    return true;
  }else{
    Serial.println(F("Err WR imulabel"));
    return false;
  }

}
bool MngSdWrite::beginSdcard(int cs_pin)
{
  pinMode(cs_pin, OUTPUT);
  if (!this->begin(cs_pin)) { // If card present and initialized, then proceed:
   Serial.println(F("SD Card failed, or not present"));
   return; // stop process.
  }
  Serial.println(F("SD Card Initialized."));
}
File MngSdWrite::fileOpen(String sfilename)
{
  return this->open(sfilename, FILE_WRITE);
}
void MngSdWrite::fileClose(File fp)
{
  fp.close();
}
/*
void MngSdWrite::writeSd(File fp,String sdata_string){

  //fp.println(sdata_string);
  fp.println("test");

}*/

bool MngSdWrite::createFile(String sfilename){
  File myFile;
/*
    if (SD.exists(sfilename)) {
       Serial.println(sfilename+" exists.");
    } else {
      Serial.println(sfilename+"dn't exist.");
      // open a new file and immediately close it:
*/
      if(myFile = this->open(sfilename, FILE_WRITE)){
        Serial.println(F("Success File creation"));
        myFile.close();
        return true;
      }else{
        Serial.println(F("Fail File creation"));
        //Serial.println("file name must be under 8 characters");
        return false;
      }





}
