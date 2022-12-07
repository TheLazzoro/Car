#include <ChristmasLED.h>
#include <Car.h>


void setup()
{
	Serial.begin(9600);
	Serial.println();

	Car::Setup();

	ChristmasLED one(D0);
	ChristmasLED two(D5);
	ChristmasLED three(D8);
	ChristmasLED four(D6);
	ChristmasLED five(D7);
}

void loop()
{
	Car::Update();
	ChristmasLED::Update();
}