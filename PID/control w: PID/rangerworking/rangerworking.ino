#include<Servo.h>
 //2, 3, 4, 5, 6, 7, 46

#include <PID_v1.h>
#include "MS5837.h"

double rKp = 2, rKi = 5, rKd = 1;
double yKp = 2, yKi = 5, yKd = 1;
double pKp = 2, pKi = 5, pKd = 1;
double dKp = 2, dKi = 5, dKd = 1;

double rollSetpoint, rollInput, rollOutput;
double yawSetpoint, yawInput, yawOutput;
double pitchSetpoint, pitchInput, pitchOutput;
double depthSetpoint, depthInput, depthOutput;

PID rollPID(&rollInput, &rollOutput, &rollSetpoint,rKp, rKi, rKd, DIRECT);
PID yawPID(&yawInput, &yawOutput, &yawSetpoint,yKp, yKi, yKd, DIRECT);
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint,pKp, pKi, pKd, DIRECT);
PID depthPID(&depthInput, &depthOutput, &depthSetpoint,dKp, dKi, dKd, DIRECT);



#include "AHRSAlgorithms.h"
#include "ICM20948.h"

#define AHRS true         // Set to false for basic data read
#define SerialDebug false  // Set to true to get Serial output for debugging

// Pin definitions
int myLed  = 13;  // Set up pin 13 led for toggling
MS5837 depthSensor;
ICM20948 myIMU;
 Servo front_right_vertical;
 Servo front_left_vertical;
 Servo back_right_vertical;
 Servo back_left_vertical;
 Servo front_right_horizontal;
 Servo front_left_horizontal;
 Servo back_right_horizontal;
 Servo back_left_horizontal;
  Servo claw;

 void setup() {
     int claw_pin = 2;
        Wire.begin();
     Serial.begin(115200);  
     Serial.println("RESET");
     //8, 46, 5, 7
     //3, 6, 4, 2
     front_right_vertical.attach(6);
     front_left_vertical.attach(2);
     back_right_vertical.attach(46); //4 and  46
     back_left_vertical.attach(5);
     
     front_right_horizontal.attach(3);
     front_left_horizontal.attach(8);
     back_right_horizontal.attach(7);
     back_left_horizontal.attach(4);
          claw.attach(claw_pin);

     front_right_vertical.writeMicroseconds(1500);
     front_left_vertical.writeMicroseconds(1500);
     back_right_vertical.writeMicroseconds(1500);
     back_left_vertical.writeMicroseconds(1500);
     front_right_horizontal.writeMicroseconds(1500);
     front_left_horizontal.writeMicroseconds(1500);
     back_right_horizontal.writeMicroseconds(1500);                                                                                                                                         
     back_left_horizontal.writeMicroseconds(1500);
     delay(7000);
     Serial.println("ready");
      rollPID.SetOutputLimits(-400, 400);
      yawPID.SetOutputLimits(-400, 400);
      pitchPID.SetOutputLimits(-400, 400);
      depthPID.SetOutputLimits(-400, 400);
    
      rollPID.SetMode(AUTOMATIC);
      yawPID.SetMode(AUTOMATIC);
      pitchPID.SetMode(AUTOMATIC);
      depthPID.SetMode(AUTOMATIC);
    
      IMUsetup();
      depthSensorSetup();
 } 

 void loop() {
  updateIMU();
  depthSensor.read();

  rollInput = myIMU.roll;
  yawInput = myIMU.yaw;
  pitchInput = myIMU.pitch;
  depthInput = depthSensor.depth();
  
  rollPID.Compute();
  yawPID.Compute();
  pitchPID.Compute();
  depthPID.Compute();

   Serial.println("roll input "+String(rollInput)+" output "+String(rollOutput)+" setpoint "+String(rollSetpoint));
  Serial.println("yaw input "+String(yawInput)+" output "+String(yawOutput)+" setpoint "+String(yawSetpoint));
  Serial.println("pitch input "+String(pitchInput)+" output "+String(pitchOutput)+" setpoint "+String(pitchSetpoint));
  Serial.println("depth input "+String(depthInput)+" output "+String(depthOutput)+" setpoint "+String(depthSetpoint));

  
   while (Serial.available()){
    String input = Serial.readStringUntil('x');
    if (input.substring(0, input.length() -2).equals("open-claw")){
        claw.writeMicroseconds(0);
    }
    if (input.substring(0, input.length() -2).equals("close-claw")){
        claw.writeMicroseconds(255);
    }
    int dir = input.substring(0, 1).toInt();
    int thruster_value = input.substring(2, input.length() - 1).toFloat() * 100;
   
    Serial.println("direction "+String(dir)+" thruster_value "+String(thruster_value));
    //sway
    if (dir == 0){
    //  Serial.println("writing values to FRH, BRH, FLH, BLH");
      front_right_vertical.writeMicroseconds(1500);
      back_right_vertical.writeMicroseconds(1500);
      front_left_vertical.writeMicroseconds(1500);
      back_left_vertical.writeMicroseconds(1500);
      
      front_right_horizontal.writeMicroseconds(thruster_value  * -1 + 1500);
      back_right_horizontal.writeMicroseconds(thruster_value + 1500);
      front_left_horizontal.writeMicroseconds(thruster_value + 1500);
      back_left_horizontal.writeMicroseconds(thruster_value*-1 + 1500);
      
    }
    //surge
    else if (dir == 1){
      front_right_vertical.writeMicroseconds(1500);
      back_right_vertical.writeMicroseconds(1500);
      front_left_vertical.writeMicroseconds(1500);
      back_left_vertical.writeMicroseconds(1500);
      
      front_right_horizontal.writeMicroseconds(thruster_value + 1500);
      front_left_horizontal.writeMicroseconds(thruster_value + 1500);
      back_right_horizontal.writeMicroseconds(thruster_value + 1500);
      back_left_horizontal.writeMicroseconds(thruster_value + 1500);
    }
    //heave
    else if (dir == 3){
      front_right_horizontal.writeMicroseconds(1500);
      back_right_horizontal.writeMicroseconds(1500);
      front_left_horizontal.writeMicroseconds(1500);
      back_left_horizontal.writeMicroseconds(1500);
      
      
      front_right_vertical.writeMicroseconds(thruster_value + 1500);
      front_left_vertical.writeMicroseconds(thruster_value  + 1500);
      back_right_vertical.writeMicroseconds(thruster_value  + 1500);
      back_left_vertical.writeMicroseconds(thruster_value + 1500);
    }
    //yaw
    else if (dir == 4){
      front_right_vertical.writeMicroseconds(1500);
      back_right_vertical.writeMicroseconds(1500);
      front_left_vertical.writeMicroseconds(1500);
      back_left_vertical.writeMicroseconds(1500);
      
      front_right_horizontal.writeMicroseconds(thruster_value + 1500);
      back_right_horizontal.writeMicroseconds(thruster_value + 1500);
      front_left_horizontal.writeMicroseconds(-1 * thruster_value + 1500);
      back_left_horizontal.writeMicroseconds(-1*thruster_value + 1500);
    }
   
    
   }   
 }
 
