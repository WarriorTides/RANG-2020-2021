#include<Servo.h>
 //2, 3, 4, 5, 6, 7, 46

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

   // put your setup code here, to run once:
     Serial.begin(9600);  
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
