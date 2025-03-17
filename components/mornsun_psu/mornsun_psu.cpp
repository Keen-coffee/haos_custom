#include "mornsun_psu.h"
#include "esphome.h"

MornsunPowerSupply::MornsunPowerSupply() : PollingComponent(10000) {}  // 10-second polling interval

void MornsunPowerSupply::setup() {
  ESP_LOGD("MornsunPSU", "Setup complete for I2C address 0x%02X", i2c_address_);
}

void MornsunPowerSupply::update() {
  uint16_t vout = read_register(0x8B);      // Main Voltage (10mV)
  uint16_t iout = read_register(0x8C);      // Main Current (10mA)
  uint16_t pout = read_register(0x96);      // Main Power (100mW)
  uint16_t aux_vout = read_register(0xC4);  // Aux Voltage (10mV)
  uint16_t aux_iout = read_register(0xC5);  // Aux Current (10mA)
  uint16_t fault_bits = read_register(0xC7); // Fault Status

  if (vout != 0xFFFF) main_voltage->publish_state(vout * 0.01);    // 10mV to V
  if (iout != 0xFFFF) main_current->publish_state(iout * 0.01);    // 10mA to A
  if (pout != 0xFFFF) main_power->publish_state(pout * 0.1);      // 100mW to W
  if (aux_vout != 0xFFFF) aux_voltage->publish_state(aux_vout * 0.01); // 10mV to V
  if (aux_iout != 0xFFFF) aux_current->publish_state(aux_iout * 0.01); // 10mA to A

  if (fault_bits != 0xFFFF) {
    fan1_fault->publish_state(fault_bits & (1 << 0));
    fan2_fault->publish_state(fault_bits & (1 << 1));
    aux_fault->publish_state(fault_bits & (1 << 2));
    main_ov->publish_state(fault_bits & (1 << 3));
  }
}

uint16_t MornsunPowerSupply::read_register(uint8_t reg) {
  uint8_t data[2];
  if (!i2c_->write(i2c_address_, ®, 1)) {
    ESP_LOGW("MornsunPSU", "Failed to write register 0x%02X", reg);
    return 0xFFFF;
  }
  if (!i2c_->read(i2c_address_, data, 2)) {
    ESP_LOGW("MornsunPSU", "Failed to read register 0x%02X", reg);
    return 0xFFFF;
  }
  return (data[0] << 8) | data[1];  // Big-endian 16-bit
}
