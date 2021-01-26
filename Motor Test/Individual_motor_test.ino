
//Enter the 3 letter code for the motor to turn it on/off



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
 void setup() {
   // put your setup code here, to run once:
     Serial.begin(9600);  
     Serial.println("RESET");
     //8, 46, 5, 7
     //3, 6, 4, 2
     front_right_vertical.attach(6);
     front_left_vertical.attach(2);
     back_right_vertical.attach(46); //4 and  46
     back_left_vertical.attach(5);
     
     front_right_horizontal.attach(4);
     front_left_horizontal.attach(8);
     back_right_horizontal.attach(7);
     back_left_horizontal.attach(3);
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
int FRV = 100;
int FLV = 100;
int BRV = 100;
int BLV = 100;
int FRH = 100;
int FLH = 100;
int BRH = 100;
int BLH = 100;
 void loop() {
   String r = Serial.readString();
   r.trim();
   if (r == "FRV"){
    Serial.print("Front right vertical write ");
    Serial.println(1600 + FRV);
    front_right_vertical.writeMicroseconds(1600 + FRV);
    FRV *= -1;
   }
   if (r == "FLV"){
    Serial.print("Front left vertical write ");
    Serial.println(1600 + FLV);
    front_left_vertical.writeMicroseconds(1600 + FLV);
    FLV *= -1;
   }
   if (r == "BRV"){
      Serial.print("Back right vertical write ");
      Serial.println(1600 + BRV);
      back_right_vertical.writeMicroseconds(1600 + BRV);
      BRV *= -1;
    }
   if (r == "BLV"){
     Serial.print("Back left vertical write ");
      Serial.println(1600 + BLV);
      back_left_vertical.writeMicroseconds(1600 + BLV);
      BLV *= -1;
   }
   if (r == "FRH"){
    Serial.print("Front right horizontal write ");
    Serial.println(1600 + FRH);
    front_right_horizontal.writeMicroseconds(1600 + FRH);
    FRH *= -1;
   }
   if (r == "FLH"){
    Serial.print("Front left horizontal write ");
    Serial.println(1600 + FLH);
    front_left_horizontal.writeMicroseconds(1600 + FLH);
    FLH *= -1;
   }
   if (r == "BRH"){
      Serial.print("Back right horizontal write ");
      Serial.println(1600 + BRH);
      back_right_horizontal.writeMicroseconds(1600 + BRH);
      BRH *= -1;
    }
   if (r == "BLH"){
     Serial.print("Back left horizontal write ");
      Serial.println(1600 + BLH);
      back_left_horizontal.writeMicroseconds(1600 + BLH);
      BLH *= -1;
   }
   
   

   
 }
