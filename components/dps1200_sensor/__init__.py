import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME, UNIT_VOLT, UNIT_AMPERE, UNIT_WATT, UNIT_CELSIUS, UNIT_RPM

# Namespace and class definition
dps1200_sensor_ns = cg.esphome_ns.namespace("dps1200_sensor")
DPS1200Sensor = dps1200_sensor_ns.class_("DPS1200Sensor", cg.Component)

# Schema for individual sensors with appropriate units
SENSOR_SCHEMA = cv.Schema({
    cv.Required(CONF_NAME): cv.string,
    cv.GenerateID(): cv.declare_id(sensor.Sensor),  # Ensure each sensor has a unique ID
}).extend({
    cv.Optional("unit_of_measurement"): cv.string,  # Optional, but useful for Home Assistant
    cv.Optional("accuracy_decimals"): cv.positive_int,
})

# Main component schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DPS1200Sensor),
    cv.Optional("volt_in"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_VOLT): cv.string,
    }),
    cv.Optional("amp_in"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_AMPERE): cv.string,
    }),
    cv.Optional("watt_in"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_WATT): cv.string,
    }),
    cv.Optional("volt_out"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_VOLT): cv.string,
    }),
    cv.Optional("amp_out"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_AMPERE): cv.string,
    }),
    cv.Optional("watt_out"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_WATT): cv.string,
    }),
    cv.Optional("internal_temp"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_CELSIUS): cv.string,
    }),
    cv.Optional("fan_rpm"): SENSOR_SCHEMA.extend({
        cv.Optional("unit_of_measurement", default=UNIT_RPM): cv.string,
    }),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Create the DPS1200Sensor component instance
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Register each sensor
    for key in ["volt_in", "amp_in", "watt_in", "volt_out", "amp_out", "watt_out", "internal_temp", "fan_rpm"]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}_sensor")(sens))