# RC_Scale
Code for (4) Load Cells to measure weights of RC vehicles per wheel

System uses Arduino Nano and (4) ADC and 5kg load cells. Display is a OLED with (4) buttons and (2) leds for infications

To Do:

Functions:

- Boot: Display Splash screen and scale status
- Page_Total: Provide total vehicle weight (sum all four scales) in grams and lbs
- Page_Corners: Indicate each wheel weight in grams
- Page_Ratio: Indicated ratio from front to rear and left to right

Claibration Data:

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