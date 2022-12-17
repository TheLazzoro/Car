#include <ESP8266WiFi.h>

class Car
{
public:
    /*
    Board pin | NodeMCU GPIO |  Arduino IDE
      A-           1             5 or D1
      A+           3             0 or D3
      B-           2             4 or D2
      B+           4             2 or D4
    */
    static const int pwmMotorA = D1;
    static const int pwmMotorB = D2;
    static const int dirMotorA = D3;
    static const int dirMotorB = D4;
    static int motorSpeedA;
    static int motorSpeedB;
    static bool isRightReverse;
    static bool isLeftReverse;

    // WiFi
    static const char *ssid;
    static const char *pass;
    static const char *serverNameJoystick;

    // Procedures
    static void Setup();
    static void Update();
};