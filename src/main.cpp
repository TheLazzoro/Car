#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>
#include <ChristmasLED.h>

/*
   Board pin | NodeMCU GPIO |  Arduino IDE
	  A-           1             5 or D1
	  A+           3             0 or D3
	  B-           2             4 or D2
	  B+           4             2 or D4
*/

const char *ssid = "Car-Access-Point";
const char *pass = "car";

const char *serverNameJoystick = "http://192.168.4.1/joystick";

const int pwmMotorA = D1;
const int pwmMotorB = D2;
const int dirMotorA = D3;
const int dirMotorB = D4;

int motorSpeedA = 0;
int motorSpeedB = 0;
bool isRightReverse = false;
bool isLeftReverse = false;
int b = 0; // joystick "middle" button

std::string text;

void setup()
{
	Serial.begin(9600);
	Serial.println();

	pinMode(pwmMotorA, OUTPUT);
	pinMode(pwmMotorB, OUTPUT);
	pinMode(dirMotorA, OUTPUT);
	pinMode(dirMotorB, OUTPUT);
	Serial.println("Motor Shield 12E Initialized");

	ChristmasLED one(D0);
	ChristmasLED two(D5);

	// connecting -> disconnecting -> reconnecting makes it work for whatever reason.
	WiFi.begin(ssid);
	delay(1000);
	WiFi.disconnect();
	delay(1000);
	WiFi.begin(ssid);
	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to Wifi network: ");
	Serial.println(ssid);
	Serial.print("Local IP: ");
	Serial.println(WiFi.localIP());

	delay(500);
}

void loop()
{
	ChristmasLED::Update();

	if (WiFi.status() == WL_CONNECTED)
	{
		WiFiClient client;
		HTTPClient http;
		http.begin(client, serverNameJoystick);
		int responseCode = http.GET();
		if (responseCode != 200)
		{
			delay(100);
			return;
		}
		String payload = http.getString();
		int length = payload.length();
		char str[length + 1];
		strcpy(str, payload.c_str());

		// string split
		int values[] = {0, 0, 0, 0}; // initialize values
		int index = 0;
		String s = "";
		for (int i = 0; i < length; i++)
		{
			if (str[i] == ':')
			{
				values[index] = s.toInt();
				s = "";
				index++;
				continue;
			}
			s = s + str[i];
		}
		values[3] = s.toInt(); // hack

		motorSpeedA = values[0];
		motorSpeedB = values[1];
		isRightReverse = values[2] == 1;
		isLeftReverse = values[3] == 1;

		analogWrite(pwmMotorA, motorSpeedA);
		analogWrite(pwmMotorB, motorSpeedB);
		digitalWrite(dirMotorA, isRightReverse);
		digitalWrite(dirMotorB, isLeftReverse);

		// Serial.println("A Speed: " + String(motorSpeedA) + "    B Speed: " + String(motorSpeedB));
		Serial.println(payload);

		delay(60);
		http.end(); // free
		return;
	}

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