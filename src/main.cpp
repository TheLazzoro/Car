/*
#include <Arduino.h>
byte myPin = D4;

void setup()
{
  pinMode(myPin, OUTPUT);
}

void loop()
{
  delay(1000);
  digitalWrite(myPin, HIGH);
  delay(1000);
  digitalWrite(myPin, LOW);
}
*/

#include <ESP8266WiFi.h>

const char* ssid = "WiFimodem-9846";
const char* password = "jwmymdz4yw";


WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(5, 0);
  digitalWrite(4, 0);
  
  digitalWrite(0, 1);
  digitalWrite(2, 1);
}

void loop() {
  digitalWrite(5, HIGH);
  digitalWrite(0, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(5, LOW);
  digitalWrite(0, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  delay(10000);
}
