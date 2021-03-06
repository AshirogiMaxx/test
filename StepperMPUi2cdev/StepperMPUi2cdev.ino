/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.
 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").
 Contact information
 -------------------
 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#include <Stepper.h>
#include <PWM.h>
#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include "I2Cdev.h"
#include "MPU6050.h"

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
#define OUTPUT_READABLE_ACCELGYRO
Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
MPU6050 accelgyro;


/* IMU Data */
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t tempRaw;

double gxangle, gyangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

#define motorSteps 200     // change this depending on the number of steps
                           // per revolution of your motor 360/1.8 Degrees
#define motorPin1 8 // AIN1
#define motorPin2 9 // AIN2
#define motorPin3 10 // BIN1
#define motorPin4 11 // BIN2
#define ledPin 13
#define STBY 3 // SBTY
int PWMA = 12;                // the pin that the PWMA and PWMB Is conected 
int Cycle = 120;         // 
int32_t frequency = 100000;

// initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1,motorPin2, motorPin3, motorPin4); 
int cantidad = 1;

int previous = 0;
void setup() {

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, LOW);
  accelgyro.initialize();
  myStepper.setSpeed(60);
  bool success = SetPinFrequencySafe(PWMA, frequency);
  Serial.begin(115200);

  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

 

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(ay, az) * RAD_TO_DEG;
  double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
  double pitch = atan2(-ax, az) * RAD_TO_DEG;
#endif

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gxangle = roll;
  gyangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;

  timer = micros();
}

void loop() {
  /* Update all the values */
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  pwmWrite(PWMA, Cycle);
  digitalWrite(STBY, HIGH);

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(ay, az) * RAD_TO_DEG;
  double pitch = atan(-ax / sqrt(ay * ay + az * az)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
  double pitch = atan2(-ax, az) * RAD_TO_DEG;
#endif

  double gxrate = gx / 131.0; // Convert to deg/s
  double gyrate = gy / 131.0; // Convert to deg/s

#ifdef RESTRICT_PITCH
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gxangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gxrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90)
    gyrate = -gyrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleY = kalmanY.getAngle(pitch, gyrate, dt);
#else
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyangle = pitch;
  } else
    kalAngleY = kalmanY.getAngle(pitch, gyrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleY) > 90)
    gxrate = -gxrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleX = kalmanX.getAngle(roll, gxrate, dt); // Calculate the angle using a Kalman filter
#endif

  gxangle += gxrate * dt; // Calculate gyro angle without any filter
  gyangle += gyrate * dt;
  //gxangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gxrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gxangle < -180 || gxangle > 180)
    gxangle = kalAngleX;
  if (gyangle < -180 || gyangle > 180)
    gyangle = kalAngleY;

  /* Print Data */
#if 1 // Set to 1 to activate
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.print(gz); Serial.print("\t");
  Serial.print("\t");
#endif

//Serial.print(roll); Serial.print("\t");
  //Serial.print(gxangle); Serial.print("\t");
  //Serial.print(compAngleX); Serial.print("\t");
  Serial.print(kalAngleX); Serial.print("\t");

  Serial.print("\t");

  //Serial.print(pitch); Serial.print("\t");
  //Serial.print(gyangle); Serial.print("\t");
  //Serial.print(compAngleY); Serial.print("\t");
  Serial.print(kalAngleY); Serial.print("\t");


  Serial.print("\r\n");
  delay(10);

  int valuex = kalAngleX;  
 

myStepper.step(valuex - previous);


  previous = valuex;



}
