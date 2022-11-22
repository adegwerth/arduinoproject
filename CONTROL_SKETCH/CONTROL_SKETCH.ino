#include <SPI.h>
#include <Wire.h>

#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 6
MFRC522 mfrc522(SS_PIN, RST_PIN);

#include "MessageHandler.h"
MessageHandler messageHandler;
HardwareSerial& serialCom = Serial3;

#include "display.h"
Display lcd(0x27, 16, 2);

#include "ButtonHandler.h"

const int CALBUT = 2;
const int LOCBUT = 3;
const int HORBUT = 4;
const int FREBUT = 5;

ButtonHandler buttonHandler(CALBUT, LOCBUT, HORBUT, FREBUT);

const int JOYHORZ = A0;
const int JOYVERT = A1;


const int POTI = A2;

int joyHorz = 0;
int joyVert = 0;

bool calibrationMode = false;
bool calibState = false;

bool unlocked = false;  //CHANGE!! AFTER DEBUGGING

int speedState = 0;

int keyId[]{ 108, 53, 208, 110 };

unsigned long MESSAGE_SEND_INTERVALL = 100;
unsigned long timeSendMessage = 0;
unsigned long time = 0;

void setup() {
  lcd.init();

  Serial.begin(9600);
  serialCom.begin(9600);

  buttonHandler.init();

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  time = millis();

  if (unlocked == true) {
    lcd.displayClear();
    joyRead();

    if (buttonHandler.isButtonPressed(LOCBUT)) {
      unlocked = false;
      lcd.print(0, 0, "Gesperrt", true);
    }



    if (buttonHandler.isButtonPressed(CALBUT) && (calibrationMode == false)) {
      calibrationMode = true;
      calibMode();
      messageHandler.sendMessage(serialCom, '2', " ");
      //delay(200);
    }

    if (calibrationMode == true) {
      lcd.printWait();
    }

    if (buttonHandler.isButtonPressed(HORBUT)) {
      messageHandler.sendMessage(serialCom, '4', " ");
      delay(500);
    }

    messageHandler.pollMessage(serialCom);
    if (messageHandler.isMessageAvailable()) {

      Serial.println("Message available");
      char id;
      const char* data = messageHandler.getMessage(&id);

      // handle message
      switch (id) {
        case 'a':
          {
            Serial.println("data got");
            calibrationMode = false;
            calibMode();
            break;
          }
      }
    }

  } else if (unlocked == false) {
    readRFID();
  }
}

void joyRead() {
  if ((time - timeSendMessage) >= MESSAGE_SEND_INTERVALL) {
    joyHorz = 1023 - analogRead(A0);
    joyVert = 1023 - analogRead(A1);
    speedState = analogRead(POTI);

    char msg[MessageHandler::MESSAGE_BUFF_SIZE];
    sprintf(&(msg[0]), "%d;%d;%d", joyHorz, joyVert, speedState);
    //Serial.println("Message send");
    messageHandler.sendMessage(serialCom, '1', msg);
    timeSendMessage = time;
  }
}

void readRFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    unlocked = false;
    lcd.print(0, 0, "Bitte entsperren");
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  if (mfrc522.uid.uidByte[0] == keyId[0] && mfrc522.uid.uidByte[1] == keyId[1] && mfrc522.uid.uidByte[2] == keyId[2] && mfrc522.uid.uidByte[3] == keyId[3]) {
    unlocked = true;
    lcd.print(0, 0, "Entsperrt", true);
    return;
  }
  unlocked = false;
  lcd.print(0, 0, "Bitte entsperren");
}

void calibMode() {
  if (calibrationMode == true) {
    lcd.print(0, 0, "Kalibrierung:", true);
    lcd.print(1, 0, "gestartet");
  };
  if (calibrationMode == false) {
    lcd.print(0, 0, "Kalibrierung:", true);
    lcd.print(1, 0, "abgeschlossen");
  }
}
