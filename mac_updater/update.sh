#! /bin/bash

echo -e "Enter COM port : \c"

avrdude "-Cavrdude.conf" -v -V -patmega328p -carduino "-PCOM$port" -b115200 -D "-Uflash:w:Sputterizer.ino.hex:i"
