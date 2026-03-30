#include <Wire.h>
#include <Servo.h>
Servo ServoCompartment;
Servo ServoScissorLift;

#define PUZZLE_1 6
#define PUZZLE_2 7

#define SERVO_COMPARTMENT 11
#define SERVO_SCISSORLIFT 12

bool sent1 = false;
bool sent2 = false;
bool rsent = false;
void setup() {
  Serial.begin(9600);
  pinMode(PUZZLE_1, INPUT_PULLUP);
  pinMode(PUZZLE_2, INPUT_PULLUP);
  ServoCompartment.attach(SERVO_COMPARTMENT);
  ServoCompartment.write(100);
  ServoScissorLift.attach(SERVO_SCISSORLIFT);
  ServoScissorLift.write(92);
  delay(1000);      // give servo time
  Wire.begin();     // start I2C AFTER servo
  Serial.println("=== MEGA RESET ===");
  Wire.beginTransmission(8); // Send to Puzzle 1: address 8
  Wire.write("S");
  Wire.endTransmission();
  Serial.println("Transmission Sent");
}

void loop() {
  
  if (digitalRead(PUZZLE_1) == LOW && !sent1){
      sent1 = true;
      Serial.println("PUZZLE 1 Complete");
      Wire.beginTransmission(9); // Send to Slave address 9, puzzle 2
      Wire.write("S");       // Send data
      Wire.endTransmission();    // Stop transmitting
      Serial.println("sent to puzzle 2");
;
      ServoCompartment.write(200);
      //delay(2000);  

  }
  if (digitalRead(PUZZLE_2) == LOW && !sent2){
      sent2 = true;
      Serial.println("PUZZLE 2 Complete");
      ServoScissorLift.write(180);
      delay(1500);  
      
      ServoScissorLift.write(92);


  }
  else if (digitalRead(PUZZLE_1) == HIGH){
    ServoCompartment.write(100);
    //ServoScissorLift.write(90);
    //Serial.println("RESET");
    sent1 = false;
    if (!rsent){
    Wire.beginTransmission(9); // Send to Slave address 9, puzzle 2
    Wire.write("R");       // Send data
    Wire.endTransmission();    // Stop transmitting]
    delay(200);}
    sent2 = false;
    rsent = true;
  }

  // Wire.requestFrom(8, 2);
  //   while (Wire.available()) { 
  //   char c = Wire.read();      
  //   Serial.print(c);
  // }
  // Serial.println(); // Add a newline for readability

  delay(500);
}
