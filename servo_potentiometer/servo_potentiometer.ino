#include <Servo.h>

Servo servo;

int angle;
void setup() {
  Serial.begin(9600);
  servo.attach(8);// servo pin
  // put your setup code here, to run once:

}

void loop() {
  angle=analogRead(A1);// read from A1 from potentiometer
  angle=map(angle,0,1023,0,180);// change it to angles
  servo.write(angle);// rotation
  delay(15);// for working properly
  Serial.println(angle);
  
  // put your main code here, to run repeatedly:

}
