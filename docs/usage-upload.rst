Uploading new images
====================

To upload images, **make sure the switch is OFF** and connect the staff to a
computer. It should appear as an external USB drive - as if you had plugged
in a USB flash drive. Just drag the images to the staff to upload. All images
must be saved to the root directory of the staff drive - please do not create
subdirectories.

Technical requirements for images:

* Images must be in bitmap (BMP) format, with 24 bit color depth. Use any
  software you like to convert images in other formats to bmp - e.g. you can use
  Microsoft Paint 3D.

* The images are shown one **horizontal** line at a time; thus,  it is
  recommended to use images with **width** of 72 pixels - to match the number
  of LEDs on the staff. Maximal image size is 72x288 pixels.

* Image filenames must be at most 30 symbols long and can only contain letters,
  numbers, dashes and underscores. No spaces or special symbols!

You can create your own images or search for existing ones.  A good source for
POV image patterns is `Visual  POI Zone <https://visualpoi.zone/patterns/`__.
When downloading images from there, make sure to choose `Visual poi V4 mini (72px)`
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


.. code-block:: python

   image1.bmp 20
   image2.bmp 41
   image5.bmp 10
   image1.bmp


You can include some image file more than once, or not at all - it is your
choice. The name of your image list must be `imagelist.txt`, all lower case.

Once you have saved the image list,  eject the POV staff as you would normally
do a USB flash drive, and unplug the cable.

To help you figure out correct rotation and orientation of images, the pictures
below show the same image in BMP file and how it will look when using the POV staff.


.. figure:: images/image_infile.png
   :alt: Image in file
   :width: 80%

.. figure:: images/image_onstaff.png
   :alt: Image on staff
   :width: 80%



Directory `images` in the GitHub repository https://github.com/shurik179/povstaff
contains some sample images and `imagelist.txt` file; to get started, you can
just copy the contents  of that directory to root directory of your staff.
