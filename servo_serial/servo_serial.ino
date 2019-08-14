// this program is used to control servo from potentiometer and serial monitor.
// 
#include <Servo.h>

Servo servo;

int angle;
int inangle;// initial angle
int angle1;
void setup() {
  Serial.begin(9600);
  servo.attach(8);// servo pin
  // put your setup code here, to run once:
  //angle1=map(analogRead(A1),0,1023,0,180);
}

void loop() {
//  inangle=map(analogRead(A1),0,1023,0,180);// read potentiometer value and converts it to degree
//  if (abs(angle1-inangle)>3){// if there is any changes it starts to work, 3 degree is taken to avoid noise level
//  angle1=analogRead(A1);// read from A1 from potentiometer
//  angle1=map(angle1,0,1023,0,180);// change it to angles
//  angle=angle1;
//  }

if (Serial.available()>0){
    angle=Serial.parseInt();//set serial monitor to noline ending othervise it will recognise \n as 0
}
 servo.write(angle);// rotation
  delay(15);// for working properly
  Serial.println(angle);

  
  // put your main code here, to run repeatedly:

}
