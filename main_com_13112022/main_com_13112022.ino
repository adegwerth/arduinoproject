#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

#include "MessageHeader.h"

MessageHandler messageHandler;

// create a web server listening on port 80
AsyncWebServer server(80);

// the wlan SSID
const char* ssid = "adstest";
// the wlan password -> none
const char* password = NULL;

// Message callback. This is called if we receive a message from 
// the web.
void recvMsg(uint8_t *data, size_t len) { 
/*
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
*/
  int id;
  char buff[MessageHandler::MESSAGE_BUFF_SIZE];
  sscanf((const char*)data, "%d;%s", &id, &(buff[0]));
  messageHandler.sendMessage(Serial, (byte)id, buff);

  // TODO: send message TO serial port
}

void setup() {
    Serial.begin(9600);

    // enable the internal LED
    pinMode(LED_BUILTIN, OUTPUT);
    // set the LED to off
    digitalWrite(LED_BUILTIN, HIGH);

    // initialize the WIFI
    WiFi.softAP(ssid, password);

    /*
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    */

    // WebSerial is accessible at "<IP Address>/webserial" in browser
    WebSerial.begin(&server);

    // Attach Message Callback
    WebSerial.msgCallback(recvMsg);

    // start the webserver
    server.begin();
}

void loop() {
  messageHandler.pollMessage(Serial);
  if (messageHandler.isMessageAvailable()) {
    byte id;
    const char* data = messageHandler.getMessage(&id);
    WebSerial.print(id);
    WebSerial.print(";");
    WebSerial.print(data);
    //WebSerial.println(data);

    // switch the internal LED to give a visual feeback
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) > 0 ? LOW : HIGH);
  }
}

/*
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "adstest";
const char* password =  NULL;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  if(type == WS_EVT_CONNECT) {

    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");

  } else if(type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");

  } else if(type == WS_EVT_DATA) {
    Serial.println("Received data.");
    // TODO: send the data to the serial bus
  }
}

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();
}

void loop(){
  // TODO: check if message from serial
  // ws.textAll("test")
}
*/