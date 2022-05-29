#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <Arduino.h>
#include <env.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35       36 // ESP32 pin GIOP36 (ADC0) connected to LM35
#define test_payload
const int LEDPIN = 22;
void setup() {
pinMode(LEDPIN,OUTPUT);

  // put your setup code here, to run once:
Serial.begin(115200);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("ATTEMPTING TO CONNECT TO WIFI");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }   
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
 
  // put your main code here, to run repeatedly:
  int getWaterLevel ();
int water_level = getWaterLevel();
Serial.println("Water level");
Serial.println(water_level);
String mac_address = WiFi.macAddress();
Serial.println("Mac Address: ");
Serial.println(mac_address);
HTTPClient http;
  http.begin("http://192.168.100.212:3000/tank");
  http.addHeader("Content-Type", "application/json");

  String httpRequestData = "";
  StaticJsonDocument <96> doc;
  doc["tank_id"]= mac_address;
  doc["water_level"]= water_level;

  serializeJson(doc, httpRequestData);
  int httpResponseCode = http.POST(httpRequestData);

String payload = http.getString();
Serial.println(payload);
String test_load = payload.substring(46,50); 
Serial.println("load : ");
Serial.println(test_load);

if(test_load == "true")
{
    digitalWrite(LEDPIN,HIGH);
}
else{
    digitalWrite(LEDPIN,LOW);
}
  

 
 
  delay(2000);

}

int getWaterLevel() {
  // read the ADC value from the temperature sensor
  int adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in °C
  int tempC = milliVolt / 10;
  // convert the °C to °F
  //Serial.print("Temperature: ");
  //Serial.print(tempC);   // print the temperature in °C
  //Serial.print("°C");
  //delay(1000);
  return tempC;
}