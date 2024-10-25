// #include <SevSeg.h>

// SevSeg sevseg;  // Instantiate a seven-segment controller object

// // Define joystick and LED pins
// const int VRx = A0;
// const int VRy = A1;
// const int SW = 4;

// const int LED1 = A2;
// const int LED2 = A3;
// const int LED3 = A4;

// // Variables to hold joystick readings
// int xValue = 0;
// int yValue = 0;
// int buttonState = 0;

// void setup() {
//   // 7-Segment display setup
//   byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Pins for a, b, c, d, e, f, g, DP
//   byte digitPins[] = {10, 11, 12, 13};           // Pins for digits 1 to 4
  
//   sevseg.begin(COMMON_CATHODE, 4, digitPins, segmentPins);
//   sevseg.setBrightness(90);  // Set brightness (0-100)

//   // Initialize joystick button
//   pinMode(SW, INPUT_PULLUP);
  
//   // Initialize LEDs
//   pinMode(LED1, OUTPUT);
//   pinMode(LED2, OUTPUT);
//   pinMode(LED3, OUTPUT);

//   Serial.begin(9600);
// }

// void loop() {
//   // Call the function to read joystick values
//   readJoystick();

//   // Display joystick values on the 7-segment display
//   int displayValue = (xValue / 10) * 100 + (yValue / 10);
//   sevseg.setNumber(700);
//   sevseg.refreshDisplay();

//   // Control LEDs based on joystick movements
//   if (xValue > 800) {
//     digitalWrite(LED1, HIGH);  // LED1 lights up if joystick pushed to the right
//   } else {
//     digitalWrite(LED1, LOW);
//   }

//   if (yValue > 800) {
//     digitalWrite(LED2, HIGH);  // LED2 lights up if joystick pushed upward
//   } else {
//     digitalWrite(LED2, LOW);
//   }

//   if (buttonState == LOW) {
//     digitalWrite(LED3, HIGH);  // LED3 lights up if joystick button pressed
//   } else {
//     digitalWrite(LED3, LOW);
//   }

//   delay(100);  // Small delay for stability
// }

// // Function to read joystick values
// void readJoystick() {
//   xValue = analogRead(VRx);   // Read X-axis
//   yValue = analogRead(VRy);   // Read Y-axis
//   buttonState = digitalRead(SW);  // Read button state
// }

#include "SevSeg.h"
SevSeg sevseg;

// Joystick Pins
const int SW_pin = A5;
const int X_pin = A0;
const int Y_pin = A1;

// LED Pins
const int Q0_PIN = A2;
const int Q1_PIN = A3;
const int Q2_PIN = A4;
const int IR_RECEIVE_PIN = 10;

// 4-Digit 7 Segment Pins
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Pins for a, b, c, d, e, f, g, DP
  byte digitPins[] = {10, 11, 12, 13}; 

const byte numDigits = 4;

// Game state
/*
Question select -> 0
Question Viewing -> 1
*/
int gameState = 0;

int currentQuestion = 0;

int q0Code = -1;
int q0Index = 0;
int *q0Values;

int q1Code = -1;
int q1Index = 0;
int *q1Values;

int q2Code = -1;
int q2Index = 0;
int *q2Values;

// Question Codes
const int EDUCATION = 1100;
const int LANGUAGES = 2101;
const int OP_SYSTEM = 2111;
const int AI_BENEFIT = 3201;
const int AI_IN_DEV = 3204;
const int AI_CHALLENGES = 3303;

const int EDUCATION_VALUES[] = {10, 13, 41, 26, 11};
const int LANGUAGES_VALUES[] = {62, 53, 51, 51, 39};
const int OP_SYSTEM_VALUES[] = {60, 32,28,18, 17};
const int AI_BENEFIT_VALUES[] = {81, 62, 59, 30, 25, 8};
const int AI_IN_DEV_VALUES[] = {82, 68, 57, 40, 13, 27};
const int AI_CHALLENGES_VALUES[] = {66, 26, 32, 31, 63, 13};

const int QUESTION_VALUES_LIST[][7] = {EDUCATION_VALUES, LANGUAGES_VALUES, OP_SYSTEM_VALUES,
AI_BENEFIT_VALUES, AI_IN_DEV_VALUES, AI_CHALLENGES_VALUES};

const int QUESTION_CODE_LIST[] = {EDUCATION, LANGUAGES, OP_SYSTEM, 
  AI_BENEFIT, AI_IN_DEV, AI_CHALLENGES};

