#ifndef GPIO_H
#define GPIO_H

#include "OneButton.h"

// LED Pins
int PIN_LEDred   = 7;
int PIN_LEDgreen = 8;

// Button Pins
int PIN_Button[4] = {A0, A1, A2, A3};

int PIN_TARE = A0;
int PIN_UP = A3;
int PIN_SELECT = A2;
int PIN_DN = A1;

// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_tare(PIN_TARE, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_select(PIN_SELECT, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_up(PIN_UP, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_dn(PIN_DN, true);

// ============================================================================
// Funtion Definitions
void tare_click();
void tare_hold();
void select_click();
void select_hold();
void up_click();
void dn_click();
void led_on(enum LED_COLOR);
void led_off();
void led_blink();

// ============================================================================
// Setup
// ============================================================================

// setup code here, to run once:
void begin_GPIO() {

    // LED pin setup
  pinMode(PIN_LEDred, OUTPUT);
  pinMode(PIN_LEDgreen, OUTPUT);

  digitalWrite(PIN_LEDred, HIGH);
  digitalWrite(PIN_LEDgreen, HIGH);

  // Button TARE link functions
  btn_tare.attachClick(tare_click);
  btn_tare.attachLongPressStop(tare_hold);

  // Button SELECT link functions
  btn_select.attachClick(select_click);
  btn_select.attachLongPressStop(select_hold);

  // Button UP link functions
  btn_up.attachClick(up_click);

  // Button DOWN link functions
  btn_dn.attachClick(dn_click);

  delay(250);

  digitalWrite(PIN_LEDred, LOW);
  digitalWrite(PIN_LEDgreen, LOW);

}  // setup

// ============================================================================
// Loop
// ============================================================================

// main code here, to run repeatedly:
void update_GPIO() {

  // keep watching the push buttons:
  btn_tare.tick();
  btn_select.tick();
  btn_up.tick();
  btn_dn.tick();

  // update leds


}  // loop

// ====================================
// Buttons
// ====================================

// ----- button 1 callback functions

// TARE button functions
void tare_click() {
  Serial.println("tare click.");
}

void tare_hold() {
  Serial.println("tare longPress stop");
}

// Select button functions
void select_click(){
  Serial.println("select click");
}

void select_hold() {
  Serial.println("select longPress stop");
}

// Up button functions
void up_click() {
  Serial.println("up click.");
}

// Dn button functions
void dn_click() {
  Serial.println("dn click.");
}

// ====================================
// LED's
// ====================================

// void led_on(Enum LED_COLOR color) {
//   switch (color) {
//     case LED_RED:
//       digitalWrite(PIN_LEDred, HIGH);
//       digitalWrite(PIN_LEDgreen, LOW);
//       break;
//     case LED_GREEN:
//       digitalWrite(PIN_LEDred, LOW);
//       digitalWrite(PIN_LEDgreen, HIGH);
//       break;
//     case LED_YELLOW:
//       digitalWrite(PIN_LEDred, HIGH);
//       digitalWrite(PIN_LEDgreen, HIGH);
//       break;
//     default:
//       break;
//   }

// }

// void led_off() {

// }

// void led_blink() {

// }

// End
 #endif