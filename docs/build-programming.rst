Step 7: Uploading firmware
===============================

If you are using a kit from our Tindie store which included ItsyBitsy M4 MCU, you can skip
this step - the MCU comes already pre-programmed.


Using precomplied binaries (M4 only)
------------------------------------
The easiest way to program the controller is by   using pre-built
binaries (for Itsy Bitsy M4 only).

1. Download the latest release of the project from |github|.
   Extract the downloaded archive to some place on your computer, and navigate to
   directory `code/binaries/ItsyBitsyM4`.

2. Connect the controller assembly to your computer using a micro USB cable. If
   you are doing it for the first time, you might get a message about
   installing drivers; in this case, wait until it tells you that your hardware
   is ready to use. Double-click the reset button on ItsyBitsy; your ItsyBitsy
   should appear as an external drive on your computer, with a name like `ITSYM4BOOT`.
   Open that drive in your file browser window; inside you should see files
   `CURRENT.UF2`, `INDEX.HTM`, and `UF2_INFO.TXT`.

3. First, you need to format the flash storage on ItsyBitsy board as FAT filesystem;
   you only need to do that once. To do that, inside extracted  directory `code/binaries/ItsyBitsyM4`, find the file
   `formatter.UF2` and drag and drop it to `ITSYM4BOOT`.  If you get a question
   "do you want to copy this file without its properties", click "Yes".
   After that, the ItsyBitsy will reboot, `ITSYM4BOOT` will disappear from your
   computer, and the formatter script will run; you won't see any visible output.

4. Double-click the reset button on ItsyBitsy again; `ITSYM4BOOT` drive should
   again appear on your computer. This time, drag `povstaff-vX.X.UF2` file to it.
   Again, the ItsyBitsy will reboot. The microcontroller now contains
   the povstaff software.

5. Press `Reset` button once (or disconnect and reconnect the USB cable) to restart
   the ItsyBitsy. It should now appear as an external drive with the name
   `POVSTAFF` in your computer, which should be empty.  Find in the extracted
   archive folder `images` and drag and drop files `rg-lines.bmp` and
   `imagelist.txt` to `POVSTAFF`. Eject the `POVSTAFF` drive and disconnect
   the USB cable; it is now ready for use.

Building from source
---------------------
An alternative way to program the controller is by building the software from
source. You will need Arduino IDE 1.8 or later to do that. It is assumed that
you have some experience with Arduino, e.g. know how to install libraries using
library manager.

1. Start Arduino IDE and install the board support files for your Itsy Bitsy (see instructions for
   `M4 <https://learn.adafruit.com/introducing-adafruit-itsybitsy-m4/setup>`__,
   `nRF52840 <https://learn.adafruit.com/adafruit-itsybitsy-nrf52840-express/arduino-support-setup>`__).

2. Make sure that you have the following libraries installed. All of them can
   be installed using library manager:

   *  FastLED
   *  Adafruit_TinyUSB
   *  Adafruit_SPIFlash
   *  Sd_Fat (adafruit fork)
   *  Adafruit_Sensor
   *  Adafruit_MPU6050
   *  POV-library

3. Connect the ItsyBitsy MCU to the computer using a microUSB cable.
   In `Tools` menu of Arduino IDE, select the correct board and port.
   Also, if using  Itsy Bitsy M4, select `Tools->USB stack->TinyUSB`.

4. First, you
   need to format the QSPI flash storage included on the board. To do so, find
   in the menu `File->Examples->Adafruit SPIFlash->SdFat_format`. Edit the line

   .. code-block:: C

      #define DISK_LABEL    "EXT FLASH"

   replacing `EXT FLASH` by a label of your choice, up to 11 symbols (e.g. "POVSTAFF").
   Upload the sketch to your board and start the serial monitor at 115200 baud.
   You should see a message asking you to confirm reformatting; respond "OK" to
   confirm and you should see message "Formatted flash".

5. Now, you are ready to upload the POV firmware.
   Download the latest release of POV staff project from |github| if you
   haven't done so already. Extract the archive file and find there folder
   `code/povstaff`. Copy the `povstaff` folder to your Arduino sketch folder.
   Open the sketch file `povstaff.ino` inside `povstaff` folder in Arduino IDE.

6. Click `Upload` to compile and upload the code. It can take  a while (mostly
   due to use of FastLED, which is a massive library). Once you get the message
   `Done uploading`, the programming is complete.


7. Press `Reset` button once (or disconnect and reconnect the USB cable) to restart
   the ItsyBitsy. It should now appear as an external drive with the name
   `POVSTAFF` (or whatever name you have chosen in step 4) in your computer,
   which should be empty.  Find in the extracted
   archive folder `images` and drag and drop files `rg-lines.bmp` and
   `imagelist.txt` to `POVSTAFF`. Eject the `POVSTAFF` drive and disconnect
   the USB cable; it is now ready for use.
