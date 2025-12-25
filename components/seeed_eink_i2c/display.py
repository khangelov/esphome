import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, spi, time
from esphome.const import CONF_ID

seeed_ns = cg.esphome_ns.namespace("seeed_eink_spi")
SeeedEInk = seeed_ns.class_("SeeedEInk", display.Display, spi.SPIDevice)

CONF_TIME_ID = "time_id"

CONFIG_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(SeeedEInk),
    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
}).extend(spi.spi_device_schema())

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)
    await spi.register_spi_device(var, config)
    time_var = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time(time_var))
