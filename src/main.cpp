#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// ====================================
// Global Variables

int currentWeight[4] = {0, 0, 0, 0}; // current weight for each scale in grams

/*
add comon used variable to save mem
serialBuffer[16]

*/

#include <scales.h>

// ====================================
// Calabration Settings

// #define CAL_ENABLE // enable calabration mode

#ifdef CAL_ENABLE
#include <cal.h>
#else

// ====================================
// RAM Functions

int freeRam()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0
                        ? (int)&__heap_start
                        : (int)__brkval);
}

void display_freeram()
{
  Serial.print(F("SRAM left: "));
  Serial.println(freeRam());
}

// ====================================
// Mode Settings

enum modeEnum
{
  MODE_STARTUP,
  MODE_RUN,
  MODE_TERMINAL
};
enum pageEnum
{
  PAGE_SPLASH,
  PAGE_STARTUP,
  PAGE_MAIN,
  PAGE_TOTALS,
  PAGE_CORNERS,
  PAGE_RATIOS
};

modeEnum currentMode = MODE_STARTUP;
pageEnum currentPage = PAGE_MAIN;
bool enteringNewMode = true;

enum LED_COLOR {RED, GREEN};
enum LED_STATE{OFF, ON, BLINK};

// ====================================
// Flags

uint8_t sysFlags = 0x00;
// const byte sysFlag_ = 0; // sysFlags bit 0:  0 = inactive	1 = active
// const byte sysFlag_ = 1;  // sysFlags bit 1:  0 = inactive	1 = active
// const byte sysFlag_ = 2;             // sysFlags bit 2:  0 = inactive	1 = active
const byte sysFlag_TareHold = 3;            // sysFlags bit 3:  0 = inactive	1 = active
const byte sysFlag_Tare = 4;            // sysFlags bit 4:  0 = inactive	1 = active
const byte sysFlag_Select = 5;            // sysFlags bit 5:  0 = inactive	1 = active
const byte sysFlag_UP = 6;             // sysFlags bit 6:  0 = inactive	1 = active
const byte sysFlag_DN = 7;           // sysFlags bit 7:  0 = inactive	1 = active

// ====================================
// OLED Display Initiation
uint8_t PIN_I2C_SDA = A4;
uint8_t PIN_I2C_SCL = A5;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/U8X8_PIN_NONE); // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

unsigned long lastDisplayUpdateTime = 0;  // the last time the display was updated
int displayUpdateInterval = 1000;       // the display update interval

uint8_t g_rowHeight = 8; // total of 8
uint8_t g_colWidth = 16; // total of 8

// ====================================
// Include Files

#include <GPIO.h>
#include <GUI.h>

// ============================================================================
// Function Declarations
void stateMachine();
void cyclePages();
void begin_Display();
void updateDisplay(pageEnum page);

// ============================================================================
// Setup
// ============================================================================
void setup()
{
  Serial.begin(115200);
  Serial.println(F("Setup Begin"));

  // GPOI
  begin_GPIO();

  // Display
  begin_Display();

  // Scales
  begin_scales();

  Serial.println(F("Setup Complete"));
  display_freeram();

  delay(1000);
}

// ============================================================================
// Loop
// ============================================================================

void loop()
{

  // update_GPIO();
  // update_scales();

  stateMachine();

  // Serial.println(millis());

  // delay(250);
}

// ====================================
// State Machine
// ====================================

