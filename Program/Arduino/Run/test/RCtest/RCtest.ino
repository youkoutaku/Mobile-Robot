#include <Servo.h>

Servo RC1servo;

int RC1_pos=0;            //RC1の角度


void setup() {
  // put your setup code here, to run once:
  RC1servo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
      RC1servo.write(RC1_pos);
      RC1_pos+=5;    
      delay(50);
  
}
