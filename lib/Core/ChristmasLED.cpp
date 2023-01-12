#include <ESP8266WiFi.h>
#include <ChristmasLED.h>

std::vector<uint8> lights;
static uint8 current_light_index = 0;
unsigned int millis_light_on = 60;
unsigned int light_threshold = 0;

ChristmasLED::ChristmasLED(const uint8 ledPin)
{
    pinMode(ledPin, OUTPUT);
    lights.push_back(ledPin);
}

void ChristmasLED::Update()
{
    // turns off all lights
    for (size_t i = 0; i < lights.size(); i++)
    {
        digitalWrite(lights.at(i), LOW);
    }
    digitalWrite(lights.at(current_light_index), HIGH); // turns on light at current index

    unsigned long time = millis();
    if (time > light_threshold)
    {
        light_threshold = time + millis_light_on;
        current_light_index++;
        if (current_light_index == lights.size())
            current_light_index = 0;
    }
}

int ChristmasLED::Count() {
    return lights.size();
}
