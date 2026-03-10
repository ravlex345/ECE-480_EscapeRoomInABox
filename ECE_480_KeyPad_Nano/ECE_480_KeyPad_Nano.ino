#include "Keypad.h"
#include <Wire.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pins
#define GREEN_LED 10 // Correct password
#define RED_LED 11 // Wrong password / keypress feedback
#define RESET_BUTTON 12 // Physical reset button
#define INTERRUPT_PIN 15
// Password settings
const String PASSWORD = "247B";
const String RESETCODE = "DDDD";
const int MAX_TRIES = 3;

String enteredCode = "";
int attempts = 0;
bool solved = false;
bool lockedOut = false;
unsigned long lockoutEnd = 0;

void setup() {
  Serial.begin(9600);
    Wire.begin(8);                // Join I2C bus as Slave with address 8
  Wire.onReceive(receiveEvent); // Register a function to handle incoming data

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN, OUTPUT);
  resetSystem();
  Serial.println("=== KEYPAD TEST ===");
  Serial.print("Password: ");
  Serial.println(PASSWORD);
  Serial.println("Red flashes on keypress");
  Serial.println("Press # to submit, * to clear");
  Serial.println();

  digitalWrite(INTERRUPT_PIN, HIGH);

}

void loop() {
// Check reset button
if (digitalRead(RESET_BUTTON) == LOW) {
delay(50);
if (digitalRead(RESET_BUTTON) == LOW) {
Serial.println(">>> RESET <<<");
resetSystem();
while (digitalRead(RESET_BUTTON) == LOW);
}
}
// Check serial reset
if (Serial.available()) {
char cmd = Serial.read();
if (cmd == 'r' || cmd == 'R') {
resetSystem();
}
}
// Handle lockout
if (lockedOut && millis() > lockoutEnd) {
lockedOut = false;
attempts = 0;
digitalWrite(RED_LED, LOW);
Serial.println("Lockout ended.");
}
// Skip if solved or locked out
if ( lockedOut) return;
char key = keypad.getKey();
if (key) handleKey(key);
}

void handleKey(char key) {
Serial.print("Key: ");
Serial.println(key);
// RED FLASH on every keypress
flashRedQuick();
// Clear with *
if (key == '*') {
enteredCode = "";
Serial.println("--- Cleared ---");
return;
}
// Submit with #
if (key == '#') {
checkPassword();
return;
}
// Add to code
if (enteredCode.length() < 4) {
enteredCode += key;
Serial.print("Entered: ");
Serial.println(enteredCode);
}
}

void checkPassword() {
Serial.print("Checking: ");
Serial.println(enteredCode);
if (enteredCode == PASSWORD) {
// CORRECT! Green on, red off
solved = true;
digitalWrite(GREEN_LED, HIGH);
digitalWrite(RED_LED, LOW);
Serial.println("*** CORRECT! ***");
digitalWrite( INTERRUPT_PIN, LOW);
victoryFlash();
} 
else if (enteredCode == RESETCODE) {
  resetSystem();
  }
else {
// WRONG! Red stays on
attempts++;
Serial.print("WRONG! Try ");
Serial.print(attempts);
Serial.println("/3");
digitalWrite(RED_LED, HIGH);
delay(500);
digitalWrite(RED_LED, LOW);
// Build up: 1 flash = 1 wrong, 2 flashes = 2 wrongs, etc
for (int i = 1; i < attempts; i++) {
delay(200);
digitalWrite(RED_LED, HIGH);
delay(200);
digitalWrite(RED_LED, LOW);
}
if (attempts >= MAX_TRIES) {
lockedOut = true;
lockoutEnd = millis() + 10000;
digitalWrite(RED_LED, HIGH);
Serial.println("!!! LOCKED OUT !!!");
}
}
enteredCode = "";
}

void flashRedQuick() {
// Quick red flash for keypress feedback
digitalWrite(RED_LED, HIGH);
delay(30);
digitalWrite(RED_LED, LOW);
}

void resetSystem() {
enteredCode = "";
attempts = 0;
solved = false;
lockedOut = false;
digitalWrite(GREEN_LED, LOW);
digitalWrite(RED_LED, LOW);
Serial.println(">>> RESET <<<");
digitalWrite( INTERRUPT_PIN, HIGH);

}

void victoryFlash() {
for (int i = 0; i < 10; i++) {
digitalWrite(GREEN_LED, LOW);
delay(100);
digitalWrite(GREEN_LED, HIGH);
delay(100);
}
}

void receiveEvent(int howMany) {
  while (Wire.available()) { 
    char c = Wire.read();      // Receive byte as a character
    Serial.print(c);
  }
  Serial.println();
}
