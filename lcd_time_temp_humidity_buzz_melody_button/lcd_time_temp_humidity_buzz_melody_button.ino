 #include "Wire.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define DS3231_I2C_ADDRESS 0x68
#include "DHT.h"
#define DHTPIN 12     // what digital pin we're connected to
// for swtiching alarm option ant set time option
int pushButton=2;//pin of the button
int state=0;
int reading;
int previous=LOW;
long time=0;
long debounce=200;
//for setting alarm time
int ahour=0;
int aminute=0;
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
// this part for melody
int speakerPin = 8;

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
// melody done

void setup()
{
  Wire.begin();
  Serial.begin(9600);
   lcd.begin(16,2);
   dht.begin();
   pinMode(speakerPin,OUTPUT);
   pinMode(pushButton, INPUT);
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
    if (hour ==ahour && minute==aminute && (second>0&&second<10)){
     for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
     }
    }
}

void loop()
{
  reading=digitalRead(pushButton);
  if (reading==HIGH && previous==LOW && millis()-time>debounce){
    if(state==0)
    state=1;
    else if (state==1)
    state=2;
    else
    state=0;
    time=millis();
  }
  previous=reading;
  if(state==0){
  displayTime();// display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
  }
  if (state==1){
    lcd.clear();
  lcd.print("SET ALARM ");
  lcd.print(ahour);
  lcd.print(":");
  lcd.print(aminute);
  delay(1000);
  }
  if (state==2){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  lcd.clear();
  lcd.print("SET TIME ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  delay(1000);
  }
  
  
  }

