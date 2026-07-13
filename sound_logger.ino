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

namespace {

constexpr uint8_t kSoundPin = A0;
constexpr uint32_t kSampleIntervalMs = 10; // 100 samples/sec
constexpr uint32_t kBaudRate = 115200;

uint32_t lastSampleMs = 0;

} // namespace

void setup() {
  Serial.begin(kBaudRate);
  while (!Serial) {
    ; // wait for the serial port to be ready
  }
}

void loop() {
  const uint32_t nowMs = millis();

  if (nowMs - lastSampleMs >= kSampleIntervalMs) {
    lastSampleMs = nowMs;
    const uint16_t value = static_cast<uint16_t>(analogRead(kSoundPin)); // 0-1023, 10-bit ADC
    Serial.println(value);
  }
}
