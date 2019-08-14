#include "Wire.h"
int led = 13;                // the pin that the LED is atteched to(13 builtin sensor)
int sensor = 7;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define DS3231_I2C_ADDRESS 0x68
#include "DHT.h"
#define DHTPIN 12     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
   lcd.begin(16,2);
   lcd.setBacklightPin(3,POSITIVE);
   lcd.setBacklight(HIGH);
   dht.begin();

   pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
  //pinMode(speakerPin,OUTPUT); // initialize buzzer
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,13,22,1,23,10,16);
}

//void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
//dayOfMonth, byte month, byte year)
//{
  // sets time and date data to DS3231
  //Wire.beginTransmission(DS3231_I2C_ADDRESS);
  //Wire.write(0); // set next input to start at the seconds register
  //Wire.write(decToBcd(second)); // set seconds
  //Wire.write(decToBcd(minute)); // set minutes
  //Wire.write(decToBcd(hour)); // set hours
  //Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  //Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  //Wire.write(decToBcd(month)); // set month
  //Wire.write(decToBcd(year)); // set year (0 to 99)
  //Wire.endTransmission();
//}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  lcd.clear();
  lcd.print(hour);
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  lcd.print(":");
  if (minute<10)
  {
    Serial.print("0");
    lcd.print("0");
  }
  lcd.print(minute);
  Serial.print(minute, DEC);
  Serial.print(":");
  lcd.print(":");
  if (second<10)
  {
    Serial.print("0");
    lcd.print("0");
  }
  lcd.print(second);
  Serial.print(second, DEC);
  Serial.print(" ");
  lcd.print(" ");
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  lcd.print(h,0);
  Serial.print(h);
  lcd.print("%");
  Serial.print(" %\t");
  lcd.print(" ");
  Serial.print("Temperature: ");
  Serial.print(t);
  
  lcd.print(t,0);
  lcd.print("C");
  
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
//  Serial.print(hic);
  //Serial.print(" *C ");
  //Serial.print(hif);
  //Serial.println(" *F");
  
  Serial.print(dayOfMonth, DEC);
  lcd.setCursor(0,1);
  lcd.print(dayOfMonth);
  Serial.print("/");
  lcd.print("/");
  lcd.print(month);
  Serial.print(month, DEC);
  Serial.print("/");
  lcd.print("/");
  Serial.print(year, DEC);
  lcd.print(year);
  Serial.print(" Day of week: ");
  lcd.print(" ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    lcd.print("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    lcd.print("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    lcd.print("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    lcd.print("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
     lcd.print("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    lcd.print("Friday");
    break;
  case 7:
    lcd.print("Saturday");
    Serial.println("Saturday");
    break;
  }
}

void loop()
{
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    lcd.setBacklight(HIGH);
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
if (state == LOW) {
      Serial.println("Motion detected!"); 
              state = HIGH;       // update variable state to HIGH
       //displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(200); // every second
    }
      else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(1000);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        //lcd.clear();
        lcd.setBacklight(LOW); //
        state = LOW;       // update variable state to LOW
       
    }
  }
}
   
