#include <Wire.h>
#include <Servo.h>
Servo ServoCompartment;
Servo ServoScissorLift;

#define PUZZLE_1 6
#define PUZZLE_2 7

#define SERVO_COMPARTMENT 11
#define ACTUATOR_UP 12
#define ACTUATOR_DOWN 13


bool sent1 = false;
bool sent2 = false;
bool rsent = false;
void setup() {
  Serial.begin(9600);
    Serial.println("Initialize");

  pinMode(PUZZLE_1, INPUT_PULLUP);
  pinMode(PUZZLE_2, INPUT_PULLUP);
  pinMode(ACTUATOR_UP, OUTPUT);
  pinMode(ACTUATOR_DOWN, OUTPUT);
  digitalWrite(ACTUATOR_UP, HIGH);
  digitalWrite(ACTUATOR_DOWN, HIGH);//Deactivate both relays to brake the motor
  delay(18000);// wait 2 seconds

  ServoCompartment.attach(SERVO_COMPARTMENT);

  Wire.begin();     // start I2C AFTER servo
  Serial.println("=== MEGA RESET ===");
  Wire.beginTransmission(8); // Send to Puzzle 1: address 8
  Wire.write("S");
  Wire.endTransmission();
  Serial.println("Transmission Sent");
}

void loop() {
  
  // Serial.println("Set to high");
  // digitalWrite(ACTUATOR_UP, LOW);
  // digitalWrite(ACTUATOR_DOWN, LOW);//Deactivate both relays to brake the motor
  // delay(19000);// wait 2 seconds
  // Serial.println("Set to low");
  // digitalWrite(ACTUATOR_UP, LOW);
  // digitalWrite(ACTUATOR_DOWN, HIGH);//Deactivate both relays to brake the motor
  // delay(6000);// wait 2 seconds


  if (digitalRead(PUZZLE_1) == LOW && !sent1){
      sent1 = true;
      rsent = false;
      Serial.println("PUZZLE 1 Complete");
      Wire.beginTransmission(9); // Send to Slave address 9, puzzle 2
      Wire.write("S");       // Send data
      Wire.endTransmission();    // Stop transmitting
      ServoCompartment.writeMicroseconds(500);  
      delay(9000);
      ServoCompartment.write(90);  
      Serial.println("sent to puzzle 2");
;
      //delay(2000);  

  }
  if (digitalRead(PUZZLE_2) == LOW && !sent2){
      sent2 = true;
      Serial.println("PUZZLE 2 Complete");
      digitalWrite(ACTUATOR_UP, LOW);
      digitalWrite(ACTUATOR_DOWN, LOW);//Deactivate both relays to brake the motor
      delay(18000);  
      digitalWrite(ACTUATOR_UP, HIGH);
      digitalWrite(ACTUATOR_DOWN, LOW);//Deactivate both relays to brake the motor




  }
  else if (digitalRead(PUZZLE_1) == HIGH && sent1){
    Serial.println("Reseting");

    digitalWrite(ACTUATOR_UP, HIGH);
    digitalWrite(ACTUATOR_DOWN, HIGH);//Deactivate both relays to brake the motor
    delay(18000);  
    digitalWrite(ACTUATOR_UP, HIGH);
    digitalWrite(ACTUATOR_DOWN, LOW);//Deactivate both relays to brake the motor

    ServoCompartment.writeMicroseconds(1800);
    delay(12000);
    ServoCompartment.write(90);  

    
    if (!rsent){
    Wire.beginTransmission(9); // Send to Slave address 9, puzzle 2
    Wire.write("R");       // Send data
    Wire.endTransmission();    // Stop transmitting]
    delay(200);}
    sent1 = false;
    sent2 = false;
    rsent = false;
    Serial.println("Reset Sent");

  }


  delay(500);
}
