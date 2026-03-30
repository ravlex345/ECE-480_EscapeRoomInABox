#include "Wire.h"

int pushButton1 = 2;
int pushButton2 = 3;
int pushButton3 = 4;
int pushButton4 = 5;


int pushButton5 = 6;

enum buttonStates {initial,correct1, wrong1, correct2, wrong2, correct3, wrong3, correct4, wrong4};
buttonStates state;

int redLight = 8;
int greenLight =9;

bool PuzzleActive = false;

#define INTERRUPT_PIN 11

void printState(buttonStates s) {
  switch(s) {
    case initial: Serial.println("initial"); break;
    case correct1: Serial.println("correct1"); break;
    case wrong1: Serial.println("wrong1"); break;
    case correct2: Serial.println("correct2"); break;
    case wrong2: Serial.println("wrong2"); break;
    case correct3: Serial.println("correct3"); break;
    case wrong3: Serial.println("wrong3"); break;
    case correct4: Serial.println("correct4"); break;
    case wrong4: Serial.println("wrong4"); break;
  }
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  pinMode( pushButton1, INPUT_PULLUP);
  pinMode( pushButton2, INPUT_PULLUP);
  pinMode( pushButton3, INPUT_PULLUP);
  pinMode( pushButton4, INPUT_PULLUP);
  pinMode( pushButton5, INPUT_PULLUP);

  pinMode (redLight , OUTPUT);
  pinMode (greenLight , OUTPUT);

  pinMode( INTERRUPT_PIN, OUTPUT);
  state = initial;
  digitalWrite(redLight, LOW);
  digitalWrite(greenLight, LOW);
  digitalWrite(INTERRUPT_PIN, HIGH);

  Wire.begin(9);                // I2C slave address
  Wire.onReceive(receiveEvent); // I2C receive handler

  Serial.println("Setup for puzzle 2 done");
}

void loop() {
  if (!PuzzleActive) return;

  // put your main code here, to run repeatedly:
  bool value1 = digitalRead(pushButton1); 
  bool value2 = digitalRead(pushButton2);
  bool value3 = digitalRead(pushButton3);
  bool value4 = digitalRead(pushButton4);
  bool value5 = digitalRead(pushButton5);
  
//  Serial.println(value1);
//  Serial.println(value2);
//  Serial.println(value3);
//  Serial.println(value4);

  if (value5 == 0)
  {
    state = initial;
  }
  printState(state);
  Serial.println(value5);

delay(300);

  switch(state){
    case initial:
        digitalWrite(redLight, LOW);
        digitalWrite(greenLight, LOW);

      if (value1 == 0) {
        state = correct1;
        flashRedQuick();
      }
      else if (value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong1;
        flashRedQuick();
      }
      break;
      
    case correct1:
      
      if (value2 == 0) {
        state = correct2;
        flashRedQuick();
      }
      else if (value1 == 0 or value3 == 0 or value4 == 0){
        state = wrong2;
        flashRedQuick();
      }
      break;

   case correct2:
    if (value3 == 0) {
      state = correct3;
      flashRedQuick();
    }
    else if (value2 == 0 or value1 == 0 or value4 == 0){
      state = wrong2;
      flashRedQuick();
    }
    break;

    case correct3:
    if (value4 == 0){
      state = correct4;
    
      flashRedQuick();
    }
    else if (value2 == 0 or value3 == 0 or value1 == 0){
      state = wrong4;
      flashRedQuick();
    }
    break;
    
    case correct4:
      digitalWrite(greenLight, HIGH);
      digitalWrite(redLight, LOW);
      digitalWrite(INTERRUPT_PIN, LOW);
      break;
      
    case wrong1:
      if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong2;
        flashRedQuick();
       }
      break;

    case wrong2:
    
    if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong3;
        flashRedQuick();
       }
       break;

    case wrong3:
      if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong4;
        flashRedQuick();
       }
       break;
      
    case wrong4:
      digitalWrite(redLight, HIGH);
      digitalWrite(greenLight, LOW);
      break;
  }

  
}

void flashRedQuick() {
// Quick red flash for keypress feedback
digitalWrite(redLight, HIGH);
delay(30);
digitalWrite(redLight, LOW);
}

// ===================== I2C RECEIVE =====================
void receiveEvent(int howMany) {
  String word = "";
  while (Wire.available()) {
    char c = Wire.read();
    word += c;
  }
  if (word == "S"){
    PuzzleActive = true;
  }
  if (word == "R"){
    PuzzleActive = false;
    state = initial;
    digitalWrite(INTERRUPT_PIN, HIGH);
    digitalWrite(redLight, LOW);
    digitalWrite(redLight, HIGH);

  }
  Serial.print("Got message");
}