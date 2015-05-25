#ifndef _ALMABRAXAS2_MODEM_H_
#define _ALMABRAXAS2_MODEM_H_

void modemSetup();
void modemLoop();
void modemSend(const char *message);

extern bool modemAwaken;

#endif
