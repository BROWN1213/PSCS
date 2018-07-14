import processing.serial.*;

Serial myPort;  // Create object from Serial class
String rx_datas;
int lf = 10;    // Linefeed in ASCII
void serialSetup(){
  //String portName = Serial.list()[13];
  String portName = "/dev/tty.usbserial-A906EHC5";
  //printArray(Serial.list());
  
  myPort = new Serial(this, portName, 115200);  
}

void serialReceive(){
  
  while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil(lf);;   
    if (inBuffer != null) {
      print(inBuffer);
    }
  }
  
  
}