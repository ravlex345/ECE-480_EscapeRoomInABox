#include <Wire.h>
#include <Servo.h>
Servo Servo1;

#define PUZZLE_1 6
#define SERVO 11
bool sent = false;

void setup() {
  Serial.begin(9600);
  pinMode(PUZZLE_1, INPUT_PULLUP);
  Servo1.attach(SERVO);
  delay(1000);      // give servo time
  Wire.begin();     // start I2C AFTER servo
  Serial.println("=== MEGA RESET ===");

}

void loop() {
  if (digitalRead(PUZZLE_1) == LOW && !sent){
    sent = true;
      Serial.println("Connection");
      Servo1.write(0);
      Servo1.write(75);
      delay(2000);
      Servo1.write(130);
      delay(2000);      
      // Wire.beginTransmission(8); // Send to Slave address 8
      // Serial.println("Mega Master Initialized");
      // Wire.write("PUZZLE ENDED - MASTER");       // Send data
      // Wire.endTransmission();    // Stop transmitting


  }
  // Wire.requestFrom(8, 2);
  //   while (Wire.available()) { 
  //   char c = Wire.read();      
  //   Serial.print(c);
  // }
  // Serial.println(); // Add a newline for readability

  delay(500);
}