void stateMachine() {
  unsigned long currentTime = millis(); // Get the current time

  switch (currentMode)
  {
  case MODE_STARTUP:
    if (enteringNewMode)
    {
      enteringNewMode = false;
      Serial.println(F("Mode: Startup"));
      updateDisplay(PAGE_STARTUP); // Print border and message

      //Print Status for Load Cells, RAM, cal values,etc
      enteringNewMode = true;
      currentMode = MODE_RUN; // set to next mode
    }
      break;

    case MODE_RUN:
      if (enteringNewMode)
      {
        enteringNewMode = false;
        Serial.println(F("Mode: Run"));
        updateDisplay(PAGE_MAIN); // Print border and message
      }

      update_GPIO();  //process GPIO input and output
      update_scales();//update scales

      // Update local OLED display based on refresh period
      if ((currentTime - lastDisplayUpdateTime) >= displayUpdateInterval)
      {
        updateDisplay(currentPage); // Update display with the desired page
        lastDisplayUpdateTime = currentTime;
      }

      if (bitRead(sysFlags, sysFlag_Tare))
      {
        bitClear(sysFlags, sysFlag_Tare);
        cyclePages();
      }  

      if (bitRead(sysFlags, sysFlag_TareHold))
      {
        bitClear(sysFlags, sysFlag_TareHold);
        // cyclePages();
        tareAll();
      }  

      // // Terminal Check
      // if (bitRead(sysFlags, sysflag_terminalActive))
      // {
      //   currentMode = TERMINAL_MODE;
      //   enteringNewMode = true;
      // }

      break;

    case MODE_TERMINAL:
      if (enteringNewMode)
      {
        enteringNewMode = false;
      }

      // // check for incoming telnet data
      // if (telnet.available())
      // {
      //   String command = readTelnetString();
      //   processTelnetString(command);
      //   lastTelnetActivationTime = millis(); // reset telnet activity time when new data is recived
      // }

      // // Display Update
      // if ((currentTime - lastDisplayUpdateTime) >= displayUpdateInterval)
      // {
      //   updateDisplay(PAGE_STATUS); // Update display with the desired page
      //   lastDisplayUpdateTime = currentTime;
      // }   

      // // Terminal Check for termination
      // if (!bitRead(sysFlags, sysflag_terminalActive))
      // {
      //   currentMode = RUN_MODE;
      //   enteringNewMode = true;
      // }
      break;

      // case MODE_STARTUP:
      //   /* code */
      //   break;

    default:
      break;
    }
}

// ====================================
// Setup Functions
// ====================================

// void setupDisplay()
// {

//   u8g2.begin();
//   u8g2.clearBuffer();
//   // g_lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent(); // set y positin based on font height

//   u8g2.setFont(u8g2_font_7x14_tr);
//   g_lineHeight = 8; // set y position per line of display

//   Serial.println("Setup Complete: Display");
//   delay(100);
//   updateDisplay(PAGE_SPLASH); // Print border and booting message
// }

void cyclePages() {
  const pageEnum firstPage = PAGE_TOTALS;  // Define the first page in the enum
  const pageEnum lastPage = PAGE_RATIOS;  // Define the last page in the enum

  Serial.println(F("Page Change"));

  currentPage = static_cast<pageEnum>(currentPage + 1); // Increment the current page
  if (currentPage > lastPage) // Wrap around if it exceeds the last page
  {
      currentPage = firstPage;
  }
}

void begin_Display() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_7x14_tr);
  updateDisplay(PAGE_SPLASH);
}

