# VoltVigil Low-Voltage Wiring Map (HW-777 Expansion Board)

This document details the exact pinning and power distribution for the "Brain" of the VoltVigil system. By utilizing the HW-777 ESP32 30P Expansion Board, we eliminate messy breadboards and ensure stable power delivery to industrial sensors.

## 1. Board Configuration & Power Strategy

To safely run a 12V relay, 5V sensors, and a 3.3V ESP32 simultaneously, the board must be configured exactly as follows:

* **Power Source:** 12V DC, 1A (or 2A) adapter plugged into the Black Barrel Jack.
* **Yellow Jumper:** Set to **3.3V**. 
    * *Why?* This forces the entire Red "V" row to output 3.3V, protecting the ESP32's logic pins from accidental 5V exposure while keeping the OLED safe.
* **Common Ground:** The board internally shares ground. All Black "G" pins are safe to use as a common 0V reference.

---

## 2. Master S-V-G Connection Table

Use standard female-to-female jumper wires to connect the modules to the Expansion Board headers.

| Module | Pin on Module | Expansion Board Target | Voltage Level | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **12V Relay** | **VCC** | **VCC Pin (Right Side block)** | **12V** | Raw power from the barrel jack. |
| | **GND** | **GND Pin (Right Side block)** | 0V | |
| | **IN** (Signal) | **S-Row at D26** (Blue) | 3.3V Logic | Triggers the physical switch. |
| **PZEM-004T** | **VCC** | **VIN Pin** (Near USB) | **5V** | Powers internal optoisolators. |
| | **GND** | **G-Row** (Black) | 0V | |
| | **TX** | **S-Row at D16** (Blue) | 3.3V Logic | Maps to ESP32 RX2. |
| | **RX** | **S-Row at D17** (Blue) | 3.3V Logic | Maps to ESP32 TX2. |
| **ZMPT101B** | **VCC** | **VIN Pin** (Near USB) | **5V** | Required for clean Op-Amp wave. |
| | **GND** | **G-Row** (Black) | 0V | |
| | **OUT** | **S-Row at D34** (Blue) | Analog | Requires calibration via blue potentiometer! |
| **OLED (I2C)** | **VCC** | **V-Row** (Red) | **3.3V** | Safe logic voltage from jumper. |
| | **GND** | **G-Row** (Black) | 0V | |
| | **SDA** | **S-Row at D21** (Blue) | 3.3V Logic | |
| | **SCL** | **S-Row at D22** (Blue) | 3.3V Logic | |

---

## 3. Signal Integrity & Protection Additions

Due to electromagnetic interference (EMI) from the WiFi antenna and AC mains, the following passive components **must** be added to the jumper wires to prevent bogus readings and hardware freezes.

1.  **ZMPT101B Noise Filter:**
    * **10kΩ Resistor:** Spliced in series between ZMPT OUT and D34.
    * **0.1µF Ceramic Capacitor:** Bridged between the D34 S-Pin and the adjacent G-Pin.
2.  **OLED I2C Pull-ups (If screen freezes):**
    * **4.7kΩ Resistor:** Bridged between D21 (SDA) and the adjacent V-Pin (3.3V).
    * **4.7kΩ Resistor:** Bridged between D22 (SCL) and the adjacent V-Pin (3.3V).
3.  **Relay Flyback Protection:**
    * **1N4007 Diode:** Bridged across the Relay's VCC and GND pins (only required if the relay module lacks a built-in diode).

---

## 4. Pre-Flight Check

Before introducing 230V AC Mains:
1.  Plug in the 12V DC adapter.
2.  Verify the OLED turns on and displays "LOCAL MODE".
3.  Ensure the ESP32 does not feel excessively hot to the touch. 
4.  The system should read `0.0 V` and display `STATUS: !!TRIPPED!!` (Relay OFF) because no mains voltage is present yet.