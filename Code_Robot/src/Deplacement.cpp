#include "Adafruit_TCS34725.h"
#include "Arduino.h"
#include "LibRobus.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

Adafruit_MPU6050 mpu;

// variable gyro
constexpr int calibrationSamples = 500;
constexpr float radiansToDegrees = 57.2957795f;

float gyroZBias = 0.0f;
float headingDegrees = 0.0f;
unsigned long previousMicros = 0;
unsigned long previousPrintMillis = 0;

// variable couleur
Adafruit_TCS34725 tcs;

int getHeading(void);

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }

    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050. Check the wiring.");
        while (true)
        {
            delay(10);
        }
    }
    if (tcs.begin())
    {
        Serial.println("Found sensor");
    }
    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
    }
    
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Keep the robot still: calibrating gyro...");
    for (int i = 0; i < calibrationSamples; ++i)
    {
        sensors_event_t acceleration, gyro, temperature;
        mpu.getEvent(&acceleration, &gyro, &temperature);
        gyroZBias += gyro.gyro.x;
        delay(2);
    }
    gyroZBias /= calibrationSamples;

    headingDegrees = 0.0f;
    previousMicros = micros();
    previousPrintMillis = millis();

    Serial.println("Ready. Heading is zeroed. Rotate the robot to test.");
    Serial.println("Heading (degrees, 0-360):");
}

void loop()
{
    uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    Serial.print("Color Temp: ");
    Serial.print(colorTemp, DEC);
    Serial.print(" K - ");
    Serial.print("Lux: ");
    Serial.print(lux, DEC);
    Serial.print(" - ");
    Serial.print("R: ");
    Serial.print(r, DEC);
    Serial.print(" ");
    Serial.print("G: ");
    Serial.print(g, DEC);
    Serial.print(" ");
    Serial.print("B: ");
    Serial.print(b, DEC);
    Serial.print(" ");
    Serial.print("C: ");
    Serial.print(c, DEC);
    Serial.print(" ");
    Serial.println(" ");
}

int getHeading(void)
{
    sensors_event_t acceleration, gyro, temperature;
    mpu.getEvent(&acceleration, &gyro, &temperature);

    const unsigned long nowMicros = micros();
    const float elapsedSeconds =
        (nowMicros - previousMicros) / 1000000.0f;
    previousMicros = nowMicros;

    headingDegrees +=
        (gyro.gyro.x - gyroZBias) * radiansToDegrees * elapsedSeconds;

    headingDegrees = fmodf(headingDegrees, 360.0f);
    if (headingDegrees < 0.0f)
    {
        headingDegrees += 360.0f;
    }

    const unsigned long nowMillis = millis();
    if (nowMillis - previousPrintMillis >= 100)
    {
        previousPrintMillis = nowMillis;
        return (int)roundf(headingDegrees);
    }
}
