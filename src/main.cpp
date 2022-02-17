#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266Wifi.h>

bool luz = false;
#define SSID "conlu2.4"
#define PASS "22433299"

void switch_light() {
  luz = !luz;
  digitalWrite(2, luz);
}

void connectWiFi(/*String SSID, String PASS*/) {
//    char *ssid = new char[SSID.length()]+1;
//    strcpy(ssid, SSID.c_str());
//    char *pass = new char[PASS.length()+1];
//    strcpy(ssid, PASS.c_str());
//    Serial.println(ssid);
//    Serial.println(pass);
    delay(20);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi..");
//        WiFi.hostname("esphost");
        delay(20);
        WiFi.mode(WIFI_STA);
        WiFi.begin(/*ssid, pass*/SSID, PASS);
        delay(5500);
    }
    delay(400);
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  delay(4000);
  connectWiFi(/*SSID, PASS*/);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch_light();
  delay(400);
}
