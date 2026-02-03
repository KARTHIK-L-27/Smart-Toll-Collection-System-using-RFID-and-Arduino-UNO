#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
Servo gate;

int buzzer = 8;

void setup() {
  SPI.begin();
  rfid.PCD_Init();
  gate.attach(6);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  gate.write(0); // Gate closed
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("Vehicle RFID Detected: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  Serial.println("Toll Deducted. Gate Opening...");
  digitalWrite(buzzer, HIGH);
  gate.write(90);  // Open gate
  delay(3000);

  gate.write(0);   // Close gate
  digitalWrite(buzzer, LOW);

  rfid.PICC_HaltA();
}
