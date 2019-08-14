int pushButton=2;
int pushButton2=4;

int state=0;
int state2=0;

int reading;
int reading2;

int previous=LOW;
int previous2=LOW;

long time=0;
//long time2=0;
long debounce=200;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(pushButton, INPUT);
}

void loop() {
  reading =digitalRead(pushButton);
  reading2=digitalRead(pushButton2);
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
  if (reading2==HIGH && previous2==LOW&& millis()-time>debounce){
    if (state2==0)
    state2=1;
    else if (state2==1)
    state2=2;
    else
    state2=0;
    time=millis();
  }
  previous2=reading2;
   if (reading==HIGH&&reading2==HIGH &&previous==LOW&& previous2==LOW&& millis()-time>debounce){
   state=0;
    state2=0;
    time=millis();
  }
  previous=reading;
  previous2=reading2;
  
  Serial.print(state);
  Serial.print(" ");
  Serial.println(state2);
  delay(500);
  // put your main code here, to run repeatedly:

}
