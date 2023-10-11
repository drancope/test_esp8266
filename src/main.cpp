#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266Wifi.h>
#include <Wire.h>
#include "priv_data.h"
#include <DHT.h>
#include <ArduinoJson.h>
#include <SFE_BMP180.h>
#include <BH1750.h>
#include <NTPClient.h>
#include "WiFiUdp.h"

#define DHTPIN 12
#define DHTTYPE DHT11
#define ALTITUDE 840.0

//funciones
void callback(char* topic, byte* payload, unsigned int length);

//variables
bool luz = true;

char broker[] = BROKER;
WiFiClient Client;
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
SFE_BMP180 pressureSensor;
PubSubClient client(broker, 1883, callback, Client);
WiFiUDP wifiUdp;
NTPClient timeClient(wifiUdp, "es.pool.ntp.org", 1 * 3600, 60000);  // Ajust for your location


//código
void callback(char* topic, byte* payload, unsigned int length) {
  ;
}

float readPressureFromSensor()
{
  char status;
  double T, P, p0;

  status = pressureSensor.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressureSensor.getTemperature(T);
    if (status != 0)
    {
      status = pressureSensor.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressureSensor.getPressure(P,T);
        if (status != 0)
        {
          p0 = pressureSensor.sealevel(P,ALTITUDE);
          return p0;
        }
      }
    }
  }
  return -1;
}

void switch_light() {
  luz = !luz;
  digitalWrite(2, luz);
}

void reconnectMQTTClient(char *broker)
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection to ");
        Serial.print(broker);
        if (client.connect(CLIENT_NAME.c_str()))
        {
            Serial.println("  ...connected");
        }
        else
        {
            Serial.print(" Retying in 5 seconds - failed, rc=");
            Serial.println(client.state());

            delay(5000);
        }
    }
}

void createMQTTClient(char *broker)
{
    client.setServer(broker, 1883);
    reconnectMQTTClient(broker);
}

void connectWiFi(String SSIDD, String PASSD) {
  char *ssid = new char[SSIDD.length() +1 ];
  strcpy(ssid, SSIDD.c_str());
  char *pass = new char[PASSD.length() +1 ];
  strcpy(pass, PASSD.c_str());
  WiFi.mode(WIFI_STA);
//  WiFi.config(ip, gateway, subnet, dns1);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(2, OUTPUT);
//  pinMode(14, OUTPUT);  //intento de alimentación de sensores mediante pin de salida HIGH
  pinMode(16, OUTPUT);
  pinMode(13, INPUT);
  Serial.begin(115200);
  delay(1000);
  connectWiFi(ssidd, passs);
  createMQTTClient(broker);
//  digitalWrite(14, HIGH);
  dht.begin();
  pressureSensor.begin();
  lightMeter.begin();
//  digitalWrite(14, LOW);
  timeClient.begin();
  timeClient.update();
  digitalWrite(16, LOW);
}

void loop() {
/*  if ((WiFi.status() == WL_CONNECTED)) {
     Serial.println("Conectado.");
  }*/
  int frecuencia = 600000;

  int selector = digitalRead(13);
  if (selector == 0) frecuencia = 30000;
  reconnectMQTTClient(broker);
  client.loop();
//  digitalWrite(14, HIGH);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  uint16_t light = lightMeter.readLightLevel();
  float presion = readPressureFromSensor();
  timeClient.update();
  String timeNTP = timeClient.getFormattedDate();
//  digitalWrite(14, LOW);
  DynamicJsonDocument doc(1024);
  doc["hora"] = timeNTP;
  doc["luz"] = light;
  doc["temperatura"] = t;
  doc["humedad"] = h;
  doc["presion"] = presion;

  String telemetry;
  JsonObject obj = doc.as<JsonObject>();
  serializeJson(obj, telemetry);
  client.publish(CLIENT_TELEMETRY_TOPIC.c_str(), telemetry.c_str());
  Serial.print(CLIENT_TELEMETRY_TOPIC);
  Serial.println(telemetry);
  switch_light();
  delay(100);
  switch_light();
  delay(frecuencia);
}
