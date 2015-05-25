#ifndef _UART_MAILBOX_H_
#define _UART_MAILBOX_H_

#include <Arduino.h>

#ifndef UART_MESSAGE_SIZE // So you can override
#define UART_MESSAGE_SIZE 160
#endif

// unique size to heap fight fragmentation

struct UARTMessage {

  UARTMessage():_len(0), _next(NULL) { }

  char _buffer[UART_MESSAGE_SIZE];
  uint8_t _len;
  UARTMessage *_next;

};

class UARTMailbox {
 public:
 UARTMailbox():_first(NULL), _last(NULL) { _startNewMessage(); }

  void addChar(char ch) {
    if (ch != '\n') {
      if (!_left) return;
      *++_p = ch;
      _left--;
      return;
    }

    // post
    int len = UART_MESSAGE_SIZE-_left-2; // Discard ending <CR>
    if (len < 0) len = 0;
    _inWriting->_len = len;
    _inWriting->_buffer[len] = '\0';
    if (_last) {
      _last->_next = _inWriting;
      _last = _inWriting;
    } else {
      _first = _last = _inWriting;
    }
    _startNewMessage();
  }

  UARTMessage *current() {
    return _first;
  }

  void pop() {
    UARTMessage *m = _first;
    if (!_first) return;
    _first = _first->_next;
    if (!_first) _last = NULL;
    delete m;
  }

  void consume(Stream &s) {
    int ch;
    while((ch = s.read()) != -1) addChar(ch);
  }

 private:

  void _startNewMessage() {
    _inWriting = new UARTMessage;
    _p = _inWriting->_buffer-1;
    _left = UART_MESSAGE_SIZE-1;
  }

 private:
  UARTMessage *_inWriting, *_first, *_last;
  char *_p;
  int _left;
  
  
};

#endif
