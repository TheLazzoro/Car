#include <Car.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>

/*
   Board pin | NodeMCU GPIO |  Arduino IDE
      A-           1             5 or D1
      A+           3             0 or D3
      B-           2             4 or D2
      B+           4             2 or D4
*/

const int pwmMotorA = D1;
const int pwmMotorB = D2;
const int dirMotorA = D3;
const int dirMotorB = D4;

int motorSpeedA = 0;
int motorSpeedB = 0;
bool isRightReverse = false;
bool isLeftReverse = false;
int b = 0; // joystick "middle" button

/*
    WiFi connection
*/
const char *ssid = "Car-Access-Point";
const char *pass = "car";
const char *serverNameJoystick = "http://192.168.4.1/joystick";


unsigned int refresh_rate = 60;
unsigned int counter = 0;

void Car::Setup()
{
    pinMode(pwmMotorA, OUTPUT);
	pinMode(pwmMotorB, OUTPUT);
	pinMode(dirMotorA, OUTPUT);
	pinMode(dirMotorB, OUTPUT);
	Serial.println("Motor Shield 12E Initialized");

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

void Car::Update()
{
    int doUpdate = millis() > refresh_rate * counter;

    if (WiFi.status() == WL_CONNECTED && doUpdate)
    {
        counter++;

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

        Serial.println(payload);

        http.end(); // free
    }
}
