
#include <Wire.h>
#include <SeeedOLED.h>
#include <TH02_dev.h>
#include "Arduino.h"

void setup()
{
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        //digital pin 8, LED glow indicates Film properly Connected .
  PORTB |= 0x21;
//hello
  TH02.begin(); //Begin Temperature sensor
  
  Serial.begin(9600);        // start serial for output
  
  SeeedOled.clearDisplay(); // Completely clear OLED

}

void loop()
{
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("IAMGROOT");
  
  float temper = TH02.ReadTemperature(); 
  Serial.println("Temperature: ");   
  Serial.print(temper);
  Serial.println("C\r\n");
   
  float humidity = TH02.ReadHumidity();
  Serial.println("Humidity: ");
  Serial.print(humidity);
  Serial.println("%\r\n");
  
  SeeedOled.setTextXY(1,0);          //Set the cursor to Xth Page, Yth Column 
  SeeedOled.putString("Temp: "); 
  SeeedOled.putFloat(temper); //Print the String
  SeeedOled.putString(" C"); 
  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Humi: ");  
  SeeedOled.putFloat(humidity); //Print the String
  SeeedOled.putString(" %");
  
  delay(1000);
}

