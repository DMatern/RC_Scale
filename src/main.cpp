#include <Arduino.h>
#include <Wire.h>

// ====================================
// Calabration Settings

// #define CAL_ENABLE // enable calabration mode

#ifdef CAL_ENABLE
#include <cal.h>
#else

// ====================================
// Global Variables

// ====================================
// Include Files

#include <U8g2lib.h>
#include <HX711.h>

HX711 scale0;
HX711 scale1;
HX711 scale2;
HX711 scale3;

HX711 scales[4] = {scale0, scale1, scale2, scale3};

//  adjust pins if needed
const uint8_t dataPin[4] = {3, 4, 5, 6};
const uint8_t clockPin = 7;

//  TODO you need to adjust to your calibrated scale values
float calib[4] = {420.0983, 421.365, 419.200, 410.236};

// ============================================================================
// GPIO Setup
int PIN_LEDred = A6;
int PIN_LEDgeen = A7;

int PIN_Button[4] = {A0, A1, A2, A3};



uint32_t count = 0;
// ============================================================================
// OLED Display Setup
int PIN_I2C_SDA = A4;
int PIN_I2C_SCL = A5;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/U8X8_PIN_NONE); // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

unsigned long lastDisplayUpdateTime = 0;  // the last time the display was updated
unsigned long displayUpdateInterval = 50; // the display update interval

int g_lineHeight = 0;
/*
Display is fixed two color and has 8 rows of 8 pixels, top two rows are yellow and rest are blue

         pixel
  | Row 1 (8) yellow
  | Row 2 (8) yellow
  | Row 3 (8) blue
  | Row 4 (8) blue
  | Row 5 (8) blue
  | Row 6 (8) blue
  | Row 7 (8) blue
  | Row 8 (8) blue

when setting hight:
> top row is 0, so each line must be -1
> add additonal -1 for space below text and center in row*2

*/

// ============================================================================
// Function Declarations

// ============================================================================
// Setup
// ============================================================================
void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("HX711_LIB_VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  Serial.println("Setup Begin");

  //GPIO
  for (int i = 0; i < 4; i++)
  {
    pinMode(PIN_Button[i], INPUT_PULLUP);
  }

  pinMode(PIN_LEDred, OUTPUT);
  pinMode(PIN_LEDgeen, OUTPUT);

  digitalWrite(PIN_LEDred, LOW);
  digitalWrite(PIN_LEDgeen, LOW);

  // Display
  u8g2.begin();

  u8g2.clearBuffer();                  // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
  u8g2.drawStr(0, 10, "Hello World!"); // write something to the internal memory
  u8g2.sendBuffer();                   // transfer internal memory to the display

  // Scales
  // for (int i = 0; i < 4; i++)
  // {
  //   scales[i].begin(dataPin[i], clockPin);
  //   scales[i].set_scale(calib[i]);
  //   //  reset the scale to zero = 0
  //   scales[i].tare();
  // }

  Serial.println("Setup Complete");

}

// ============================================================================
// Loop
// ============================================================================
void loop()
{
  // count++;
  // Serial.print(count);
  // for (int i = 0; i < 4; i++)
  // {
  //   Serial.print("\t");
  //   Serial.print(scales[i].get_units(5));
  // }
  // Serial.println();

  for (int i = 0; i < 4; i++)
  {
    if(digitalRead(PIN_Button[i]) == LOW)
    {
      Serial.print("Button Pressed: ");
      Serial.println(i);
      digitalWrite(PIN_LEDred, HIGH);
      delay(1000);
      digitalWrite(PIN_LEDred, LOW);
    }
  }



  delay(250);
}

// ====================================
// Setup Functions
// ====================================

#endif