void depthSensorSetup()
{
  depthSensor.setModel(MS5837::MS5837_02BA);
  depthSensor.init();
  depthSensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void IMUsetup()
{
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);
  // Reset ICM20948
  myIMU.writeByte(ICM20948_ADDRESS, PWR_MGMT_1, READ_FLAG);
  delay(100);
  myIMU.writeByte(ICM20948_ADDRESS, PWR_MGMT_1, 0x01);
  delay(100);
  
  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(ICM20948_ADDRESS, WHO_AM_I_ICM20948);
  Serial.print(F("ICM20948 I AM 0x"));
  Serial.print(c, HEX);
  Serial.print(F(" I should be 0x"));
  Serial.println(0xEA, HEX);

  if (c == 0xEA) // WHO_AM_I should always be 0x71
  {
    Serial.println(F("ICM20948 is online..."));

    // Start by performing self test and reporting values
    myIMU.ICM20948SelfTest(myIMU.selfTest);
    Serial.print(F("x-axis self test: acceleration trim within : "));
    Serial.print(myIMU.selfTest[0],1); Serial.println("% of factory value");
    Serial.print(F("y-axis self test: acceleration trim within : "));
    Serial.print(myIMU.selfTest[1],1); Serial.println("% of factory value");
    Serial.print(F("z-axis self test: acceleration trim within : "));
    Serial.print(myIMU.selfTest[2],1); Serial.println("% of factory value");
    Serial.print(F("x-axis self test: gyration trim within : "));
    Serial.print(myIMU.selfTest[3],1); Serial.println("% of factory value");
    Serial.print(F("y-axis self test: gyration trim within : "));
    Serial.print(myIMU.selfTest[4],1); Serial.println("% of factory value");
    Serial.print(F("z-axis self test: gyration trim within : "));
    Serial.print(myIMU.selfTest[5],1); Serial.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateICM20948(myIMU.gyroBias, myIMU.accelBias);

    myIMU.initICM20948();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("ICM20948 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK09916_ADDRESS, WHO_AM_I_AK09916);
    Serial.print("AK8963 ");
    Serial.print("I AM 0x");
    Serial.print(d, HEX);
    Serial.print(" I should be 0x");
    Serial.println(0x09, HEX);

    if (d != 0x09)
    {
      // Communication failed, stop here
      Serial.println(F("Communication with magnetometer failed, abort!"));
      Serial.flush();
      abort();
    }

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK09916();
    // Initialize device for active mode read of magnetometer
    Serial.println("AK09916 initialized for active data mode....");
  
  /*
    if (SerialDebug)
    {
      //  Serial.println("Calibration values: ");
      Serial.print("X-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[0], 2);
      Serial.print("Y-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[1], 2);
      Serial.print("Z-Axis factory sensitivity adjustment value ");
      Serial.println(myIMU.factoryMagCalibration[2], 2);
    }
  */

    // Get sensor resolutions, only need to do this once
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();

    // The next call delays for 4 seconds, and then records about 15 seconds of
    // data to calculate bias and scale.
    myIMU.magCalICM20948(myIMU.magBias, myIMU.magScale);
    Serial.println("AK09916 mag biases (mG)");
    Serial.println(myIMU.magBias[0]);
    Serial.println(myIMU.magBias[1]);
    Serial.println(myIMU.magBias[2]);

    Serial.println("AK09916 mag scale (mG)");
    Serial.println(myIMU.magScale[0]);
    Serial.println(myIMU.magScale[1]);
    Serial.println(myIMU.magScale[2]);
    delay(2000); // Add delay to see results before serial spew of data
  } // if (c == 0x71)
  else
  {
    Serial.print("Could not connect to ICM20948: 0x");
    Serial.println(c, HEX);

    // Communication failed, stop here
    Serial.println(F("Communication failed, abort!"));
    Serial.flush();
    abort();
  }
}
void updateIMU()
{
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  if (myIMU.readByte(ICM20948_ADDRESS, INT_STATUS_1) & 0x01)
  {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - myIMU.accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - myIMU.accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - myIMU.accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes - myIMU.magBias[0];
    myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes - myIMU.magBias[1];
    myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes - myIMU.magBias[2];
  } // if (readByte(ICM20948_ADDRESS, INT_STATUS) & 0x01)

  // Must be called before updating quaternions!
  myIMU.updateTime();

  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // ICM20948, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
  MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                         myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                         myIMU.mx, myIMU.mz, myIMU.deltat);

  if (!AHRS)
  {
    myIMU.delt_t = millis() - myIMU.count;
    if (myIMU.delt_t > 500)
    {
      if(SerialDebug)
      {
        // Print acceleration values in milligs!
        Serial.print("X-acceleration: "); Serial.print(1000 * myIMU.ax);
        Serial.print(" mg ");
        Serial.print("Y-acceleration: "); Serial.print(1000 * myIMU.ay);
        Serial.print(" mg ");
        Serial.print("Z-acceleration: "); Serial.print(1000 * myIMU.az);
        Serial.println(" mg ");

        // Print gyro values in degree/sec
        Serial.print("X-gyro rate: "); Serial.print(myIMU.gx, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Y-gyro rate: "); Serial.print(myIMU.gy, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Z-gyro rate: "); Serial.print(myIMU.gz, 3);
        Serial.println(" degrees/sec");

        // Print mag values in degree/sec
        Serial.print("X-mag field: "); Serial.print(myIMU.mx);
        Serial.print(" mG ");
        Serial.print("Y-mag field: "); Serial.print(myIMU.my);
        Serial.print(" mG ");
        Serial.print("Z-mag field: "); Serial.print(myIMU.mz);
        Serial.println(" mG");

        myIMU.tempCount = myIMU.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        myIMU.temperature = ((float) myIMU.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        Serial.print("Temperature is ");  Serial.print(myIMU.temperature, 1);
        Serial.println(" degrees C");
      }

      myIMU.count = millis();
      digitalWrite(myLed, !digitalRead(myLed));  // toggle led
    } // if (myIMU.delt_t > 500)
  } // if (!AHRS)
  else
  {
    // Serial print and/or display at 0.5 s rate independent of data rates
    myIMU.delt_t = millis() - myIMU.count;

    // update LCD once per half-second independent of read rate
    if (myIMU.delt_t > 500)
    {
      if(SerialDebug)
      {
        Serial.print("ax = ");  Serial.print((int)1000 * myIMU.ax);
        Serial.print(" ay = "); Serial.print((int)1000 * myIMU.ay);
        Serial.print(" az = "); Serial.print((int)1000 * myIMU.az);
        Serial.println(" mg");

        Serial.print("gx = ");  Serial.print(myIMU.gx, 2);
        Serial.print(" gy = "); Serial.print(myIMU.gy, 2);
        Serial.print(" gz = "); Serial.print(myIMU.gz, 2);
        Serial.println(" deg/s");

        Serial.print("mx = ");  Serial.print((int)myIMU.mx);
        Serial.print(" my = "); Serial.print((int)myIMU.my);
        Serial.print(" mz = "); Serial.print((int)myIMU.mz);
        Serial.println(" mG");

        Serial.print("q0 = ");  Serial.print(*getQ());
        Serial.print(" qx = "); Serial.print(*(getQ() + 1));
        Serial.print(" qy = "); Serial.print(*(getQ() + 2));
        Serial.print(" qz = "); Serial.println(*(getQ() + 3));
      }

// Define output variables from updated quaternion---these are Tait-Bryan
// angles, commonly used in aircraft orientation. In this coordinate system,
// the positive z-axis is down toward Earth. Yaw is the angle between Sensor
// x-axis and Earth magnetic North (or true North if corrected for local
// declination, looking down on the sensor positive yaw is counterclockwise.
// Pitch is angle between sensor x-axis and Earth ground plane, toward the
// Earth is positive, up toward the sky is negative. Roll is angle between
// sensor y-axis and Earth ground plane, y-axis up is positive roll. These
// arise from the definition of the homogeneous rotation matrix constructed
// from quaternions. Tait-Bryan angles as well as Euler angles are
// non-commutative; that is, the get the correct orientation the rotations
// must be applied in the correct order which for this configuration is yaw,
// pitch, and then roll.
// For more see
// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// which has additional links.
      myIMU.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ()
                    * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3)
                    * *(getQ()+3));
      myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ()
                    * *(getQ()+2)));
      myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2)
                    * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3)
                    * *(getQ()+3));
      myIMU.pitch *= RAD_TO_DEG;
      myIMU.yaw   *= RAD_TO_DEG;

      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
      //  8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
      // - http://www.ngdc.noaa.gov/geomag-web/#declination
      myIMU.yaw  -= 8.5;
      myIMU.roll *= RAD_TO_DEG;

      if(SerialDebug)
      {
        Serial.print("Yaw, Pitch, Roll: ");
        Serial.print(myIMU.yaw, 2);
        Serial.print(", ");
        Serial.print(myIMU.pitch, 2);
        Serial.print(", ");
        Serial.println(myIMU.roll, 2);

        Serial.print("rate = ");
        Serial.print((float)myIMU.sumCount / myIMU.sum, 2);
        Serial.println(" Hz");
      }

      myIMU.count = millis();
      myIMU.sumCount = 0;
      myIMU.sum = 0;
    } // if (myIMU.delt_t > 500)
  } // if (AHRS)
}
