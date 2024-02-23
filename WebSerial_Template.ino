#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <esp_adc_cal.h>

AsyncWebServer server(80);

const char* ssid = "JvvHuisAP";          // Your WiFi SSID
const char* password = "aweMAsekind";  // Your WiFi Password

#define BOARD_BAT_ADC_PIN    35

float batteryVoltage = 0.0;

uint32_t readADC_Cal(int ADC_Raw)
{
    esp_adc_cal_characteristics_t adc_chars;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    return (esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
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
  //WebSerial.msgCallback(recvMsg);

  server.begin();
  Serial.println("Ready");
}

char buf[256];

void loop() {
  WebSerial.println("Hello!");

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  uint16_t battery_voltage = esp_adc_cal_raw_to_voltage(analogRead(BOARD_BAT_ADC_PIN), &adc_chars) * 2;

  snprintf(buf, 256, "Battery:%umV", battery_voltage);

  WebSerial.println(buf);

  WebSerial.println(readADC_Cal(BOARD_BAT_ADC_PIN));

  delay(5000);
}