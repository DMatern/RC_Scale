# RC_Scale
Code for (4) Load Cells to measure weights of RC vehicles per wheel

System uses Arduino Nano and (4) ADC and 5kg load cells. Display is a OLED with (4) buttons and (2) leds for infications

To Do:

General:

- Add global variables
- Add Global Flags
- Setup Buttons with OneButton
- Setup LED's with COntrol function (On, Off, Blink w/ rate)
- 

1. Startup
	- Setup One Button
	- Display Setup and Splash Screen
	- Set Status for Load Cells
2. Post Boot (Serial and OLED)
	- Provide post boot status messages: Load Cell Connected 1-4, Boot Sucsessfull	
3. Loop
- Display Pages
- Read Load Cells

Claibration Data

CALIBRATION - 0
===========
remove all weight from the loadcell
and press enter


Determine zero weight offset
OFFSET: 625931

place a weight on the loadcell
enter the weight in (whole) grams and press enter
50
WEIGHT: 50
SCALE:  391.597503

use scale.set_offset(625931); and scale.set_scale(391.597503);
in the setup of your project