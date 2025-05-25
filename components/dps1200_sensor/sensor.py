import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_MOISTURE,
    CONF_TEMPERATURE,
    CONF_ILLUMINANCE,
    CONF_CALIBRATION,
    DEVICE_CLASS_MOISTURE,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_ILLUMINANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
    UNIT_CELSIUS,
    UNIT_LUX,
    CONF_RAW,
)

DEPENDENCIES = ["i2c"]

CONF_MIN_CAPACITY = "min_capacity"
CONF_MAX_CAPACITY = "max_capacity"
CONF_BRIGHTNESS_COEFFICIENT = "coefficient"
CONF_BRIGHTNESS_CONSTANT = "constant"
CONF_NEW_ADDRESS = "new_address"

chirp_ns = cg.esphome_ns.namespace("chirp")
chirpComponent = chirp_ns.class_(
    "I2CSoilMoistureComponent", cg.PollingComponent, i2c.I2CDevice
)

TEMPERATURE_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
)

MOISTURE_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_MOISTURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Optional(CONF_CALIBRATION): cv.Schema(
            {
                cv.Optional(CONF_MIN_CAPACITY, default=245): cv.uint16_t,
                cv.Optional(CONF_MAX_CAPACITY, default=550): cv.uint16_t,
                cv.Optional(CONF_RAW, default=False): cv.boolean,
            }),
        }
    )
)

ILLUMINANCE_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Optional(CONF_CALIBRATION): cv.Schema(
            {
                cv.Optional(CONF_BRIGHTNESS_COEFFICIENT, default=-1.525): cv.float_,
                cv.Optional(CONF_BRIGHTNESS_CONSTANT, default=100000): cv.uint32_t,
                cv.Optional(CONF_RAW, default=False): cv.boolean,
            }),
        }
    )
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(chirpComponent),
            cv.Optional(CONF_MOISTURE): MOISTURE_SCHEMA,
            cv.Optional(CONF_TEMPERATURE): TEMPERATURE_SCHEMA,
            cv.Optional(CONF_ILLUMINANCE): ILLUMINANCE_SCHEMA,
            cv.Optional(CONF_NEW_ADDRESS): cv.i2c_address,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(i2c.i2c_device_schema(0x20))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if moisture_config := config.get(CONF_MOISTURE):
        sens = await sensor.new_sensor(moisture_config)
        cg.add(var.set_moisture(sens))

        if calibration := moisture_config.get(CONF_CALIBRATION):
            cg.add(var.calib_capacity(
                calibration[CONF_MIN_CAPACITY],
                calibration[CONF_MAX_CAPACITY],
                calibration[CONF_RAW]),
            )

    if temperature_config := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature_config)
        cg.add(var.set_temperature(sens))

    if lum_config := config.get(CONF_ILLUMINANCE):
        sens = await sensor.new_sensor(lum_config)
        cg.add(var.set_light(sens))

        if calibration := lum_config.get(CONF_CALIBRATION):
            cg.add(var.calib_light(
                calibration[CONF_BRIGHTNESS_COEFFICIENT],
                calibration[CONF_BRIGHTNESS_CONSTANT],
                calibration[CONF_RAW]),
            )

    if address := config.get(CONF_NEW_ADDRESS):
        cg.add(var.set_address(address))