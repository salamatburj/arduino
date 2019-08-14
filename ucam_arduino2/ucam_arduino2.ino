#include <uCamII.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SdFat.h>
#include <Wire.h>
//#include "RTClib.h"



//#define BUZZER          5
#define CAM_ON          7
#define chipSelect      10

SoftwareSerial mySerial(3, 2); // RX, TX (or tx on the end and rx on the end)
UCAMII camera;
SdFat SD1;
//RTC_DS1307 RTC;
int y=0; //time stamp constants year to secod
int m=0;
int d=0;
int h=0;
int w=0, x=0, v=0;
int mi=0;
int s=0;

void setup() {
  Serial.begin(115200);
  mySerial.begin(57600);
  mySerial.println("trying ..");
  Wire.begin();
  //RTC.begin();
//  pinMode(BUZZER, OUTPUT); // buzzer on/off
  //pinMode(CAM_ON, OUTPUT);  //camera on/off
//  digitalWrite(BUZZER, LOW);       // turn off the buzzer
  digitalWrite(CAM_ON, LOW);       // turn off the camera

  if(!SD1.begin(chipSelect))           // initialize SD 
  Serial.println("Failed to initialize SD1!");
  // unComment the #define cameraDebugSerial in uCamII.cpp 
  // and set the debug output interface below
  // camera.setDebug(&mySerial);
}

void loop() {
    digitalWrite(CAM_ON, HIGH);       // turn on the camera
    delay(100);
    if (camera.init()) {
    //delay(100);
    camera.takePicture();
    Serial.print("Image size: ");
    Serial.println(camera.imageSize, DEC);
    Serial.print("number of packages: ");
    Serial.println(camera.numberOfPackages(), DEC);

    //DateTime now=RTC.now();       
    //m=now.month(),DEC;
    //d=now.day(),DEC;
    //h=now.hour(),DEC;
    //mi=now.minute(),DEC;
    //s=now.second(),DEC;

   digitalWrite(chipSelect, LOW); // select SD card
  
  // Create an image with the name IMAGExx.JPG
  char filename[14];
  strcpy(filename, "00000000.JPG");
  
    filename[0] = '0'+d/10;
    filename[1] = '0'+d%10;
    filename[2] = '0'+h/10;
    filename[3] = '0'+h%10;
    filename[4] = '0'+mi/10;
    filename[5] = '0'+mi%10;
    filename[6] = '0'+s/10;
    filename[7] = '0'+s%10;
       
    File imgFile = SD1.open(filename, FILE_WRITE);
    short x = 0;
    int bytes;
    Serial.println("Capturing the image");
    while ( bytes = camera.getData() ) {
      imgFile.write(camera.imgBuffer, bytes);      
    }
    imgFile.close();
    delay(10);
    digitalWrite(chipSelect,HIGH);
    Serial.println("done downloading"); 
    digitalWrite(CAM_ON, LOW);       // turn off the camera}

    }
}

