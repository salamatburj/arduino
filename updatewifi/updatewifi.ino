int ch_pd;
int io0=2;
void setup() {
  // put your setup code here, to run once:
pinMode(ch_pd,OUTPUT);
pinMode(io0,OUTPUT);
digitalWrite(io0,LOW);
digitalWrite(ch_pd,LOW);
digitalWrite(ch_pd,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
