int state=0;
void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  // put your setup code here, to run once:
Serial.begin(9600);
int state=0;
}

void loop() {
  if (Serial.available()>0){
    state=Serial.parseInt();
    Serial.println(state);
    if (state==1)
    digitalWrite(LED_BUILTIN,HIGH);
    else if (state==0)
    digitalWrite(LED_BUILTIN,LOW);
 
  }
  
  // put your main code here, to run repeatedly:

}
