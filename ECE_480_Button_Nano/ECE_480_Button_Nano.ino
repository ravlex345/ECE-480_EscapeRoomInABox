#include "Wire.h"

// ===================== BUTTON PINS =====================
int pushButton1 = 12;
int pushButton2 = 10;
int pushButton3 = 6;
int pushButton4 = 4;
int pushButton5 = 8;

// Tuple-style grouping (array in Arduino)
int buttons[4] = {pushButton1, pushButton2, pushButton3, pushButton4};

// ===================== STATE =====================
enum buttonStates {initial, correct1, wrong1, correct2, wrong2, correct3, wrong3, correct4, wrong4};
buttonStates state;

bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};

// ===================== OUTPUTS =====================
int redLight = 2;
int greenLight = 3;

#define INTERRUPT_PIN A1

bool PuzzleActive = false;

// ===================== FUNCTION DECLARATIONS =====================
void handleButtonPress(int i);
void performFullReset();

// ===================== SETUP =====================
void setup() {
  Serial.begin(9600);

  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP);
  pinMode(pushButton4, INPUT_PULLUP);
  pinMode(pushButton5, INPUT_PULLUP);

  pinMode(redLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
  pinMode(INTERRUPT_PIN, OUTPUT);

  performFullReset();

  Wire.begin(9);
  Wire.onReceive(receiveEvent);

  Serial.println("Puzzle 2 Ready");
}

// ===================== MAIN LOOP =====================
void loop() {

  // 1. Physical Reset Button
  if (digitalRead(pushButton5) == LOW) {
    performFullReset();
    delay(200); // debounce
    Serial.println(5);
    return;
    
  }

  // 2. Only process if active
  if (!PuzzleActive) return;

  // 3. Check buttons (edge detection)
  for (int i = 0; i < 4; i++) {
    bool currentState = digitalRead(buttons[i]);

    if (currentState == LOW && lastButtonState[i] == HIGH) {
      handleButtonPress(i);
      delay(50); // small debounce
      Serial.println(i);
    }

    lastButtonState[i] = currentState;
  }
}

// ===================== BUTTON LOGIC =====================
void handleButtonPress(int i) {

  switch(state) {

    case initial:
      if (i == 0) state = correct1;
      else state = wrong1;
      break;

    case correct1:
      if (i == 1) state = correct2;
      else state = wrong2;
      break;

    case correct2:
      if (i == 2) state = correct3;
      else state = wrong3;
      break;

    case correct3:
      if (i == 3) state = correct4;
      else state = wrong4;
      break;

    case correct4:
      // already solved
      break;

    case wrong1:
      state = wrong2;
      break;

    case wrong2:
      state = wrong3;
      break;

    case wrong3:
      state = wrong4;
      break;

    case wrong4:
      // stay failed
      break;
  }

  // ===================== OUTPUT RESPONSE =====================
  if (state == correct4) {
    digitalWrite(greenLight, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(INTERRUPT_PIN, LOW);
  }
  else if (state == wrong4) {
    digitalWrite(redLight, HIGH);
    digitalWrite(greenLight, LOW);
  }
  else {
    // intermediate feedback (quick flash)
    digitalWrite(redLight, HIGH);
    delay(30);
    digitalWrite(redLight, LOW);
  }

  //Serial.println(state);

}

// ===================== RESET =====================
void performFullReset() {
  state = initial;
  digitalWrite(redLight, LOW);
  digitalWrite(greenLight, LOW);
  digitalWrite(INTERRUPT_PIN, HIGH);
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = HIGH;
  }

}

// ===================== I2C RECEIVE =====================
void receiveEvent(int howMany) {
  if (Wire.available()) {
    char c = Wire.read();

    if (c == 'S') {
      PuzzleActive = true;
      performFullReset();
    } 
    else if (c == 'R') {
      PuzzleActive = false;
      performFullReset();
    }
  }
}