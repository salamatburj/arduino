#include <uCamII.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX (or tx on the end and rx on the end)

void setup() {
  mySerial.begin(115200);
  Serial.begin(115200);
  Serial.println("trying ..");


  UCAMII camera;
  short x = 0;
  int bytes;

  // unComment the #define cameraDebugSerial in uCamII.cpp 
  // and set the debug output interface below
  camera.setDebug(&mySerial);

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
