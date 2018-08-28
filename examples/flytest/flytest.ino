#include <CansatSystem.h>
#include <CansatBT.h>
#include<PSCS_System.h>
#include <CmdMessenger.h>  // CmdMessenger


/*
 * Defines
 */
#define TIME_MARGIN 700  //800ms
/*
 * Task Instance
 */

TskFreeFall tskFreeFall;
TskGps tskGps;
TskLocation tskLocation;
TskNavigation tskNavigation;
TskAhrs tskAhrs;
TskSd tskSd;
/*   MUST change CmdMessenger.h for reduce amount of variables
 *   #define MAXCALLBACKS        30   // The maximum number of commands   (default: 50)
 *   #define MESSENGERBUFFERSIZE 64   // The length of the commandbuffer  (default: 64)
 *   #define MAXSTREAMBUFFERSIZE 64  // The length of the streambuffer   (default: 64)#define MAXCALLBACKS        30   // The maximum number of commands   (default: 50)
 *   #define MESSENGERBUFFERSIZE 64   // The length of the commandbuffer  (default: 64)
 *   #define MAXSTREAMBUFFERSIZE 64  // The length of the streambuffer   (default: 64)
 * 
 */
CmdMessenger cmdMessenger = CmdMessenger(Serial);

/*
 *  vatiables
 */


uint32_t log_time_index=0;
uint32_t led_count;
void setup() {
  // put your setup code here, to run once:
  CansatSystemInit();
  LEDsetup(); 
  cansatBT.begin();
  
 
  LEDBlinkAll(1000);
  LEDOffAll();
  // wait until BT is connnected
  led_count=millis();
  while(!cansatBT.isConnected()){
    if( (millis()-led_count)>1000){
      led_count=millis();
      LEDBlink(LED0,200);
    }
    LEDOff(LED0);
  }
  // send welcome messege to base station
  Serial.println(F("BT connected"));
  Serial.println(F("Welcome to Cansat"));  
  LEDBlinkAll(2000);
  
  // begin tasks
  tskFreeFall.begin(D1);
  tskGps.begin();
  tskLocation.begin();
  tskNavigation.begin(D0,winchCallback);
  tskNavigation.setNavigationMode(1); //manual mode
  //tskAhrs.begin(); //Do not begin after tskGps.begin()
  tskSd.begin(SS);
  
  // set Destination (cchamchi's House) for test 
  GpsCoordinates gps_coord;
  //gps_coord.lat=37.287413;
  //gps_coord.lng=127.062302;
  gps_coord.lat=37.289868;
  gps_coord.lng=127.064932;
  gps_coord.alt=82.0;
  tskLocation.setDestination(gps_coord);
  Serial.println(F("Destination GPS set"));
  
  //setup Command Messenger 
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  
  Serial.println(F("Wait GPS signals at least 5 satellites... "));

  while(tskGps.getNumOfSatellites()<5){
    tskGps.update();
  }
  Serial.println(F("GPS Locked"));

  
  tskSd.setFileName(tskGps.getDate(),tskGps.getTime());
  if(!(tskSd.saveLabel(LOG_LABEL)&tskSd.saveLabelImu(IMU_LABEL))){
    Serial.println(F("!!SD problem"));
    while(1){
      LEDOn(LED0);LEDOn(LED1);LEDOff(LED2);LEDOff(LED3);
      delay(1000);
      LEDOff(LED0);LEDOff(LED1);LEDOn(LED2);LEDOn(LED3);
      delay(1000);
    }
  }

  LEDBlinkAll(3000);
}


void reMakeFile(){
  tskSd.begin(SS);
  tskSd.setFileName(tskGps.getDate(),tskGps.getTime());
  if(!(tskSd.saveLabel(LOG_LABEL)&tskSd.saveLabelImu(IMU_LABEL))){
    Serial.println(F("!!SD reMake file"));
  }
  
}


