# TINAH Arduino Core and Libraries

This package includes the Arduino core libraries, and support libraries for the motor and servo outputs on the UBC Engineering Physics TINAH board.

To install, in the Arduino IDE, go to File → Preferences, Additional Boards Manager 
URLs and 
add the URL `https://ENPH253.github.io/arduino-packages/package_ENPH253_index.json` to the list, separating any existing URLs with commas. Then, go to Tools → Board → Boards Manager, search for TINAH, and install the package. The Arduino IDE will check and notify you of any updates when they become available.

Make sure any old versions of the TINAH libraries, installed in Arduino/hardware and Arduino/libraries, are deleted before installing this through the boards manager.

Documentation can be found at:

https://projectlab.engphys.ubc.ca/enph253_2018/tinah-2018/#software

## Bootloader

This package has a new bootloader for the TINAH which uses the built-in LCD and buttons. There is still a start-up delay, but now pressing START will run the application code immediately, while pressing stop will make the bootloader wait indefinitely for code to upload. The display shows the version and current status of the bootloader.

It must be installed through an AVR-ISP programmer. From the Arduino IDE, use Tools → Burn Bootloader after selecting the programmer you are  using.