int questionCodeIndex = 0;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  pinMode(Q0_PIN, OUTPUT);
  pinMode(Q1_PIN, OUTPUT);
  pinMode(Q2_PIN, OUTPUT);

  bool resistorsOnSegments = 0; 
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void reset() {
  gameState = 0;
  currentQuestion = 0;

  q0Code = -1;
  q0Index = 0;

  q1Code = -1;
  q1Index = 0;

  q2Code = -1;
  q2Index = 0;
}

// Handle joystick
void handleJoystick() {
  int press = digitalRead(SW_pin);
  int xCoord = analogRead(X_pin);
  int yCoord = analogRead(Y_pin);

  // if (xCoord <= 400) {
  //   currentQuestion > 0? currentQuestion = currentQuestion-1: currentQuestion = 2;
  // } else if (xCoord >= 700) {
  //   currentQuestion < 2? currentQuestion = currentQuestion+1: currentQuestion = 0;
  // }

  if (yCoord >= 700) {
    if (gameState == 0) {
      questionCodeIndex < 0? questionCodeIndex = questionCodeIndex-1: questionCodeIndex;
    } else {
      if (currentQuestion == 0) {
        q0Index > 0? q0Index = q0Index - 1: q0Index = sizeof(q0Values) / sizeof(q0Values[0])-1;
      } else if (currentQuestion == 1) {
        q1Index > 0? q1Index = q1Index - 1: q1Index = sizeof(q1Values) / sizeof(q1Values[0])-1;
      } else {
        q2Index > 0? q2Index = q2Index - 1: q2Index = sizeof(q2Values) / sizeof(q2Values[0])-1;
      }
    }
  } else if (yCoord <= 300) {
    if (gameState == 0) {
      questionCodeIndex < 0? questionCodeIndex = questionCodeIndex-1: questionCodeIndex;
    } else {
      if (currentQuestion == 0) {
        q0Index > 0? q0Index = q0Index - 1: q0Index = sizeof(q0Values) / sizeof(q0Values[0])-1;
      } else if (currentQuestion == 1) {
        q1Index > 0? q1Index = q1Index - 1: q1Index = sizeof(q1Values) / sizeof(q1Values[0])-1;
      } else {
        q2Index > 0? q2Index = q2Index - 1: q2Index = sizeof(q2Values) / sizeof(q2Values[0])-1;
      }
    }    
  }

  if (press == 0) {
    currentQuestion > 0? currentQuestion = currentQuestion-1: currentQuestion = 2;
    // if (gameState == 0) {
    //   if (q0Code != -1 && q1Code != -1 && q2Code != -1) {
    //     gameState = 1;
    //   } else if (currentQuestion == 0) {
    //     q0Code = QUESTION_CODE_LIST[questionCodeIndex];
    //     q0Values = QUESTION_VALUES_LIST[questionCodeIndex];
    //     currentQuestion = currentQuestion+1;
    //   } else if (currentQuestion == 1) {
    //     q1Code = QUESTION_CODE_LIST[questionCodeIndex];
    //     q1Values = QUESTION_VALUES_LIST[questionCodeIndex];
    //     currentQuestion = currentQuestion+1;
    //   } else {
    //     q2Code = QUESTION_CODE_LIST[questionCodeIndex];
    //     q2Values = QUESTION_VALUES_LIST[questionCodeIndex];
    //   }
    // } else {
    //   reset();
    // } 
  }

}
// Control LEDs
void updateLEDs() {
  digitalWrite(Q0_PIN, currentQuestion == 0? HIGH: LOW);
  digitalWrite(Q1_PIN, currentQuestion == 1? HIGH: LOW);
  digitalWrite(Q2_PIN, currentQuestion == 2? HIGH: LOW);

}

void updateScreen() {
  if (gameState == 0) {
      sevseg.setNumber(QUESTION_CODE_LIST[questionCodeIndex]);
  } else {
    if (currentQuestion == 0) {
      sevseg.setNumber(q0Values[q0Index]);
    } else if (currentQuestion == 1) {
      sevseg.setNumber(q1Values[q1Index]);
    } else {
      sevseg.setNumber(q2Values[q2Index]);
    }
  }
  sevseg.refreshDisplay();
}

void loop() {
  Serial.print("curr question \n");
  Serial.print(currentQuestion);
  handleJoystick();
  updateScreen();
  updateLEDs();
  Serial.print("curr question \n");
  Serial.print(currentQuestion);
  // currentQuestion > 0? currentQuestion = currentQuestion-1: currentQuestion = 2;
}
