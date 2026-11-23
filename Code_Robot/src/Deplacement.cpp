#include "Adafruit_TCS34725.h"
#include "Arduino.h"
#include "LibRobus.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

// function declarations here:
const char* getColor(void);
void updateAngle(void);
void setupMPU(void);
void straightBot(void);
void turn(float degrees);
void driveFor(unsigned long ms_time);

//Declaration and global variables for the TCS34725 color sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_16X);
const char* currentColor = "Other";
unsigned long lastColorTime = 0;

//Declaration and global variables for the MPU6050 accelerometer
Adafruit_MPU6050 mpu;
float angleX = 0;        // current angle in degrees
float gyroXOffset = 0;   // measured at startup
unsigned long lastTime;

// Variables for the movement
float vitesse = 0.40;


void setup(void)
{
    BoardInit(); 
    Serial.begin(9600);

    // Init the TCS34725 color sensor
    if (tcs.begin()) {
        Serial.println("Found TCS34725");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
    }

    // Init the MPU6050 accelerometer
    if (mpu.begin()) {
        Serial.println("Found MPU6050");
    }
    else {
        Serial.println("No MPU6050 found ... check your connections");
        while (1);
    }

    setupMPU();
}

void loop()
{
    driveFor(1000); // Example: drive straight for 1 seconds
    delay(300); 
    turn(90); // Example: turn 90 degrees to the right
    delay(300);
    
}

const char* getColor(void){

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
        return "Black";
    } 
    else if (b > r && b > g) {
        return "Blue";
    } 
    else {
        return "Other";
    }
}

void setupMPU() {
    //Set the range and bandwidth of the MPU6050
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    // Calibration of the gyro (no moving type shi)
    Serial.println("Calibrating gyro...");
    float sum = 0;
    for (int i = 0; i < 500; i++)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        sum += g.gyro.x;
        delay(5);
    }
    gyroXOffset = sum / 500.0;

    lastTime = micros();
}

void updateAngle() // run this as often as possible for accurate angle
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    unsigned long now = micros();
    float dt = (now - lastTime) / 1000000.0;  // seconds
    lastTime = now;

    // convert rad/s to deg/s
    float rate = (g.gyro.x - gyroXOffset) * 180.0 / PI;

    if (abs(rate) < 0.3) rate = 0;  // ignore noise

    angleX += rate * dt;
    //Serial.print("Angle: ");
    //Serial.println(angleX);
}

void straightBot() {
    // Code to make the robot go straight
    updateAngle();
    float correction = angleX * 0.01; // Correction with Kp
    MOTOR_SetSpeed(RIGHT,vitesse - correction);
    MOTOR_SetSpeed(LEFT, vitesse + correction);
    
}

void turn(float degrees)
{
    angleX = 0;
    float target = degrees;

    while (abs(target - angleX) > 1.0)   // 1° tolerance
    {
        updateAngle();
        if (angleX < target) {
            MOTOR_SetSpeed(LEFT, -0.2);
            MOTOR_SetSpeed(RIGHT, +0.2);
        } else {
            MOTOR_SetSpeed(LEFT, +0.2);
            MOTOR_SetSpeed(RIGHT, -0.2);
        }
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void driveFor(unsigned long ms_time)
{
    unsigned long start = millis();
    angleX = 0; // Reset angle for straight driving
    
    while (millis() - start < ms_time)
    {
        straightBot();

        if (millis() - lastColorTime > 100) { // check color every 100ms
            currentColor = getColor();
            lastColorTime = millis();
        }
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}