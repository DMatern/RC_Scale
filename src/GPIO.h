#include "OneButton.h"

// LED Pins
int PIN_LEDred   = A6;
int PIN_LEDgreen = A7;

// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_tare(PIN_INPUT1, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_select(PIN_INPUT2, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_up(PIN_INPUT2, true);
// Setup a new OneButton on pin PIN_INPUT2.
OneButton btn_dn(PIN_INPUT2, true);

// setup code here, to run once:
void begin_GPIO() {

  // Button TARE link functions
  btn_tare.attachClick(tare_click);
//  button1.attachDoubleClick(doubleclick1);
  btn_tare.attachLongPressStart(tare_hold);
//  button1.attachLongPressStop(longPressStop1);
//  button1.attachDuringLongPress(longPress1);

  // Button SELECT link functions
  btn_select.attachClick(select_click);
//  button2.attachDoubleClick(doubleclick2);
  btn_select.attachLongPressStart(longPressStart2);
//  button2.attachLongPressStop(longPressStop2);
//  button2.attachDuringLongPress(longPress2);

  // Button UP link functions
  btn_up.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);

  // Button DOWN link functions
  btn_dn.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longPressStart1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);

  // LED pin setup
  pinMode(PIN_LEDred, OUTPUT);
  pinMode(PIN_LEDgreen, OUTPUT);

}  // setup


// main code here, to run repeatedly:
void update_GPIO() {
    
  // keep watching the push buttons:
  btn_tare.tick();

  btn_select.tick();
  btn_up.tick();
  btn_dn.tick();

  // update leds
  

}  // loop


// ----- button 1 callback functions

// This function will be called when the button1 was pressed 1 time (and no 2. button press followed).
void click1() {
  Serial.println("Button 1 click.");
}  // click1


// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
  Serial.println("Button 1 doubleclick.");
}  // doubleclick1


// This function will be called once, when the button1 is pressed for a long time.
void longPressStart1() {
  Serial.println("Button 1 longPress start");
}  // longPressStart1


// This function will be called often, while the button1 is pressed for a long time.
void longPress1() {
  Serial.println("Button 1 longPress...");
}  // longPress1


// This function will be called once, when the button1 is released after beeing pressed for a long time.
void longPressStop1() {
  Serial.println("Button 1 longPress stop");
}  // longPressStop1


// ... and the same for button 2:

void click2() {
  Serial.println("Button 2 click.");
}  // click2


void doubleclick2() {
  Serial.println("Button 2 doubleclick.");
}  // doubleclick2


void longPressStart2() {
  Serial.println("Button 2 longPress start");
}  // longPressStart2


void longPress2() {
  Serial.println("Button 2 longPress...");
}  // longPress2

void longPressStop2() {
  Serial.println("Button 2 longPress stop");
}  // longPressStop2

//=====================

const int ledPin1 = A6;
const int ledPin2 = A7;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
bool ledState1 = LOW;
bool ledState2 = LOW;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void ledOn(int pin) {
  digitalWrite(pin, HIGH);
}

void ledOff(int pin) {
  digitalWrite(pin, LOW);
}

void ledBlink(int pin, unsigned long interval, unsigned long &previousMillis, bool &ledState) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(pin, ledState);
  }
}

void loop() {
  // Example usage: Uncomment one of these lines to test
  // ledOn(ledPin1);
  // ledOff(ledPin2);
  ledBlink(ledPin1, 500, previousMillis1, ledState1); // Adjust the blink interval for LED 1
  ledBlink(ledPin2, 700, previousMillis2, ledState2); // Adjust the blink interval for LED 2
}


// End