void updateDisplay(pageEnum page)
{
  // u8g2.clearBuffer();

  switch (page)
  {
  case PAGE_SPLASH:
    u8g2.firstPage();
    do
    {
      displaySplash();
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

  case PAGE_STARTUP:
    u8g2.firstPage();
    do
    {
      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); //Draws Border to size of Display
      // u8g2.drawStr(3, 16, "Booting...");
      u8g2.setCursor(3, 16);
      u8g2.print(F("Startup Complete"));
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

  case PAGE_MAIN:
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_7x14_tr);

      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draws Border to size of Display
      u8g2.setCursor(5, (g_rowHeight * 2) -1);
      u8g2.print(F("RUNNING"));      
      
      u8g2.setCursor(3, 32);
      u8g2.print(currentWeight[0]);

      u8g2.setCursor(3, 48);
      u8g2.print((currentWeight[0] * 0.00220462));

    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

    case PAGE_TOTALS:
    u8g2.firstPage();
    do
    {

      u8g2.setFont(u8g2_font_7x14_tr);

      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draws Border to size of Display
      u8g2.setCursor(5, g_rowHeight * 2);
      u8g2.print(F("TOTALS"));

      u8g2.setCursor(5, g_rowHeight * 4);
      u8g2.print(currentWeight[0]);
      u8g2.print(" grams");

      u8g2.setCursor(5, g_rowHeight * 6);
      u8g2.print((currentWeight[0] * 0.00220462));
      u8g2.print(" lbs");
      
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

    case PAGE_CORNERS:
    u8g2.firstPage();
    do
    {
      // u8g2.setFont(u8g2_font_7x14_tr);

      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draws Border to size of Display
      // u8g2.setCursor(5, (g_rowHeight * 2) - 1);
      // u8g2.print(F("CORNERS (grams)"));

      u8g2.setFont(u8g2_font_5x7_tr);

      u8g2.setCursor(g_colWidth * 1, (g_rowHeight * 1) - 0);
      u8g2.print(F("FRONT R"));
      u8g2.setCursor(g_colWidth * 1, (g_rowHeight * 8) - 2);
      u8g2.print(F("FRONT L"));
      u8g2.setCursor(g_colWidth * 5, (g_rowHeight * 1) - 0);
      u8g2.print(F("REAR  R"));
      u8g2.setCursor(g_colWidth * 5, (g_rowHeight * 8) - 2);
      u8g2.print(F("REAR  L"));

      // u8g2.setFont(u8g2_font_7x14B_tn);
      u8g2.setFont(u8g2_font_profont22_tn);

      u8g2.setCursor((g_colWidth * 1) -5, (g_rowHeight * 3) + 3);
      u8g2.print(currentWeight[0]);
      u8g2.setCursor((g_colWidth * 1) -4, (g_rowHeight * 7) - 6);
      u8g2.print(currentWeight[1]);
      u8g2.setCursor((g_colWidth * 5) -5, (g_rowHeight * 3) + 3);
      u8g2.print(currentWeight[3]);
      u8g2.setCursor((g_colWidth * 5) -4, (g_rowHeight * 7) - 6);
      u8g2.print(currentWeight[2]);
 
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

    case PAGE_RATIOS:
    u8g2.firstPage();
    do
    {
      // u8g2.setFont(u8g2_font_7x14_tr);

      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draws Border to size of Display
      // u8g2.setCursor(5, (g_rowHeight * 2) - 1);
      // u8g2.print(F("CORNERS (grams)"));

      // u8g2.setFont(u8g2_font_5x7_tr);

      // u8g2.setCursor(g_colWidth * 1, (g_rowHeight * 1) - 0);
      // u8g2.print(F("FRONT R"));
      // u8g2.setCursor(g_colWidth * 1, (g_rowHeight * 8) - 2);
      // u8g2.print(F("FRONT L"));
      // u8g2.setCursor(g_colWidth * 5, (g_rowHeight * 1) - 0);
      // u8g2.print(F("REAR  R"));
      // u8g2.setCursor(g_colWidth * 5, (g_rowHeight * 8) - 2);
      // u8g2.print(F("REAR  L"));

      // u8g2.setFont(u8g2_font_7x14B_tn);
      u8g2.setFont(u8g2_font_profont22_tn);

      u8g2.setCursor((g_colWidth * 1), (g_rowHeight * 4) + 0);
      u8g2.print(ratioFront);
      u8g2.setCursor((g_colWidth * 1), (g_rowHeight * 7) - 6);
      u8g2.print(ratioLeft);
      u8g2.setCursor((g_colWidth * 6), (g_rowHeight * 3) + 3);
      u8g2.print(ratioRear);
      u8g2.setCursor((g_colWidth * 6), (g_rowHeight * 7) - 6);
      u8g2.print(ratioRight);
 
    } while (u8g2.nextPage()); // transfer internal memory to the display

    break;

  default:
    break;
  }

  // u8g2.sendBuffer();

}

#endif