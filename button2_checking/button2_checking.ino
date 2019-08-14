//int pushButton=2;
int pushButton2=4;
int minute=0;
int state=0;
//int reading;
int reading2;
int previous=LOW;
long time=0;
long debounce=200;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//pinMode(pushButton, INPUT);
pinMode(pushButton2, INPUT);// second button
}

void loop() {
  reading2 =digitalRead(pushButton2);
  
  
  if (reading2==HIGH && previous==LOW&& millis()-time>debounce){
    if (state==0)
    state=1;
    else if (state==1)
    state=2;
    else
    state=0;
    time=millis();
  }
  previous=reading2;
  
  Serial.print(state);
  
    delay(500);
  }
  
  
  //Serial.println(state);
  //delay(500);
  // put your main code here, to run repeatedly:


