#include <ESP8266WiFi.h>
#include <ChristmasLED.h>

std::vector<uint8> lights;
static uint8 ind = 0;
unsigned int millis_light_on = 60;
unsigned int counter_lights = 0;

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

    bool changed = millis() > millis_light_on * counter_lights;
    if (changed)
    {
        counter_lights++;
        ind++;
        if (ind == lights.size())
            ind = 0;
    }
}
