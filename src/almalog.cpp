#include <almabraxas2.h>

void almalogSetup() {
  Serial.begin(115200);
}

void almalog(const char *subsystem, const char *message) {
  Serial.print(clock / 1000);
  Serial.print(' ');
  Serial.print(subsystem);
  Serial.print(' ');
  Serial.println(message);
}

