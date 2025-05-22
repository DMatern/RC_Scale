#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// ====================================
// Calabration Settings

#define CAL_ENABLE // enable calabration mode

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
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

// ====================================
// Global Variables

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
  PAGE_1,
  PAGE_2
};

modeEnum currentMode = MODE_STARTUP;
pageEnum currentPage = PAGE_STARTUP;

bool enteringNewMode = true;

enum LED_COLOR
{
  RED,
  GREEN
};

enum LED_STATE
{
  OFF,
  ON,
  BLINK
};

// Flags
uint8_t sysFlags = 0x00;
const byte sysFlag_LF = 0; // sysFlags bit 0:  0 = inactive	1 = active
const byte sysFlag_RF = 1;  // sysFlags bit 1:  0 = inactive	1 = active
const byte sysFlag_RR = 2;             // sysFlags bit 2:  0 = inactive	1 = active
const byte sysFlag_LR = 3;            // sysFlags bit 3:  0 = inactive	1 = active
const byte sysFlag_Tare = 4;            // sysFlags bit 4:  0 = inactive	1 = active
const byte sysFlag_Select = 5;            // sysFlags bit 5:  0 = inactive	1 = active
const byte sysFlag_UP = 6;             // sysFlags bit 6:  0 = inactive	1 = active
const byte sysFlag_DN = 7;           // sysFlags bit 7:  0 = inactive	1 = active

// ====================================
// OLED Display Initiation
int PIN_I2C_SDA = A4;
int PIN_I2C_SCL = A5;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/PIN_I2C_SCL, /* data=*/PIN_I2C_SDA, /* reset=*/U8X8_PIN_NONE); // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

unsigned long lastDisplayUpdateTime = 0;  // the last time the display was updated
unsigned long displayUpdateInterval = 50; // the display update interval

int g_lineHeight = 0;

// ====================================
// Include Files

#include <GPIO.h>
#include <GUI.h>
#include <scales.h>
// #include <cal.h>

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
void stateMachine();
void updateDisplay(pageEnum page);

    // ============================================================================
    // Setup
    // ============================================================================
    void setup()
{
  Serial.begin(115200);
  Serial.println("Setup Begin");

  // GPOI
  begin_GPIO();


  // Display
  u8g2.begin();
  u8g2.setFont(u8g2_font_7x14_tr);
  g_lineHeight = u8g2.getHeight() + 1;
  updateDisplay(PAGE_SPLASH);

  // Scales
  begin_scales();

  Serial.println("Setup Complete");
  display_freeram();

  delay(1000);

}

// ============================================================================
// Loop
// ============================================================================
void loop()
{

  update_GPIO();
  // update_scales();

  stateMachine();

  // delay(250);
}

// ====================================
// State Machine
// ====================================

void stateMachine() {

  switch (currentMode)
  {
  case MODE_STARTUP:
    if (enteringNewMode)
    {
      enteringNewMode = false;
      Serial.println("Mode: Startup");
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
        Serial.println("Mode: Run");
        updateDisplay(PAGE_MAIN); // Print border and message
      }

      // // Update local OLED display based on refresh period
      // if ((currentTime - lastDisplayUpdateTime) >= displayUpdateInterval)
      // {
      //   updateDisplay(PAGE_SENSORS); // Update display with the desired page
      //   lastDisplayUpdateTime = currentTime;
      // }


      // }

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

      // // Send data packet based on period
      // if ((currentTime - lastUpdateInterval) >= updateInterval_telnet)
      // {
      //   sendWXDEvent(getEpochTime(), getTemperatureF(), getHumidity(), getPressure(), getWindSpeed(), getLightLevel(), getEnclosureTemp());
      //   lastUpdateInterval = currentTime;
      // }

      // // Check for telnet connection timeout
      // if ((millis() - lastTelnetActivationTime) >= telnetTimeoutPeriod)
      // {
      //   telnet.disconnectClient(); // Force disconnect
      //   currentMode = RUN_MODE;
      //   enteringNewMode = true;
      //   bitClear(sysFlags, sysflag_terminalActive);
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
      u8g2.print("Startup Complete");
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

  case PAGE_MAIN:
    u8g2.firstPage();
    do
    {
      u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draws Border to size of Display
      u8g2.setCursor(3, 16);
      u8g2.print("RUNNING");
    } while (u8g2.nextPage()); // transfer internal memory to the display
    break;

  default:
    break;
  }

  // u8g2.sendBuffer();

}

#endif