#define LED_PIN 13
#define BUTTON_PIN 2
boolean trigger=false;
boolean previous=false;
void setup() {
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  // put your setup code here, to run once:

}

void loop() {
  int current=digitalRead(BUTTON_PIN);
  if (current && !previous)
  {
    trigger=!trigger;
  }
  previous=current;
  // put your main code here, to run repeatedly:
digitalWrite(LED_PIN, trigger);

}
