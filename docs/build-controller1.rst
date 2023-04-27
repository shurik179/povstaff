Step 6: Controller assembly
===========================
Materials

* Bag E from the kit

* soldering iron

1. Before doing any work, get  the  ESP32-S2 Qt Py
   microcontroller from the bag and  test it. To do that, connect it by USB
   cable to a computer or
   charger to power it up. The on-board NeoPixel LED should blink red, then turn
   blue. **Wait  for 20-30 seconds** and open  the WiFi search dialog of your computer. You
   should see there a new WiFi network with the name `POVSTAFFXXXX`, where XXXX
   will be replaced by a 4-digit number, unique for each staff. Connect to this
   network, using the password. The password is in the information sheet
   which was included with your kit -  if you misplaced it,
   email `irobotics.store@gmail.com` for help.
   Type in the URL field of the browser the following address: `povstaff.local:8080`.
   You should see the webpage of the staff, which looks as follows:

   If you see the files `imagelist.txt`, `rg-lines.bmp` and `bg-triangles.bmp`,
   everything is good - you can disconnect the USB cable and proceed. Otherwise,
   check the troubleshooting section.

2. Solder the headers to the custom  POV controller shield as shown below.
   **make sure to solder it on the correct side of the board.**  The easiest way
   to keep the headers straight while soldering is to plug them into a breadboard.

   .. figure:: images/controller-1.jpg
       :alt: Kit of parts
       :width: 60%



   .. figure:: images/controller-2.jpg
       :alt: Kit of parts
       :width: 60%

3. Solder the ESP32-S2 QT Py  board to the headers, forming a "sandwich" of two boards
   as shown in the photos below. Make sure to solder it in the correct orientation:
   the USB connector should be facing the same way as the black 6-pin header on
   the controller shield.

   .. figure:: images/controller-3.jpg
      :alt: Controller
      :width: 80%

4. Take the 4-pin wire and solder it to the controller board on one side and to
   the switch connector board on the other as shown below. On each board, the
   wire with white stripe should go into the hole labeled "VCC".

   .. figure:: images/controller-4.jpg
      :alt: Controller
      :width: 80%
