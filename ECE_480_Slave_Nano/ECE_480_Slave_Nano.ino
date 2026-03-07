#include <Servo.h>

Servo s;

void setup() {
  s.attach(11);
  s.write(75);
  delay(2000);
  s.write(130);
  delay(2000);
  //s.write(180);
  //delay(2000);

}

void loop() {
}