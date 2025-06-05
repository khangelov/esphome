import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID, CONF_UPDATE_INTERVAL,
    ICON_FLASH, UNIT_VOLT, UNIT_AMPERE, UNIT_CELSIUS, UNIT_WATT,
    ICON_RADIATOR, ICON_POWER, ICON_THERMOMETER, ICON_FAN, DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT, DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_POWER,
)

CODEOWNERS = ["@yourusername"]
DEPENDENCIES = ["i2c"]

dps1200_ns = cg.esphome_ns.namespace("dps_1200")
DPS1200 = dps1200_ns.class_("DPS1200", cg.PollingComponent, i2c.I2CDevice)

CONF_GRID_VOLTAGE = "grid_voltage"
CONF_GRID_CURRENT = "grid_current"
CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_CURRENT = "output_current"
CONF_INTERNAL_TEMPERATURE = "internal_temperature"
CONF_FAN_RPM = "fan_rpm"
CONF_WATT_IN = "input_power"
CONF_WATT_OUT = "output_power"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DPS1200),
    cv.Optional(CONF_GRID_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    ),
    cv.Optional(CONF_GRID_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_POWER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT
    ),
    cv.Optional(CONF_OUTPUT_VOLTAGE): sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_VOLTAGE
    ),
    cv.Optional(CONF_OUTPUT_CURRENT): sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_POWER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_CURRENT
    ),
    cv.Optional(CONF_INTERNAL_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE
    ),
    cv.Optional(CONF_FAN_RPM): sensor.sensor_schema(
        icon=ICON_FAN,
        accuracy_decimals=0
    ),
    cv.Optional(CONF_WATT_IN): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER
    ),
    cv.Optional(CONF_WATT_OUT): sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        icon=ICON_FLASH,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_POWER
    ),
}).extend(cv.polling_component_schema("60s")).extend(i2c.i2c_device_schema(0x58))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    args = []
    for key in [
        CONF_GRID_VOLTAGE, CONF_GRID_CURRENT,
        CONF_OUTPUT_VOLTAGE, CONF_OUTPUT_CURRENT,
        CONF_INTERNAL_TEMPERATURE, CONF_FAN_RPM,
        CONF_WATT_IN, CONF_WATT_OUT
    ]:
        args.append(await sensor.new_sensor(config[key]) if key in config else cg.nullptr)

    cg.add(var.set_sensors(*args))