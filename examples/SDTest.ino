#include <CansatSystem.h>
#include <PSCS_System.h>

TskSd tskSd;

void setup() {
  // put your setup code here, to run once:
  uint32_t start_time;
  Serial.begin(115200);
  CansatSystemInit();
  tskSd.begin(SS);

  start_time=millis();
  tskSd.setFileName("Logtest");
  Serial.println(millis()-start_time);
  tskSd.saveLabel(LOG_LABEL);
  millis();
  saveGpsData();
  
 }

void loop() {
  // put your main code here, to run repeatedly:

}

void saveGpsData(){
  //takes 25ms
  LogPacket log_datas;  
  
  log_datas.date=60718;
  log_datas.time=201655;
  log_datas.time_index=3;
  log_datas.mode='0';
  log_datas.latitude=38.234234;
  log_datas.longitude=127.123123;
  log_datas.altitude=106.2;
  log_datas.numsat=12;
  log_datas.speed=10;
  log_datas.cource=40;
  log_datas.bearing=-130;
  log_datas.distance=200;
  log_datas.controlangle=30;
  tskSd.saveData(&log_datas); 

}
