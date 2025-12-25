import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, i2c
from esphome.const import CONF_ID

seeed_ns = cg.esphome_ns.namespace("seeed_eink_i2c")
SeeedEInk = seeed_ns.class_("SeeedEInk", display.Display, i2c.I2CDevice)

CONFIG_SCHEMA = (
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(SeeedEInk),
        }
    )
    .extend(i2c.i2c_device_schema(0x3C))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)
    await i2c.register_i2c_device(var, config)
