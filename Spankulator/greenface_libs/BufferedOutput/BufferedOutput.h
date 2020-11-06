/*
  BufferedOutput -  library 
  Copyright (c) 2020 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef BufferedOutput_h
#define BufferedOutput_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class BufferedOutput
{
  // user-accessible "public" interface
public:
  BufferedOutput(byte);
  void begin(void);
  bool get(void);
  void put(bool);
  void set(void);
  void reset(void);
  void toggle(void);

  // library-accessible "private" interface
private:
  byte pin;
};

#endif
