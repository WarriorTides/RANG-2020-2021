#include<Servo.h>

//Servo Initialization
Servo front_right_vertical;
Servo front_left_vertical;
Servo back_right_vertical;
Servo back_left_vertical;
Servo front_right_horizontal;
Servo front_left_horizontal;
Servo back_right_horizontal;
Servo back_left_horizontal;
Servo claw;
Servo claw_rotate;
Servo camera_one;
Servo camera_two;

//Store Thruster Values
bool canMerge = false;
double yaw_surge[] = {0.0,0.0};

Servo thrusters[] = {front_right_vertical,front_left_vertical,back_right_vertical,back_left_vertical,front_right_horizontal,front_left_horizontal,back_right_horizontal,back_left_horizontal};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino Program Starting");
  Serial.println("Attaching thrusters to Pins");
  front_right_vertical.attach(5);
  front_left_vertical.attach(2);
  back_right_vertical.attach(6);
  back_left_vertical.attach(7);
  front_right_horizontal.attach(8);
  front_left_horizontal.attach(4);
  back_right_horizontal.attach(46);
  back_left_horizontal.attach(3);
  claw.attach(9);
  claw_rotate.attach(10);
  camera_one.attach(11);
  camera_two.attach(45);
  Serial.println("Setting up thrusters");
  front_right_vertical.writeMicroseconds(1500);
  front_left_vertical.writeMicroseconds(1500);
  back_right_vertical.writeMicroseconds(1500);
  back_left_vertical.writeMicroseconds(1500);
  front_right_horizontal.writeMicroseconds(1500);
  front_left_horizontal.writeMicroseconds(1500);
  back_right_horizontal.writeMicroseconds(1500);
  back_left_horizontal.writeMicroseconds(1500);
  claw.write(1500);
  claw_rotate.write(2250);
  camera_one.write(1500);
  camera_two.write(1500);
  Serial.println("thrusters Set up, Pausing for 5 seconds");
  delay(2000);
  Serial.println("Ready!");
}

void loop() {
  //Serial.println("Hi");
  // put your main code here, to run repeatedly
  while (Serial.available()) {
    String input = Serial.readStringUntil('x');
    int dir = input.substring(0, 1).toInt();
    int misc = input.substring(1, 2).toInt();
    float val = input.substring(2, input.length() - 1).toFloat();
    
    if (dir == 4){
      claw.write((val * 700) + 1500);
      Serial.println("Moving claw at " + String((val * 700) + 1500));
    }
    if (dir == 5){
      claw_rotate.write((val * 750) + 1500);
      Serial.println("Moving claw wrist at " + String((val * 750) + 1500));
    }
    if (dir == 6){
      camera_one.write((val * 400) + 1500);
      Serial.println("Moving Cam One to " + String((val * 400) + 1500));
    }
    if (dir == 7){
      camera_two.write((val * 400) + 1500);
      Serial.println("Moving Cam Two to " + String((val * 400) + 1500));
    }
    if(dir == 9){
      for(int i = 0;i < 8;i = i + 1){
        thrusters[i].writeMicroseconds(1500);
      }
      Serial.println("Stopped All Thrusters");
    }
    if(dir == 0){
      yaw_surge[0] = val;
      
    }
    if(dir == 1){
      yaw_surge[1] = val;
    }
    if(dir == 2){
      front_right_horizontal.writeMicroseconds(1500 + (val * 400));
      front_left_horizontal.writeMicroseconds((1500 + (val * 400)) * -1);
      back_right_horizontal.writeMicroseconds(1500 + (val * 400));
      back_left_horizontal.writeMicroseconds((1500 + (val * 400)) * -1);
      Serial.println("Swaying");
    }
    if(dir == 3){
      front_right_vertical.writeMicroseconds(1500 + (val * 400));
      front_left_vertical.writeMicroseconds(1500 + (val * 400));
      back_right_vertical.writeMicroseconds(1500 + (val * 400));
      back_left_vertical.writeMicroseconds(1500 + (val * 400));
      Serial.println("Heaving");
    }
    if(yaw_surge[0] != 0.0 && yaw_surge[1] != 0.0){
      front_right_horizontal.writeMicroseconds(((1500 + (val * 400)) + ((1500 + (val * 400) * -1))) / 2);
      front_left_horizontal.writeMicroseconds(((1500 + (val * 400)) + ((1500 + (val * 400)))) / 2);
      back_right_horizontal.writeMicroseconds(((1500 + (val * 400)) + (1500 + ((val * 400) * -1))) / 2);
      back_left_horizontal.writeMicroseconds(((1500 + (val * 400)) + ((1500 + (val * 400)))) / 2);
      Serial.println("Merging Surge/Yaw");
    }
    else{
      if(yaw_surge[1] != 0.0){
        front_right_horizontal.writeMicroseconds(1500 + (val * 400));
        front_left_horizontal.writeMicroseconds(1500 + (val * 400));
        back_right_horizontal.writeMicroseconds(1500 + (val * 400));
        back_left_horizontal.writeMicroseconds(1500 + (val * 400));
        Serial.println("Surging");
      }
      else if(yaw_surge[0] != 0.0){
        front_right_horizontal.writeMicroseconds(1500 + ((val * 400) * -1));
        front_left_horizontal.writeMicroseconds(1500 + (val * 400));
        back_right_horizontal.writeMicroseconds(1500 + ((val * 400) * -1));
        back_left_horizontal.writeMicroseconds(1500 + (val * 400));
        Serial.println("Yawing");
      }
    }
    
  }
  

}
