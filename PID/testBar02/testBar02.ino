//worked previously, not tested recently?
#include <PID_v1.h>
#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;

void setup() {  
  // put your setup code here, to run once:
  Wire.begin();
  sensor.setModel(MS5837::MS5837_02BA);
  sensor.init();
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
  Serial.begin(9600);
}

void loop() {
  Serial.println(getCurrentDepth());
}
double getCurrentDepth(){
  sensor.read();
  return sensor.depth();
}
