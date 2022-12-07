#include <ESP8266WiFi.h>

class ChristmasLED
{
public:
    ChristmasLED(const uint8 ledPin);
    static void Update();
};