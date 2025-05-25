
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID, CONF_NAME,
    UNIT_VOLT, UNIT_AMPERE, UNIT_WATT, UNIT_CELSIUS, UNIT_EMPTY,
    ICON_FLASH, ICON_THERMOMETER, ICON_POWER, ICON_FAN
)

CODEOWNERS = ["@yourgithub"]
DEPENDENCIES = ["i2c"]

dps_1200_ns = cg.esphome_ns.namespace("dps_1200")
DPS1200Sensor = dps_1200_ns.class_("HPPSUMonitorSensor", cg.PollingComponent, i2c.I2CDevice)

CONF_VIN = "vin"
CONF_VOUT = "vout"
CONF_IIN = "iin"
CONF_IOUT = "iout"
CONF_PIN = "pin"
CONF_POUT = "pout"
CONF_TEMP = "temp"
CONF_FAN = "fan"

SENSOR_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(sensor.Sensor),
})

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(DPS1200Sensor),
        cv.Optional(CONF_VIN): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="VIN"): cv.string,
        }),
        cv.Optional(CONF_VOUT): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="VOUT"): cv.string,
        }),
        cv.Optional(CONF_IIN): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="IIN"): cv.string,
        }),
        cv.Optional(CONF_IOUT): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="IOUT"): cv.string,
        }),
        cv.Optional(CONF_PIN): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="Power In"): cv.string,
        }),
        cv.Optional(CONF_POUT): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="Power Out"): cv.string,
        }),
        cv.Optional(CONF_TEMP): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="Temperature"): cv.string,
        }),
        cv.Optional(CONF_FAN): SENSOR_SCHEMA.extend({
            cv.Optional(CONF_NAME, default="Fan Speed"): cv.string,
        }),
    }).extend(cv.polling_component_schema("10s")).extend(i2c.i2c_device_schema(0x58))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for key, method in [
        (CONF_VIN, "set_vin_sensor"),
        (CONF_VOUT, "set_vout_sensor"),
        (CONF_IIN, "set_iin_sensor"),
        (CONF_IOUT, "set_iout_sensor"),
        (CONF_PIN, "set_pin_sensor"),
        (CONF_POUT, "set_pout_sensor"),
        (CONF_TEMP, "set_temp_sensor"),
        (CONF_FAN, "set_fan_sensor"),
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, method)(sens))
