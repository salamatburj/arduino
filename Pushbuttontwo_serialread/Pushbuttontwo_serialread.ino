int pushButton=2;
int pushButton2=4;
int minute=10;
int hour=0;
int state=0;
int reading;
int reading2;
int previous=LOW;
long time=0;
long debounce=200;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(pushButton, INPUT);
pinMode(pushButton2, INPUT);// second button
}

void loop() {
  reading =digitalRead(pushButton);
  reading2 = digitalRead(pushButton2);
  
  if (reading==HIGH &&reading2==HIGH && previous==LOW&& millis()-time>debounce){
    if (state==0)
    state=1;
    else if (state==1)
    state=2;
    else
    state=0;
    time=millis();
  }
  previous=reading;
  if (state==1 ){
    reading=digitalRead(pushButton);
    reading2=digitalRead(pushButton2);
    if (reading2==HIGH&&minute<60)
    minute=minute+1;
    else if (minute==60)
    minute=0;
    else if (reading==HIGH&&hour<24&&millis()-time<debounce)
    hour=hour+1;
    else if (hour==24)
    hour=0;
  
    
  }
  //Serial.print(state);
 // Serial.print("m ");
Serial.print(hour);
Serial.print(":"); 
    Serial.print(minute);
    Serial.print(" ");
    delay(100);
  }
  
  
  //Serial.println(state);
  //delay(500);
  // put your main code here, to run repeatedly:


