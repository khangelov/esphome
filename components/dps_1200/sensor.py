import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import (
    CONF_ID, UNIT_VOLT, UNIT_AMPERE, UNIT_RPM, UNIT_CELSIUS,
    ICON_FLASH, ICON_CURRENT_AC, ICON_THERMOMETER, ICON_FAN
)

CONF_GRID_VOLTAGE = "grid_voltage"
CONF_GRID_CURRENT = "grid_current"
CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_CURRENT = "output_current"
CONF_INTERNAL_TEMPERATURE = "internal_temperature"
CONF_FAN_RPM = "fan_rpm"
CONF_WATT_IN = "input_power"
CONF_WATT_OUT = "output_power"


dps1200_ns = cg.esphome_ns.namespace("dps1200")
DPS1200Sensor = dps1200_ns.class_("DPS1200Sensor", cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(DPS1200Sensor),
        cv.Optional(CONF_GRID_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=2,
        ),
        cv.Optional(CONF_GRID_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=2,
        ),
        cv.Optional(CONF_OUTPUT_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=2,
        ),
        cv.Optional(CONF_OUTPUT_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=2,
        ),
        cv.Optional(CONF_INTERNAL_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
        ),
        cv.Optional(CONF_FAN_RPM): sensor.sensor_schema(
            unit_of_measurement=UNIT_RPM,
            icon=ICON_FAN,
            accuracy_decimals=0,
        ),
        cv.Optional(CONF_WATT_IN): sensor.sensor_schema(
            unit_of_measurement="W",
            icon=ICON_FLASH,
            accuracy_decimals=2,
        ),
        cv.Optional(CONF_WATT_OUT): sensor.sensor_schema(
            unit_of_measurement="W",
            icon=ICON_FLASH,
            accuracy_decimals=2,

        ),
    }).extend(cv.polling_component_schema("10s")).extend(i2c.i2c_device_schema(0x5A))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    watt_in = None
    watt_out = None

    if CONF_GRID_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_GRID_VOLTAGE])
        cg.add(var.set_sensors(sens, None, None, None, None, None))

    if CONF_GRID_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_GRID_CURRENT])
        cg.add(var.set_sensors(None, sens, None, None, None, None))

    if CONF_OUTPUT_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_OUTPUT_VOLTAGE])
        cg.add(var.set_sensors(None, None, sens, None, None, None))

    if CONF_OUTPUT_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_OUTPUT_CURRENT])
        cg.add(var.set_sensors(None, None, None, sens, None, None))

    if CONF_INTERNAL_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_INTERNAL_TEMPERATURE])
        cg.add(var.set_sensors(None, None, None, None, sens, None))

    if CONF_FAN_RPM in config:
        sens = await sensor.new_sensor(config[CONF_FAN_RPM])
        cg.add(var.set_sensors(None, None, None, None, None, sens))
    
    if CONF_WATT_IN in config:
        watt_in = await sensor.new_sensor(config[CONF_WATT_IN])

    if CONF_WATT_OUT in config:
        watt_out = await sensor.new_sensor(config[CONF_WATT_OUT])       
    
    cg.add(var.set_sensors(
        config.get(CONF_GRID_VOLTAGE), config.get(CONF_GRID_CURRENT),
        config.get(CONF_OUTPUT_VOLTAGE), config.get(CONF_OUTPUT_CURRENT),
        config.get(CONF_INTERNAL_TEMPERATURE), config.get(CONF_FAN_RPM),
        watt_in, watt_out
    ))