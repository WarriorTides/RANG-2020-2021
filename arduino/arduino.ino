#include<Servo.h>
 //2, 3, 4, 5, 6, 7, 46

 Servo front_right;
 Servo front_left;
 Servo back_right;
 Servo back_left;
 Servo mid_right;
 Servo mid_left;
 Servo claw;
 
 void setup() {
      int claw_pin = 2;
   // put your setup code here, to run once:
     Serial.begin(9600);  
     Serial.println("RESET");
     front_right.attach(8);
     front_left.attach(46);
     back_right.attach(5); //4 and  46
     back_left.attach(7);
     mid_right.attach(3);
     mid_left.attach(6);
     claw.attach(claw_pin);
     front_right.writeMicroseconds(1500);
     front_left.writeMicroseconds(1500);
     back_right.writeMicroseconds(1500);
     back_left.writeMicroseconds(1500);
     mid_right.writeMicroseconds(1500);
     mid_left.writeMicroseconds(1500);
     delay(7000);
     Serial.println("ready");
 }

 void loop() {
   while (Serial.available()){
    String input = Serial.readStringUntil('x');
    if (input.substring(0, input.length() -2).equals("open-claw")){
        claw.writeMicroseconds(0);
    }
    if (input.substring(0, input.length() -2).equals("close-claw")){
        claw.writeMicroseconds(255);
    }
    int dir = input.substring(0, 1).toInt();
    int thruster_value = input.substring(2, input.length() - 1).toFloat() * 230  ;
   
    Serial.println("direction "+String(dir)+" thruster_value "+String(thruster_value));
    //surge sway
    if (dir == 0){
    //  Serial.println("writing values to FRH, BRH, FLH, BLH");
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(-1 * thruster_value + 1500);
      back_left.writeMicroseconds(-1 * thruster_value + 1500);
      
    }
    //surge 
    else if (dir == 1){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value * -1 + 1500);
      back_left.writeMicroseconds(thruster_value * -1 + 1500);
    }
    //heave
    else if (dir == 3){
      mid_right.writeMicroseconds(1500);
      mid_left.writeMicroseconds(1500);    
      mid_right.writeMicroseconds(thruster_value + 1500);
      mid_left.writeMicroseconds(thruster_value + 1500);

    }
    //yaw
    else if (dir == 4){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(thruster_value * -1 + 1500);
      back_left.writeMicroseconds(thruster_value * -1 + 1500);
    }

   }   
 }
 
