# RC_Scale  
**Arduino‑based 4‑corner RC vehicle scale using load cells and HX711 ADCs**

This project measures the weight distribution of RC vehicles using **four 5 kg load cells**, each connected to its own **HX711 ADC**, controlled by an **Arduino Nano**. An **OLED display**, **four buttons**, and **two indicator LEDs** provide an interactive interface for viewing total weight, corner weights, and weight ratios.

---

## ✨ Features

- **Total Weight Mode**  
  Displays the combined weight of all four load cells in **grams** and **pounds**.

- **Corner Weight Mode**  
  Shows individual wheel weights for LF, RF, LR, RR.

- **Weight Ratio Mode**  
  Calculates:
  - Front vs. Rear percentage  
  - Left vs. Right percentage  

- **Boot/Splash Screen**  
  Shows system status and initialization progress.

- **Calibration Support**  
  Built‑in workflow to determine:
  - Zero offset  
  - Scale factor (grams per ADC unit)

---

## 🛠 Hardware Overview

| Component | Description |
|----------|-------------|
| **MCU** | Arduino Nano |
| **Load Cells** | Four × 5 kg strain gauge load cells |
| **ADC Modules** | Four × HX711 24‑bit ADC boards |
| **Display** | 128×64 OLED (I²C) |
| **Inputs** | 4 × momentary pushbuttons |
| **Indicators** | 2 × LEDs for status/feedback |

---

## 📁 Project Structure (PlatformIO)

```
RC_Scale/
├── include/        # Header files
├── lib/            # External libraries
├── src/            # Main application code
├── test/           # Unit tests
├── platformio.ini  # PlatformIO environment config
└── README.md
```

---

## ⚙️ Installation & Setup

### 1. Clone the Repository
```bash
git clone https://github.com/DMatern/RC_Scale.git
cd RC_Scale
```

### 2. Open with PlatformIO  
Use VS Code + PlatformIO extension.

### 3. Upload Firmware  
Connect your Arduino Nano and click **Upload** in PlatformIO.

---

## 🔧 Calibration Procedure

The system includes a guided calibration mode. The process (as shown in the repo’s documentation):

### **Step 1 — Zero Offset**
1. Remove all weight from the load cell.
2. Press **Enter**.
3. The system determines the **OFFSET**, e.g.:

```
OFFSET: 625931
```

### **Step 2 — Scale Factor**
1. Place a known weight on the load cell.
2. Enter the weight in whole grams.
3. The system calculates the **scale factor**, e.g.:

```
WEIGHT: 50
SCALE: 391.597503
```

### **Step 3 — Apply Calibration**
Add these lines to your setup:

```cpp
scale.set_offset(625931);
scale.set_scale(391.597503);
```

Repeat for each of the four load cells.

---

## 📸 User Interface

- **OLED Display** cycles through:
  - Total weight
  - Corner weights
  - Ratio screen
- **Buttons** navigate pages and confirm calibration steps.
- **LEDs** indicate system status (ready, error, calibration).

---

## 🚧 To‑Do / Future Enhancements

- Add EEPROM storage for calibration values  
- Add Bluetooth or USB serial output  
- Add data logging mode  
- Add graphical UI improvements  

---

## 📜 License

This project is open‑source. Add your preferred license here (MIT, GPL, etc.).

---
