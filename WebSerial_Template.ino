/*

Basic code template for the ESP32 to debug or print serial over WIFI.

Required hardware:
-ESP32

Libaries:
-WebSerial by Ayush Sharma
-ESPAsyncWebServer by lacamera
-AsyncTCP by dvarrel

Tutorial Guidance:
- https://randomnerdtutorials.com/esp32-webserial-library/

*/


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <esp_adc_cal.h>

AsyncWebServer server(80);

const char* ssid = "yourwifiname";          // Your WiFi SSID
const char* password = "yourwifipassword";  // Your WiFi Password

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);  
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);

  server.begin();
  Serial.println("Ready");
}


void loop() {
  WebSerial.println("Hello!");
  delay(5000);
}