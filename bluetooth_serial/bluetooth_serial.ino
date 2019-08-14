#include <SoftwareSerial.h>
//#include <Servo.h>
//Servo servo;
//int angle;


SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  //servo.attach(8); // servo pin
  //servo.write(78);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);// serial communication
  mySerial.begin(9600);
  //while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  //}


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
    int angle=mySerial.read();//set serial monitor to noline ending othervise it will recognise \n as 0
 //servo.write(angle);// rotation
}
  
//  if (Serial.available()) {
//    Serial.write(Serial.read());
//    angle=Serial.read();//set serial monitor to noline ending othervise it will recognise \n as 0
// servo.write(angle);// rotation
//  }
  //delay(200);// for working properly
}
