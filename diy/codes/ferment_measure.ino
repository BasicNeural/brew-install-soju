#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define PIN_LED 16
#define URL "http:/server.domain:3000/co2-interval/put"

bool is_on = true;
void setup()
{
  Serial.begin(115200);
  Serial.println();

  
  WiFi.begin("ASUS_D0", "GeogeBool1815.");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
   WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(PIN_LED, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(2, HIGH);
  int resp = -1;
  
}

void loop() {
  int r = analogRead(A0);
  if (is_on && r > 650) {
    digitalWrite(PIN_LED, LOW);
    is_on = false;
    delay(50);
  } else if (!is_on && r < 100) {
    digitalWrite(PIN_LED, HIGH);
    is_on = true;
    unsigned long tick = 0;
    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(2, LOW);
      unsigned long start;
      unsigned long end;
      start = millis();
      WiFiClient client;
      HTTPClient http;
      
      http.begin(client, URL);
      
      int resp = http.GET();
      http.end();
      digitalWrite(2, HIGH);
      end = millis();
      tick = end - start;
    } else {
      while(WiFi.status() != WL_CONNECTED) {
          digitalWrite(2, LOW);
          delay(100);
          digitalWrite(2, HIGH);
          delay(100);
        }
    }
    if (tick < 50)
      delay(50 - tick);
  } else {
    delay(10);
  }
}
