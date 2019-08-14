
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
int sensorValue=analogRead(A0);

Serial.println(sensorValue);
tone(8,sensorValue*3);
//analogWrite(13, sensorValue/4);
delay(1);
}
