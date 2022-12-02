#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <string>

/*
   Board pin | NodeMCU GPIO |  Arduino IDE
	  A-           1             5 or D1
	  A+           3             0 or D3
	  B-           2             4 or D2
	  B+           4             2 or D4
*/

const char *ssid = "Car-Access-Point";
const char *pass = "car";
AsyncWebServer server(80);
const char *PARAM_MESSAGE = "message";

void action(AsyncWebServerRequest *request);
void test(AsyncWebServerRequest *request);

const int pwmMotorA = D1;
const int pwmMotorB = D2;
const int dirMotorA = D3;
const int dirMotorB = D4;

int motorSpeedA = 0;
int motorSpeedB = 0;
int b = 0; // joystick "middle" button

bool isRightReverse = false;
bool isLeftReverse = false;

std::string text;

void setup()
{
	Serial.begin(9600);
	Serial.println();

	Serial.println("Access point setup...");
	WiFi.softAP(ssid);
	// For some reason the network does not show up when setting a password.
	// WiFi.softAP(ssid, pass);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("IP: ");
	Serial.println(IP);

	server.on("/test", HTTP_GET, test);
	server.on("/speed", HTTP_POST, action);
	server.begin();
	Serial.println("Server started.");

	pinMode(pwmMotorA, OUTPUT);
	pinMode(pwmMotorB, OUTPUT);
	pinMode(dirMotorA, OUTPUT);
	pinMode(dirMotorB, OUTPUT);

	Serial.println("Motor Shield 12E Initialized");
	delay(500);
}

void test(AsyncWebServerRequest *request)
{
	Serial.println("hiiiii"); // for debugging
	request->send_P(200, "text/html", "response");
}

void action(AsyncWebServerRequest *request)
{
	String message;
	if (request->hasParam(PARAM_MESSAGE, true))
	{
		message = request->getParam(PARAM_MESSAGE, true)->value();
		Serial.println("action"); // for debugging
	}
	else
	{
		message = "No message sent";
	}
	request->send(200, "text/plain", "Hello, POST: " + message);
}

void loop()
{
	delay(10);
	return;

	Serial.println("Activate A");
	analogWrite(pwmMotorA, motorSpeedA);
	digitalWrite(dirMotorA, LOW);
	delay(1500);

	for (int i = 0; i < 512; i++)
	{
		text = "Speed: " + std::to_string(i);
		Serial.println(text.c_str());
		analogWrite(pwmMotorA, i);
		delay(100);
	}

	Serial.println("Reverse A");
	digitalWrite(dirMotorA, HIGH);
	delay(1500);

	Serial.println("Stop A");
	digitalWrite(pwmMotorA, 0);
	digitalWrite(dirMotorA, LOW);
	delay(3000);

	Serial.println("Activate B");
	digitalWrite(pwmMotorB, motorSpeedA);
	digitalWrite(dirMotorB, LOW);
	delay(1500);

	Serial.println("Reverse B");
	digitalWrite(dirMotorB, HIGH);
	delay(1500);

	Serial.println("Stop B");
	digitalWrite(pwmMotorB, 0);
	digitalWrite(dirMotorB, LOW);
	delay(3000);
}

/// @brief Controls the car based on analogue inputs.
/// @param x Turn signal 0 - 1023.
/// @param y Move direction signal 0 - 1023.
void controls(int x, int y)
{
	x = analogRead(D0) / 2 - 256;
	y = analogRead(D1) / 2 - 256;
	b = digitalRead(9);
	Serial.print("\n");

	float speedFactorA = y < 0 ? 1.0 : 1.0 - (float)y / (float)255;
	float speedFactorB = y > 0 ? 1.0 : 1.0 + (float)y / (float)255;

	motorSpeedA = x;
	isRightReverse = motorSpeedA < 0;
	if (isRightReverse)
		motorSpeedA = x + (-2 * x);

	motorSpeedA = motorSpeedA * speedFactorA;
	if (motorSpeedA < 0)
		motorSpeedA = 0;

	motorSpeedB = x;
	isLeftReverse = motorSpeedB < 0;
	if (isLeftReverse)
		motorSpeedB = x + (-2 * x);

	motorSpeedB = motorSpeedB * speedFactorB;
	if (motorSpeedB < 0)
		motorSpeedB = 0;

	Serial.print("B Speed: " + String(motorSpeedB) + "     A Speed: " + String(motorSpeedA) + "    revA: " + String(isRightReverse) + "   turn: " + String(y));

	delay(10);
}