# POV Staff Software

This folder contains software for POV Staff project, V4.  It is intended to be complied
and uploaded to the QT Py ESP32-S3 (or QT Py ESP32-S2) using Arduino IDE  version
1.8 or later.   You will need the following libraries:

  * POV-ESP32 library from  https://github.com/shurik179/pov-esp32

  * Adafruit DotStar

  * Adafruit NeoPixel

  * ESPxWebFlMgr from   https://github.com/holgerlembke/ESPxWebFlMgr

You will need the arduino core for ESP32, see https://learn.adafruit.com/adafruit-qt-py-esp32-s2/arduino-ide-setup
for installation instructions.

Make sure that you choose the following options in the Arduino IDE tools menu
(for ESP32-S3):

* Partition scheme: *Default (3MB APP/1.5MB SPIFFS)*

* Erase All Flash Before Sketch Upload: *Disabled*
