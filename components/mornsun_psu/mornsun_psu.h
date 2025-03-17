#pragma once

#include "esphome.h"

class MornsunPowerSupply : public PollingComponent {
 public:
  MornsunPowerSupply();

  void setup() override;
  void update() override;  // Replaces loop() for PollingComponent

  // Sensors
  Sensor *main_voltage = new Sensor();
  Sensor *main_current = new Sensor();
  Sensor *main_power = new Sensor();
  Sensor *aux_voltage = new Sensor();
  Sensor *aux_current = new Sensor();

  // Binary Sensors
  BinarySensor *fan1_fault = new BinarySensor();
  BinarySensor *fan2_fault = new BinarySensor();
  BinarySensor *aux_fault = new BinarySensor();
  BinarySensor *main_ov = new BinarySensor();

  // Configuration setters
  void set_i2c_address(uint8_t address) { i2c_address_ = address; }

 protected:
  uint16_t read_register(uint8_t reg);
  uint8_t i2c_address_;
};
