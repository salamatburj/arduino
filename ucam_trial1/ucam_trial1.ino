#include <uCamII.h>
//#include <SoftwareSerial.h>


void setup() {

  Serial.begin(57600);
  Serial.println("trying ..");


  UCAMII camera;
  short x = 0;
  int bytes;

  // unComment the #define cameraDebugSerial in uCamII.cpp 
  // and set the debug output interface below
//  camera.setDebug(&mySerial);
camera.init()=1;
  if (camera.init()) {
    camera.takePicture();
    Serial.print("Image size: ");
    Serial.println(camera.imageSize, DEC);
    Serial.print("number of packages: ");
    Serial.println(camera.numberOfPackages(), DEC);

    while ( bytes = camera.getData() ) {
      for (x = 0; x < bytes; x++) {
        Serial.print("0x");
        Serial.print(camera.imgBuffer[x], HEX);
        Serial.print(" ");
      }
      Serial.println("");
    }
    Serial.println("done downloading");

  }
}

void loop() {
  // put your main code here, to run repeatedly:


}
