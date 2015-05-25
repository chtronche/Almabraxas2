#include <gps.h>

#include <UARTMailbox.h>

uint32_t gpsKo = 0, gpsN = 0;

void gpsSetup() {
  Serial1.begin(9600);
}

static UARTMailbox mbox;

static const char hex[] = "0123456789ABCDEF";

static bool checkSum(const char *gpsMessage) {
  if (*gpsMessage != '$') return false;
  int8_t max = 90; // NMEA says 82, but there could have longer proprietary messages
  byte v = 0;
  for(;;) {
    if (--max <= 0) return false; // Protect from run away if no *
    byte b = *++gpsMessage;
    if (b == '*') break;
    v ^= b;
  }
  unsigned char digit1 = hex[(v & 0xf0) >> 4];
  unsigned char digit2 = hex[(v & 0x0f)];
  return gpsMessage[1] == digit1 && gpsMessage[2] == digit2;
}

void gpsProcess() {
//if (strncmp(gps._buffer, "$GPRMC,", 7)) return;
//  log("gps", gps._buffer);
}

static void gpsLoop_(const char *msg, uint8_t) {
  ++gpsN;
  if (!checkSum(msg)) {
    ++gpsKo;
    return;
  }
  almalog("gps", msg);  
}

void gpsLoop() {
  UARTMessage *m = mbox.current();
  if (!m) return;
  gpsLoop_(m->_buffer, m->_len);
  mbox.pop();
}

void serialEvent1() {
  mbox.consume(Serial1);
}

