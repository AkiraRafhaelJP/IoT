int led = 23;
int led2 = 01;
int led3 = 21;
int ledState = LOW;
int ledOn = 1;
unsigned long currentMillis;
int interval = 1000;
unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    if(ledState == LOW){
      ledState = HIGH;
    }
    else{
      ledState = LOW;
    }

    if(ledOn == 1){
      digitalWrite(led, ledState);
      if(ledState == LOW){
        ledOn = 2;
      }
    }
      
    else if(ledOn == 2){
      digitalWrite(led2, ledState);
      if(ledState == LOW){
        ledOn = 3;
      }
    }
      
    else{
      digitalWrite(led3, ledState);
      if(ledState == LOW){
        ledOn = 1;
      }
    }   
  }
}
