import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME

# Define the namespace and component class
empty_compound_sensor_ns = cg.esphome_ns.namespace('empty_compound_sensor')
EmptyCompoundSensor = empty_compound_sensor_ns.class_('EmptyCompoundSensor', sensor.Sensor, cg.Component)

# Configuration schema for the component
CONFIG_SCHEMA = sensor.sensor_schema(
    EmptyCompoundSensor,
    icon="mdi:counter",
    unit_of_measurement="",
    accuracy_decimals=0,
).extend({
    cv.Required(CONF_ID): cv.declare_id(EmptyCompoundSensor),
    cv.Optional(CONF_NAME, default="Empty Compound Sensor"): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Create a new instance of the component
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)