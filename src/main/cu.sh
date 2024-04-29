#!/bin/bash
FQBN='arduino:samd:mkrwifi1010'
DEVICE='/dev/ttyACM0'
[ "$1" = "c" ] && { arduino-cli compile --fqbn $FQBN; } || { arduino-cli compile --fqbn $FQBN && sudo arduino-cli upload -p $DEVICE -b $FQBN; }
