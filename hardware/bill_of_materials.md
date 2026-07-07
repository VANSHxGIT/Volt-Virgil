# VoltVigil Bill of Materials (BOM)

This document lists all required hardware, sensors, and safety components to build the VoltVigil system using the ESP32 Expansion Board architecture.

## 1. Core Processing & Power
| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **ESP32 DevKit V1** | 1 | 30-Pin NodeMCU (Wi-Fi/BLE) | The main microcontroller and web server. |
| **HW-777 Expansion Board** | 1 | ESP32 30P Expansion Board | Provides S-V-G pins and handles 12V to 5V/3.3V regulation. |
| **12V DC Power Adapter** | 1 | 12V, 1A or 2A (5.5x2.1mm Jack) | Powers the expansion board, ESP32, and the 12V Relay. |

## 2. Sensors & Display
| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **PZEM-004T (v3.0)** | 1 | AC Digital Power Monitor Module | Measures RMS Voltage, Current, Power, Energy, Frequency, PF. |
| **PZCT-02** | 1 | 100A Split-Core Current Transformer | Clips onto the Live wire to measure current safely. |
| **ZMPT101B** | 1 | Single-Phase AC Voltage Transformer | Captures the analog AC sine wave for distortion analysis. |
| **0.96" OLED Display** | 1 | I2C, SSD1306, 128x64 Pixels | Local UI to display voltage and system status. |

## 3. High-Voltage Switching
| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **12V Relay Module** | 1 | 1-Channel, 10A/250VAC | The "Muscle." Switches the AC load (bulb) on/off based on ESP32 logic. |

## 4. Safety & Signal Integrity (The "Safety Kit")
*Crucial for protecting the ESP32 from noise and preventing electrical hazards.*

| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **Glass Fuse & Holder** | 1 | 1A or 2A, 250V Inline | Fire prevention. Placed on the main AC Live wire. |
| **MOV (7D471K)** | 1 | Metal Oxide Varistor, 470V | Surge protection. Wired across Live and Neutral. |
| **Capacitor** | 1 | 0.1µF Ceramic | Signal filter for ZMPT101B (placed between D34 and GND). |
| **Resistor** | 1 | 10kΩ | Current limiter for ZMPT101B signal line. |
| **Resistor** | 2 | 4.7kΩ | I2C Pull-ups for OLED stability (SDA/SCL to 3.3V). |
| **Diode** | 1 | 1N4007 (Optional) | Flyback diode for Relay coil (if the module lacks one). |

## 5. Prototyping & Wiring
| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **Jumper Wires** | 1 set | Female-to-Female | For connecting modules to the HW-777 expansion header. |
| **Breadboard** | 1 | Standard Half or Full Size | Used strictly to create a Common Ground rail. |
| **AC Copper Wire** | 1m | 1.0mm sq (Insulated) | Routing 230V mains through Relay, PZEM, and load. |
| **Heat Shrink Tubing** | 1 pk | Assorted diameters | Securing and insulating 230V AC joints. |

## 6. Testing Equipment (Bench Setup)
| Item | Qty | Description / Specification | Purpose |
| :--- | :---: | :--- | :--- |
| **AC Power Cord** | 1 | 3-Pin Plug to bare wire | Connecting the project to the wall socket. |
| **Bulb Holder** | 1 | Standard AC Socket | Holding the test load. |
| **LED Bulb** | 1 | 9W (Safe testing load) | Visual confirmation of the "EXCELLENT" relay state. |

---

### Hardware Notes
* **3.3V Jumper:** The yellow jumper on the HW-777 board must be set to **3.3V** to protect the ESP32 data pins.
* **12V Relay Requirement:** Because a 12V relay is used, it cannot be powered by the 5V USB. The 12V DC Adapter is mandatory for the relay to actuate.
* **Common Ground:** The 12V power supply ground, ESP32 ground, and all sensor grounds must be tied together.