void loop() {
  float controlangle;
  ImuPacket array_imu_datas[10];
  uint16_t imu_datas_offset;
  uint32_t loop_start_time;
  // put your main code here, to run repeatedly:
  tskGps.update();
  /*
   * This loop is triggered by GPS every 1000ms
   * GPS+Location+Navigation+SD takes 40~50ms
   * 
   */
  if(tskGps.isGpsDataNew()){
    // remember the start time, we have to finish in 1 sec 
    // and prepair to listen the next GPS data
    LEDOn(LED1);
    loop_start_time=millis();  

    tskGps.printGpsInfoSimple();
    

    // get current GPS coord
    LocationInfo gps_location=tskGps.getLocation();
    
    //update currrent GPS coord 
    //To calculate distance and bearing from destination
    
    GpsCoordinates gps_coord;
    gps_coord.lat=gps_location.lat;
    gps_coord.lng=gps_location.lng;
    tskLocation.updateCurrentCoord(gps_coord);
    tskLocation.calculateDistanceBearing();

    //Calculate conrtol angle and turn winchservo
    float d_alt=gps_location.alt-tskLocation.getDestination().alt;
    tskNavigation.updateNavigationParamers(tskLocation.getDistance(),tskLocation.getBearingAngle(),gps_location.course,d_alt);
    tskNavigation.updateControlAngle();    
    if(!tskNavigation.getNavigationMode()){ //auto=0 manu =1
      tskNavigation.winchControl(tskNavigation.getControlAngle()); //automode 
    }
    tskNavigation.printNavigationInfo();  

  
    if(!saveGpsData()){
       reMakeFile();
    }
    
    

    /*
     * This loop to use GPS interval 1000ms
     * AHRS give datas with 10Hz(100ms) interval
     * So you can get AHRS data ...
     */
    imu_datas_offset=0; // Must set to zero 
    tskAhrs.begin();
    tskAhrs.rx_empty();
    while((millis()-loop_start_time)<TIME_MARGIN ){ 
      
        tskAhrs.update();
        if(tskAhrs.isAhrsDataNew()){
          LEDOn(LED2);
          //tskAhrs.printAhrsInfo();

          if(imu_datas_offset<10){
            appendImuData(array_imu_datas,imu_datas_offset++);
          }else{
            imu_datas_offset=10;
          }        
        }
        LEDOff(LED2);
        tskNavigation.timerRun();
        cmdMessenger.feedinSerialData();            
    }
    if(!tskSd.saveDataImu(array_imu_datas,imu_datas_offset)){
      reMakeFile();
    }
    tskGps.begin();    
    tskGps.rx_empty();   
    LEDOff(LED1);
    
  }//end if(tskGps.isGpsDataNew())
  
  tskNavigation.timerRun();
  cmdMessenger.feedinSerialData();

}

/*  callback fuction for winch to neutral after certain micro times 
 *  This function must be attached when tskNavigation is begin
 *  Callback for timer must be plain function pointer not be class member
 *  Atter turning the winchServo, the winchCallback be called
 */
void winchCallback(){
  tskNavigation.winchNeutral();
  Serial.println(F("winch callback"));
}

bool saveGpsData(){
  //takes 25ms

  LogPacket log_datas;  
  LocationInfo gpsLocation=tskGps.getLocation();
  
  log_datas.date=tskGps.getDate();
  log_datas.time=tskGps.getTime();
  log_datas.time_index=log_time_index++;
  log_datas.mode=tskNavigation.getNavigationMode();
  log_datas.latitude=gpsLocation.lat;
  log_datas.longitude=gpsLocation.lng;
  log_datas.altitude=gpsLocation.alt;
  log_datas.numsat=tskGps.getNumOfSatellites();
  log_datas.speed=gpsLocation.speed;
  log_datas.cource= gpsLocation.course;
  log_datas.bearing=tskLocation.getBearingAngle();
  log_datas.distance=tskLocation.getDistance();
  log_datas.controlangle=tskNavigation.getControlAngle();
  return(tskSd.saveData(&log_datas)); 

}
void appendImuData(ImuPacket* array_imu_datas,uint16_t offset){
  ImuPacket imu_datas; 
  GyroInfo gyro=tskAhrs.getGyro();
  AcceleroInfo accelero=tskAhrs.getAccelero();

  
  imu_datas.gpsTime=tskGps.getTime();
  imu_datas.systemTime=millis();
  imu_datas.roll=gyro.roll;
  imu_datas.pitch=gyro.pitch;
  imu_datas.yaw=gyro.yaw;
  imu_datas.ax=accelero.ax;
  imu_datas.ay=accelero.ay;
  imu_datas.az=accelero.az;
  imu_datas.motion_acceleration=tskAhrs.getMotionAcceleration();
  *(array_imu_datas+offset)=imu_datas;
  
}

void LEDsetup(){
  pinMode(LED0,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
}

void LEDOn(int pin){
  digitalWrite(pin,LOW);
}
void LEDOff(int pin){
  digitalWrite(pin,HIGH);
}
void LEDOffAll(){
  digitalWrite(LED0,HIGH);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
}
void LEDOnAll(){
  digitalWrite(LED0,LOW);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
}
void LEDBlink(int pin,uint32_t timems){
  uint32_t start_time=millis();
  
  while((millis()-start_time)<timems){
    LEDOn(pin);
    delay(100);
    LEDOff(pin);
    delay(100);
  }
}
void LEDBlinkAll(uint32_t timems){
  uint32_t start_time=millis();
  
  while((millis()-start_time)<timems){
    LEDOnAll();
    delay(100);
    LEDOffAll();
    delay(100);
  }
}


