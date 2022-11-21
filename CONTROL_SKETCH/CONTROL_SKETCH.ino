#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "MessageHandler.h"
MessageHandler messageHandler;
HardwareSerial& serialCom = Serial3;
#define SS_PIN 53
#define RST_PIN 6

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

const int JOYHORZ = A0;
const int JOYVERT = A1;

const int CALBUT = 2;
const int LOCBUT = 3;
const int HORBUT = 4;
const int FREBUT = 5;

const int POTI = A2;

int joyHorz = 0;
int joyVert = 0;

bool calibButton = false;
bool calibrationMode = false;
bool calibState = false;
bool lockButton = false;
bool hornButton = false;
bool freeButton = false;

bool unlocked = false;  //CHANGE!! AFTER DEBUGGING

int speedState = 0;

int keyId[]{ 108, 53, 208, 110 };

long msgCount = 0;

unsigned long MESSAGE_SEND_INTERVALL = 100;
unsigned long timeSendMessage = 0;
unsigned long time = 0;

unsigned long DISPLAY_CLEAN = 5000;
unsigned long timeLastClean = 0;

char strWait[] = "|/-/";

unsigned long lastTimeWaitIndicator = 0;
int curWaitIndicator = 0;

void printWait()
{
  if ((time - lastTimeWaitIndicator) >= 500) {
    lastTimeWaitIndicator = time;
    lcd.setCursor(15, 1);
    lcd.print(strWait[curWaitIndicator++]);

    if (curWaitIndicator > (strlen(strWait) - 1)) {
      curWaitIndicator = 0;
    }
  }
}

void lcdPrint(int row, int col, const char* msg, bool clean = false);

void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  serialCom.begin(9600);
  Serial2.begin(9600);


  pinMode(CALBUT, INPUT);
  pinMode(FREBUT, INPUT);
  pinMode(HORBUT, INPUT);
  pinMode(LOCBUT, INPUT);

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  time = millis();

  if (unlocked == true) {
    displayClear();
    buttonRead();
    joyRead();

    if (lockButton == true) {
      unlocked = false;
      lcdPrint(0, 0, "Gesperrt", true);
    }



    if (calibButton == true) {
      calibrationMode = true;
      calibMode();
      messageHandler.sendMessage(serialCom, '2', " ");
    }

    if (calibrationMode == true) {
      printWait();
    }

    if (hornButton == true) {
      messageHandler.sendMessage(serialCom, '4', " ");
    }

    messageHandler.pollMessage(Serial2);
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

    if (freeButton == true) {
      barrierDetected();
    }

  } else if (unlocked == false) {
    readRFID();
  }
}

void buttonRead() {
  calibButton = digitalRead(CALBUT);
  lockButton = digitalRead(LOCBUT);
  hornButton = digitalRead(HORBUT);
  freeButton = digitalRead(FREBUT);
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
    lcdPrint(0, 0, "Bitte entsperren");
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  if (mfrc522.uid.uidByte[0] == keyId[0] && mfrc522.uid.uidByte[1] == keyId[1] && mfrc522.uid.uidByte[2] == keyId[2] && mfrc522.uid.uidByte[3] == keyId[3]) {
    unlocked = true;
    lcdPrint(0, 0, "Entsperrt", true);
    return;
  }
  unlocked = false;
  lcdPrint(0, 0, "Bitte entsperren");
}


void progressIndicator() {
  int progressPos = 3;


  for (int i = 0; i < 100; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Progress: " + String(i) + "%");

    lcd.setCursor(2, 1);
    lcd.print("[");
    lcd.setCursor(13, 1);
    lcd.print("]");

    delay(250);

    if (((i % 10) == 0) && (i != 0)) {
      lcd.setCursor(progressPos, 1);
      lcd.print("=");
      if (progressPos <= 12) {
        progressPos += 1;
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done!");
  delay(1000);
  lcd.clear();
}

void displayClear() {
  if ((time - timeLastClean) >= DISPLAY_CLEAN) {
    lcd.clear();
    timeLastClean = time;
  }
}


void lcdPrint(int row, int col, const char* msg, bool clean = false) {
  if (clean == true) {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(msg);
  timeLastClean = time;
}

void calibMode() {
  if (calibrationMode == true) {
    lcdPrint(0, 0, "Kalibrierung:", true);
    lcdPrint(1, 0, "gestartet");
  };
  if (calibrationMode == false) {
    lcdPrint(0, 0, "Kalibrierung:", true);
    lcdPrint(1, 0, "abgeschlossen");
  }
}

void barrierDetected() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hindernis");
  lcd.setCursor(0, 1);
  lcd.print("erkannt");
}