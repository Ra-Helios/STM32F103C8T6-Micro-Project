# BMI Calculator using OLED and 4x3 Matrix Keypad

To design and implement a **BMI (Body Mass Index) calculator** using the **STM32F103C8T6 microcontroller**, a **4x3 matrix keypad** for user input, and an **OLED display** for output visualization.

---

## 📘 Description of the Work
This project focuses on developing an **embedded system** capable of calculating and displaying the **Body Mass Index (BMI)** based on weight and height inputs provided by the user.

### 🔧 Hardware Used
- STM32F103C8T6 microcontroller
- 4x3 matrix keypad (for numeric input of weight and height)
- OLED Display (SSD1306 driver, I2C interface)
- ST-Link V2 Debugger
- Jumper Wires

### 💻 Software Used
- **STM32CubeIDE** (for code development and flashing)
- **HAL library** for GPIO and I2C handling
- `ssd1306.h`, `ssd1306.c`, `fonts.h`, and `fonts.c` (from OLED lab experiment for OLED handling)

---

## ⚙️ Working Principle
1. Upon powering the circuit, the text **“BMI Calculator”** appears on the OLED display.
2. The user is prompted to enter **weight (in kilograms)** and **height (in centimeters)** using the keypad.
3. The microcontroller computes BMI using the standard formula:

   \[
   BMI = \frac{Weight (kg)}{[Height (m)]^2}
   \]

4. The calculated BMI value is displayed on the OLED along with a **health status category**:
   - Underweight
   - Normal
   - Overweight
   - Obese

---

## 🔁 Program Flow
- Initialization of GPIO pins and OLED communication through I2C.
- Keypad scanning through row-column technique to detect user input.
- Data conversion and computation of BMI.
- OLED output update with the final BMI result and classification.

---

## 🔌 Hardware Connections

### 🧭 Pin Connection Table

| Component | Pin Name | STM32F103C8T6 Pin | Description |
|------------|-----------|-------------------|--------------|
| **OLED Display (SSD1306, I²C)** | VCC | 3.3V | Power Supply |
| | GND | GND | Ground |
| | SCL | PB6 | I²C Clock Line |
| | SDA | PB7 | I²C Data Line |
| **4x3 Matrix Keypad** | R1 | PA0 | Row 1 |
| | R2 | PA1 | Row 2 |
| | R3 | PA2 | Row 3 |
| | R4 | PA3 | Row 4 |
| | C1 | PA4 | Column 1 |
| | C2 | PA5 | Column 2 |
| | C3 | PA6 | Column 3 |
| **ST-Link V2** | SWDIO | SWDIO | Programming & Debugging |
| | SWCLK | SWCLK | Clock Line |
| | GND | GND | Common Ground |
| | 3.3V | 3.3V | Target MCU Power |

---

## ⚡ Power Supply Notes
- The entire circuit operates on **3.3V logic**.
- If using an external power source, ensure the **OLED and keypad** share a common ground with the STM32 board.
- Power can be supplied via **USB (through ST-Link)** or a **regulated 3.3V line**.

---

## ✅ Result
- The system successfully accepts user inputs for **Weight** and **Height** through the 4x3 keypad.
- The OLED display accurately shows the **computed BMI value** and corresponding category.
- The results matched **theoretical BMI calculations**.
- The interface and code were verified and debugged using **STM32CubeIDE** and real hardware testing.

---

## 🎥 Project Tutorial
Watch the full video tutorial here:
👉 [BMI Calculator Micro Project - Video Tutorial](https://drive.google.com/file/d/14zd5o82n87hdbuwisN_whjTRKsUC9sWI/view?usp=sharing)
