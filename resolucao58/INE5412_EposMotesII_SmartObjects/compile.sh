#!/bin/bash

echo "compiling"
make APPLICATION=led
echo "compilation complete, changing image"
arm-objcopy -I elf32-little -O binary img/led.img img/led.bin
echo "image complete, sending to epos"
sudo python red-bsl.py -t /dev/ttyUSB0 -f img/led.bin

