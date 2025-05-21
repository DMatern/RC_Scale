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