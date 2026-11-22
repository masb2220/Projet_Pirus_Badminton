#include "Adafruit_TCS34725.h"
#include "Arduino.h"
#include "LibRobus.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

// function declarations here:
char* getColor(void);

//Setup for the TCS34725 color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void)
{
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  
}

void loop()
{
  Serial.println(getColor());
}

char* getColor(void){
    const char* black = "Black";
    const char* blue = "Blue";
    const char* othercolor = "Other";

    uint16_t r, g, b, c, colorTemp, lux;

    tcs.getRawData(&r, &g, &b, &c);
    //colorTemp = tcs.calculateColorTemperature(r, g, b);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);

    //uncomment this section to calibrate colors manually
    /* Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");  */

    if (r < 150 && g < 150 && b < 150) {
        //Serial.println(black);
        return black;
    } else if (b > r && b > g) {
        //Serial.println(blue);
        return blue;
    } else {
        //Serial.println(othercolor);
        return othercolor;
    }
}

