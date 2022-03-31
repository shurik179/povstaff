Tools and materials
===================

You will need the following materials:

* `Kit of parts <https://www.tindie.com/products/irobotics/pov-staff-kit/>`__ from
  my Tindie store. Make sure  to choose the variant matching the order of signals
  on your LED strip (see details in the next section)

*  **APA102 (DotStar) LED strip, 144 LED/meter, black PCB**, for example
   this one `from Amazon <https://www.amazon.com/dp/B07BPX2KFD>`__. You need
   4 strips of 50cm (72 LEDs); you can also buy longer
   strips and cut them into 50cm pieces. The strips should not be waterproofed.
   Adafruit strips come with waterproof sheathing which you can just remove and
   discard.

*  Two **18650 Li-Ion batteries**. Look for high-capacity (at least 3000mAh),
   **unprotected** batteries from a reputable manufacturer such as Panasonic, Samsung,
   or Sanyo. Do not try to save money by buying no-name battery on eBay or Amazon;
   instead, use a specialized  store such as `18650batterystore.com <http://18650batterystore.com>`__.

*  `Polycarbonate tube, 11F(55in/141cm) <https://flowtoys.com/long-tubes-5f-to-13f>`__,
   1 inch outer diameter, from flowtoys.com. Note: while the tube is listed as 55",
   the actual length is 54.5".

*  1/2 inch (or 12 mm) square **wood dowel**, from HomeDepot or any other hardware
   store. You need two  pieces, each at least 51 cm (slightly over 20")

*  3d-printed spacers. You can order them with the kit of parts as an
   option or you can use your own 3d printer. The 3d models (in STEP and STL
   formats) are in `hardware/3d_printed` folder in github repository. You will
   need 2 copies of `spacer.stl` and one copy each of `controller cap`, `midspacer`,
   `switch_protector` and `switch_spacer`. I suggest printing at 0.2mm resolution
   for speed.

*  Adafruit `ItsyBitsy M4 <https://www.adafruit.com/product/3800>`__ microcontroller.
   Advanced users can also try  using
   ItsyBitsy `nRF52840 <https://www.adafruit.com/product/4481>`__;
   is is slightly more expensive, but it comes with Bluetooth, which
   opens many additional possibilities. However, current version of code
   doesn't use Bluetooth - this is planned for future upgrades. You will need
   to compile firmware from source if you plan on using nRF52840.

   Do not use
   32u4 or M0 ItsyBitsy - they do not have enough RAM for our purposes.

Tools
-----
You will need the usual tools and supplies:

* wire strippers

* pliers 

* flush cutters

* scissors

* decent soldering iron, solder, and flux

* electric tape

* heat gun (for shrinking the shrink wrap)

* sharp model knife or utility knife.


You will also need a computer to program the microcontroller and a USB charger
(with microUSB cable) to charge the batteries.

In addition, you will need black spray paint for painting the wood dowels and
epoxy glue.
