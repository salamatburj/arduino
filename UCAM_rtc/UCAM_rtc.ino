#include <uCamII.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SdFat.h>
#include <Wire.h>

#define DS3231_I2C_ADDRESS 0x68 // rtc adress

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

#define chipSelect      53


UCAMII camera;
SdFat SD1;

int y=0; //time stamp constants year to secod
int m=0;
int d=0;
int h=0;
int w=0, x=0, v=0;
int mi=0;
int s=0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(57600);
  Serial.println("trying ..");
  Wire.begin();


  if(!SD1.begin(chipSelect))           // initialize SD 
  Serial.println("Failed to initialize SD1!");
  // unComment the #define cameraDebugSerial in uCamII.cpp 
  // and set the debug output interface below
  // camera.setDebug(&mySerial);
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
//  lcd.clear();
//  lcd.print(hour);
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");

  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");

  if (second<10)
  {
    Serial.print("0");

  }
  Serial.print(second, DEC);
  Serial.print(" ");

  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  
  Serial.print(year, DEC);
  
  Serial.print(" Day of week: ");
  
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    
    break;
  case 2:
    Serial.println("Monday");
    
    break;
  case 3:
    Serial.println("Tuesday");
    
    break;
  case 4:
    Serial.println("Wednesday");
    
    break;
  case 5:
    Serial.println("Thursday");
  
    break;
  case 6:
    Serial.println("Friday");
    
    break;
  case 7:
    Serial.println("Saturday");
   
    break;
  }
}
void loop() {
    
    delay(1000);
    if (camera.init()) {
    delay(100);
    camera.takePicture();
    Serial.print("Image size: ");
    Serial.println(camera.imageSize, DEC);
    Serial.print("number of packages: ");
    Serial.println(camera.numberOfPackages(), DEC);

//    DateTime now=RTC.now();       
//    m=now.month(),DEC;
//    d=now.day(),DEC;
//    h=now.hour(),DEC;
//    mi=now.minute(),DEC;
//    s=now.second(),DEC;
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  s=second;
  mi=minute;
  h=hour;
  d=dayOfMonth;
  m=month;
  

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
//    digitalWrite(CAM_ON, LOW);       // turn off the camera}

    }
}
