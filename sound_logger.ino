// SEN0487 sound sensor logger for Arduino UNO R4 Minima
//
// Wiring:
//   SEN0487 GND -> Arduino GND
//   SEN0487 VCC -> Arduino 5V
//   SEN0487 A   -> Arduino A0
//
// This sketch samples the analog output of the mic and sends each
// reading over USB Serial as "millis,value". A separate Python
// script on your computer listens to this and writes it to a .txt
// file (see serial_logger.py).

#include <Arduino.h>
#include <cstdint>
#include <cmath>


namespace {

constexpr float sampleRate = 44100; // 44.1kHz the sample rate of CDs
constexpr uint8_t kSoundPin = A0;
constexpr uint32_t kSampleIntervalMs = 1/sampleRate; 
constexpr uint32_t kBaudRate = 115200;

uint32_t lastSampleMs = 0;

} // namespace

void setup() {
  Serial.begin(kBaudRate);
  while (!Serial) {
    ; // wait for the serial port to be ready
  }
  analogReadResolution(14);
}

void loop() {
  const uint32_t nowMs = millis();
  const int N = 64; // Windowing size
  

  if (nowMs - lastSampleMs >= kSampleIntervalMs) {
    lastSampleMs = nowMs;
    const uint16_t value = static_cast<uint16_t>(analogRead(kSoundPin)); // 0-1023, 10-bit ADC
    // Serial.println(value);
    float voltage = value * (5.0 / 16383.0);

    const float amplitude = 20*log10(2*value/N);
    Serial.print(value);
    Serial.print(" Arduino units, "); 
    Serial.print(amplitude);
    Serial.print("dB, "); 
    Serial.print(voltage);
    Serial.print("V");
    Serial.println(" ");
  }
}
