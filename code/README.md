# POV Staff Software

This folder contains software for POV Staff project.  It should be uploaded
to the ItsyBitsy (M4 or nRF52840 only) using Arduino IDE version 1.8.6 or later.
See *Software* section of the project description at
[instructables.com](http://instructables.com) for details.

## Required Libraries

- Adafruit DotStar
- Adafruit BusIO
- Adafruit TinyUSB
- Adafruit Unified Sensor
- Adafruit MPU6050
- Adafruit SPIFlash
- SdFat - Adafruit fork.

## Code Structure
All code is inside `povstaff` folder. To use it, copy the whole folder inside
your Arduino sketch folder.

This folder contains the following files:

- `config.h`: contains various configuration settings, such as number of
  LEDs in the strip or maximal filename length. If you are doing the project
  exactly as described in instructables guide, there is no need to change anyhting
  in these settings

- `flashstorage.h`, `flashstorage.cpp`: functions and classes needed for
  accessing the filesystem on the QSPI flash chip on the ItsyBitsy. In
  particular, it contains the code for making the staff show as external USB
  storage device when connected to the computer. Again, most likely you do not
  need to touch anything in these files.  

- `bmpimage.h`, `bmpimage.cpp`: definitions and functions of the two classes,
  `BMPimage`, describing a BMP image object, with functions to read it from
  flash memory, loading it into RAM, etc, and `BMPimageList`, for an ordered
  list of such images, with usual functions such as `advance to next image`.

- `staff.h`, `staff.cpp`: define the main class  `POVstaff`, which describes the
  staff as a whole, with functions for showing line of an image, showing voltage, getting rotation velocity, etc.

- `povstaff.ino`: main Arduino sketch, using the above classes. This is the
  file you need to edit if you want to modify the staff behavior.
