# VoltVigil Troubleshooting & FAQ

If your VoltVigil system is exhibiting "bogus" behavior, failing to compile, or not triggering the relay, consult this guide before altering your hardware.

---

## 1. Sensor & Data Errors (PZEM & ZMPT)

### 🔴 Issue: PZEM-004T shows `NaN` or `0.0V` when AC is ON.
* **Fix 1 (The RX/TX Swap):** 90% of the time, the data wires are backwards. Ensure PZEM **TX** connects to ESP32 **RX2 (GPIO 16)**, and PZEM **RX** connects to ESP32 **TX2 (GPIO 17)**.
* **Fix 2 (Voltage Starvation):** The PZEM-004T requires 5V to power its internal optoisolators. If you powered it from a 3.3V pin (the Red V-row on the expansion board), it will fail to communicate. Move its VCC wire to the **VIN (5V) pin**.
* **Fix 3 (No Common Ground):** Ensure the PZEM GND is connected to the same ground plane as the ESP32.

### 🔴 Issue: PZEM shows correct Voltage, but `0.00A` Current.
* **Fix (The CT Trap):** The PZCT-02 split-core transformer (the black ring) must be clipped around **ONLY the Live wire**. If you clip it around the entire AC power cord (Live + Neutral), the magnetic fields cancel out and it reads zero. 
* **Fix (Incomplete Closure):** Ensure the black clip is snapped entirely shut. Even a 1mm gap ruins the magnetic reading.

### 🔴 Issue: ZMPT101B wave is flatlined or clipping at the top.
* **Fix (Blue Potentiometer):** The signal is exceeding the ESP32's 3.3V limit. Use the Arduino IDE **Serial Plotter** (Baud 115200) and run the `zmpt_wave_plotter.ino` tool. Turn the small blue screw on the ZMPT module until the wave fits smoothly between 0 and 4095 without flattening at the top or bottom.

---

## 2. Relay & Muscle Failures

### 🔴 Issue: The ESP32 says "EXCELLENT", the Relay LED turns on, but there is no "CLICK" (Bulb stays off).
* **Fix (The 12V Power Trap):** A 12V relay cannot physically move its internal switch if powered by 5V (USB) or 3.3V. 
  1. Ensure a **12V DC adapter** is plugged into the Expansion Board barrel jack.
  2. Ensure the Relay VCC is connected to the **Right-Side VCC pin** (which outputs raw 12V), *not* the red V-row pins.
* **Fix (Missing Common Ground):** The Relay GND must connect back to the Expansion Board GND, or it will ignore the 3.3V trigger signal from GPIO 26.

---

## 3. Stability & Noise Issues

### 🔴 Issue: "Bogus" symbols (⸮⸮⸮) in the Serial Monitor.
* **Fix (Baud Rate):** The code is set to `Serial.begin(115200);`. Ensure the dropdown menu in the bottom right corner of your Serial Monitor matches `115200 baud`. If it is set to `9600`, the text will look like alien symbols.

### 🔴 Issue: Voltage readings randomly jump up and down (Jitter).
* **Fix (WiFi Noise Filter):** When the ESP32 turns on its WiFi radio, it creates electromagnetic interference (EMI) that messes with the Analog pins (like GPIO 34). 
  * Add a **0.1µF Ceramic Capacitor** between GPIO 34 and GND.
  * Add a **10kΩ Resistor** in series on the signal wire.

### 🔴 Issue: OLED screen freezes or fails to boot ("OLED Allocation Failed").
* **Fix (I2C Pull-ups):** The I2C lines need a firm voltage state. Add a **4.7kΩ resistor** from D21 (SDA) to 3.3V, and another from D22 (SCL) to 3.3V. 
* **Fix (I2C Address):** Most 0.96" OLEDs use `0x3C`, but some use `0x3D`. Try changing `display.begin(SSD1306_SWITCHCAPVCC, 0x3C)` to `0x3D` in the `voltvigil_master.ino` file.

---

## 4. Compilation & Code Errors

### 🔴 Issue: Code fails to compile with `mbedtls/md5.h` errors.
* **Fix:** This is a known bug between the `ESPAsyncWebServer` library and newer ESP32 board definitions. The `voltvigil_master.ino` file includes a "Compatibility Shield" at the very top. Do not remove these lines:
```cpp
#include <mbedtls/md5.h>
#define mbedtls_md5_starts_ret mbedtls_md5_starts
#define mbedtls_md5_update_ret mbedtls_md5_update
#define mbedtls_md5_finish_ret mbedtls_md5_finish
```
