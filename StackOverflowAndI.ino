#include "SevSeg.h"
SevSeg sevseg;

// Joystick Pins
const int SW_pin = A5;
const int X_pin = A0;
const int Y_pin = A1;

// Joystick debounce handling
const unsigned long DEBOUNCE_DELAY = 50; // Adjust debounce time as needed
unsigned long lastPressTime = 0;
unsigned long lastJoystickTime = 0;

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

const int EDUCATION_VALUES[] = {"1 10", "2 13", "3 41", "4 26", "5 11"};
const int LANGUAGES_VALUES[] = {"1 62", "2 53", "3 51", "4 51", "5 39"};
const int OP_SYSTEM_VALUES[] = {"1 60", "2 32","3 28","4 18", "5 17"};
const int AI_BENEFIT_VALUES[] = {"1 81", "2 62", "3 59", "4 30", "5 25", "6  8"};
const int AI_IN_DEV_VALUES[] = {"1 82", "2 68", "3 57", "4 40", "5 13", "6 27"};
const int AI_CHALLENGES_VALUES[] = {"1 66", "2 26", "3 32", "4 31", "5 63", "6 13"};

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

  unsigned long currentTime = millis();

  if (currentTime - lastJoystickTime > DEBOUNCE_DELAY) {
    if (press == 0) {
      handlePress();
      lastJoystickTime = currentTime;
    } else if (yCoord > 900) {
      handleVertical(true);
      lastJoystickTime = currentTime;
    } else if (yCoord < 200) {
      handleVertical(false);
      lastJoystickTime = currentTime;
    } else if (gameState == 1 && xCoord > 900) {
      handleHorizontal(true);
      lastJoystickTime = currentTime;
    }else if (gameState == 1 && xCoord < 200) {
      handleHorizontal(false);
      lastJoystickTime = currentTime;
    } 
  } else {
    updateLEDs();
  }

  if (gameState == 0) {
      sevseg.setNumber(QUESTION_CODE_LIST[questionCodeIndex]);
  } else {
    if (currentQuestion == 0) {
      sevseg.setChars(q0Values[q0Index]);
    } else if (currentQuestion == 1) {
      sevseg.setChars(q1Values[q1Index]);
    } else {
      sevseg.setChars(q2Values[q2Index]);
    }
  }
  sevseg.refreshDisplay();
}
