#include <almabraxas2.h>

void blinkSetup() {
  pinMode(13, OUTPUT);
  digitalWrite(2, HIGH);
}

static unsigned long next = 0;
static bool status = true;

void blinkLoop() {
  if (clock < next) return;
  digitalWrite(13, status ? HIGH : LOW);  
  next += status ? 100 : 1000;
  status = !status;
}

