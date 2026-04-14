#define SERVO 3
#include "Servo.h"

Servo Servo1;
void setup() {
  // put your setup code here, to run once:
  Servo1.attach(SERVO);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Servo1.write(180);
  delay(3000);
  Servo1.write(0);  
  delay(3000);
  Serial.println("RAN");
}
