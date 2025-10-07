//www.elegoo.com
//2016.12.09

/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example to change UID of changeable MIFARE card.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to set the UID on a UID changeable MIFARE card.
 * NOTE: for more informations read the README.rst
 * 
 * @author Tom Clement
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN     9
#define SS_PIN      10
#define BUZZER_PIN  8   // passive buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Short Mario theme snippet (NES style)
int melody[] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 392};
int duration[] = {150, 150, 150, 150, 150, 150, 150, 150, 300, 300};

void playMarioTheme() {
  for (int i = 0; i < 10; i++) {
    if (melody[i] == 0) {
      delay(duration[i]);
    } else {
      tone(BUZZER_PIN, melody[i], duration[i]);
      delay(duration[i]);
      noTone(BUZZER_PIN);
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for Serial Monitor
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Card detection sketch with passive buzzer (Mario theme)"));

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Check for a new card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    //delay(50);
    return;
  }

  // Card detected → print UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Play Mario theme on passive buzzer
  playMarioTheme();

  // Halt card and wait before next detection
  mfrc522.PICC_HaltA();
  delay(2000);
}
