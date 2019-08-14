/*  
    Arduino with PIR motion sensor
    For complete project details, visit: http://RandomNerdTutorials.com/pirsensor
    Modified by Rui Santos based on PIR sensor by Limor Fried
    Further modified by Salamat Burzhuev(buzzer is added)
*/
 
int led = 13;                // the pin that the LED is atteched to(13 builtin sensor)
int sensor = 7;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)
int speakerPin=8;            // buzzer pin

void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
  pinMode(speakerPin,OUTPUT); // initialize buzzer
}

void loop(){
  val = digitalRead(sensor);   // read sensor value
  //Serial.println(val);
  //delay(100);
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    tone(speakerPin, 3000);
    delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
        tone(speakerPin,3000);
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      noTone(speakerPin);
      delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
        noTone(speakerPin);
    }
  }
}
