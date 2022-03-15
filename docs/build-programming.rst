Step 7: Uploading firmware
===============================
Here, we describe the easiest way to program the controller, using pre-built
binaries. You can also build software from source, which allows you to modify
the code any way you like. Please visit |github| for instructions.

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
   `povstaff` in your computer, which should be empty.  Find in the extracted
   archive folder `images` and drag and drop files `rg-lines.bmp` and
   `imagelist.txt` to `povstaff`. Eject the `povstaff` drive and disconnect
   the USB cable; it is now ready for use.
