//const int ledPin = 13; //led attached to this pin
const int buttonPin = 4; //push button attached to this pin
 
int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
int ledState = -1; //this variable tracks the state of the LED, negative if off, positive if on
 
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  //sample the state of the button - is it pressed or not?
  buttonState = digitalRead(buttonPin);
 
  //filter out any noise by setting a time buffer
  if ( (millis() - lastDebounceTime) > debounceDelay) {
 
    //if the button has been pressed, lets toggle the LED from "off to on" or "on to off"
    if ( (buttonState == HIGH) && (ledState < 0) ) {
 
      digitalWrite(LED_BUILTIN, HIGH); //turn LED on
      ledState = -ledState; //now the LED is on, we need to change the state
      lastDebounceTime = millis(); //set the current time
    }
    else if ( (buttonState == HIGH) && (ledState > 0) ) {
 
      digitalWrite(LED_BUILTIN, LOW); //turn LED off
      ledState = -ledState; //now the LED is off, we need to change the state
      lastDebounceTime = millis(); //set the current time
    }//close if/else
 
  }//close if(time buffer)
 
}//close void loop

