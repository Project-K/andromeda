
#include <Wire.h>
#include <SeeedOLED.h>
#include <TH02_dev.h>
#include "Arduino.h"


int pin = 2;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

void setup()
{
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        //digital pin 8, LED glow indicates Film properly Connected .
  PORTB |= 0x21;
  TH02.begin(); //Begin Temperature sensor
  
  Serial.begin(9600);        // start serial for output
  SeeedOled.clearDisplay(); // Completely clear OLED
  
  //Dust Sensor
  pinMode(2,INPUT);
  starttime = millis();//get the current time;

}

void loop()
{
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("IAMGROOT");
  
  float temper = TH02.ReadTemperature(); 
  float humidity = TH02.ReadHumidity();
  
  Serial.print("Temp: ");  
  Serial.print(temper);
  Serial.print(" C");
  Serial.println();
  Serial.print("Humi: ");
  Serial.print(humidity);
  Serial.print(" %");
   Serial.println(); 
  
  SeeedOled.setTextXY(1,0);          //Set the cursor to Xth Page, Yth Column 
  SeeedOled.putString("Temp: "); 
  SeeedOled.putFloat(temper); //Print the String
  SeeedOled.putString(" C"); 
  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Humi: ");  
  SeeedOled.putFloat(humidity); //Print the String
  SeeedOled.putString(" %");
  
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("LPO: ");
    Serial.print(lowpulseoccupancy);
    Serial.println();
    Serial.print("Ratio: ");
    Serial.print(ratio);
    Serial.println();
    Serial.print("Conc: ");
    Serial.print(concentration);
    Serial.println();
    
    SeeedOled.setTextXY(3,0);          //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("LPO: ");  
    SeeedOled.putFloat(lowpulseoccupancy); //Print the String

    SeeedOled.setTextXY(4,0);          //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("Ratio: ");  
    SeeedOled.putFloat(ratio); //Print the String
    
    SeeedOled.setTextXY(5,0);          //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("Conc: ");  
    SeeedOled.putFloat(concentration); //Print the String
    
    lowpulseoccupancy = 0;
    starttime = millis();
    

    
  }
  Serial.println("************");
  delay(1000);
}

