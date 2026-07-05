# VoltVigil High-Voltage (230V AC) Routing & Safety Guide

**⚠️ DANGER: LETHAL VOLTAGE HAZARD**
This project handles live 230V AC mains electricity. Never touch the circuit while plugged in. Always unplug the system from the wall before making any adjustments to the wiring, sensors, or the ESP32.

## 1. The "Safety Loop" (Input Protection)
Before the AC power reaches any of the VoltVigil components, it must pass through the safety loop. This protects your home's breaker and the ESP32 from catastrophic failure.

* **The Fuse:** Connect a **1A or 2A Glass Fuse** in series with the incoming **LIVE** wire. If any component shorts out, this fuse will blow instantly, cutting power.
* **The MOV (Surge Suppressor):** Wire a **7D471K MOV** directly across the **LIVE** (after the fuse) and **NEUTRAL** wires. If a voltage spike comes from the grid, the MOV will absorb it and protect the PZEM-004T.

---

## 2. Master AC Routing Map
Use thick, insulated copper wire (minimum 1.0mm sq) for all connections in this section. Do not use standard breadboard jumper wires.

| AC Mains Wire | Connects To... | Purpose |
| :--- | :--- | :--- |
| **LIVE (Brown/Red)** | 1. **Fuse Input** | The main hot wire from the wall plug. |
| **FUSED LIVE** | 1. **PZEM-004T (Terminal L)**<br>2. **Relay (COM Terminal)** | Supplies measured power to the sensor and feeds the relay switch. |
| **NEUTRAL (Blue/Black)** | 1. **PZEM-004T (Terminal N)**<br>2. **Bulb Holder (Terminal 1)** | Completes the AC circuit. |
| **SWITCHED LIVE** | 1. **Relay (NO Terminal)** $\rightarrow$ **Bulb Holder (Terminal 2)** | Only becomes live when the ESP32 activates the relay (Safe Zone). |

---

## 3. The Current Transformer (PZCT-02) Rule
The PZCT-02 is a split-core transformer (the black ring). It measures the magnetic field created by the current flowing to your bulb.

* **Wiring:** Connect the two small wires from the CT to the two terminals marked **"CT"** on the PZEM-004T. (Polarity does not matter here).
* **Placement (CRITICAL):** Open the CT clip and snap it around **ONLY the Switched Live wire** going from the Relay to the Bulb. 
* **The "Zero Amp" Trap:** If you clip the CT around *both* the Live and Neutral wires (like an entire power cable), the magnetic fields will cancel each other out, and the PZEM will always read `0.00 A`.

---

## 4. Final Physical Assembly Checklist
Before plugging the system into the wall for the first time, verify the following:

- [ ] **No Exposed Copper:** All AC screw terminals (on the PZEM and Relay) are tightened down with no bare wire sticking out.
- [ ] **Physical Isolation:** The 230V AC wires are physically separated from the 3.3V/5V DC jumper wires. Do not bundle them together.
- [ ] **No USB Connection:** Your laptop is **unplugged** from the ESP32. (Power the ESP32 via the 12V DC Jack on the expansion board during live testing).
- [ ] **CT Snapped Shut:** The PZCT-02 clip is completely closed. A tiny gap will result in inaccurate readings.

## 5. Expected Behavior on Power-Up
When you plug the system into the wall (assuming the 12V DC adapter is already powering the ESP32):
1. The PZEM will instantly register the grid voltage.
2. If the voltage is between the software-defined safe limits (`215V - 240V`), the ESP32 will trigger the relay.
3. You will hear a loud "CLICK," the bulb will illuminate, and the OLED will display `STATUS: EXCELLENT`.