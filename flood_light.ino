#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN       9
#define SS_PIN        10
#define BUZZER_PIN    8   // Passive buzzer
#define LED_PIN_EVEN  2
#define LED_PIN_ODD   3

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Short Mario theme snippet (NES style)
int melody[]   = {659, 659, 0, 659, 0, 523, 659, 0, 784, 392};
int duration[] = {150, 150, 150, 150, 150, 150, 150, 150, 300, 400};

int noteIndex = 0;               // Current note index
unsigned long noteStart = 0;     // Timestamp when the note started
bool notePlaying = false;        // Is a note currently playing?

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Card detection sketch with non-blocking Mario theme"));

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_EVEN, OUTPUT);
  pinMode(LED_PIN_ODD, OUTPUT);
  digitalWrite(LED_PIN_EVEN, LOW);
  digitalWrite(LED_PIN_ODD, LOW);
}

void loop() {
  // Check for new RFID card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print(F("Card UID:"));
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Start playing melody
    noteIndex = 0;
    notePlaying = false;

    mfrc522.PICC_HaltA(); // Halt card
  }

  // Play the Mario theme in a non-blocking way
  playMarioThemeNonBlocking();
}

// Non-blocking melody playback
void playMarioThemeNonBlocking() {
  if (noteIndex >= 10) return; // Melody finished

  unsigned long currentMillis = millis();

  if (!notePlaying) {
    // Start a new note
    if (melody[noteIndex] != 0) {
      tone(BUZZER_PIN, melody[noteIndex]);
      if (noteIndex % 2 == 0) {
        digitalWrite(LED_PIN_EVEN, HIGH);
        digitalWrite(LED_PIN_ODD, LOW);
      } else {
        digitalWrite(LED_PIN_EVEN, LOW);
        digitalWrite(LED_PIN_ODD, HIGH);
      }
    }
    noteStart = currentMillis;
    notePlaying = true;
  }

  // Check if current note duration is over
  if (currentMillis - noteStart >= duration[noteIndex]) {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN_EVEN, LOW);
    digitalWrite(LED_PIN_ODD, LOW);
    noteIndex++;          // Move to next note
    notePlaying = false;  // Ready to start next note
  }
}

