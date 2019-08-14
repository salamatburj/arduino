#include <Servo.h>;
Servo servo;
int angle=0;

void setup() {
  // put your setup code here, to run once:
servo.attach(8);
Serial.begin(9600);
servo.write(78);// inital position. Supposed to be 90 but probably due to the poor connection
}

void loop() {
  // put your main code here, to run repeatedly:
  //angle=servo.read();
Serial.println(servo.read());
if (Serial.available()){
  angle=Serial.parseInt();
servo.write(angle);
}
delay(100);
}
