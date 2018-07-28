#include <CansatSystem.h>
#include <PSCS_System.h>
#include <CansatBT.h>
#include <CmdMessenger.h>  // CmdMessenger

TskGps tskGps;
TskLocation tskLocation;
TskNavigation tskNavigation;
TskAhrs tskAhrs;

CmdMessenger cmdMessenger = CmdMessenger(Serial);

void setup() {
  // put your setup code here, to run once:
  CansatSystemInit(); 
  
  tskGps.begin();
  tskLocation.begin();
  tskNavigation.begin(D0,winchCallback);
  tskNavigation.setNavigationMode(1); //manual mode

  // set Destination (cchamchi's House) for test 
  GpsCoordinates gps_coord;
  //gps_coord.lat=37.287413;
  //gps_coord.lng=127.062302;
  gps_coord.lat=37.289868;
  gps_coord.lng=127.064932;
  gps_coord.alt=68.0;
  tskLocation.setDestination(gps_coord);
  Serial.println(F("Destination GPS set"));
  //setup Command Messenger 
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  Serial.println(F("Wait GPS signals at least 5 satellites... "));

  while(tskGps.getNumOfSatellites()<5){
    tskGps.update();
    //cansatBT.send(tskGps.getNumOfSatellites());
    //delay(2000);
  }
  Serial.println(F("GPS Locked"));
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
    loop_start_time=millis();  
/////////////////////////////////////////////////////////////////////////
    tskGps.printGpsInfo(); //Print Gps Data
/////////////////////////////////////////////////////////////////////////

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

  
    //saveGpsData();
    //Serial.println((float)(millis()-loop_start_time) );
    

    /*
     * This loop to use GPS interval 1000ms
     * AHRS give datas with 10Hz(100ms) interval
     * So you can get AHRS data ...
     */
    imu_datas_offset=0; // Must set to zero 
     
    tskGps.begin();       
    
    }//end if(tskGps.isGpsDataNew())
  
    tskNavigation.timerRun();
    cmdMessenger.feedinSerialData();

}
  void winchCallback(){
  tskNavigation.winchNeutral();
  Serial.println(F("winch callback"));
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
  *(array_imu_datas+offset)=imu_datas;
  
}
