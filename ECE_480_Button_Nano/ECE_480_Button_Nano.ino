
int pushButton1 = 14;
int pushButton2 = 15;
int pushButton3 = 16;
int pushButton4 = 17;
int pushButton5 = 2;

enum buttonStates {initial,correct1, wrong1, correct2, wrong2, correct3, wrong3, correct4, wrong4};
buttonStates state;

int redLight = 6;
int greenLight = 7;

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
  
  pinMode( pushButton1, INPUT);
  pinMode( pushButton2, INPUT);
  pinMode( pushButton3, INPUT);
  pinMode( pushButton4, INPUT);
  pinMode( pushButton5, INPUT);

  pinMode (redLight , OUTPUT);
  pinMode (greenLight , OUTPUT);

  state = initial;
}

void loop() {
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
      }
      else if (value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong1;
      }
      break;
      
    case correct1:
      if (value2 == 0) {
        state = correct2;
      }
      else if (value1 == 0 or value3 == 0 or value4 == 0){
        state = wrong2;
      }
      break;

   case correct2:
    if (value3 == 0) {
      state = correct3;
    }
    else if (value2 == 0 or value2 == 0 or value4 == 0){
      state = wrong2;
    }
    break;

    case correct3:
    if (value4 == 0){
      state = correct4;
    }
    else if (value2 == 0 or value3 == 0 or value1 == 0){
      state = wrong4;
    }
    break;
    
    case correct4:
      digitalWrite(greenLight, HIGH);
      
      break;
      
    case wrong1:
      if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong2;
       }
      break;

    case wrong2:
    
    if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong3;
       }
       break;

    case wrong3:
      if (value1 == 0 or value2 == 0 or value3 == 0 or value4 == 0){
        state = wrong4;
       }
       break;
      
    case wrong4:
      digitalWrite(redLight, HIGH);
      break;
  }


  
}
