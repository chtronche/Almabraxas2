#include <almabraxas2.h>
#include <modem.h>
#include <UARTMailbox.h>

void modemSetup() {
  Serial2.begin(19200);
}

UARTMailbox mbox;

bool modemAwaken = false;

static void modemPing() {
  static bool initialized = false;

  if (modemAwaken) {
    if (initialized) return;
    modemSend("ATE0");
    initialized = true;
    return;
  }

  static unsigned long next = 0;

  if (clock < next) return;
  next += 1000;
  modemSend("AT");
}

void modemLoop() {
  modemPing();
  UARTMessage *m = mbox.current();
  if (!m) return;
  if (!modemAwaken) {
    almalog("modem-watchdog", "awaken");
    modemAwaken = true;
  }
  const char *buffer = m->_buffer;
  if (buffer[0]) {
    almalog("modem", buffer);
  }
  mbox.pop();
}

static void modemCmd(const char *message) {
  Serial2.print(message);
}

void modemSend(const char *message) {
  Serial2.println(message);
}

void serialEvent2() {
  mbox.consume(Serial2);
}

