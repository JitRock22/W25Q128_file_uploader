# 📂 W25Q128 Flash Memory File Uploader using ESP32

This project allows you to **upload files from your computer to a W25Q128 flash memory chip** via an **ESP32 microcontroller**.  
It includes firmware for the ESP32 and a Python-based uploader script for seamless file transfer.

---

## 🧠 Overview

**Components:**
- **`Receiver.ino`** – ESP32 firmware that handles communication and file storage.
- **`Uploader.py`** – Python script for uploading files or folders to flash memory via serial.

**Goal:**  
Easily transfer files such as text, configuration data, or images to external flash storage connected to an ESP32.

---

## ⚙️ Hardware Setup

| W25Q128 Pin | ESP32 Pin |
|--------------|------------|
| CS           | D5         |
| D0 (MISO)    | D19        |
| D1 (MOSI)    | D23        |
| CLK (SCK)    | D18        |
| VCC          | 3.3V       |
| GND          | GND        |

> ⚠️ **Note:** Power the W25Q128 with **3.3V only**. Connecting it to 5V can permanently damage the chip.

---

## 🧩 Step 1: Flash the ESP32 Receiver Firmware

1. **Install Arduino IDE**  
   [Download here](https://www.arduino.cc/en/software)

2. **Install ESP32 Board Support**  
   - Open **File → Preferences**  
   - Add this URL under *Additional Boards Manager URLs*:  
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to **Tools → Board → Boards Manager → Search “ESP32” → Install**

3. **Install Required Libraries**
   - `SPI` (included by default)
   - `Adafruit SPIFlash` (install via Library Manager)

4. **Upload the Firmware**
   - Open `Receiver.ino` in Arduino IDE  
   - Select your ESP32 board under **Tools → Board**  
   - Select the correct **COM port**  
   - Click **Upload**

---

## 🗂️ Step 2: Prepare Your Files

- Organize files you want to upload in a folder.  
- Supported file types: text, images, JSON, configuration files, etc.  
- Ensure total size ≤ **16 MB (W25Q128 capacity)**.

---

## 💻 Step 3: Upload Files with Python Script

1. **Install Python 3.8+**  
   [Download here](https://www.python.org/downloads/)

2. **Install Required Library**
   ```bash
   pip install pyserial
3.**Run Uploader.py**
   ```bash
   Python uploader.py COM4 cute.bin 0x000000
