#!/bin/bash
arduino-cli compile --fqbn arduino:avr:uno && sudo arduino-cli upload -p /dev/ttyACM0 -b arduino:avr:uno
