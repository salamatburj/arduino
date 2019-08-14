
#include "Wire.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
int pushButton=2;//pin of the button to set time and hour
int pushButton2=4;
int state=0;
int reading;// for the first button
int reading2;//for the second button
int previous=LOW;
long time=0;
long debounce=200;//debounce
int shour=0;// set time and minute
int sminute=0;
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
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,13,22,1,23,10,16);
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
   //sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

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
  //Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  //Serial.print(":");
  lcd.print(":");
  if (minute<10)
  {
    //Serial.print("0");
    lcd.print("0");
  }
  lcd.print(minute);
  //Serial.print(minute, DEC);
  //Serial.print(":");
  lcd.print(":");
  if (second<10)
  {
    //Serial.print("0");
    lcd.print("0");
  }
  lcd.print(second);
  //Serial.print(second, DEC);
  //Serial.print(" ");
  lcd.print(" ");
  //Serial.print(dayOfMonth, DEC);
  lcd.setCursor(0,1);
  lcd.print(dayOfMonth);
  //Serial.print("/");
  lcd.print("/");
  lcd.print(month);
  //Serial.print(month, DEC);
  //Serial.print("/");
  lcd.print("/");
  //Serial.print(year, DEC);
  lcd.print(year);
  //Serial.print(" Day of week: ");
  lcd.print(" ");
  switch(dayOfWeek){
  case 1:
    //Serial.println("Sunday");
    lcd.print("Sunday");
    break;
  case 2:
    //Serial.println("Monday");
    lcd.print("Monday");
    break;
  case 3:
    //Serial.println("Tuesday");
    lcd.print("Tuesday");
    break;
  case 4:
    //Serial.println("Wednesday");
    lcd.print("Wednesday");
    break;
  case 5:
    //Serial.println("Thursday");
     lcd.print("Thursday");
    break;
  case 6:
    //Serial.println("Friday");
    lcd.print("Friday");
    break;
  case 7:
    lcd.print("Saturday");
    //Serial.println("Saturday");
    break;
  }
   shour=hour;
  sminute=minute;
}

void loop()
{
    reading=digitalRead(pushButton);
  reading2=digitalRead(pushButton2);
  if (reading==HIGH && reading2==HIGH && previous==LOW && millis()-time>debounce){
    if(state==0)
    state=1;
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
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
   //retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
   reading=digitalRead(pushButton);
  reading2=digitalRead(pushButton2);
//shour=hour;
//sminute=minute;
        lcd.clear();
  lcd.print("SET TIME ");
  lcd.print(shour);
  lcd.print(":");
  lcd.print(sminute);

     if (reading2==HIGH&&sminute<59)
    sminute=sminute+1;
    else if (reading2==HIGH&&sminute==59)
    sminute=0;
    else if (reading==HIGH&&shour<23)
    shour=shour+1;
    else if (reading==HIGH&&shour==23)
    shour=0;
    //time=millis();
//    Serial.print("real");
//    Serial.print(hour,DEC);
//    Serial.print(":");
//    Serial.print(minute,DEC);
//    Serial.print(" ");
//    Serial.print("Set time ");
//    Serial.print(shour);
//    Serial.print(sminute); 
//    Serial.print(" ");
   if (shour==hour && sminute==minute){
    Serial.print("true");
   }
   else{
   Serial.print("false");}
setDS3231time(0,sminute,shour,dayOfWeek, dayOfMonth, month,
  year);
  delay(500);
  
  }
  
  
  
}
