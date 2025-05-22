#ifndef SCALES_H
#define SACLES_H

#include <HX711.h>

HX711 scale0; //Left Front
HX711 scale1; //Right Front
HX711 scale2; //Right Rear
HX711 scale3; //Left Rear

HX711 scales[4] = {scale0, scale1, scale2, scale3};
bool scalesReady[4] = {false, false, false, false};

const uint8_t dataPin[4] = {5, 4, 3, 2};
const uint8_t clockPin = 6;

//  TODO you need to adjust to your calibrated scale values
float calib[4] = {420.0983, 421.365, 419.200, 410.236};

unsigned long lastScaleUpdate = 0;
const int scaleUpdateInterval = 250;

// ============================================================================
// Funtion Definitions

// ============================================================================
// Setup
// ============================================================================

// setup code here, to run once:
void begin_scales()
{
  Serial.println(__FILE__);
  Serial.print(F("HX711_LIB_VERSION: "));
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  // configure the scales
  for (int i = 0; i < 4; i++)
  {
    scales[i].begin(dataPin[i], clockPin);
    delay(250); // Give time for HX711 to power up

    if (scales[i].is_ready())
    {
      // scales[i].set_scale(calib[i]);
      // scales[i].tare();
      Serial.print(F("Scale "));
      Serial.print(i);
      Serial.println(F(" ready and tared."));
      // Optionally set a flag here
      // bitSet(sysFlags, i);
      scalesReady[i] = true;
    }
    else
    {
      Serial.print(F("Scale "));
      Serial.print(i);
      Serial.println(F(" NOT connected or not ready!"));
      // bitClear(sysFlags, i);
    }
  }

  Serial.println(F("Scale Setup Complete"));

} // setup

// ============================================================================
// Loop
// ============================================================================

// main code here, to run repeatedly:
void update_scales()
{
  unsigned long now = millis();

  if (now - lastScaleUpdate >= scaleUpdateInterval)
  {
    lastScaleUpdate = now;
    // count++;
    // Serial.print(count);
    for (int i = 0; i < 4; i++)
    {
      if (scales[i].is_ready())
      {
        Serial.print("\t");
        Serial.print(scales[i].get_units(5));
      }
      else
      {
        Serial.print("\t");
        Serial.print("N/A");
      }
    }

    Serial.println();

  }
}

// ====================================
// Buttons
// ====================================

#endif