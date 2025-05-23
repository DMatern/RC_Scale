#ifndef SCALES_H
#define SACLES_H

#include <HX711.h>

HX711 scale0; //Left Front
HX711 scale1; //Right Front
HX711 scale2; //Right Rear
HX711 scale3; //Left Rear

HX711 scales[4] = {scale0, scale1, scale2, scale3};
bool scalesReady[4] = {false, false, false, false}; // LF, RF, RR, LR

const uint8_t dataPin[4] = {5, 4, 3, 7};
const uint8_t clockPin = 6;

float calib[4] = {391.597503, 421.365, 419.200, 410.236};
unsigned long offset[4] = {625931, 0, 0, 0}; // offset values for each scale

unsigned long lastScaleUpdate = 0;
const int scaleUpdateInterval = 1000;

// ============================================================================
// Ratio Variables
int ratioFront = 0;
int ratioRear = 0;
int ratioLeft = 0;
int ratioRight = 0;

// ============================================================================
// Funtion Definitions
void getRatio_FB();
void getRatio_LR();

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
    delay(1000); // Give time for HX711 to power up

    scales[i].set_scale(calib[i]);
    scales[i].set_offset(offset[i]);

    if (scales[i].is_ready())
    {
      // scales[i].set_scale(calib[i]);
      scales[i].set_raw_mode();
      scales[i].tare();
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
      scalesReady[i] = false;
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
    for (int i = 0; i < 4; i++)
    {
      if(scalesReady[i]) {
        if (scales[i].is_ready())
        {
          // scalesReady[i] = true;
          // currentWeight[i] = scales[i].get_units(5);

          // Get the weight as a whole number (integer)
          currentWeight[i] = static_cast<int>(scales[i].get_units(5)); // Cast to int

          if(currentWeight[i] != previousWeight[i]) {
            previousWeight[i] = currentWeight[i];
            bitSet(sysFlags, sysFlag_newData);
          }

          Serial.print("\t");
          Serial.print(currentWeight[i]);
        }
        else
        {
          Serial.print("\t");
          Serial.print("N/A");
          // scalesReady[i] = false;
        }

      }
    }
    Serial.println();

    getRatio_FB();
    getRatio_LR();

  }
}

void tareAll() {
  for (int i = 0; i < 4; i++)
    {
      if (scales[i].is_ready())
      {
        scalesReady[i] = true;
        // currentWeight[i] = scales[i].get_units(5);
        scales[i].tare();
        // Serial.print("\t");
        // Serial.print(currentWeight[i]);
      }
      else
      {
        // Serial.print("\t");
        // Serial.print("N/A");
        scalesReady[i] = false;
      }
    }
}

void getRatio_FB() {
  // Calculate total front and rear weights
    int frontWeight = currentWeight[0] + currentWeight[1]; // Front Left + Front Right
    int rearWeight = currentWeight[2] + currentWeight[3];  // Rear Left + Rear Right

    // Calculate the total weight
    int totalWeight = frontWeight + rearWeight;

    // Avoid division by zero
    if (totalWeight == 0) {
        Serial.println(F("Error: Total weight is zero, cannot calculate ratio."));
        return;
    }

    // Calculate the front and rear weight percentages
    ratioFront = (frontWeight / totalWeight) * 100.0;
    ratioRear = (rearWeight / totalWeight) * 100.0;

}

void getRatio_LR() {
    // Calculate total front and rear weights
    int leftWeight = currentWeight[0] + currentWeight[3];
    int rightWeight = currentWeight[1] + currentWeight[2];

    // Calculate the total weight
    int totalWeight = leftWeight + rightWeight;

    // Avoid division by zero
    if (totalWeight == 0) {
        Serial.println(F("Error: Total weight is zero, cannot calculate ratio."));
        return;
    }

    // Calculate the front and rear weight percentages
    ratioLeft = (leftWeight / totalWeight) * 100.0;
    ratioRight = (rightWeight / totalWeight) * 100.0;
}

// ====================================
// Buttons
// ====================================

#endif