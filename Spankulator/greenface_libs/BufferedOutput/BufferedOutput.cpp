/*
  BufferedOutput.cpp - BufferedOutput library
  Copyright (c) 2020 Shannon Vance.  All right reserved.
*/

// include this library's description file
#include "BufferedOutput.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

BufferedOutput::BufferedOutput(byte _pin)
{
  pin = _pin;
  begin();
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
void BufferedOutput::begin()
{
  pinMode(pin, OUTPUT);
  reset();
}

bool BufferedOutput::get()
{
  return !digitalRead(pin);
}

void BufferedOutput::put(bool o)
{
  digitalWrite(pin, !o);
}

void BufferedOutput::set()
{
  put(true);
}

void BufferedOutput::reset()
{
  put(false);
}

void BufferedOutput::toggle()
{
  put(!get());
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
