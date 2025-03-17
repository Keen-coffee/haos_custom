import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_I2C_ADDRESS

DEPENDENCIES = ['i2c']

# Define the namespace and class for the C++ component
mornsun_psu_ns = cg.esphome_ns.namespace('mornsun_psu')
MornsunPowerSupply = mornsun_psu_ns.class_('MornsunPowerSupply', cg.PollingComponent, i2c.I2CDevice)

# Configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MornsunPowerSupply),
    cv.Optional(CONF_I2C_ADDRESS, default=0x00): cv.i2c_address,
}).extend(cv.polling_component_schema('10s')).extend(i2c.i2c_device_schema(0x00))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    if CONF_I2C_ADDRESS in config:
        cg.add(var.set_i2c_address(config[CONF_I2C_ADDRESS]))
