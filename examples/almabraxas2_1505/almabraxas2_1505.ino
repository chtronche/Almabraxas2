#include <Wire.h>

#include <almabraxas2.h>
#include <blink.h>
#include <gps.h>
#include <modem.h>
#include <pressureTempSensor.h>
#include <voltLed.h>

extern void gpsInit();

unsigned long clock;

void setup() {
  blinkSetup();
  almalogSetup();
  gpsSetup();
  modemSetup();
  pressureTempSensorSetup();
  voltLedSetup();
}

unsigned long next = 0;

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];


#include <unistd.h>

// function from the sdFat library (SdFatUtil.cpp)
// licensed under GPL v3
// Full credit goes to William Greiman.
int FreeRam() {
    char top;
    #ifdef __arm__
        return &top - reinterpret_cast<char*>(sbrk(0));
    #else  // __arm__
        return __brkval ? &top - __brkval : &top - &__bss_end;
    #endif  // __arm__
}

extern bool modemAwaken;
extern uint32_t gpsKo, gpsN;

#define V_6_5 (630)
#define V_9 (875)
#define V_FACTOR (90 - 65) / (V_9 - V_6_5)

int volt() {
  return (analogRead(2) - V_6_5) * V_FACTOR + 65;
}

void loop() {
  static uint32_t speed = 0;
  ++speed;
  clock = millis();
  blinkLoop();
  modemLoop();
  gpsLoop();
  voltLedLoop();
  if (clock >= next) {
    modemSend("AT+CSQ");
    next += 2000;

    int v = volt();
    voltLedSetValue(v);
    
    int temp;
    float pressure, altitude;
    pressureTempSensorGetValues(temp, pressure, altitude);
    snprintf(buffer, BUFFER_SIZE, "%ld freemem=%d cpuspeed=%ld modem=%d gps=%ld gpsKo=%ld volt=%d T=%d ALT=%.f", 
            clock, FreeRam(), speed, modemAwaken, gpsN, gpsKo, v, temp, altitude);
    almalog("X", buffer);
    
    speed = 0;
  }
}

