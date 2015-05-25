#include <Arduino.h>

#include <almabraxas2.h>
#include <voltLed.h>

void voltLedSetup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
}

static int ledOn = 4;
static unsigned long next = 0;
static int status = LOW;

void voltLedSetValue(int volt) {
  int lastOn = ledOn;
  switch(ledOn) {
    case 4:
      if (volt < 80) ledOn = 3;
      break;
    case 3:
      if (volt < 70) 
        ledOn = 2;
      else if (volt > 80)
        ledOn = 4;
      break;
    case 2:
      if (volt > 70) ledOn = 3;
  }
  if (lastOn != ledOn && status == HIGH) digitalWrite(lastOn, LOW);
}

void voltLedLoop() {
  if (clock < next) return;
  digitalWrite(ledOn,status);
  if (status == HIGH) {
    next += 100;
    status = LOW;
  } else  {
    next += 1000;
    status = HIGH;
  }
}

