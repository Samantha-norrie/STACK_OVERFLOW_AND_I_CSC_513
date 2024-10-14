#include "SevSeg.h"
SevSeg sevseg;

// LED Pins
const int Q0_PIN = 13;
const int Q1_PIN = 12;
const int Q2_PIN = 11;
const int IR_RECEIVE_PIN = 10;

// 4-Digit 7 Segment Pins
const byte segmentPins[] = {1, 2, 3, 4, 5, 6, 7,8}; // Segment pins A-G
const byte digitPins[] = {A0, A1, A2, A3}; // Digit pins D1-D4

const byte numDigits = 4;

// Game state
/*
Question select -> 0
Question Viewing -> 1
*/
int gameState = 0;

int currentQuestion = 0;



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

const int QUESTION_CODE_LIST[] = {EDUCATION, LANGUAGES, OP_SYSTEM, 
  AI_BENEFIT, AI_IN_DEV, AI_CHALLENGES};
void setup() {
  // put your setup code here, to run once:
  pinMode(Q0_PIN, OUTPUT);
  pinMode(Q1_PIN, OUTPUT);
  pinMode(Q2_PIN, OUTPUT);

  bool resistorsOnSegments = 0; 
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

// Handle joystick

// Control LEDs
void updateLEDs() {
  digitalWrite(Q0_PIN, currentQuestion == 0? HIGH: LOW);
  digitalWrite(Q1_PIN, currentQuestion == 1? HIGH: LOW);
  digitalWrite(Q2_PIN, currentQuestion == 2? HIGH: LOW);
}



void loop() {
  sevseg.setNumber(1100);
  sevseg.refreshDisplay();

}
