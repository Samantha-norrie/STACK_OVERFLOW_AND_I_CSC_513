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
int q0Size = -1;
int *q0Values;

int q1Code = -1;
int q1Index = 0;
int q1Size = -1;
int *q1Values;

int q2Code = -1;
int q2Index = 0;
int q2Size = -1;
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

const int CODE_LIST_SIZE = 6;

const int* QUESTION_VALUES_LIST[CODE_LIST_SIZE] = {
    EDUCATION_VALUES, LANGUAGES_VALUES, OP_SYSTEM_VALUES,
    AI_BENEFIT_VALUES, AI_IN_DEV_VALUES, AI_CHALLENGES_VALUES
};

const int QUESTION_CODE_LIST[] = {EDUCATION, LANGUAGES, OP_SYSTEM, 
  AI_BENEFIT, AI_IN_DEV, AI_CHALLENGES};

// temp workaround
const int QUESTION_VALUES_LIST_SIZES[CODE_LIST_SIZE] = {5, 5, 5, 6, 6, 6};

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
  q0Size = -1;

  q1Code = -1;
  q1Index = 0;
  q1Size = -1;

  q2Code = -1;
  q2Index = 0;
  q2Size = -1;
}

// Handle joystick
void handleHorizontal(bool left) {
  if (left) {
    currentQuestion > 0? currentQuestion = currentQuestion-1: currentQuestion = 2;
  } else {
    currentQuestion < 2? currentQuestion = currentQuestion+1: currentQuestion = 0;
  }
}

void handleVertical(bool up) {
  if(up) {
    if (gameState == 0) {
      questionCodeIndex = questionCodeIndex > 0? questionCodeIndex-1: CODE_LIST_SIZE-1;
    } else {
      if (currentQuestion == 0) {
        q0Index = q0Index > 0? q0Index - 1: q0Size-1;
      } else if (currentQuestion == 1) {
        q1Index = q1Index > 0? q1Index - 1: q1Size-1;
      } else {
        q2Index = q2Index > 0? q2Index - 1: q2Size-1;
      }
    }
  } else {
    if (gameState == 0) {
    questionCodeIndex =  questionCodeIndex != CODE_LIST_SIZE-1? questionCodeIndex+1: 0;
    } else {
      if (currentQuestion == 0) {
        q0Index = q0Index != q0Size-1? q0Index+1: 0;
      } else if (currentQuestion == 1) {
        q1Index = q1Index != q1Size-1? q1Index+1: 0;
      } else {
        q2Index = q2Index != q2Size-1? q2Index+1: 0;
      }
    }
  }
}

void handlePress() {
  if (gameState == 0) {
    if (currentQuestion == 0) {
      q0Code = QUESTION_CODE_LIST[questionCodeIndex];
      q0Values = QUESTION_VALUES_LIST[questionCodeIndex];
      q0Size = QUESTION_VALUES_LIST_SIZES[questionCodeIndex];
      currentQuestion = currentQuestion+1;
    } else if (currentQuestion == 1) {
      q1Code = QUESTION_CODE_LIST[questionCodeIndex];
      q1Values = QUESTION_VALUES_LIST[questionCodeIndex];
      q1Size = QUESTION_VALUES_LIST_SIZES[questionCodeIndex];
      currentQuestion = currentQuestion+1;
    } else {
      q2Code = QUESTION_CODE_LIST[questionCodeIndex];
      q2Values = QUESTION_VALUES_LIST[questionCodeIndex];
      q2Size = QUESTION_VALUES_LIST_SIZES[questionCodeIndex];
      gameState = 1;
    } 
  }else {
    reset();
  }   
}
// Control LEDs
void updateLEDs() {
  digitalWrite(Q0_PIN, currentQuestion == 0? HIGH: LOW);
  digitalWrite(Q1_PIN, currentQuestion == 1? HIGH: LOW);
  digitalWrite(Q2_PIN, currentQuestion == 2? HIGH: LOW);
}

void loop() {
  int press = digitalRead(SW_pin);
  int xCoord = analogRead(X_pin);
  int yCoord = analogRead(Y_pin);

  if (press == 0) {
    handlePress();
  } else if (yCoord > 900) {
    handleVertical(true);
  } else if (yCoord < 200) {
    handleVertical(false);
  } else if (gameState == 1 && xCoord > 900) {
    handleHorizontal(true);
  }else if (gameState == 1 && xCoord < 200) {
    handleHorizontal(false);
  } else {
    updateLEDs();
  }

  if (gameState == 0) {
      sevseg.setNumber(QUESTION_CODE_LIST[questionCodeIndex]);
  } else {
    // sevseg.setNumber(9999);
    if (currentQuestion == 0) {
      // sevseg.setNumber(9999);
      sevseg.setNumber(q0Values[q0Index]);
    } else if (currentQuestion == 1) {
      // sevseg.setNumber(8888);
      sevseg.setNumber(q1Values[q1Index]);
    } else {
      // sevseg.setNumber(7777);
      sevseg.setNumber(q2Values[q2Index]);
    }
  }
  sevseg.refreshDisplay();
}
