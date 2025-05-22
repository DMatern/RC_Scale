#include <HX711.h>

HX711 scale0; // Left Front
HX711 scale1; // Right Front
HX711 scale2; // Right Rear
HX711 scale3; // Left Rear

HX711 scales[4] = {scale0, scale1, scale2, scale3};

const uint8_t dataPin[4] = {5, 4, 3, 2};
const uint8_t clockPin = 6;

//  TODO you need to adjust to your calibrated scale values
// float calib[4] = {420.0983, 421.365, 419.200, 410.236};

// uint32_t count = 0;

// unsigned long lastScaleUpdate = 0;
// const unsigned long scaleUpdateInterval = 250;

void calibrate(HX711 &myScale);

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

  // configure the scales
  for (int i = 0; i < 4; i++)
  {
    scales[i].begin(dataPin[i], clockPin);
    delay(100); // Give time for HX711 to power up

    if (scales[i].is_ready())
    {
      Serial.print("Scale ");
      Serial.print(i);
      Serial.println(" ready.");
    }
    else
    {
      Serial.print("Scale ");
      Serial.print(i);
      Serial.println(" NOT connected or not ready!");
    }
  }

  Serial.println("Scale Setup Complete");

}

// ============================================================================
// Loop
// ============================================================================

void loop()
{
  Serial.println("Press any key to begin calibration");
  while (Serial.available()) Serial.read();

  while (!Serial.available())
  {
    // wait for user input
  }

  while (Serial.available())Serial.read();

  Serial.println("Enter Scale to Calibrate (0-3) followed by enter: ");

  while (Serial.available() == 0)
  {
    // wait for user input
  }

  char input[16] = {0}; // Buffer for user input
  Serial.readBytesUntil('\n', input, sizeof(input) - 1);
  int scaleNum = atoi(input);
  Serial.print("Calibrating Scale: ");
  Serial.println(scaleNum);
  calibrate(scales[scaleNum]);
}

void calibrate(HX711 &myScale)
{
  Serial.println("\n\nCALIBRATION\n===========");
  Serial.println("remove all weight from the loadcell");
  //  flush Serial input
  while (Serial.available()) Serial.read();

  Serial.println("and press enter\n");
  while (Serial.available() == 0);

  Serial.println("Determine zero weight offset");
  //  average 20 measurements.
  myScale.tare(20);
  int32_t offset = myScale.get_offset();

  Serial.print("OFFSET: ");
  Serial.println(offset);
  Serial.println();


  Serial.println("place a weight on the loadcell");
  //  flush Serial input
  while (Serial.available()) Serial.read();

  Serial.println("enter the weight in (whole) grams and press enter");
  uint32_t weight = 0;
  while (Serial.peek() != '\n')
  {
    if (Serial.available())
    {
      char ch = Serial.read();
      if (isdigit(ch))
      {
        weight *= 10;
        weight = weight + (ch - '0');
      }
    }
  }
  Serial.print("WEIGHT: ");
  Serial.println(weight);
  myScale.calibrate_scale(weight, 20);
  float scale = myScale.get_scale();

  Serial.print("SCALE:  ");
  Serial.println(scale, 6);

  Serial.print("\nuse scale.set_offset(");
  Serial.print(offset);
  Serial.print("); and scale.set_scale(");
  Serial.print(scale, 6);
  Serial.print(");\n");
  Serial.println("in the setup of your project");

  Serial.println("\n\n");
}


//  -- END OF FILE --
