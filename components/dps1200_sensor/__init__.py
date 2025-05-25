import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import CONF_ID, UNIT_VOLT, UNIT_AMPERE, UNIT_WATT, CONF_ADDRESS

DEPENDENCIES = ['i2c']

dps1200_sensor_ns = cg.esphome_ns.namespace('dps1200_sensor')
DPS1200Sensor = dps1200_sensor_ns.class_('DPS1200Sensor', cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DPS1200Sensor),
    cv.Required('vin'): sensor.sensor_schema(unit_of_measurement=UNIT_VOLT, accuracy_decimals=2),
    cv.Required('iin'): sensor.sensor_schema(unit_of_measurement=UNIT_AMPERE, accuracy_decimals=2),
    cv.Required('pin'): sensor.sensor_schema(unit_of_measurement=UNIT_WATT, accuracy_decimals=2),
    cv.Required('vout'): sensor.sensor_schema(unit_of_measurement=UNIT_VOLT, accuracy_decimals=2),
    cv.Required('iout'): sensor.sensor_schema(unit_of_measurement=UNIT_AMPERE, accuracy_decimals=2),
    cv.Required('pout'): sensor.sensor_schema(unit_of_measurement=UNIT_WATT, accuracy_decimals=2),
}).extend(cv.polling_component_schema('1s')).extend(i2c.i2c_device_schema(default_address=0x58))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield i2c.register_i2c_device(var, config)

    # Register sensors
    for key in ['vin', 'iin', 'pin', 'vout', 'iout', 'pout']:
        sens = yield sensor.new_sensor(config[key])
        cg.add(getattr(var, f'set_{key}_sensor')(sens))