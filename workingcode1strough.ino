#include <RTCTimedEvent.h>
#include <Wire.h> //needed for I2C
#include "RTClib.h"
#include <LiquidCrystal.h>

RTC_DS1307 RTC;

//pins used in LCD screen
LiquidCrystal lcd(8,9,4,5,6,7); 
//sensors& readings
int lightsensorpin = A1; //analog pin #1. Defines light sensing diode
int lightReading;  //defined later as reading of light sensor
int watersensorpin = A3; // analong pin #3. Defines water sensing (fsr)
int waterReading;  //defined later as reading of water sensor
//relays
int lightsRelay = 12; // digital pin #2. Defines lights relay
int pumpRelay = 2; // digital pin #12. Defines pump relay



void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  lcd.begin(16,2);
  pinMode(lightsRelay,OUTPUT);
  pinMode(pumpRelay,OUTPUT);
  digitalWrite(pumpRelay,LOW);
  digitalWrite(lightsRelay,HIGH);
 
   
   if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  
  //initial buffer for 3 timers
  RTCTimedEvent.initialCapacity = sizeof(RTCTimerInformation)*18;

  //Pump Alarm 1 ON- 8:00AM
  RTCTimedEvent.addTimer(0,         //minute
                         8,         //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  
  //Pump Alarm 1 OFF- 8:03AM
  RTCTimedEvent.addTimer(3,         //minute
                         8, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  
  //Pump Alarm 2 ON - 11:00AM
  RTCTimedEvent.addTimer(0, //minute
                         11, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
                         
  //Pump Alarm 2 OFF - 11:03AM                       
  RTCTimedEvent.addTimer(3, //minute
                         11, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 3 ON - 2:00PM                       
  RTCTimedEvent.addTimer(0, //minute
                         14, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  //Pump Alarm 3 OFF - 2:03PM                       
  RTCTimedEvent.addTimer(3, //minute
                         14, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 4 ON  - 5:00PM                       
  RTCTimedEvent.addTimer(0, //minute
                         17, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  //Pump Alarm 4 OFF - 5:03PM                       
  RTCTimedEvent.addTimer(3, //minute
                         17, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 5 ON - 8:00PM                       
  RTCTimedEvent.addTimer(0, //minute
                         20, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  //Pump Alarm 5 OFF - 8:03PM
  RTCTimedEvent.addTimer(3, //minute
                         20, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 6 ON - 11:00PM                   
  RTCTimedEvent.addTimer(0, //minute
                         23, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
                         
  //Pump Alarm 6 OFF - 11:03PM                       
  RTCTimedEvent.addTimer(3, //minute
                         23, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 7 ON - 2:00AM                       
  RTCTimedEvent.addTimer(0, //minute
                         2, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  //Pump Alarm 7 OFF - 2:03AM                       
  RTCTimedEvent.addTimer(3, //minute
                         2, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Pump Alarm 8 ON - 5:030M                       
  RTCTimedEvent.addTimer(0, //minute
                         5, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpCall);
  //Pump Alarm 8 OFF - 5:03AM                       
  RTCTimedEvent.addTimer(3, //minute
                         5, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         pumpOffCall);
  //Light Alarm ON                       
  RTCTimedEvent.addTimer(0, //minute
                         9, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         lightCall);
  //Light Alarm OFF                       
  RTCTimedEvent.addTimer(0, //minute
                         17, //hour
                         TIMER_ANY, //day fo week
                         TIMER_ANY, //day
                         TIMER_ANY, //month
                         lightOffCall);
}

void loop() {
  RTCTimedEvent.loop();
  DateTime now = RTC.now();
    Serial.print("DATE- ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print("TIME- ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
 
}

void pumpCall(RTCTimerInformation* Sender) {
  digitalWrite(pumpRelay, HIGH); //turn pump on
  Serial.println("Pump is running.");//print pump is running to serial
  lcd.setCursor(0, 1); 
  lcd.print("PUMP ACTIVE");//print pump is on to LCD
  lcd.setCursor(0, 1);
  delay(10000);
  lcd.clear();  
}

void pumpOffCall(RTCTimerInformation* Sender) {
  
  digitalWrite(pumpRelay, LOW); //turn pump on
  Serial.println("Pump is not running.");//print pump isnt running to serial
  lcd.setCursor(0, 1); 
  lcd.print("PUMP INACTIVE");//print pump is off to LCD
  lcd.setCursor(0, 1);
  delay(10000);
  lcd.clear(); 
}

void lightCall(RTCTimerInformation* Sender) {
  
  digitalWrite(lightsRelay, HIGH); //turn lights on
  Serial.println("Lights turning on.");
  lcd.setCursor(0, 1);
  lcd.print("LIGHTS ACTIVE");
  delay(10000);
  lcd.setCursor(0, 1);
  lcd.clear();
}

void lightOffCall(RTCTimerInformation* Sender) {
  
  digitalWrite(lightsRelay, LOW); //turn lights on
  Serial.println("Lights turning off.");
  lcd.setCursor(0, 1);
  lcd.print("LIGHTS INACTIVE");
  delay(10000);
  lcd.setCursor(0, 1);
  lcd.clear();
}

