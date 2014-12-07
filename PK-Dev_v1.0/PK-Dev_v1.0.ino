/////////////////////////////////
// PK-Dev_v1.0
// Sensor and Devices Utilized:
//   (1) OLED
//   (2) T and RH
//   (3) Dust
/////////////////////////////////

#include <Wire.h>
#include <SeeedOLED.h>
#include <TH02_dev.h>
#include "Arduino.h"

int pin = 2;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000; //sampe 1s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);            // start serial for output
  TH02.begin();                 //Begin Temperature sensor
  pinMode(pin,INPUT);           //Set pin for input ie. Dust Sensor
  SeeedOled.init();              //initialze SEEED OLED display
      SeeedOled.clearDisplay();        // Completely clear OLED
      SeeedOled.setNormalDisplay();    //Set display to normal mode (i.e non-inverse mode)
      SeeedOled.setHorizontalMode();
  
  //Print out the column titles
  Serial.print("Temp(C) RH(%) LPO Ratio Conc(#)");
  Serial.println();
}

void loop()
{
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  
  if ((millis()-starttime) > sampletime_ms);
  {
    float temper = TH02.ReadTemperature(); 
    float humidity = TH02.ReadHumidity();
    
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    
        Serial.print(temper);
        Serial.print(" ");
        Serial.print(humidity);
        Serial.print(" ");
        Serial.print(lowpulseoccupancy);
        Serial.print(" ");
        Serial.print(ratio);
        Serial.print(" ");
        Serial.print(concentration);
        Serial.println(" ");
    
    SeeedOled.setTextXY(0,0);           //Set the cursor to Xth Page, Yth Column 
        SeeedOled.putString("Temp: "); 
        SeeedOled.putFloat(temper);     //Print the String
        SeeedOled.putString(" C"); 
    
    SeeedOled.setTextXY(1,0);           //Set the cursor to Xth Page, Yth Column
        SeeedOled.putString("Humi: "); SeeedOled.putFloat(humidity); SeeedOled.putString(" %");  //Print the String
        
      
    SeeedOled.setTextXY(2,0);           //Set the cursor to Xth Page, Yth Column
        SeeedOled.putString("Conc: ");  
        SeeedOled.putFloat(concentration,2); //Print the String
    
    SeeedOled.putString(" #");
    
    lowpulseoccupancy = 0;
    starttime = millis();
  } //End of if statement
}//End of loop

