#include <Arduino.h>
#include <unity.h>
#include <Car.cpp>
#include <ESP8266WiFi.h>

void test_pins(void)
{
    int expected = LOW;
    int actual = analogRead(D1);
    TEST_ASSERT_EQUAL(expected, actual);
}

// Test when controller is off
void test_WiFi_Connection_Disconnected(void)
{
    auto expected = WL_DISCONNECTED;
    auto actual = WiFi.status();
    TEST_ASSERT_EQUAL(expected, actual);
}

int main()
{
    
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Car::Setup();

    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_pins);
    RUN_TEST(test_WiFi_Connection_Disconnected);
    UNITY_END();
}

void loop()
{
}
