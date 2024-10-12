const int Q1_PIN = 13;
const int Q2_PIN = 12;
const int Q3_PIN = 11;
void setup() {
  // put your setup code here, to run once:
  pinMode(Q1_PIN, OUTPUT);
  pinMode(Q2_PIN, OUTPUT);
  pinMode(Q3_PIN, OUTPUT);
  Serial.begin(9600);
}

// Handle remote

// Control lights

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Q1_PIN, HIGH);
}
