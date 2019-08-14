int pushButton=4;
int state=0;
int reading;
int previous=LOW;
long time=0;
long debounce=200;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(pushButton, INPUT);
}

void loop() {
  reading =digitalRead(pushButton);
  if (reading==HIGH && previous==LOW&& millis()-time>debounce){
    if (state==0)
    state=1;
    else if (state==1)
    state=2;
    else
    state=0;
    time=millis();
  }
  
  previous=reading;
  Serial.println(state);
  delay(500);
  // put your main code here, to run repeatedly:

}
