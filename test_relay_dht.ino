#include <WiFi.h>
#include <MQTT.h>
#include "DHT.h"
#define RELAYPIN 23
#define DHTPIN 19
#define DHTTYPE DHT11

const char ssid[] = "ANDEKO 07";
const char pass[] = "LANTAI 1 MAS";

//
//const char ssid[] = "ANDEKO 06";
//const char pass[] = "LANTAI 2 MAS";

const char espname[] = "ESP32";
const char username[] = "1bdbde55";
const char password[] = "8d6971631d05bb0b";
char floatConverted[10];

boolean isTemperatureToogled = false;

WiFiClient net;
MQTTClient client;

DHT dht(DHTPIN, DHTTYPE);

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(espname, username, password)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/suhu");
  client.publish("/suhu", "Mati");
  // client.unsubscribe("/");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(payload == "ON"){
    digitalWrite(RELAYPIN, LOW);
    Serial.println("SUHU NYALA");
    isTemperatureToogled = true;
  } else if(payload == "OFF"){
    digitalWrite(RELAYPIN, HIGH);
    Serial.println("SUHU MATI");
    isTemperatureToogled = false;
    client.publish("/suhu", "Mati");
  }
}

void calculateTemperature(){
  delay(2000);
  float t = dht.readTemperature();
  
  if (isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      client.publish("/suhu", "error");
      return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C\n"));
  sprintf(floatConverted, "%.2f", t);
  client.publish("/suhu", floatConverted);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH);
  dht.begin();
  connect();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  if(isTemperatureToogled){
    calculateTemperature();
  }
}
