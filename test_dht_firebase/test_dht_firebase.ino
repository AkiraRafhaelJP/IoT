#include <WiFi.h>
#include "DHT.h"
#include "FirebaseESP32.h"
#define DHTPIN 19
#define DHTTYPE DHT11
#define FIREBASE_HOST "https://test-ffa31.firebaseio.com/"
#define FIREBASE_AUTH "NaeWXNy1gkz9LW5KkjKZlEnDEJLL5zGM19q3oZfJ"

const char ssid[] = "ANDEKO 07";
const char pass[] = "LANTAI 1 MAS";
//
//const char ssid[] = "ANDEKO 06";
//const char pass[] = "LANTAI 2 MAS";

FirebaseData firebaseData;
DHT dht(DHTPIN, DHTTYPE);
String path = "/Node1";
float oldSuhu = 0;
float newSuhu = 0;
String oldStatus = "";
String newStatus = "";
boolean isTemperatureToggled = false;

void connect(){
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  }
  Serial.println("\n\n");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void calculateTemperature(){
  delay(2000);
  float t = dht.readTemperature();
  
  if (isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C\n"));
  newSuhu = t;
  if(t < 35){
    newStatus = "Aman";
  } else if(t <45){
    newStatus = "Siaga";
  } else{
    newStatus = "Bahaya";
  }
}

void setSuhuFirebase(){
  if(newSuhu != oldSuhu){
    Firebase.setDouble(firebaseData, path + "/suhu", newSuhu);
    oldSuhu = newSuhu;
  }
}

void setStatusFirebase(){
  if(newStatus != oldStatus){
    Firebase.setString(firebaseData, path + "/status", newStatus);
    oldStatus = newStatus;
  }
}

void getBtnToggle(){
  if(Firebase.getInt(firebaseData, path + "/suhuToggled")){
    if(firebaseData.intData() == 0){
      isTemperatureToggled = false;
    } else{
      isTemperatureToggled = true;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(DHTPIN, OUTPUT);

  dht.begin();
  connect();
}

void loop() {
  getBtnToggle();
  if(isTemperatureToggled){
    calculateTemperature();
    setSuhuFirebase();
    setStatusFirebase();
  }
}
