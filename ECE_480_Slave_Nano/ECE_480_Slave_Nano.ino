#include <Servo.h>
Servo s;

void setup() {
  Serial.begin(9600);
  s.attach(4);
  s.write(90);
  pinMode(6, INPUT_PULLUP);
 
}

void loop() {

  if(digitalRead(6) == 0){
    Serial.println("LOW");
     s.write(90);
  }
  else{
    Serial.println("HIGH");
    s.write(0);

  } 

}