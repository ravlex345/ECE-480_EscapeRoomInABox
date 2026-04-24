#define SERVO 6
#define SERVO2 11

#include "Servo.h"

Servo Servo1;
Servo Servo2;

void setup() {
  // put your setup code here, to run once:
  Servo1.attach(SERVO);
    Servo2.attach(SERVO2);

  Serial.begin(9600);
    Serial.println("RESET");

  // Servo2.writeMicroseconds(1800);

  // delay(20000);

}

void loop() {
  // put your main code here, to run repeatedly:
     delay(4500);
    Servo2.writeMicroseconds(500);  

   delay(9000);
   Servo2.write(90);  

  delay(1000);
  Servo2.writeMicroseconds(1800);

  delay(10000);
  Servo2.write(90);  
  Serial.println("RAN");
}
