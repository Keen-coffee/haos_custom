from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_ADDRESS,
    CONF_REGISTER,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    ICON_FLASH,
)

DEPENDENCIES = ["i2c"]

pmbus_ns = cg.esphome_ns.namespace("pmbus_sensor")
PMBusSensor = pmbus_ns.class_("PMBusSensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PMBusSensor),
        cv.Required(CONF_ADDRESS): cv.i2c_address,
        cv.Required(CONF_REGISTER): cv.hex_uint8_t,
    }
).extend(cv.polling_component_schema("5s")).extend(i2c.i2c_device_schema(0x5A))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS], config[CONF_REGISTER])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)
