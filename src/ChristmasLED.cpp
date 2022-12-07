#include <ESP8266WiFi.h>
#include <ChristmasLED.h>

std::vector<uint8> lights;
static uint8 ind = 0;
static int myTime = 0;

ChristmasLED::ChristmasLED(const uint8 ledPin)
{
    pinMode(ledPin, OUTPUT);
    lights.push_back(ledPin);
}

void ChristmasLED::Update()
{
    for (size_t i = 0; i < lights.size(); i++)
    {
        digitalWrite(lights.at(i), LOW);
    }
    digitalWrite(lights.at(ind), HIGH);

    int changed = (int) millis() * 0.005;
    if (myTime < changed)
    {
        myTime++;
        ind++;
        if (ind == lights.size())
            ind = 0;
    }
}
