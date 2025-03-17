#include "pmbus_sensor.h"

// Define ESPHome component registration
esphome::sensor::PMBusSensor::PMBusSensor(uint8_t address, uint8_t register_address, float multiplier)
    : PollingComponent(5000), address_(address), register_address_(register_address), multiplier_(multiplier) {}

void esphome::sensor::PMBusSensor::setup() {
  Wire.begin();
}

void esphome::sensor::PMBusSensor::update() {
  Wire.beginTransmission(address_);
  Wire.write(register_address_);
  Wire.endTransmission();

  Wire.requestFrom(address_, (uint8_t)2);
  if (Wire.available() == 2) {
    uint16_t raw_value = Wire.read() | (Wire.read() << 8);
    float value = raw_value * multiplier_;
    publish_state(value);
  }
}
