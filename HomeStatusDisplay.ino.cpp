# 1 "C:\\Users\\andreas\\AppData\\Local\\Temp\\tmp0q3oohei"
#include <Arduino.h>
# 1 "C:/Users/andreas/Documents/PlatformIO/Projects/statusmon/src/HomeStatusDisplay.ino"
#include "HomeStatusDisplay.hpp"

static const char* VERSION = "0.6_dev";
static const char* IDENTIFIER = "HomeStatusDisplay";

HomeStatusDisplay display;
void setup();
void loop();
#line 8 "C:/Users/andreas/Documents/PlatformIO/Projects/statusmon/src/HomeStatusDisplay.ino"
void setup()
{
  display.begin(VERSION, IDENTIFIER);
}

void loop()
{
  display.work();
}