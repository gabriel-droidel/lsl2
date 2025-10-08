/*****************************************/
// Pin definitions
const int ball_switch = 8;   // Ball switch input
const int buzzer = 4;        // Active buzzer output

// Timing variables
unsigned long alarmStartTime = 0;  // Time when alarm started
const unsigned long alarmDuration = 250; // Alarm duration in milliseconds 

void setup() {
  pinMode(buzzer, OUTPUT);         // Set buzzer as output
  pinMode(ball_switch, INPUT);     // Set ball switch as input
  digitalWrite(ball_switch, HIGH); // Enable internal pull-up resistor
}

void loop() {
  int digitalVal = digitalRead(ball_switch); // Read ball switch state

  // If ball switch is triggered (LOW)
  if (digitalVal == LOW) {
    alarmStartTime = millis();   // Start the alarm
  }

  // Keep buzzer on for the duration after last trigger
  if (millis() - alarmStartTime < alarmDuration) {
    digitalWrite(buzzer, HIGH);  // Buzzer ON
  } else {
    digitalWrite(buzzer, LOW);   // Buzzer OFF
  }
}
