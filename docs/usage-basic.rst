Basic usage
===========
To turn the staff on and off, use the switch at one end of the staff. It is a
3-position switch;  middle position is always OFF; one of the two other
positions is ON.

As soon as you have turned the staff on, it will show the battery voltage, by
lighting part of the LED strips. The longer the strip, the higher the battery charge.

If you only see one or two LEDs light up, the battery charge is too low for
use. You need to recharge the batteries as described in the next section.

After using the staff, **make sure to turn it off**! If left on, LEDs will
drain the battery rather fast - even if you are not using the staff. To remind
you to turn it off, the staff will be blinking  red LEDs once every 30 seconds
while it is on and not being used.


To start the show,  turn the staff ON and start rotating it.
As soon as rotation speed is high enough, the staff will begin showing your first image, one line at a time,
and will continue doing that as long as you are twirling the staff. It will
adjust the interval between successive lines depending on the rotation speed,
to keep the image ratio close to original regardless of how fast you are
rotating the staff.

You can find a number of staff spinning tutorials on YouTube - just search for
"staff spinning tutorials for beginners". I learned a lot from these two:
https://www.youtube.com/watch?v=L0madJzk3Wc
https://www.youtube.com/watch?v=gm5D8TS3mNY



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
