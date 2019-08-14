#include <SoftwareSerial.h>
 
#define DEBUG true

String network="SINGTEL-3152";
String password="0000667986";
 
SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
  
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
esp8266.print("AT+RST\r\n");
delay(1000);
//String c=esp8266.read();
Serial.print(c);
esp8266.print("AT\r\n");
delay(1000);
String c=esp8266.read();
Serial.print(c);
esp8266.print("AT\r\n");
String c=esp8266.read();
Serial.print(c);
 Serial.print(esp8266.read()); 
//  sendData("AT+RST\r\n",2000,DEBUG); // reset module
//  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
//    //esp8266.println("AT+CWJAP="SINGTEL-3152","0000667986");
//  // delay(1000);
//  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
//  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
//  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

}
 
