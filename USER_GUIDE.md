# POV Staff User Guide
## by Alexander Kirillov

<img src="https://github.com/shurik179/povstaff/blob//main/main_photo.jpg?raw=true" height="300" style="float:right;"/>
This document describes the use of the POV staff. It assumes you have already
fully assembled it and uploaded the software. For more information, please
check the description of the project on instructables.com:
https://www.instructables.com/Persistence-of-Vision-LED-Staff/

The latest version of this document can always be found on the project github page:
https://github.com/shurik179/povstaff

## Turning on/off
To turn the staff on and off, use the switch at one end of the staff. As soon
as you have turned it on, it will show the battery voltage, by lighting part
of the LED strips. The longer the strip, the higher the battery charge.

If you only see one or two LEDs light up, the battery charge is too low for
use. You need to recharge the batteries as described below.

After using the staff, **make sure to turn it off**! If left on, LEDs will
drain the battery rather fast - even if you are not using the staff. To remind
you to turn it off, the staff will be blinking  red LEDs once every 30 seconds
while it is on and not being used.

## Charging/recharging
To charge the staff, remove the endcap from the end opposite to the switch.
Inside you will see a microcontroller with a micro-USB connector. Plug in a
USB charger; for best results, use a charger that can provide at least 1A
charging current.

For charging, the switch must be in ON position. Once you completed charging,
remember to switch the staff off.

Full charge should take about 5 hrs.

## Uploading images and creating image list
To upload images, make sure the switch is OFF and connect the staff to a
computer. It should appear as an external USB drive - as if you had plugged
in a USB flash drive. Just drag the images to the staff to upload. All images
must be saved to the root directory of the staff drive - please do not create
subdirectories.

Technical requirements for images:

* Images must be in bitmap (BMP) format, with 24 bit color depth. Use any
  software you like to convert images in other formats to bmp - e.g. you can use
  Microsoft Paint 3D.

* Maximal image size is 72x288 pixels. It is recommended to use images with
  width of 72 pixels - to match the number of LEDs on the staff

* Image filenames must be at most 30 symbols long and can only contain letters,
  numbers, dashes and underscores. No spaces or special symbols!

You can create your own images or search for existing ones.  A good source for
POV image patterns is [Visual  POI Zone](https://visualpoi.zone/patterns/).
When downloading images from there, make sure to choose *Visual poi V4 mini (72px)*
option. You will also need to rotate images 90 degrees.

Please note that the same color (i.e. the same RGB values) can look quite
different on the screen of your computer and on LED strip. Experiment with
colors to get some feel for it.

Once you have uploaded the images, you need to create a list showing in which
order these images should be played during the show. Create the file
`imagelist.txt` in the root directory of the POV staff (or open the file if
it already exists) and put there the list of image files in the order you want
to use them in your show, one filename per line, including `.bmp` extension.
Optionally, you can also add how long the image should be shown, in seconds
(whole numbers only!), separated from  filename by one or more spaces
```
image1.bmp 20
image2.bmp 41
image5.bmp 10
image1.bmp
```
You can include some image file more than once, or not at all - it is your
choice. The name of your image list must be `imagelist.txt`, all lower case.

Once you have saved the image list,  eject the POV staff as you would normally
do a USB flash drive, and unplug the cable.

To help you figure out correct rotation and orientation of images, the pictures
below show the same image in BMP file and how it will look when using the POV staff.

<img src="https://github.com/shurik179/povstaff/blob/main/image_infile.png?raw=true" height="200"/><img src="https://github.com/shurik179/povstaff/blob/main/image_onstaff.png?raw=true" height="200"/>


Directory `images` in the (github repository)[https://github.com/shurik179/povstaff]
contains some sample images and `imagelist.txt` file; to get started, you can
just copy the contents  of that directory to root directory of your staff.


## Using the staff
Now we are finally ready to use the staff. Turn the switch  ON; keep the
staff (more or less) stationary and  wait until it shows the voltage indicator.
After that the staff will go blank  waiting for you to start the show.  

To start the show, just start rotating the staff. As soon as rotation speed is
high enough, the staff will begin showing your first image, one line at a time,
and will continue doing that as long as you are twirling the staff. It will
adjust the interval between successive lines depending on the rotation speed,
to keep the image ratio close to original regardless of how fast you are
rotating the staff.  

The staff will continue showing the image for the duration given in
`imagelist.txt` file or until you pause the staff as described below, whatever
comes first. If duration for a given image was not provided, the staff will
continue showing this image until you pause.

To pause the show or move to the next image in the list, stop the staff
**in horizontal position**. It will go blank; as soon as you start twirling it
again, it will resume the show, moving to the next image. After reaching the
last image in the list, it will loop over, starting again with the first image.

After you are done, do not forget to **turn the staff OFF**, to avoid draining
the batteries.
