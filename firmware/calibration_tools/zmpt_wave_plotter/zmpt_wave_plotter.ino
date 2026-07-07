// ==========================================
// ZMPT101B WAVE CALIBRATION TOOL
// Hardware: ESP32, ZMPT101B Voltage Transformer
// Pin: GPIO 34 (ADC1)
// ==========================================

#define ZMPT_PIN 34

void setup() {
  // High baud rate is required to push data fast enough 
  // to draw a smooth 50Hz/60Hz wave.
  Serial.begin(115200);
  
  // Optional but good practice for ESP32 analog pins
  pinMode(ZMPT_PIN, INPUT);

  Serial.println("Starting ZMPT101B Wave Plotter...");
  delay(1000);
}

void loop() {
  // Read the raw analog value (0 to 4095 on ESP32)
  int waveValue = analogRead(ZMPT_PIN);
  
  // Print ONLY the value so the Serial Plotter can graph it.
  // We print a fixed upper and lower bound to lock the graph scale.
  Serial.print("Max_Limit:4095,");
  Serial.print("Min_Limit:0,");
  Serial.print("Wave:");
  Serial.println(waveValue);

  // A 1ms to 2ms delay gives us roughly 500-1000 samples per second.
  // A 50Hz AC wave takes 20ms per cycle, so we get ~10-20 points per wave.
  delay(2); 
}