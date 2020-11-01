#include "DHT.h"
#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include<WiFiClient.h>
#define BLYNK_PRINT Serial
#define LDR 25
#define DHTPIN 22
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WidgetLED ledLux(V6);
WidgetLED ledTemp(V7);
BlynkTimer timer;

int led = 17;
int led2 = 16;
int slider_suhu, slider_lux, Button, lux;
float t;
char ssid[] = "ANDEKO 01";
char pass[] = "ANDEKO06";
char auth_token[] = "-G2hT5hFG7qBgH4aGO89-s_mCIv0S2bj";

BLYNK_CONNECTED(){
  Blynk.syncAll();
}

BLYNK_WRITE(V4){
  slider_lux = param.asInt();
}

BLYNK_WRITE(V5){
  slider_suhu = param.asInt();
}

BLYNK_WRITE(V8){
  Button = param.asInt();
}

void writeSuhuToBlynk(){
  t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V3, t);
}

void apalah(){
  int x = analogRead(LDR);
  lux = (0.009768*x)+10;
  Blynk.virtualWrite(V0, lux);
  Blynk.virtualWrite(V2, lux);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth_token, ssid, pass, IPAddress(192,168,1,12), 8080);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  dht.begin();
  timer.setInterval(2000L, writeSuhuToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
  apalah();
  if(Button == HIGH){
    Serial.println(t);
    if(t >= slider_suhu){
      digitalWrite(led,HIGH);
      ledTemp.on();
    }
    else{
      digitalWrite(led,LOW);
      ledTemp.off();
    }
    
    if(lux >= slider_lux){
      digitalWrite(led2,HIGH);
      ledLux.on();
    }
    else{
      digitalWrite(led2,LOW);
      ledLux.off();
    }
  }
  else{
   Serial.println("Sistem mati");
  }
}
