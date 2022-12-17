#include <Car.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string>

const int Car::pwmMotorA;
const int Car::pwmMotorB;
const int Car::dirMotorA;
const int Car::dirMotorB;
int Car::motorSpeedA = 0;
int Car::motorSpeedB = 0;
bool Car::isRightReverse = false;
bool Car::isLeftReverse = false;

const char *Car::ssid = "Car-Access-Point";
const char *Car::pass = "car";
const char *Car::serverNameJoystick = "http://192.168.4.1/joystick";

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
    int timeout = 10;
    while (WiFi.status() != WL_CONNECTED && timeout < 0)
    {
        delay(1000);
        Serial.print(".");
        timeout--;
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("");
        Serial.print("Unsuccessful WiFi connection.");
        return;
    }

    Serial.println("");
    Serial.print("Connected to Wifi network: ");
    Serial.println(ssid);
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    delay(500);
}

uint8 refresh_rate = 60;
unsigned int updates = 0;
unsigned long drive_threshold = 0;
uint16 timeout = 1000;

void Car::Update()
{
    unsigned long time = millis();

    // stop car in case wifi is not connected
    if (time > drive_threshold)
    {
        analogWrite(pwmMotorA, 0);
        analogWrite(pwmMotorB, 0);
        digitalWrite(dirMotorA, 0);
        digitalWrite(dirMotorB, 0);
    }

    int doUpdate = time > refresh_rate * updates;
    if (WiFi.status() != WL_CONNECTED || !doUpdate)
        return;

    updates++;

    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverNameJoystick);
    http.setTimeout(timeout);
    int responseCode = http.GET();
    if (responseCode != 200)
    {
        http.end(); // free
        return;
    }

    drive_threshold = time + timeout; // drive for 'timeout' seconds

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

    /*
        Prevents beeping motor noise when idle.
        The joystick registers values between 10-30 when idle,
        but this is not enough for the car to drive, hence the beeping noise.
    */
    if (motorSpeedA < 40)
        motorSpeedA = 0;
    if (motorSpeedB < 40)
        motorSpeedB = 0;

    analogWrite(pwmMotorA, motorSpeedA);
    analogWrite(pwmMotorB, motorSpeedB);
    digitalWrite(dirMotorA, isRightReverse);
    digitalWrite(dirMotorB, isLeftReverse);

    Serial.println(payload);

    http.end(); // free
}
