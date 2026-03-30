#include "Keypad.h"
#include <Wire.h>

// ===================== KEYPAD SETUP =====================
const byte ROWS = 4;
const byte COLS = 4;
bool PuzzleActive = false;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ===================== PIN DEFINITIONS =====================
#define GREEN_LED 10
#define RED_LED 11
#define RESET_BUTTON 12
#define INTERRUPT_PIN 15

// ===================== PASSWORD SETTINGS =====================
const String PASSWORD  = "247B";
const String RESETCODE = "DDDD";
const int MAX_TRIES    = 3;

// ===================== STATE VARIABLES =====================
String enteredCode = "";
int attempts = 0;

bool solved = false;
bool lockedOut = false;

unsigned long lockoutEnd = 0;

// ===================== SETUP =====================
void setup() {
  Serial.begin(9600);

  Wire.begin(8);                // I2C slave address
  Wire.onReceive(receiveEvent); // I2C receive handler

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN, OUTPUT);
  digitalWrite(INTERRUPT_PIN, HIGH);
  Serial.println("Setup for Puzzle1 complete");
}

// ===================== MAIN LOOP =====================
void loop() {
  handleSerialReset();
  handleLockout();
  if (!PuzzleActive) return;
  // Skip input if locked out
  if (lockedOut) return;
  
  char key = keypad.getKey();
  if (key) handleKey(key);
}



void handleSerialReset() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'r' || cmd == 'R') {
      Serial.println(">>> SERIAL RESET <<<");
      resetSystem();
    }
  }
}

void handleLockout() {
  if (lockedOut && millis() > lockoutEnd) {
    lockedOut = false;
    attempts = 0;
    digitalWrite(RED_LED, LOW);
    Serial.println("Lockout ended.");
  }
}

// ===================== KEYPAD LOGIC =====================
void handleKey(char key) {
  Serial.print("Key: ");
  Serial.println(key);

  flashRedQuick();

  if (key == '*') {
    clearEntry();
    return;
  }

  if (key == '#') {
    checkPassword();
    return;
  }

  if (enteredCode.length() < 4) {
    enteredCode += key;
    Serial.print("Entered: ");
    Serial.println(enteredCode);
  }
}

void clearEntry() {
  enteredCode = "";
  Serial.println("--- Entry Cleared ---");
}

// ===================== PASSWORD CHECK =====================
void checkPassword() {
  Serial.print("Checking: ");
  Serial.println(enteredCode);

  if (enteredCode == PASSWORD) {
    handleCorrectPassword();
  } 
  else if (enteredCode == RESETCODE) {
    resetSystem();
  } 
  else {
    handleWrongPassword();
  }

  enteredCode = "";
}

void handleCorrectPassword() {
  solved = true;

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(INTERRUPT_PIN, LOW);

  Serial.println("*** CORRECT PASSWORD ***");

  victoryFlash();
}

void handleWrongPassword() {
  attempts++;

  Serial.print("WRONG! Attempt ");
  Serial.print(attempts);
  Serial.print("/");
  Serial.println(MAX_TRIES);

  // Initial red feedback
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);

  // Flash count feedback
  for (int i = 1; i < attempts; i++) {
    delay(200);
    digitalWrite(RED_LED, HIGH);
    delay(200);
    digitalWrite(RED_LED, LOW);
  }

  // Lockout condition
  if (attempts >= MAX_TRIES) {
    lockedOut = true;
    lockoutEnd = millis() + 10000;

    digitalWrite(RED_LED, HIGH);
    Serial.println("!!! SYSTEM LOCKED OUT !!!");
  }
}

// ===================== LED EFFECTS =====================
void flashRedQuick() {
  digitalWrite(RED_LED, HIGH);
  delay(30);
  digitalWrite(RED_LED, LOW);
}

void victoryFlash() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(GREEN_LED, LOW);
    delay(100);
    digitalWrite(GREEN_LED, HIGH);
    delay(100);
  }
}

// ===================== RESET =====================
void resetSystem() {
  enteredCode = "";
  attempts = 0;
  solved = false;
  lockedOut = false;

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(INTERRUPT_PIN, HIGH);
  Serial.println("=== KEYPAD SYSTEM READY ===");
  Serial.print("Password: ");
  Serial.println(PASSWORD);
  Serial.println("Press # to submit, * to clear\n");

  Serial.println(">>> SYSTEM RESET <<<");
}

// ===================== I2C RECEIVE =====================
void receiveEvent(int howMany) {
  String word = "";
  while (Wire.available()) {
    char c = Wire.read();
    word += c;
  }
  if (word == "S"){
    resetSystem();
    PuzzleActive = true;
    Serial.print("Functioning");
  }
  Serial.print("Got message");
}