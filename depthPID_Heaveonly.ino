//standalone depth PID

#include <Wire.h>
#include "MS5837.h"
#include <Servo.h>
#include <PID_v1.h>

MS5837 depthSensor;

double dKp = 8, dKi = 10, dKd = 6;
double depthSetpoint, depthInput, depthOutput;
PID depthPID(&depthInput, &depthOutput, &depthSetpoint, dKp, dKi, dKd, DIRECT);

int FRVp = 7;
int FLVp = 8;
int BRVp = 2;
int BLVp = 5;
Servo front_right_vertical;
Servo front_left_vertical;
Servo back_right_vertical;
Servo back_left_vertical;

void setup() {
  
  Serial.begin(9600);
  
  Serial.println("Starting");
  
  Wire.begin();

  // Initialize pressure sensor
  // Returns true if initialization was successful
  // We can't continue with the rest of the program unless we can initialize the sensor
  while (!depthSensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(3000);
  }

  //setup depth sensor
  depthSensor.setModel(MS5837::MS5837_30BA);
  depthSensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

  depthInput = depthSensor.depth();
  depthSetpoint = 0.18;
  
  depthPID.SetMode(AUTOMATIC);
  depthPID.SetOutputLimits(-100, 100);

  front_right_vertical.attach(FRVp);
  front_left_vertical.attach(FLVp);
  back_right_vertical.attach(BRVp);
  back_left_vertical.attach(BLVp);

  front_right_vertical.writeMicroseconds(1500);
  front_left_vertical.writeMicroseconds(1500);
  back_right_vertical.writeMicroseconds(1500);
  back_left_vertical.writeMicroseconds(1500);

  delay(3000);
  Serial.println("ready");
}

void loop() {
  // Update pressure and temperature readings
  Serial.print("Depth: "); 
  Serial.print(depthSensor.depth()); 
  Serial.println(" m");
  
  depthSensor.read();
  depthInput = depthSensor.depth();
  depthPID.Compute();
  Serial.println((depthOutput * -10) + 1500);

  front_right_vertical.writeMicroseconds((depthOutput * -10) + 1500);
  front_left_vertical.writeMicroseconds((depthOutput * -10) + 1500);
  back_right_vertical.writeMicroseconds((depthOutput * -10) + 1500);
  back_left_vertical.writeMicroseconds((depthOutput * -10) + 1500);

  delay(1000);
}
